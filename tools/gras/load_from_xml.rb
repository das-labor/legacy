=begin
    load_from_xml.rb is part of the GeneRic ASsembler (GRAS)
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
require 'instructionset.rb'
require 'parameter.rb'
require 'cpu.rb'
require 'rexml/document'

include REXML

def get_fallback(tables, key)
  tables.each do |t|
    return t[key] if t[key]!=nil
  end
end

def load_instructionset_from_xml(file)
  if file.class == String
    file = File.open(file,'r')
  end
  xmld = Document.new(file)
  cpu_hash = Hash.new
  xmld.elements.each('/systeminformation/cpu/*') do |e|
    cpu_hash[e.name] = e.get_text.to_s  
  end
  if cpu_hash['name']==nil
    name = File.basename(file.path)
    name = name.match(/^([^.]*)/)[1]
    cpu_hash['name'] = name
  end
  
  cpu = Cpu.new(cpu_hash['name'],cpu_hash['registers'],cpu_hash['flags'],cpu_hash['modes'])

  ins_name = nil
  xmld.elements.each('/systeminformation/instructionset/name/*') do |e|
    ins_name=e.get_text.to_s
  end  
  if ins_name == nil
    ins_name = cpu_hash['name']
  end

  instructionset = Instructionset.new(ins_name, cpu)

  globals = Hash.new
  xmld.elements.each('/systeminformation/instructionset/*') do |e|
    if e.name != 'instruction'
      globals[e.name] = e.get_text.to_s
    else
      mnemonic = e.attributes['name'].to_s
      secondary = Hash.new
      e.each_element do |i|
        if i.name == 'param_code'
          param_code = i.attributes['name'].to_s
          local = Hash.new
          i.each_element_with_text{|n| local[n.name]=n.get_text.to_s}
#          puts "#{mnemonic}.#{param_code} ("+ local.inspect+")"
          ins = Instruction.new(mnemonic, param_code.split(','), local['opcode'],
                                get_fallback([local, secondary, globals], 'cycles'),
                                get_fallback([local, secondary, globals], 'modify_flags'),
                                get_fallback([local, secondary, globals], 'set_flags'),
                                get_fallback([local, secondary, globals], 'clear_flags'),
                                get_fallback([local, secondary, globals], 'description') )
          instructionset.add_instruction(ins)                      
       else
         secondary[i.name]=i.get_text.to_s
       end
      end
    end
  end
  return instructionset
end

