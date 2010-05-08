#!/usr/bin/ruby
# bigint_test.rb
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

$debug = true
$debug = false
require 'rubygems'
require 'serialport'
require 'getopt/std'
require 'ftools'
require 'date'
$buffer_size = 0
$conffile_check = Hash.new
$conffile_check.default = 0

def karatsuba_verbose(a,b)
  a_s = a.to_s(16)
  b_s = b.to_s(16)
  len_a = floor((a_s.length+1)/2)
  len_b = floor((b_s.length+1)/2)
  n=floor((((len_a>len_b)?len_a:len_b)+1)/2)
  
end