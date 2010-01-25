=begin
    gen_disassembler2.rb is part of the GeneRic ASsembler (GRAS)
    Copyright (C) 2010  Daniel Otte (daniel.otte@rub.de)

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


require 'mnemonic.rb'
require 'gen_assembler.rb'
require 'analyze.rb'

def xcode_to_mask_string(xcode)
  mask_str = String.new()
  xcode.each_char do |c|
    if c=='0' || c=='1'
      mask_str.concat('1')
    else
      mask_str.concat('0')
    end
  end
  return mask_str
end

def xcode_to_mask(xcode)
  return  xcode_to_mask_string(xcode).to_i(2)
end

def xcode_to_id(xcode)
  id_str = String.new()
  xcode.each_char do |c|
    if c=='1'
      id_str.concat('1')
    else
      id_str.concat('0')
    end
  end
  id_str.to_i(2)
end

def bits_set_in_mask_string(m)
  i=0
  m.each_char do |c| 
    i+=1 if c=='1'
  end
  return i
end

class DasmTableEntry
  attr_reader :length, :mask, :table
  
  def initialize(length, mask)
    @length = length
    @mask = mask
    @table = Hash.new
    @table.default = nil
  end
  
  def add_instruction(instruction)
    m=xcode_to_mask(instruction.xcode)
    return false if m!=@mask
    i=xcode_to_id(instruction.xcode)
    if @table[i]==nil
      @table[i]=Array.new
    end
    @table[i] << instruction
    return true
  end
  
  def mask_string()
    s=sprintf("%0#{@length}b", mask)
  end
  
  def <=> (b)
    x = self.length<=>b.length
    return x if x!=0
    x = bits_set_in_mask_string(b.mask_string())<=>bits_set_in_mask_string(self.mask_string())
    return x
  end
  
  def print
    printf("[%s] (length: %d)\n", self.mask_string(), @length)
    @table.each_pair do |id, instructions|
      printf("\tid = %0#{@length}b :", id)
      instructions.each {|ins| printf(" %s(%s)", ins.name, ins.parameters.join(','))}
      printf("\n")
    end
  end
end

class DasmTable
  attr_reader :master_table
  attr_reader :index_table
  
  def initialize()
    @master_table=Hash.new
    @master_table.default=nil
  end
  
  def add_instruction(instruction)
    if instruction.class != Instruction
      printf("ERROR class of instruction: %s\n", instruction.class.to_s)
      return
    end
    ms = xcode_to_mask_string(instruction.xcode)
    if @master_table[ms]==nil
      @master_table[ms]=DasmTableEntry.new(ms.length(), ms.to_i(2))
    end
    @master_table[ms].add_instruction(instruction)
  end
  
  def finalize()
    @index_table=Array.new
    @master_table.each_key{|key| @index_table<<key}
    @index_table.sort!{|a,b| @master_table[a]<=>@master_table[b]}
  end
  
  def add_instructionset(instructionset)
    if instructionset.class==Instructionset
      instructionset.instructions.each{ |i| self.add_instruction(i) }
      self.finalize  
    end
    if instructionset.class==Hash
      instructionset.each_value do |m|
        m.instructions.each_value { |i| self.add_instruction(i) }
      end
      self.finalize  
    end
    if instructionset.class==Array
      instructionset.each{|i| self.add_instruction(i)}
      self.finalize
    end
  end
  
  def print
    @index_table.each do |i|
      @master_table[i].print()
    end
  end
end

