#!/usr/bin/ruby
# performnce to wiki

=begin
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

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

=begin
 === Twister-256 performance === 
    type:                     hash
    hashsize (bits):           256
    ctxsize (bytes):            80
    blocksize (bits):          512
    init (cycles):             425
    nextBlock (cycles):      36535
    lastBlock (cycles):       8071
    ctx2hash (cycles):       19431
=end

def process_hashfunction(fin, name)
  lb = fin.readline()
  m = lb.match(/hashsize \(bits\):[\s]*([\d]*)/)
  if(!m)
  	printf("unexpected string %s\n", lb)
  end
  hashsize = m[1].to_i()
  lb = fin.readline()
  m = lb.match(/ctxsize \(bytes\):[\s]*([\d]*)/)
  ctxsize = m[1].to_i()
  lb = fin.readline()
  m = lb.match(/blocksize \(bits\):[\s]*([\d]*)/)
  blocksize = m[1].to_i()
  lb = fin.readline()
  m = lb.match(/init \(cycles\):[\s]*([\d]*)/)
  inittime = m[1].to_i()
  lb = fin.readline()
  m = lb.match(/nextBlock \(cycles\):[\s]*([\d]*)/)
  nextblocktime = m[1].to_i()  
  lb = fin.readline()
  m = lb.match(/lastBlock \(cycles\):[\s]*([\d]*)/)
  lastblocktime = m[1].to_i()
  lb = fin.readline()
  m = lb.match(/ctx2hash \(cycles\):[\s]*([\d]*)/)
  convtime = m[1].to_i()
  
  printf("| %20s || %3s || %3s || || %4d || || %4d || %4d ||" +
         " %6d || %6d || %7.2f || %6d || || || \n|-\n" , 
        name, $lang, $lang ,ctxsize, hashsize, blocksize, 
	    inittime, nextblocktime, nextblocktime.to_f/blocksize*8,
		lastblocktime+convtime)
end


$handlers = Hash.new
$handlers.default = 0
$handlers["hashfunction"] = 1 #process_hashfunction

def process_file(fname)
  fin = File.open(fname, "r")
  $lang = "asm"
  $lang = "C" if fname.match(/_c.txt$/)
  begin
    begin
	  if fin.eof()
	  	return
	  end
      lb = fin.readline()
    end while !m=lb.match(/=== (.*) performance ===/)
    name = m[1];
    lb = fin.readline()
    m = lb.match(/type:[\s]*([\w]*)/)
    type = m[1]
    if $handlers[type] != 0
    #  handlers[type](fin, name)
      process_hashfunction(fin, name)
    else
      printf("ERROR: unsupported type: %s !\n", type)
    end	
  end while(true)
  fin.close()
end

for i in (0..ARGV.size-1)
  process_file(ARGV[i])
end







