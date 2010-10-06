# autogen f1 function for BMW

header = <<EOF
/* BEGIN of automatic generated code */

static inline
void bmw_large_f1(uint64_t* q, const void* m, const void* h){ 
  uint64_t even, odd;
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
  s += sprintf("\t\t((  ROTL64(((uint64_t*)m)[%2d], %d) \n", j%16,      (j%16)+1)
  s += sprintf("\t\t  + ROTL64(((uint64_t*)m)[%2d], %d) \n", (j+ 3)%16, ((j+ 3)%16)+1)
  s += sprintf("\t\t  - ROTL64(((uint64_t*)m)[%2d], %d) \n", (j+10)%16, ((j+10)%16)+1)
  s += sprintf("\t\t  + 0x%016xULL \n", 0x0555_5555_5555_5555*(16+j))
  s += sprintf("\t\t )^ ((uint64_t*)h)[%2d] \n", (j+7)%16)
  s += sprintf("\t\t)");
  (0..15).each do |x|
    s += (x%4==0)?"\n\t\t":" "
    s += sprintf("+ S64_%d(q[%2d])", (x+1)%4, x+j)
  end
  s += ';'
  return s
end


def expand_2(j, start)
  s = sprintf("/* expand_2(%2d) */\n", j)
  s += sprintf("\tq[%2d] = \n", j+16)
  s += sprintf("\t\t((  ROTL64(((uint64_t*)m)[%2d], %d) \n", j%16,      (j%16)+1)
  s += sprintf("\t\t  + ROTL64(((uint64_t*)m)[%2d], %d) \n", (j+ 3)%16, ((j+ 3)%16)+1)
  s += sprintf("\t\t  - ROTL64(((uint64_t*)m)[%2d], %d) \n", (j+10)%16, ((j+10)%16)+1)
  s += sprintf("\t\t  + 0x%016xULL \n", 0x0555_5555_5555_5555*(16+j))
  s += sprintf("\t\t )^ ((uint64_t*)h)[%2d] \n", (j+7)%16)
  s += sprintf("\t\t)");
  if(j-1<=start) 
    if(j%2==0)
      # even
      s += sprintf("\n\t\t + ( even =  q[%2d] + q[%2d] + q[%2d]",j,j+2,j+4)
      s += sprintf("\n\t\t           + q[%2d] + q[%2d] + q[%2d] + q[%2d] )",j+6,j+8,j+10,j+12)
      s += sprintf("\n\t\t + R64_1(q[%2d]) + R64_2(q[%2d]) + R64_3(q[%2d])",j+1,j+3,j+5)
      s += sprintf("\n\t\t + R64_4(q[%2d]) + R64_5(q[%2d]) + R64_6(q[%2d])",j+7,j+9,j+11)
      s += sprintf("\n\t\t + R64_7(q[%2d]) + S64_4(q[%2d]) + S64_5(q[%2d])",j+13,j+14,j+15)
    else
      # odd
      s += sprintf("\n\t\t + ( odd  =  q[%2d] + q[%2d] + q[%2d]",j,j+2,j+4)
      s += sprintf("\n\t\t           + q[%2d] + q[%2d] + q[%2d] + q[%2d] )",j+6,j+8,j+10,j+12)
      s += sprintf("\n\t\t + R64_1(q[%2d]) + R64_2(q[%2d]) + R64_3(q[%2d])",j+1,j+3,j+5)
      s += sprintf("\n\t\t + R64_4(q[%2d]) + R64_5(q[%2d]) + R64_6(q[%2d])",j+7,j+9,j+11)
      s += sprintf("\n\t\t + R64_7(q[%2d]) + S64_4(q[%2d]) + S64_5(q[%2d])",j+13,j+14,j+15)
    end 
  else 
    s += sprintf("\n\t\t + ( %s  +=  q[%2d] - q[%2d] )",(j%2==0)?"even":"odd ",j+12,j-2)    
    s += sprintf("\n\t\t + R64_1(q[%2d]) + R64_2(q[%2d]) + R64_3(q[%2d])",j+1,j+3,j+5)
    s += sprintf("\n\t\t + R64_4(q[%2d]) + R64_5(q[%2d]) + R64_6(q[%2d])",j+7,j+9,j+11)
    s += sprintf("\n\t\t + R64_7(q[%2d]) + S64_4(q[%2d]) + S64_5(q[%2d])",j+13,j+14,j+15)  
  end 
  s += ';'
  return s
end


puts header
(0..1).each  {|x| puts expand_1(x)}
(2..15).each {|x| puts expand_2(x, 2)}
puts footer