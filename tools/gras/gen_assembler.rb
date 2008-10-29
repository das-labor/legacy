=begin
    gen_assembler.rb is part of the GeneRic ASsembler (GRAS)
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


require 'mnemonic.rb'
require 'blockparser.rb'



def gen_mnemonics(insblock)
  mnemonics = Hash.new
  def_opcode=""
  def_cycles=1
  def_set_flags=""
  def_clear_flags=""
  def_modify_flags=""

  def_opcode       = (insblock.key_value["opcode"]==nil)?(""):(insblock.key_value["opcode"])
  def_cycles       = (insblock.key_value["cycles"]==nil)?(1):(insblock.key_value["cycles"])
  def_set_flags    = (insblock.key_value["set_flags"]==nil)?(""):(insblock.key_value["set_flags"])	
  def_clear_flags  = (insblock.key_value["clear_flags"]==nil)?(""):(insblock.key_value["clear_flags"])
  def_modify_flags = (insblock.key_value["modify_flags"]==nil)?(""):(insblock.key_value["modify_flags"])
  def_desc         = (insblock.key_value["description"]==nil)?(""):(insblock.key_value["description"])

  insblock.sub_blocks.each_pair{ |mnem,data|
    mnemonics[mnem] = Mnemonic.new(mnem)

    mnem_opcode       = (data.key_value["opcode"]==nil)?(def_opcode):(data.key_value["opcode"])
    mnem_cycles       = (data.key_value["cycles"]==nil)?(def_cycles):(data.key_value["cycles"])
    mnem_set_flags    = (data.key_value["set_flags"]==nil)?(def_set_flags):(data.key_value["set_flags"])	
    mnem_clear_flags  = (data.key_value["clear_flags"]==nil)?(def_clear_flags):(data.key_value["clear_flags"])
    mnem_modify_flags = (data.key_value["modify_flags"]==nil)?(def_modify_flags):(data.key_value["modify_flags"])
    mnem_desc         = (data.key_value["description"]==nil)?(def_desc):(data.key_value["description"])

    data.sub_blocks.each_pair{ |params,insdata|
      p = Array.new;
      p = params.split(',');
    
      opcode       = (insdata.key_value["opcode"]==nil)?(mnem_opcode):(insdata.key_value["opcode"])
      cycles       = (insdata.key_value["cycles"]==nil)?(mnem_cycles):(insdata.key_value["cycles"])
      set_flags    = (insdata.key_value["set_flags"]==nil)?(mnem_set_flags):(insdata.key_value["set_flags"])	
      clear_flags  = (insdata.key_value["clear_flags"]==nil)?(mnem_clear_flags):(insdata.key_value["clear_flags"])
      modify_flags = (insdata.key_value["modify_flags"]==nil)?(mnem_modify_flags):(insdata.key_value["modify_flags"])
      desc         = (insdata.key_value["description"]==nil)?(mnem_desc):(insdata.key_value["description"])

      if m = /^\[([\d\s,+-]*)\]$/.match(opcode)
        opcode=m[1].split(',').collect {|x| x.to_i}
      end
      mnemonics[mnem].add_instruction(mnem, p, opcode, cycles, modify_flags, set_flags, clear_flags, desc)
    }
  }
  mnemonics
end

