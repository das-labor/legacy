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
require 'instruction.rb'

class Mnemonic
  attr_reader :instructions, :max_length, :min_length, :description 
  @@mnemonics = Array.new

  def initialize(name,description="")
    @name = name
    @@mnemonics << name
    @params = Array.new
    @description = description
    @instructions = Hash.new
    @max_length = -1
    @min_length = -1
  end

  def singelton_class
    class << self
      self
    end
  end

  def add_instruction_direct(instr)
 #   instr.description = @description if (instr.description == "")
    @instructions[instr.parameters] = instr
    @max_length = instr.length if instr.length > @max_length
    @min_length = instr.length if (instr.length < @min_length) or (@min_length == -1)
  end

  def add_instruction(parameters, opcode, cycles=1, modify_flags="", set_flags="", clear_flags="", description="")
    instr = Instruction.new(parameters, opcode, cycles,
      modify_flags, set_flags, clear_flags, (description=="")?(@description):(description))
    add_instruction_direct(instr)
  end

end # class Mnemonic



