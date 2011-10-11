# autogen_f0_asm.rb 
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
  tmp = +t[ 5] +t[10] +t[13] +(tr1=-t[ 7]+t[14]) ;
  q[ 0] = S32_0(tmp) + h[ 1];
  tmp = +t[ 8] +t[13] +t[ 0] -(tr2=+t[ 1]+t[10]) ;
  q[ 3] = S32_3(tmp) + h[ 4];
  tmp = -t[11] +t[13] -t[ 0] -t[ 3] +t[ 4] ;
  q[ 6] = S32_1(tmp) + h[ 7];
  tmp = +t[ 0] +(tr0=-t[ 3]+t[ 6]) +(tr1) ;
  q[ 9] = S32_4(tmp) + h[10];
  tmp = -t[ 9] -(tr0) +(tr2) ;
  q[12] = S32_2(tmp) + h[13];
  tmp = -t[ 4] +(tr0=-t[ 9]+t[12]) +(tr1=-t[ 6]+t[13]) ;
  q[15] = S32_0(tmp) + h[ 0];
  tmp = +t[ 7] +t[15] +t[ 0] -(tr0) ;
  q[ 2] = S32_2(tmp) + h[ 3];
  tmp = +t[10] +(tr0=-t[ 2]+t[15]) +(tr2=+t[ 3]-t[12]) ;
  q[ 5] = S32_0(tmp) + h[ 6];
  tmp = -t[ 5] -(tr0) +(tr1) ;
  q[ 8] = S32_3(tmp) + h[ 9];
  tmp = -t[ 0] -t[ 2] +t[ 9] +(tr0=-t[ 5]+t[ 8]) ;
  q[11] = S32_1(tmp) + h[12];
  tmp = -t[11] +(tr0) +(tr2) ;
  q[14] = S32_4(tmp) + h[15];
  tmp = +t[ 6] +(tr0=+t[11]+t[14]) -(tr1=+t[ 8]+t[15]) ;
  q[ 1] = S32_1(tmp) + h[ 2];
  tmp = +t[ 9] +t[ 1] +t[ 2] -(tr0) ;
  q[ 4] = S32_4(tmp) + h[ 5];
  tmp = -t[12] -t[14] +t[ 1] -t[ 4] -t[ 5] ;
  q[ 7] = S32_2(tmp) + h[ 8];
  tmp = -t[ 1] -(tr0=+t[ 4]+t[ 7]) +(tr1) ;
  q[10] = S32_0(tmp) + h[11];
  tmp = +t[ 2] +t[10] +t[11] +(tr0) ;
  q[13] = S32_3(tmp) + h[14];
=end
$c_code = <<EOF
  tmp = +t[ 5] +t[10] +t[13] +(tr1=-t[ 7]+t[14]) ;
  q[ 0] = S32_0(tmp) + h[ 1];
  tmp = +t[ 8] +t[13] +t[ 0] -(tr2=+t[ 1]+t[10]) ;
  q[ 3] = S32_3(tmp) + h[ 4];
  tmp = -t[11] +t[13] -t[ 0] -t[ 3] +t[ 4] ;
  q[ 6] = S32_1(tmp) + h[ 7];
  tmp = +t[ 0] +(tr0=-t[ 3]+t[ 6]) +(tr1) ;
  q[ 9] = S32_4(tmp) + h[10];
  tmp = -t[ 9] -(tr0) +(tr2) ;
  q[12] = S32_2(tmp) + h[13];
  tmp = -t[ 4] +(tr0=-t[ 9]+t[12]) +(tr1=-t[ 6]+t[13]) ;
  q[15] = S32_0(tmp) + h[ 0];
  tmp = +t[ 7] +t[15] +t[ 0] -(tr0) ;
  q[ 2] = S32_2(tmp) + h[ 3];
  tmp = +t[10] +(tr0=-t[ 2]+t[15]) +(tr2=+t[ 3]-t[12]) ;
  q[ 5] = S32_0(tmp) + h[ 6];
  tmp = -t[ 5] -(tr0) +(tr1) ;
  q[ 8] = S32_3(tmp) + h[ 9];
  tmp = -t[ 0] -t[ 2] +t[ 9] +(tr0=-t[ 5]+t[ 8]) ;
  q[11] = S32_1(tmp) + h[12];
  tmp = -t[11] +(tr0) +(tr2) ;
  q[14] = S32_4(tmp) + h[15];
  tmp = +t[ 6] +(tr0=+t[11]+t[14]) -(tr1=+t[ 8]+t[15]) ;
  q[ 1] = S32_1(tmp) + h[ 2];
  tmp = +t[ 9] +t[ 1] +t[ 2] -(tr0) ;
  q[ 4] = S32_4(tmp) + h[ 5];
  tmp = -t[12] -t[14] +t[ 1] -t[ 4] -t[ 5] ;
  q[ 7] = S32_2(tmp) + h[ 8];
  tmp = -t[ 1] -(tr0=+t[ 4]+t[ 7]) +(tr1) ;
  q[10] = S32_0(tmp) + h[11];
  tmp = +t[ 2] +t[10] +t[11] +(tr0) ;
  q[13] = S32_3(tmp) + h[14];
EOF

$registers = ["r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14"].reverse

class Array
  def chopf(n=1)
    return self[n..-1]
  end
end

class String
  def chopf(n=1)
    return self[n..-1]
  end
  
  def is_numeric?(b=10)
    n=self.to_i(b)
    s=n.to_s(b)
    (self==s) or (self==('+'+s))
  end
  
  def xtr
    return self.to_i if self.is_numeric?
    self
  end
end


class Operation
  attr_reader :read_t
  attr_reader :read_tr
  attr_reader :write_tr0, :write_tr1, :write_tr2 
  attr_reader :index, :s, :h

  def init
    @read_t = Array.new
    @read_tr = Array.new
    @write_tr0 = Array.new
    @write_tr1 = Array.new
    @write_tr2 = Array.new
    @index = -1
    @s = -1
    @h = -1
  end
  
  def parse(line)
    s = line
    while m = /([+-])t\[[\s]*([\d]*)\](.*)/.match(s)
      @read_t << m[1]+m[2]
      s = m[3]
    end
    s = line
    while m = /([+-])\(tr([012])(.*)/.match(s)
      @read_tr << m[1]+m[2]
      s = m[3]
    end
    s = line
    while m = /tr0=([+-])t\[[\s]*([\d]*)\][\s]*([+-])t\[[\s]*([\d]*)\](.*)/.match(s)
      @write_tr0 << m[1]+m[2]
      @write_tr0 << m[3]+m[4]
      s = m[5]
    end
    s = line
    while m = /tr1=([+-])t\[[\s]*([\d]*)\][\s]*([+-])t\[[\s]*([\d]*)\](.*)/.match(s)
      @write_tr1 << m[1]+m[2]
      @write_tr1 << m[3]+m[4]
      s = m[5]
    end
    s = line
    while m = /tr2=([+-])t\[[\s]*([\d]*)\][\s]*([+-])t\[[\s]*([\d]*)\](.*)/.match(s)
      @write_tr2 << m[1]+m[2]
      @write_tr2 << m[3]+m[4]
      s = m[5]
    end
    if m=/q\[[\s]*([\d]*)\]/m.match(line)
      @index = m[1].to_i
    end
    if m=/S32_([0-4])\(tmp\)/m.match(line)
      @s = m[1].to_i
    end
    if m=/h\[[\s]*([\d]*)\]/m.match(line)
      @h = m[1].to_i
    end
  end
  
end


$operations = Array.new

def parse_c_code
  i=0
  s = ''
  $c_code.each_line do |line|
#    puts "DBG: line=", line
    if /^[\s]*tmp/.match(line)
      s = line
    end
    if /^[\s]*q\[[\s\d\]]/.match(line)
      s += line 
      $operations[i] = Operation.new
      $operations[i].init
      $operations[i].parse(s)
      i+=1
    end
  end
end

class Array
  def getsmallest(i=0)
    tmp = self.sort
    tmp.each {|x| return x if x>i}
    return nil
  end
  
  def getlargestindex
    return self.index(nil) if self.index(nil)
    tmp = self.sort
    return self.index(tmp[-1])
  end
end

def find_register_to_free(registermap, regusemap, step)
  if i=registermap.index(nil)
    return i
  end
  tmp = Array.new
  registermap.each do |x|
    if x.class==Fixnum and regusemap[x.abs]
      t = regusemap[x.abs].getsmallest(step)
      tmp << t
    else
      tmp << -1
    end
  end
  return tmp.getlargestindex
end


def load_registers(regmap, stack, op, step)
  asm_out = ''
  to_load2 = Array.new
  push_list = Array.new
  # set to_load2 to all registers which are not already loaded
#  puts "DBG(a): "+regmap.inspect
  op.read_t.each do |x|
    x = x.to_i.abs
    to_load2 << x if regmap.index(x)==nil
  end
  to_load2.each do 
    regmap[find_register_to_free(regmap, $regusemap, step)] = 'x'
  end
  if op.write_tr0.length!=0
    regmap[find_register_to_free(regmap, $regusemap, step)] = 'tr0'
  end
  if op.write_tr1.length!=0
    regmap[find_register_to_free(regmap, $regusemap, step)] = 'tr1'
  end
  if op.write_tr2.length!=0
    regmap[find_register_to_free(regmap, $regusemap, step)] = 'tr2'
  end
  to_load2.sort!
=begin
  to_load2.length.times do |i|
    x = to_load2[i]
    y = to_load2[i+1]
    if x and y and (to_load2[i]+1 == to_load2[i+1]) and stack.index(x) and stack.index(y)
      rx = regmap.index('x')
      regmap[rx]=x.to_s
      ry = regmap.index('x')
      regmap[ry]=y.to_s
      asm_out += sprintf("  ldrd %s, %s, [SP, #%d*4]\n", $registers[rx], $registers[ry], x)
      to_load2[i] = nil
      to_load2[i+1] = nil
    end
  end
=end
  to_load2.delete(nil)
  to_load2.each do |x|
    next if not x
    y = regmap.index('x')
    puts "Strange error!\n" if not y
    regmap[y]=x
    if stack.index(x)
      asm_out += sprintf("  ldr %s, [SP, #%d*4]\n", $registers[y], x)
    else
      asm_out += sprintf("  ldr %s, [r1, #%d*4]\n", $registers[y], x)
      asm_out += sprintf("  ldr %s, [r2, #%d*4]\n", $registers[-1], x)
      asm_out += sprintf("  eor %s, %s\n", $registers[y], $registers[-1])
      asm_out += sprintf("  str %s, [SP, #%d*4]\n", $registers[y], x)
      stack << x
#      push_list << [$registers[y], x]
    end
  end
#  if push_list.length!=0
#    push_list.sort!{ |x,y| x[0].chopf.to_i <=> y[0].chopf.to_i}
#    push_regs = push_list.collect {|x| x[0]}
#    push_list.reverse.each {|x| stack << x[1]}
#    asm_out += sprintf("  stmdb SP, {%s}\n", push_regs.join(', '))
#  end
#  puts asm_out
#  puts "DBG(0): "+regmap.inspect
#  puts "DBG(1): "+to_load.inspect
#  puts "DBG(2): "+to_load2.inspect

  #puts 'DBG('+__LINE__.to_s+'): regmap = '+regmap.inspect
  return regmap, stack, asm_out
end

def gen_simple_assembler(operations)
  asm_out=''
  stack = Array.new
  accu = $registers.length-1
#  outr = $registers.length-4
#  tr0 =  $registers.length-3
#  tr1 =  $registers.length-2
#  tr2 =  $registers.length-4
  
  reg_cnt = $registers.length-1
  regmap = Array.new(reg_cnt)
  reg_idx=0
  step = 0
  asm_out += sprintf("  sub SP, #%d*4\n", 16)
  operations.each do |op|
    asm_out += sprintf("/*=== W[%2d] ===*/\n", op.index)
    regmap, stack, tstr = load_registers(regmap, stack, op, step-1)
    asm_out += tstr
    step += 1
    reg_hash = Hash.new
    op.read_t.each do |t|
      if regmap.index(t.chopf.to_i)==nil
        printf("ERROR: too few registers!\n")
      end
      reg_hash[t.chopf.to_i]=regmap.index(t.chopf.to_i)
    end
    if op.write_tr0.length==2
      signs_code=op.write_tr0[0][0..0]+op.write_tr0[1][0..0]
      case signs_code
        when "++"
          asm_out += sprintf("  add %s, %s, %s\n", $registers[regmap.index('tr0')], \
            $registers[reg_hash[op.write_tr0[0].chopf.to_i]], \
            $registers[reg_hash[op.write_tr0[1].chopf.to_i]])  
        when "+-"
          asm_out += sprintf("  sub %s, %s, %s\n", $registers[regmap.index('tr0')], \
            $registers[reg_hash[op.write_tr0[0].chopf.to_i]], \
            $registers[reg_hash[op.write_tr0[1].chopf.to_i]])  
        when "-+"
          asm_out += sprintf("  sub %s, %s, %s\n", $registers[regmap.index('tr0')], \
            $registers[reg_hash[op.write_tr0[1].chopf.to_i]], \
            $registers[reg_hash[op.write_tr0[0].chopf.to_i]])  
        else
          printf("ERROR: invalid signs_code (%d): %s\n", __LINE__, signs_code)
          puts op.inspect
      end
    end
    if op.write_tr1.length==2
      signs_code=op.write_tr1[0][0..0]+op.write_tr1[1][0..0]
      case signs_code
        when "++"
          asm_out += sprintf("  add %s, %s, %s\n", $registers[regmap.index('tr1')], \
            $registers[reg_hash[op.write_tr1[0].chopf.to_i]], \
            $registers[reg_hash[op.write_tr1[1].chopf.to_i]])  
        when "+-"
          asm_out += sprintf("  sub %s, %s, %s\n", $registers[regmap.index('tr1')], \
            $registers[reg_hash[op.write_tr1[0].chopf.to_i]], \
            $registers[reg_hash[op.write_tr1[1].chopf.to_i]])  
        when "-+"
  #        puts 'DBG: '+reg_hash.inspect
          asm_out += sprintf("  sub %s, %s, %s\n", $registers[regmap.index('tr1')], \
            $registers[reg_hash[op.write_tr1[1].chopf.to_i]], \
            $registers[reg_hash[op.write_tr1[0].chopf.to_i]])  
        else
          printf("ERROR: invalid signs_code (%d): %s\n", __LINE__, signs_code)
          puts op.inspect
      end
    end
    if op.write_tr2.length==2
      signs_code=op.write_tr2[0][0..0]+op.write_tr2[1][0..0]
      case signs_code
        when "++"
          asm_out += sprintf("  add %s, %s, %s\n", $registers[regmap.index('tr2')], \
            $registers[reg_hash[op.write_tr2[0].chopf.to_i]], \
            $registers[reg_hash[op.write_tr2[1].chopf.to_i]])  
        when "+-"
          asm_out += sprintf("  sub %s, %s, %s\n", $registers[regmap.index('tr2')], \
            $registers[reg_hash[op.write_tr2[0].chopf.to_i]], \
            $registers[reg_hash[op.write_tr2[1].chopf.to_i]])  
        when "-+"
          asm_out += sprintf("  sub %s, %s, %s\n", $registers[regmap.index('tr2')], \
            $registers[reg_hash[op.write_tr2[1].chopf.to_i]], \
            $registers[reg_hash[op.write_tr2[0].chopf.to_i]])  
        else
          printf("ERROR: invalid signs_code (%d): %s\n", __LINE__, signs_code)
          puts op.inspect
      end
    end
    reg_hash['0tr'] = regmap.index('tr0')
    reg_hash['1tr'] = regmap.index('tr1')
    reg_hash['2tr'] = regmap.index('tr2')
    tr_to_delete = op.read_tr.collect { |x| x.chopf}
    tr_to_delete.delete('0') if op.write_tr0.length!=0
    tr_to_delete.delete('1') if op.write_tr1.length!=0
    tr_to_delete.delete('2') if op.write_tr2.length!=0
    tr_to_delete.each do |x|
      y = regmap.index('tr'+x)
      regmap[y]=nil if y
 #     puts 'DBG('+__LINE__.to_s+') deleted tr'+x+' @ '+y.to_s
    end
    operations_to_do = op.read_t
    op.read_tr.each {|x| operations_to_do << x+'tr'}
    op.write_tr0.each {|x| operations_to_do.delete(x)}
    op.write_tr1.each {|x| operations_to_do.delete(x)}
    op.write_tr2.each {|x| operations_to_do.delete(x)}
    operations_to_do = operations_to_do.sort
    asm_out += sprintf("/*(-- should do %s --)*/\n", operations_to_do.join(', '));
    sign_code=operations_to_do[1][0..0]
    case sign_code
      when '+'
   #     puts 'DBG('+__LINE__.to_s+'): x='+operations_to_do[0]+' reg_hash='+reg_hash.inspect
        asm_out += sprintf("  add %s, %s, %s\n", $registers[accu], \
          $registers[reg_hash[operations_to_do[0].chopf.xtr]], \
          $registers[reg_hash[operations_to_do[1].chopf.xtr]])           
      when '-'
    #    puts 'DBG('+__LINE__.to_s+'): x='+x+' reg_hash='+reg_hash.inspect
        asm_out += sprintf("  sub %s, %s, %s\n", $registers[accu], \
          $registers[reg_hash[operations_to_do[0].chopf.xtr]], \
          $registers[reg_hash[operations_to_do[1].chopf.xtr]])
    end
    operations_to_do = operations_to_do[2..-1]
    operations_to_do.each do |x|
      sign_code=x[0..0]
      case sign_code
        when '+'
 #         puts 'DBG('+__LINE__.to_s+'): x='+x+' reg_hash='+reg_hash.inspect
          asm_out += sprintf("  add %s, %s\n", $registers[accu], \
            $registers[reg_hash[x.chopf.xtr]])           
        when '-'
          asm_out += sprintf("  sub %s, %s\n", $registers[accu], \
            $registers[reg_hash[x.chopf.xtr]])
      end      
    end
    outr = find_register_to_free(regmap, $regusemap, step)
    regmap[outr]=nil
    if(op.s==4)
      asm_out += sprintf("  S32_4 %s\n", $registers[accu])
      asm_out += sprintf("  ldr %s, [r1, #%d*4]\n", $registers[outr], op.h)
      asm_out += sprintf("  add %s, %s\n", $registers[accu], $registers[outr])
      asm_out += sprintf("  str %s, [r0, #%d*4]\n", $registers[accu], op.index)
    else
      asm_out += sprintf("  S32_%d %s %s\n", op.s, $registers[outr], $registers[accu])
      asm_out += sprintf("  ldr %s, [r1, #%d*4]\n", $registers[accu], op.h)      
      asm_out += sprintf("  add %s, %s\n", $registers[outr], $registers[accu])
      asm_out += sprintf("  str %s, [r0, #%d*4]\n", $registers[outr], op.index)
    end

#  asm_out += sprintf("  str %s, [r0, #%d*4]\n", $registers[accu], op.index)    
  end
  asm_out += sprintf("  add SP, #%d*4", 16)
  return asm_out
end

class RegMapEntry
  attr_accessor :usemap
  attr_accessor :nextusemap
end
=begin
class RegMap
  atrr_reader :steps
  atrr_reader :entrys
  attr_reader :regcnt
end

def gen_regmap_simple
  
end
=end
$regusemap = Array.new

def build_regusemap(operations)
  i=0
  operations.each do |op|
    op.read_t.each do |t|
      x = t.chopf.to_i
      if $regusemap[x]==nil
        $regusemap[x]=Array.new
      end
      $regusemap[x]<<i
    end
    i += 1
  end
end

#-------------------------------------------------------------------------------
# MAIN
#-------------------------------------------------------------------------------

parse_c_code
#puts $operations.inspect
build_regusemap($operations)
#puts $regusemap.inspect
puts gen_simple_assembler($operations)

