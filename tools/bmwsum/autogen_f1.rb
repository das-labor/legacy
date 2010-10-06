# autogen f1 function for BMW
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

header = <<EOF
/* BEGIN of automatic generated code */

static inline
void bmw_small_f1(uint32_t* q, const void* m, const void* h){ 
  uint32_t even, odd;
EOF

footer = <<EOF
}

/* END of automatic generated code */

EOF

=begin
  uint32_t r;
  /* r = 0x05555555*(j+16); */
  r = (   ROTL32(((uint32_t*)m)[j],      ((j+0))+1  )
         + ROTL32(((uint32_t*)m)[(j+3)],  ((j+3))+1  )
         - ROTL32(((uint32_t*)m)[(j+10)], ((j+10))+1 )
         + k_lut[j]
       ) ^ ((uint32_t*)h)[(j+7)];
  r += S32_1(q[j+ 0]) + S32_2(q[j+ 1]) + S32_3(q[j+ 2]) + S32_0(q[j+ 3]) +
     S32_1(q[j+ 4]) + S32_2(q[j+ 5]) + S32_3(q[j+ 6]) + S32_0(q[j+ 7]) +
     S32_1(q[j+ 8]) + S32_2(q[j+ 9]) + S32_3(q[j+10]) + S32_0(q[j+11]) +
     S32_1(q[j+12]) + S32_2(q[j+13]) + S32_3(q[j+14]) + S32_0(q[j+15]);
=end

def expand_1(j)
  s = sprintf("/* expand_1(%2d) */\n", j)
  s += sprintf("\tq[%2d] = \n", j+16)
  s += sprintf("\t\t((  ROTL32(((uint32_t*)m)[%2d], %d) \n", j%16,      (j%16)+1)
  s += sprintf("\t\t  + ROTL32(((uint32_t*)m)[%2d], %d) \n", (j+ 3)%16, ((j+ 3)%16)+1)
  s += sprintf("\t\t  - ROTL32(((uint32_t*)m)[%2d], %d) \n", (j+10)%16, ((j+10)%16)+1)
  s += sprintf("\t\t  + 0x%08xUL \n", 0x0555_5555*(16+j))
  s += sprintf("\t\t )^ ((uint32_t*)h)[%2d] \n", (j+7)%16)
  s += sprintf("\t\t)");
  (0..15).each do |x|
    s += (x%4==0)?"\n\t\t":" "
    s += sprintf("+ S32_%d(q[%2d])", (x+1)%4, x+j)
  end
  s += ';'
  return s
end


def expand_2(j,start)
  s = sprintf("/* expand_2(%2d) */\n", j)
  s += sprintf("\tq[%2d] = \n", j+16)
  s += sprintf("\t\t((  ROTL32(((uint32_t*)m)[%2d], %d) \n", j%16,      (j%16)+1)
  s += sprintf("\t\t  + ROTL32(((uint32_t*)m)[%2d], %d) \n", (j+ 3)%16, ((j+ 3)%16)+1)
  s += sprintf("\t\t  - ROTL32(((uint32_t*)m)[%2d], %d) \n", (j+10)%16, ((j+10)%16)+1)
  s += sprintf("\t\t  + 0x%08xUL \n", 0x0555_5555*(16+j))
  s += sprintf("\t\t )^ ((uint32_t*)h)[%2d] \n", (j+7)%16)
  s += sprintf("\t\t)")
  if(j-1<=start) 
    if(j%2==0)
      # even
      s += sprintf("\n\t\t + ( even =  q[%2d] + q[%2d] + q[%2d]",j,j+2,j+4)
      s += sprintf("\n\t\t           + q[%2d] + q[%2d] + q[%2d] + q[%2d] )",j+6,j+8,j+10,j+12)
      s += sprintf("\n\t\t + R32_1(q[%2d]) + R32_2(q[%2d]) + R32_3(q[%2d])",j+1,j+3,j+5)
      s += sprintf("\n\t\t + R32_4(q[%2d]) + R32_5(q[%2d]) + R32_6(q[%2d])",j+7,j+9,j+11)
      s += sprintf("\n\t\t + R32_7(q[%2d]) + S32_4(q[%2d]) + S32_5(q[%2d])",j+13,j+14,j+15)
    else
      # odd
      s += sprintf("\n\t\t + ( odd  =  q[%2d] + q[%2d] + q[%2d]",j,j+2,j+4)
      s += sprintf("\n\t\t           + q[%2d] + q[%2d] + q[%2d] + q[%2d] )",j+6,j+8,j+10,j+12)
      s += sprintf("\n\t\t + R32_1(q[%2d]) + R32_2(q[%2d]) + R32_3(q[%2d])",j+1,j+3,j+5)
      s += sprintf("\n\t\t + R32_4(q[%2d]) + R32_5(q[%2d]) + R32_6(q[%2d])",j+7,j+9,j+11)
      s += sprintf("\n\t\t + R32_7(q[%2d]) + S32_4(q[%2d]) + S32_5(q[%2d])",j+13,j+14,j+15)
    end 
  else 
    s += sprintf("\n\t\t + ( %s  +=  q[%2d] - q[%2d] )",(j%2==0)?"even":"odd ",j+12,j-2)    
    s += sprintf("\n\t\t + R32_1(q[%2d]) + R32_2(q[%2d]) + R32_3(q[%2d])",j+1,j+3,j+5)
    s += sprintf("\n\t\t + R32_4(q[%2d]) + R32_5(q[%2d]) + R32_6(q[%2d])",j+7,j+9,j+11)
    s += sprintf("\n\t\t + R32_7(q[%2d]) + S32_4(q[%2d]) + S32_5(q[%2d])",j+13,j+14,j+15)  
  end 
  s += ';'
  return s
end


puts header
(0..1).each  {|x| puts expand_1(x)}
(2..15).each {|x| puts expand_2(x, 2)}
puts footer