#!/usr/bin/ruby
# threefish_helper_rc.rb
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


def convert(value)
  byteshift = (value+3)/8
  bitshift = value-byteshift*8
#  printf("%d --> %d,%d\n", value,byteshift,bitshift)
  if bitshift<0
    bitshift *= -1
    bitshift += 0x08
  end
  ret = byteshift*16+bitshift
  return ret
end

r00 = [14, 52, 23,  5, 25, 46, 58, 32]
r01 = [16, 57, 40, 37, 33, 12, 22, 32]

r10 = [46, 33, 17, 44, 39, 13, 25,  8]
r11 = [36, 27, 49,  9, 30, 50, 29, 35]
r12 = [19, 14, 36, 54, 34, 10, 39, 56]
r13 = [37, 42, 39, 56, 24, 17, 43, 22]

r20 = [24, 38, 33,  5, 41, 16, 31,  9]
r21 = [13, 19,  4, 20,  9, 34, 44, 48]
r22 = [ 8, 10, 51, 48, 37, 56, 47, 35]
r23 = [47, 55, 13, 41, 31, 51, 46, 52]
r24 = [ 8, 49, 34, 47, 12,  4, 19, 23]
r25 = [17, 18, 41, 28, 47, 53, 42, 31]
r26 = [22, 23, 59, 16, 44, 42, 44, 37]
r27 = [37, 52, 17, 25, 30, 41, 25, 20]

#################################################

printf("threefish256_rc0:  .byte ")
r00.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish256_rc1:  .byte ")
r01.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("\n\n")

#################################################

printf("threefish512_rc0:  .byte ")
r10.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish512_rc1:  .byte ")
r11.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish512_rc2:  .byte ")
r12.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish512_rc3:  .byte ")
r13.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("\n\n")

#################################################

printf("threefish1024_rc0:  .byte ")
r20.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish1024_rc1:  .byte ")
r21.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish1024_rc2:  .byte ")
r22.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish1024_rc3:  .byte ")
r23.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish1024_rc4:  .byte ")
r24.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish1024_rc5:  .byte ")
r25.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish1024_rc6:  .byte ")
r26.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("")

printf("threefish1024_rc7:  .byte ")
r27.each{ |x| printf("0x%2.2x, ",convert(x))}
puts("\n\n")

#################################################

puts("REVERSE")

printf(" uint8_t rc0[8] = { ")
8.times{ |x| printf("%2.2d, ",r00[(7 - x)]) }
puts("")

printf(" uint8_t rc1[8] = { ")
8.times{ |x| printf("%2.2d, ",r01[(7 - x)]) }
puts("")

printf("threefish256_rc0:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r00[(7 - x)]) ) }
puts("")

printf("threefish256_rc1:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r01[(7 - x)]) ) }
puts("\n\n")

#################################################

printf(" uint8_t rc0[8] = { ")
8.times{ |x| printf("%2.2d, ",r10[(7 - x)]) }
puts("")

printf(" uint8_t rc1[8] = { ")
8.times{ |x| printf("%2.2d, ",r11[(7 - x)]) }
puts("")

printf(" uint8_t rc2[8] = { ")
8.times{ |x| printf("%2.2d, ",r12[(7 - x)]) }
puts("")

printf(" uint8_t rc3[8] = { ")
8.times{ |x| printf("%2.2d, ",r13[(7 - x)]) }
puts("")


printf("threefish512_rc0:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r10[(7 - x)]) ) }
puts("")

printf("threefish512_rc1:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r11[(7 - x)]) ) }
puts("")

printf("threefish512_rc2:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r12[(7 - x)]) ) }
puts("")

printf("threefish512_rc3:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r13[(7 - x)]) ) }
puts("\n\n")

#################################################

printf(" uint8_t rc0[8] = { ")
8.times{ |x| printf("%2.2d, ",r20[(7 - x)]) }
puts("")

printf(" uint8_t rc1[8] = { ")
8.times{ |x| printf("%2.2d, ",r21[(7 - x)]) }
puts("")

printf(" uint8_t rc2[8] = { ")
8.times{ |x| printf("%2.2d, ",r22[(7 - x)]) }
puts("")

printf(" uint8_t rc3[8] = { ")
8.times{ |x| printf("%2.2d, ",r23[(7 - x)]) }
puts("")

printf(" uint8_t rc4[8] = { ")
8.times{ |x| printf("%2.2d, ",r24[(7 - x)]) }
puts("")

printf(" uint8_t rc5[8] = { ")
8.times{ |x| printf("%2.2d, ",r25[(7 - x)]) }
puts("")

printf(" uint8_t rc6[8] = { ")
8.times{ |x| printf("%2.2d, ",r26[(7 - x)]) }
puts("")

printf(" uint8_t rc7[8] = { ")
8.times{ |x| printf("%2.2d, ",r27[(7 - x)]) }
puts("")

printf("threefish1024_rc0:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r20[(7 - x)]) ) }
puts("")

printf("threefish1024_rc1:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r21[(7 - x)]) ) }
puts("")

printf("threefish1024_rc2:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r22[(7 - x)]) ) }
puts("")

printf("threefish1024_rc3:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r23[(7 - x)]) ) }
puts("")

printf("threefish1024_rc4:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r24[(7 - x)]) ) }
puts("")

printf("threefish1024_rc5:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r25[(7 - x)]) ) }
puts("")

printf("threefish1024_rc6:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r26[(7 - x)]) ) }
puts("")

printf("threefish1024_rc7:  .byte ")
8.times{ |x| printf("0x%2.2x, ",convert(r27[(7 - x)]) ) }
puts("\n\n")

