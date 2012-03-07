#!/usr/bin/ruby
# nessie_check.rb
=begin
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2008  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
=end

require 'rubygems'
require 'serialport'
require 'getopt/std'

$buffer_size = 0 # set automatically in init_system
$conffile_check = Hash.new
$conffile_check.default = 0

################################################################################
# readconfigfile                                                               #
################################################################################

def read_line_from_device()
  repeat_counter = 10000
  l = nil
  s = ''
  begin
    l = $sp.gets()
    repeat_counter -= 1
  end while !l && repeat_counter > 0
#  printf("DBG: << %s\n", l.inspect)
  return l
end

def readconfigfile(fname, conf)
  return conf if $conffile_check[fname]==1
  $conffile_check[fname]=1
  section = "default"
  if not File.exists?(fname)
    return conf
  end
  file = File.open(fname, "r")
  until file.eof
    line = file.gets()
	  next if /[\s]*#/.match(line)
  	if m=/\[[\s]*([^\s]*)[\s]*\]/.match(line)
	    section=m[1]
	    conf[m[1]] = Hash.new
	    next
	  end
	  next if ! /=/.match(line)
	  m=/[\s]*([^\s]*)[\s]*=[\s]*([^\s]*)/.match(line)
	  if m[1]=="include"
	    Dir.glob(m[2]){ |fn| conf = readconfigfile(fn, conf) }
	  else
  	  conf[section][m[1]] = m[2]
	  end
  end
  file.close()
  return conf
end

################################################################################
# reset_system                                                                 #
################################################################################

def reset_system
  $sp.print("\r")
  sleep 0.1
  $sp.print("\r")
  sleep 0.1
  $sp.print("echo off\r")
  sleep 0.1
end


def read_block(f)
  d = Array.new
  begin
    l = f.gets
    x = l.split.collect { |e| e.to_i(16) }
    d += x
  end while x.length == 16
  return d
end

def goto_next_header(f)
  while l = f.gets()
    m = /^#\ (=|-)*[=-]{5}/.match(l)
    t = :subblock  if m && m[1] == '-'
    t = :mainblock if m && m[1] == '='
    if !m && n = /^#\ (.*)$/.match(l)
      id = n[1]
      id.sub!(/[\r\n]/,'')
      return t,id
    end
    if !m && !id
      t = nil
    end
  end
  return nil,nil if !l
end

def skip_file_header(f)
  while l = f.gets()
    return if m = /^#\ [=]{40}/.match(l)
  end
end

def test_parse(f)
  skip_file_header(f)
  loop do
    a,b = goto_next_header(f)
    if !b
      puts(">>EOF<<")
      return
    end
    if a
      printf(">>%sblock: %s\n", a==:mainblock ? "main":"sub", b)
      next
    end
    printf(">item: %s\n", b)
    d = read_block(f)
    printf(">length: %d (0x%x)\n>data:", d.length, d.length)
    i = 0
    d.each do |e|
      printf("\n>") if i % 16 == 0
      printf(" %02x", e)
      i += 1
    end
    puts('')
  end
end
=begin
>item: RSA modulus n:
>item: RSA public exponent e: 
>item: RSA private exponent d: 
>item: Prime p: 
>item: Prime q: 
>item: p's CRT exponent dP: 
>item: q's CRT exponent dQ: 
>item: CRT coefficient qInv: 
=end

def read_key(f)
  h = Hash.new
  8.times do
    q,id = goto_next_header(f)
    d = read_block(f)    
    m = /[\ \t]([^\ \t]*):[\ \t]*$/.match(id)
    if m
      id = m[1]
    end
    h[id] = d
  end
  req_items = ['n', 'e', 'd', 'p', 'q', 'dP', 'dQ', 'qInv']
  req_items.each do |e|
    printf("ERROR: key component %s is missing!\n", e) if !h[e]
  end
  h.each_key do |e|     
    printf("ERROR: unknown item '%s'!\n", e) if !req_items.index(e)
  end
  return h
end

=begin
>item: Message to be encrypted:
>item: Seed:
>item: Encryption:
=end

def read_tv(f)
  subst_hash = {
    'Message to be encrypted:' => 'msg',
    'Seed:' => 'seed',
    'Encryption:' => 'enc'}
  h = Hash.new
  3.times do
    q,id = goto_next_header(f)
    d = read_block(f)
    n = subst_hash[id]
    printf("ERROR: unknown item '%s'!\n", id) if !n
    h[n] = d
  end  
  req_items = ['msg', 'seed', 'enc']
  req_items.each do |e|
    printf("ERROR: testvector component %s is missing!\n", e) if !h[e]
  end
  while h['enc'][0] == 0
    h['enc'].delete_at(0)
  end 
  return h
end

def load_bigint(d)
  $sp.printf("%d\r", d.length)
  while l = read_line_from_device()
    break if /data:/.match(l)
  end
  printf "ERROR: got no answer from system!" if !l
  d.each do |e|
    $sp.printf(" %02x", e)
  end
end

def load_key(k)
  $sp.print("load-key\r")
  sleep 0.1
  v = ['n', 'e', 'p', 'q', 'dP', 'dQ', 'qInv']  
  v.each do |e|
    load_bigint(k[e])
#    printf("DBG: loaded %s\n", e)
  end 
  while l = read_line_from_device()
    break if />/.match(l)
  end
end

def check_tv(tv)
  sleep 0.1
  $sp.print("seed-test\r")
  sleep 0.1
  load_bigint(tv['msg'])
#  printf("DBG: loaded %s\n", 'msg')
  sleep 0.1
  tv['seed'].each { |e| $sp.printf(" %02x", e) } 
  while l = read_line_from_device() 
    break if /ciphertext:/.match(l)
  end
  test_enc = ''
  loop do 
    l = read_line_from_device()
    break if /decrypting/.match(l)
    test_enc += l if l
  end
  test_enc_a = Array.new
  test_enc = test_enc.split(/[\W\r\n]+/)
  test_enc.each do |e|
    v = e.sub(/[^0-9A-Fa-f]/, '') 
    test_enc_a << v if v.length == 2
  end
  test_enc_a.collect!{ |e| e.to_i(16) }
  enc_ok = (test_enc_a == tv['enc'])
  if !enc_ok
    printf("DBG: ref = %s test = %s\n", tv['enc'].inspect , test_enc_a.inspect)
  end
  m = nil
  loop do 
    l = read_line_from_device() 
    m = /(>>OK<<|ERROR)/.match(l)
    break if m
  end
  return true if enc_ok && (m[1] == '>>OK<<') 
  return false
end

def run_test(f)
  ok = 0
  fail = 0
  skip_file_header(f)
  loop do
    a,b = goto_next_header(f)
#    printf("DBG: a=%s b=%s\n", a.inspect, b.inspect)
    return ok,fail if !b
    if a == :mainblock
# Example 1: A 1024-bit RSA Key Pair
      b.sub!(/[\d]+:/) { |s| sprintf("%3d,", s.to_i)} 
      printf("\n>> %s: ", b)
    #  (35-b.length).times { putc(' ')}
    end
    if a == :subblock
      if b == 'Components of the RSA Key Pair'
        k = read_key(f)
        load_key(k)
      else
        tv = read_tv(f)
        r = check_tv(tv)
        if r
          ok += 1
          putc('*')
        else
          fail += 1
          putc('!')
        end      
      end
    end
  end
end

########################################
# MAIN
########################################


opts = Getopt::Std.getopts("c:f:")

conf = Hash.new
conf = readconfigfile("/etc/testport.conf", conf)
conf = readconfigfile("~/.testport.conf", conf)
conf = readconfigfile("testport.conf", conf)
conf = readconfigfile(opts["c"], conf) if opts["c"]

#puts conf.inspect

puts("serial port interface version: " + SerialPort::VERSION);
$linewidth = 64
params = { "baud"       => conf["PORT"]["baud"].to_i,
            "data_bits" => conf["PORT"]["databits"].to_i,
            "stop_bits" => conf["PORT"]["stopbits"].to_i,
            "parity"    => SerialPort::NONE }
params["paraty"] = SerialPort::ODD   if conf["PORT"]["paraty"].downcase == "odd"
params["paraty"] = SerialPort::EVEN  if conf["PORT"]["paraty"].downcase == "even"
params["paraty"] = SerialPort::MARK  if conf["PORT"]["paraty"].downcase == "mark"
params["paraty"] = SerialPort::SPACE if conf["PORT"]["paraty"].downcase == "space"

puts("\nPort: "+conf["PORT"]["port"]+"@"    +
                params["baud"].to_s      +
                " "                      +
                params["data_bits"].to_s +
                conf["PORT"]["paraty"][0,1].upcase +
                params["stop_bits"].to_s +
                "\n")

$sp = SerialPort.new(conf["PORT"]["port"], params)

$sp.read_timeout=1000; # 5 minutes
$sp.flow_control = SerialPort::SOFT

reset_system()

f = File.open(opts['f'], "r")
exit if !f
ok,fail = run_test(f)
printf("\nOK: %d FAIL: %d :-%s\n",ok,fail, fail==0 ? ')':'(')


