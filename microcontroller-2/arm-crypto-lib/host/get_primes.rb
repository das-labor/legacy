#!/usr/bin/ruby 
# get_primes.rb
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

$primes = [2,3,5]

def check_prime(a)
  q = Math.sqrt(a)
  $primes.each{ |p|
    return true if p>q 
	return false if a%p==0
  }
  return true
end

def find_primes(n)
  a = $primes.last+2
  while $primes.size < n
    if check_prime(a)
	  $primes << a
	end  
	a += 2
  end
end

if ARGV.size!=1
  STDERR.print <<EOF
  Usage: ruby #{$0} n
   
EOF
  exit(1)
end

find_primes(ARGV[0].to_i);
print($primes.join(', '))
puts("")
