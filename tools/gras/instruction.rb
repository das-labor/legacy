=begin
    instruction.rb is part of the GeneRic ASsembler (GRAS)
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

require 'jcode'

class Instruction
  attr_reader :name, :description, :length, :varbits, :parameters, :prefix
  attr_reader :cycles, :modify_flags, :set_flags, :clear_flags, :xcode, :pcode
  attr_reader :opcode
  attr_accessor :key_value, :sub_blocks

  def singelton_class
    class << self
      self
    end
  end

  def is_subinstruction(subinstr)
    ret = Array.new
    return nil if subinstr.class != Instruction
    return []  if subinstr.length != @length
    return []  if subinstr.varbits > @varbits

    (0...@length).each{|i|
      if (@pcode[i]== -2) or (@pcode[i]== -1)
        return [] if subinstr.pcode[i]!=@pcode[i]
        ret << @pcode[i]
      else
        ret << subinstr.pcode[i]
      end
    }
    return ret
  end

  def gen_param_code(bin_opcode)
    params = Array.new
    alphas = Hash.new
    alphas.default = 0
    bin_opcode.each_char do |x|
      if (x!="0") && (x!="1") 
        alphas[x.downcase] += 1
      end
    end
    accu = 0
    param_start = Hash.new
    param_start.default = -1
    ("a".."z").each do |x|
      if(alphas[x] != 0) 
        param_start[x] = accu
        param_start[x.upcase] = accu
        accu += alphas[x]
      end
    end
    param_code = Array.new
    last_alpha = -1
    accu=0
    bin_opcode.each_char do |x|
      if (x=="0") or (x=="1")
        param_code << -1 if x=="1"
        param_code << -2 if x=="0"
      else
        param_code << param_start[x]
        param_start[x] += 1
      end # if (x=="0") or (x=="1")
    end
    param_code
  end

  def preparse_opcode(opcode)
    opcode.delete("._")
  end

  def convert2bincode(opcode)
    bin_opcode = String.new
    if m= /^0[xX](.*)/.match(opcode)
      opcode = m[1] 
      opcode.each_char do |x| 
        case x
	  when "0"
	    bin_opcode << "0000"
	  when "1"
	    bin_opcode << "0001"
	  when "2"
	    bin_opcode << "0010"
	  when "3"
	    bin_opcode << "0011"
	  when "4"
	    bin_opcode << "0100"
	  when "5"
	    bin_opcode << "0101"
	  when "6"
	    bin_opcode << "0110"
	  when "7"
	    bin_opcode << "0111"
	  when "8"
	    bin_opcode << "1000"
	  when "9"
	    bin_opcode << "1001"
	  when "a", "A"
	    bin_opcode << "1010"
	  when "b", "B"
	    bin_opcode << "1011"
	  when "c", "C"
	    bin_opcode << "1100"
	  when "d", "D"
	    bin_opcode << "1101"
	  when "e", "E"
	    bin_opcode << "1110"
	  when "f", "F"
	    bin_opcode << "1111"
          else
            bin_opcode << x * 4
	end # case x
      end # opcode.each_char do
    # if /^0x/ =~ opcode
    elsif m= /^0([0-7].*)/.match(opcode)
      opcode=m[1];
      opcode.each_char do |x| 
        case x
	  when "0"
	    bin_opcode << "000"
	  when "1"
	    bin_opcode << "001"
	  when "2"
	    bin_opcode << "010"
	  when "3"
	    bin_opcode << "011"
	  when "4"
	    bin_opcode << "100"
	  when "5"
	    bin_opcode << "101"
	  when "6"
	    bin_opcode << "110"
	  when "7"
	    bin_opcode << "111"
          else
            bin_opcode << x * 3
	end # case x
      end # opcode.each_char do
    # if /^0[0-7]/ =~ opcode
    elsif /^0[bB]/ =~ opcode 
      bin_opcode = opcode[2,opcode.length-2]
    end
    bin_opcode
  end # def convert2bincode

  def initialize(name, parameters, opcode, cycles=1, modify_flags="", set_flags="", clear_flags="", description="")
    @name = name
    @opcode = opcode
    cycles=1          if cycles==nil
    set_flags=""      if set_flags==nil
    modify_flags = "" if modify_flags==nil
    clear_flags=""    if clear_flags==nil
    description=""    if description==nil
    
    if opcode.class == String
      opcode = preparse_opcode(opcode)
      bin_opcode = convert2bincode(opcode)
      process_code = Array.new
      process_code = gen_param_code(bin_opcode)    
    else 
      if opcode.class == Array
        process_code = opcode
      else
        puts("ERROR: invalid opcode type (#{opcode.class}) [#{opcode.inspect}]")
        return
      end
    end
    assemble_code = lambda do |param| 
      ret = String.new 
      param=param.join() 
      process_code.each do |x| 
        ret << "1" if x==-1
        ret << "0" if x==-2
        ret << param[x] if x>=0
      end
      ret
    end
    @pcode = process_code
    @xcode = ""
    bin_opcode.each_char{|x| if (x=="1" or x=="0") then @xcode += x else @xcode += 'x' end}
    @parameters = parameters
    varbits = 0;
    process_code.each{ |x| if (x<=>varbits)==1 then varbits=x end}
    @varbits = varbits += 1;
    @prefix = ""
    process_code.each{ |x| if (x===(-2..-1)) then prefix + (x+2).to_s else break end}
     
    @length = bin_opcode.length;
    @cycles = cycles;
    @modify_flags = modify_flags;
    @set_flags = set_flags;
    @clear_flags = clear_flags;
    @description = description;
    singelton_class().class_eval{define_method("assemble", assemble_code)};
  end

end

