#!/usr/bin/ruby 
# create-algo-impl-relation.rb
=begin
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2008, 2009  Daniel Otte (daniel.otte@rub.de)

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

SPEED_DIR = 'speed_log/'
SIZE_DIR  = 'size_log/'
OUT_DIR = 'algo_implementation/'

$debug = false
require 'rubygems'
require 'getopt/std'
require 'ftools'

def get_module_names(finname)
  ret = Array.new
  f = File.open(finname, 'r')
  f.gets # first line is ignored
  while(l=f.gets())
    if m=l.match(/[\s]([^\s]*)$/)
      ret << m[1]
    end
  end
  f.close()
  return ret
end


def mkalgoimplrelation_file(foutname, finname, algos)
  if algos==nil
    puts "ERROR: algos==nil! fout=#{foutname} fin=#{finname}"
    return
  end
  if File.exists?(foutname)
    puts "File #{foutname} already exists!"
    return
  end
  modules = get_module_names(finname).join(' ')
  f = File.open(foutname, 'w')
  algos.each do |algo|
    f.puts(algo+': '+modules)
  end
  f.close()
end


if not File.directory?(SPEED_DIR) 
  puts "ERROR: #{SPEED_DIR} is no directory!"
  return -1
end

if not File.directory?(SIZE_DIR) 
  puts "ERROR: #{SIZE_DIR} is no directory!"
  return -1
end

if not File.directory?(OUT_DIR) 
  puts "ERROR: #{OUT_DIR} is no directory!"
  return -1
end

list = Dir.entries(SPEED_DIR)
algo_list = Hash.new
list.each do |entry|
  if m=entry.match(/([^.]*)\.([^.]*)\.txt/)
    algo_list[m[2]] = Array.new if algo_list[m[2]]==nil
    algo_list[m[2]] << m[1]
  end
end

list = Dir.entries(SIZE_DIR)
list.each do |entry|
  if m=entry.match(/([^.]*)\.size/)
    mkalgoimplrelation_file(OUT_DIR+m[1]+'.algos', SIZE_DIR+entry, algo_list[m[1]])
  end
end


