# analyze_f0.rb 
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
  q[ 0] = (+ h[ 5] - h[ 7] + h[10] + h[13] + h[14]);
  q[ 3] = (+ h[ 8] - h[10] + h[13] + h[ 0] - h[ 1]);
  q[ 6] = (- h[11] + h[13] - h[ 0] - h[ 3] + h[ 4]);
  q[ 9] = (+ h[14] + h[ 0] - h[ 3] + h[ 6] - h[ 7]);
  q[12] = (+ h[ 1] + h[ 3] - h[ 6] - h[ 9] + h[10]);
  q[15] = (- h[ 4] - h[ 6] - h[ 9] + h[12] + h[13]);
  q[ 2] = (+ h[ 7] + h[ 9] - h[12] + h[15] + h[ 0]);
  q[ 5] = (+ h[10] - h[12] + h[15] - h[ 2] + h[ 3]);
  q[ 8] = (+ h[13] - h[15] + h[ 2] - h[ 5] - h[ 6]);
  q[11] = (- h[ 0] - h[ 2] - h[ 5] + h[ 8] + h[ 9]);
  q[14] = (+ h[ 3] - h[ 5] + h[ 8] - h[11] - h[12]);
  q[ 1] = (+ h[ 6] - h[ 8] + h[11] + h[14] - h[15]);
  q[ 4] = (+ h[ 9] - h[11] - h[14] + h[ 1] + h[ 2]);
  q[ 7] = (- h[12] - h[14] + h[ 1] - h[ 4] - h[ 5]);
  q[10] = (+ h[15] - h[ 1] - h[ 4] - h[ 7] + h[ 8]);
  q[13] = (+ h[ 2] + h[ 4] + h[ 7] + h[10] + h[11]);
=end
f0_def = 
[
  [  '+5',  '-7', '+10', '+13', '+14'], 
  [  '+8', '-10', '+13',  '+0',  '-1'], 
  [ '-11', '+13',  '-0',  '-3',  '+4'], 
  [ '+14',  '+0',  '-3',  '+6',  '-7'], 
  [  '+1',  '+3',  '-6',  '-9', '+10'], 
  [  '-4',  '-6',  '-9', '+12', '+13'], 
  [  '+7',  '+9', '-12', '+15',  '+0'], 
  [ '+10', '-12', '+15',  '-2',  '+3'], 
  [ '+13', '-15',  '+2',  '-5',  '-6'], 
  [  '-0',  '-2',  '-5',  '+8',  '+9'], 
  [  '+3',  '-5',  '+8', '-11', '-12'], 
  [  '+6',  '-8', '+11', '+14', '-15'], 
  [  '+9', '-11', '-14',  '+1',  '+2'], 
  [ '-12', '-14',  '+1',  '-4',  '-5'], 
  [ '+15',  '-1',  '-4',  '-7',  '+8'], 
  [  '+2',  '+4',  '+7', '+10', '+11']
]
  
$stat=Hash.new
$stat_location=Hash.new

def set_stat(s1,s2,i)
  if s2.to_i.abs<s1.to_i.abs
    t = s1
    s1 = s2
    s2 = t
  end
  idx = nil
  idx = 0 if s1[0].chr=='+' && s2[0].chr=='+' 
  idx = 1 if s1[0].chr=='+' && s2[0].chr=='-'
  idx = 2 if s1[0].chr=='-' && s2[0].chr=='+'
  idx = 3 if s1[0].chr=='-' && s2[0].chr=='-' 
  puts "ERROR in idx" if idx==nil
  if $stat[(s1[1..-1]+','+s2[1..-1])]==nil
    $stat[(s1[1..-1]+','+s2[1..-1])] = [0,0,0,0]   
    $stat_location[(s1[1..-1]+','+s2[1..-1])] = i.to_s()
  else
    $stat_location[(s1[1..-1]+','+s2[1..-1])] += ','+i.to_s() 
  end
  $stat[(s1[1..-1]+','+s2[1..-1])][idx]+= 1
end

def collision(x,y)
  r=0
  va = x[0].split(',')
  vb = y[0].split(',')
  va.each do |p|
    r=1 if vb.find_index(p)
  end
  return false if r==0
  va = x[2].split(',')
  vb = y[2].split(',')
  va.each do |p|
    r=2 if vb.find_index(p)
  end
  return false if r==1
  return true
end

def reg_empty(map, reg, from, to)
  (from..to).each do |x|
    return false if(map[x][reg])
  end
  return true
end

def reg_set(map, reg, from, to, value)
   (from..to).each do |x|
    map[x][reg] = value
  end
  
end

def print_reg_map(map, regs, length, indent=0)
  (regs-1).downto(0) do |r|
    print ' '*indent+'r'+r.to_s
    print ' '*(2+(2-r.to_s.length))
    print ':'
    (0..(length-1)).each do |x|
      if map[x][r]==nil
        print ' '
      else 
        print map[x][r]
      end
    end
    print "\n"
  end
end

def reg_map(steps, length)
  max_reg=0
  map = Array.new(length)
  (0..(length-1)).each {|x| map[x]=Array.new}
  steps.each do |step|
    reg=0
    a = step[2].split(',').collect { |x| x.to_i}
    a.sort!
    from = a[0]
    to = a[-1]
    while(reg_empty(map,reg,from,to)==false)
      reg += 1
    end
    reg_set(map, reg, from, to, step[3])
    max_reg=reg if reg>max_reg
  end
  return max_reg, map
end

def word_to_steps(word, list)
  steps=Array.new
  idx=0
  while(word!=0)
    if(word&1==1)
      steps << list[idx]
    end
    word >>= 1
    idx += 1
  end
  return steps
end

def print_collision_map(collisions, length)
  print '  '
  (0..(length-1)).each {|x| print ('A'[0]+x).chr+' '}
  (0..(length-1)).each do |y| 
    print "\n"+('A'[0]+y).chr+' '
    (0..(length-1)).each do |x| 
      if(collisions.find_index(('A'[0]+x).chr+('A'[0]+y).chr)) or
        (collisions.find_index(('A'[0]+y).chr+('A'[0]+x).chr))
        print('x ')
      else
        print('  ')
      end
    end 
  end 
  print("\n")
end

def check_collision(word, lut)
  (0..(lut.length-1)).each do |i|
    if word&(1<<i)!=0
      return true if word&lut[i]!=0
    end
  end
  return false
end

$bitcount_lut = Array.new(65536)

def bits_set_simple(x)
  r = 0
  while(x!=0)
    r+=1 if(x&1==1)
    x >>=1
  end
  return r
end

def init_bitcount_lut
  (0..(2**8-1)).each {|x| $bitcount_lut[x] = bits_set_simple(x)}
  ((2**8)..(2**16-1)).each {|x| $bitcount_lut[x] = bits_set(x, 8)}
end

def bits_set(x, length=16)
  r=0
  while(x!=0)
    r += $bitcount_lut[x&(2**length-1)]
    x >>= length
  end
  return r
end

def decode_word(word)
  idx='A'
  r = ''
  while(word!=0)
    if(word&1==1)
      r += idx
    end
    word >>= 1
    idx = (idx[0]+1).chr
  end
  return r
end

def generate_c_code(fout, func, optimizations=[], reg_map=[])
  out_interval = 3
  out_modulus = 16
  out_idx = 0
  opt_table = Array.new
  optimizations.each do |opt|
    opt_steps = opt[2].split(',')
    opt_steps.collect! {|x| x.to_i}
    opt_steps.each do |step|
      reg_a = opt[0].split(',')[0]
      reg_b = opt[0].split(',')[1]
      sign_a = '+' if func[step].find_index('+'+reg_a)
      sign_a = '-' if func[step].find_index('-'+reg_a)
      sign_b = '+' if func[step].find_index('+'+reg_b)
      sign_b = '-' if func[step].find_index('-'+reg_b)
      set = false
      free = false
      if step==opt_steps[0]
        sign_out='+'
        set=true
      else
        i=0
        while opt_table[opt_steps[0]][i][4]!=reg_a || opt_table[opt_steps[0]][i][5]!=reg_b
          i+=1
        end
        if(sign_a==opt_table[opt_steps[0]][i][0])
          sign_out='+'
        else
          sign_out='-'
        end
      end
      free = true if step==opt_steps[-1]
      reg_number = reg_map[step].find_index(opt[3])
      reg_name = sprintf("tr%d", reg_number)
      opt_table[step] = Array.new if opt_table[step]==nil
      opt_table[step] << [sign_a, sign_b, sign_out, reg_name, reg_a, reg_b, set, free]
    end
  end
  puts 'DBG: '+opt_table.inspect
  (0..(func.length-1)).each do |i|
    fout.printf("q[%2d] = ", out_idx)
    out_idx = (out_idx+out_interval)%out_modulus
    func[i].each do |j|
      skip = 0
      if opt_table[i]
        opt_table[i].each do |opt|
          skip = 1 if opt[4]==j[1..-1] or opt[5]==j[1..-1]
        end
      end
      fout.printf("%st[%2d] ", j[0].chr, j[1..-1].to_i) if skip==0
    end
    if opt_table[i]
      opt_table[i].each do |opt|
        fout.print(opt[2]+'('+opt[3])
        if opt[6]
          fout.printf('=%st[%2d]%st[%2d]',opt[0],opt[4].to_i,opt[1],opt[5].to_i)
        end
        fout.print(') ')
      end
    end  
    fout.print(";\n")
  end
end

(0..15).each do |i|
  (0..3). each do |j|
    ((j+1)..4).each do |k|
      set_stat(f0_def[i][j], f0_def[i][k], i)
    end
  end
end



dublets = Array.new

$stat.each_pair do |key,value|
  if value[0]+value[3]>1 || value[1]+value[2]>1
#    puts key+": \t"+value.inspect+": \t"+$stat_location[key]
  dublets << [key, value, $stat_location[key]]
  end
end

dublets.sort! do |x,y| 
  t = x[2].split(',')
  p = t[1].to_i - t[0].to_i 
  t = y[2].split(',')
  q = t[1].to_i - t[0].to_i
  if (p!=q)
    (p-q)
  else
    (x[2].split(',')[0].to_i-y[2].split(',')[0].to_i)
  end  
end

idx='A'
dublets.each {|e| e << idx; idx=(idx[0]+1).chr}

puts "useful combinations:"
dublets.each {|e| puts e[3]+': '+e[0]+' '*(5-e[0].length)+" \t"+e[1].inspect+" \t" +e[2]}

collisions = Array.new
puts "searching for collisions: "
(0..(dublets.length-2)).each do |i|
  ((i+1)..(dublets.length-1)).each do |j|
    if collision(dublets[i], dublets[j])
      print '*'
      collisions << dublets[i][3]+dublets[j][3]
    else
      print '.'
    end
  end
end
puts ''
#puts "collisions: "
#puts collisions.join(',')
#puts "collision-map: "
#print_collision_map(collisions, dublets.length)

collision_lut = Array.new(dublets.length, 0)
(0..(dublets.length-1)).each do |x| 
  (0..(dublets.length-1)).each do |y| 
    if collisions.find_index(('A'[0]+x).chr+('A'[0]+y).chr) or
       collisions.find_index(('A'[0]+y).chr+('A'[0]+x).chr)
      collision_lut[x] |= 1<<y
    end   
  end 
end


puts "initializing bitcount table..."
init_bitcount_lut

puts "collision free combinations:"
max = 0
combinations = Array.new
percent = 0
percent_step =(2**dublets.length-1)/10000.0
next_step = (2**dublets.length-1)
puts ''
(2**dublets.length-1).downto(0) do |x|
  if(x<=next_step)
    print "\x1b[s "+sprintf("%5.2f%%", percent/100.0)+"\x1b[u"
    percent += 1
    next_step -= percent_step
  end
  if check_collision(x, collision_lut) == false
    if bits_set(x)>= max
      combinations = Array.new if bits_set(x)>max
      combinations << x
      max = bits_set(x)
    end
  end
end

combinations.each do |c|
  regs, reg_map = reg_map(word_to_steps(c, dublets), f0_def.length)
  puts bits_set(c).to_s+': '+decode_word(c)+' ( '+(regs+1).to_s+' registers)'
  print_reg_map(reg_map, regs+1, f0_def.length, 4)
end
steps = word_to_steps(combinations[-1], dublets)
regs, reg_map = reg_map(steps, f0_def.length)
generate_c_code(STDOUT, f0_def,steps, reg_map)
