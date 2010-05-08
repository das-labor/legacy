#!/usr/bin/ruby 
# shavs_test.rb
=begin
    This file is part of the AVR-Crypto-Lib.
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

require 'find'

def read_record(file)
  record = Hash.new
  lb = file.gets() until /[\s]*Set[\s]*[\d]+[\s]*,[\s]*vector#[\s]*[\d]+[\s]*:[\s]*/.match(lb)
  term=false
  lastkey=nil
  begin
    term = true
    lb=file.gets();
	if m=/[\s]*([^=]+)[\s]*=[\s]*(.*)/.match(lb)
	  lastkey=m[1]
	  record[m[1]] = m[2]
	  term = false
	else
	  if m=/[\s]*([0-9a-fA-F]+)[\s]*/.match(lb) and lastkey!=nil
	    record[lastkey] += m[1]
		term = false
	  end
	end
  end until term
  record
end

def check_match(fname1, fname2)
  r1 = Hash.new
  r2 = Hash.new
  if File.directory?(fname1)
    return false
  end
  if File.directory?(fname2)
    return false
  end
  file1 = File.new(fname1, "r");
  file2 = File.new(fname2, "r");
  r1 = read_record(file1)
  r2 = read_record(file2)
  return r1==r2
end

def get_params_from_fn(fname)
  params = Array.new
  if not p = /[^\.]*\.(([\d]+)\.*).*/.match(fname)
    return params
  end
  params = p[1].split(/\./)
  return params
end

def get_params_from_fn2(fname)
  params = Array.new
  if not p = /[^\.]*\.((-([\d]+))*\.).*/.match(fname)
    return params
  end
  params = p[1].split(/\./)
  return params
end

def find_files(fname1, afnames)
  files = Array.new
  params = Array.new
  params = get_params_from_fn(fname1)
  afnames.each{ |fn|
    p = get_params_from_fn2(fn)
  	puts("#{params} ?=  #{p}")
    if params.eql?(p)
	  files << fn
	end
  }
  return files
end


if ARGV.size<2 or File.directory?(ARGV[0]) or (File.directory?(ARGV[1])==false)
  STDERR.print <<EOF
  Usage: ruby #{$0} file directory [formatstring]
   
EOF
  exit(1)
end

fmt=String.new
if ARGV.size>=3
  fmt=ARGV[2]
else
  fmt = "%s --> %s\n"
end
files = Array.new
files = Dir.entries(ARGV[1])
files.each{ |fn|
  if check_match(ARGV[0], ARGV[1]+fn) 
	printf(fmt, ARGV[0].to_s, ARGV[1]+fn.to_s)
	puts ""
#	puts("#{ARGV[0]} --> #{ARGV[1]+fn}")
  end
}

