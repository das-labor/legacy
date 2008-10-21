=begin
    mnemonic.rb is part of the GeneRic ASsembler (GRAS)
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

class Mnemonic
  @@mnemonics = Array.new

  def initialize(name,description="")
    @name = name
    @@mnemonics << name
    @params = Array.new
    @description = description
  end

  def singelton_class
    class << self
      self
    end
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
        if x==last_alpha 
          accu += 1
          param_code << accu
        else
          accu = param_start[x.downcase]
          param_code << accu
          last_alpha = x
        end
      end # if (x=="0") or (x=="1")
    end
    param_code
  end

  def preparse_opcode(opcode)
    opcode.delete("._")
  end

#=begin
  def convert2bincode(opcode)
    bin_opcode = String.new
    if /^0x/ =~ opcode 
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
    end # if /^0x/ =~ opcode
    if /^0[0-7]/ =~ opcode
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
    end # if /^0[0-7]/ =~ opcode
    if /^0b/ =~ opcode 
      bin_opcode = opcode[2,opcode.length-2]
    end
    bin_opcode
  end # def convert2bincode
#=end

  def add_instruction(parameters, opcode, cycles=1, modify_flags="")
    opcode = preparse_opcode(opcode)
    bin_opcode = convert2bincode(opcode)
    @params += parameters
    process_code = Array.new
    process_code = gen_param_code(bin_opcode)
    assemble_code = lambda {|param| 
      ret = String.new 
      param=param.join() 
      process_code.each do |x| 
        ret << "1" if x==-1
        ret << "0" if x==-2
        ret << param[x] if x>=0
      end
      ret
    } 
    singelton_class().class_eval{define_method(parameters.join("_")+"__length", lambda{bin_opcode.length})};
    singelton_class().class_eval{define_method(parameters.join("_")+"__cycles", lambda{cycles})};
    singelton_class().class_eval{define_method(parameters.join("_")+"__modifyflags", lambda{modify_flags})};
    singelton_class().class_eval{define_method(parameters.join("_")+"__assemble", assemble_code)};
  end

end # class Mnemonic



