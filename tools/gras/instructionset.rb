=begin
    instructionset.rb is part of the GeneRic ASsembler (GRAS)
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

require 'instruction.rb'
require 'parameter.rb'

class Instructionset
  attr_reader :instructions, :by_mnemonic, :parameters, :parameters_by_code
  attr_reader :cpu, :name, :is_simple, :simple_violator
  def initialize(name, cpu)
    @name = name
    @cpu = cpu
    @instructions = Array.new
    @by_mnemonic = Hash.new(nil)
    @parameters = Array.new
    @parameters_by_code = Hash.new
    @is_simple=true
    @simple_violator=Array.new
    @param_range_by_mnemonic=Hash.new
  end
  
  def add_instruction(instruction)
    return false if instruction.class!=Instruction
    @instructions << instruction
    @by_mnemonic[instruction.name] = Array.new if @by_mnemonic[instruction.name]==nil
    @by_mnemonic[instruction.name] << Instruction
    if @param_range_by_mnemonic[instruction.name]==nil
       @param_range_by_mnemonic[instruction.name]=[instruction.length, instruction.length]
    else
      if @param_range_by_mnemonic[instruction.name][0]>instruction.length
         @param_range_by_mnemonic[instruction.name][0]=instruction.length
      end
      if @param_range_by_mnemonic[instruction.name][1]<instruction.length
         @param_range_by_mnemonic[instruction.name][1]=instruction.length
      end
      if @param_range_by_mnemonic[instruction.name][1]!=@param_range_by_mnemonic[instruction.name][0]
        @is_simple = false
        @simple_violator << instruction.name unless @simple_violator.find_index(instruction.name)
      end
    end
    return true
  end # add_instruction
  
  def add_parameter(parameter)
    @parameters << parameter
    @parameters_by_code[parameter.name] = parameter
  end
  
end # Instructionset

