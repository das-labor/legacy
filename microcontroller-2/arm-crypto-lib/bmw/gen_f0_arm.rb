# gen_f0_arm.rb 
=begin
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

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
#define S32_1(x) ( (SHR32((x),   1)) ^ \
                 (SHL32((x),   2)) ^ \
                 (ROTL32((x),  8)) ^ \
                 (ROTR32((x),  9)) )

#define S32_2(x) ( (SHR32((x),   2)) ^ \
                 (SHL32((x),   1)) ^ \
                 (ROTL32((x), 12)) ^ \
                 (ROTR32((x),  7)) )

#define S32_3(x) ( (SHR32((x),   2)) ^ \
                 (SHL32((x),   2)) ^ \
                 (ROTL32((x), 15)) ^ \
                 (ROTR32((x),  3)) )

#define S32_4(x) ( (SHR32((x),   1)) ^ (x))

#define S32_5(x) ( (SHR32((x),   2)) ^ (x))

=end
$s32_0_lut = [ 1,  3,  4, 13]
$s32_1_lut = [ 1,  2,  8,  9]
$s32_2_lut = [ 2,  1, 12,  7]
$s32_3_lut = [ 2,  2, 15,  3]

$s32_lut = [$s32_0_lut, $s32_1_lut, $s32_2_lut, $s32_3_lut]

def s32_0(fout, select, reg0, reg1)
  if select<=3
    fout.puts("\tmov %s, %s", reg1, reg0)
    fout.puts("\tlsrs %s, %s, #%d", reg0, reg0, $s32_lut[select][0])
    fout.puts("\teor %s, %s, %s, lsl #%d", reg0, reg0, reg1, $s32_lut[select][1])
    fout.puts("\teor %s, %s, %s, ror #%d", reg0, reg0, reg1, 32-$s32_lut[select][2])
    fout.puts("\teor %s, %s, %s, ror #%d", reg0, reg0, reg1, $s32_lut[select][3])
  else
    fout.puts("\teor %s, %s, %s, ror #%d", reg0, reg0, reg0, 1) if select==4
    fout.puts("\teor %s, %s, %s, ror #%d", reg0, reg0, reg0, 2) if select==5
  end
end

