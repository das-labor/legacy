=begin
    analyze.rb is part of the GeneRic ASsembler (GRAS)
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

require 'gen_assembler.rb'
require 'gen_disassembler.rb'
require 'gen_disassembler2.rb'


def count_mnemonics(iset)
  c = 0
  iset.each_key{|key|
    c += 1
  }
  c
end

def count_instructions(iset)
  c = 0
  iset.each_value{|instrs|
    instrs.instructions.each_key{|key|
      c += 1
    }
  }
  c
end

def simple_instructionset(iset)
  reasons = Array.new
  iset.each_pair{|mnem,ins|
    if (ins.max_length != ins.min_length)
      reasons << mnem 
    #  puts mnem
    end
  }
 # return 1 if reasons.empty?
  reasons
end 

def length_hist(iset)
  hist = Hash.new
  hist.default = 0
  min = 65536
  max = 0
  iset.each_pair{|mnem,inst|
    inst.instructions.each_pair{|params,iblock|
      hist[iblock.length] += 1; 
  #    if(iblock.length%8!=0)
  #      puts '  '+iblock.name+'.'+iblock.parameters.join(',')+' ('+iblock.length.to_s+')'
  #    end
    }
  }
  hist.each_pair{|key,value|
    puts(key.to_s + " => " + value.to_s)
    max = key if max<key
    min = key if min>key 
  }
  [hist,min,max]
end

def find_subintructions(iset)
  iset.each_pair{ |mnem,instrs|
    instrs.instructions.each_pair{ |params,iblock|
#      puts(mnem + '.' + params + ' : ' + iblock.pcode.join(','))
      iset.each_pair{ |mnem2,instrs2|
        instrs2.instructions.each_pair{ |params2,iblock2|
          if(iblock != iblock2)
            if (s=iblock.is_subinstruction(iblock2)) != []
              puts(mnem2 + '.' + params2 + ' is subinstruction of ' + mnem + '.' + params + ' (' + s.join(',') + ')')
            end
          end
        }
      }

    }
  }
  nil
end

def gen_parampositionhistogram(iset)
  hista = Hash.new
  hista.default = 0
  histb = Hash.new
  histb.default = 0

  iset.each_value{|instrs|
    instrs.instructions.each_value{|iblock|
      i = 0
      iblock.xcode.each_char{ |c|
        hista[i] += 1 if c == 'x'
        histb[i] += 1 if c != 'x'
        i += 1
      }
    }
  }
  [hista, histb]
end

def print_parampositionhistogram(hist,length)
  (0...length).each{|i|
    printf "[%02d]: %d/%d \t(%10.6f%%)\n", i, hist[0][i], hist[1][i], hist[0][i].to_f*100 / (hist[0][i]+hist[1][i]).to_f
  }
end

def analyze(fname)
  $main_block = Block.new("root_block", nil)
  $current_block = $main_block
  $mnemonics = Hash.new
  puts 'Loading ...'
  insblock=loadfile(fname)
  puts 'generating instruction table ...'
  mnemonics=gen_mnemonics(insblock.sub_blocks["instructionset"])
  puts 'checking for simple instruction set ...'
  s = simple_instructionset(mnemonics) 
  if s == []
    puts ' is simple'
  else
    puts ' is not simple, violating mnemonics are: ' + s.join(',')
  end
  puts 'generating instruction length histogram ...'
  lh = length_hist(mnemonics)
  puts 'searching for subinstruction relations ...'
  find_subintructions(mnemonics)
  puts 'generating histogram of parameter position usage ...'
  h = gen_parampositionhistogram(mnemonics)
  print_parampositionhistogram(h, lh[2])
  lg2 = Math.log(2)
  printf( "%3d mnemonics (%f bits) / %3d instructions (%f bits)\n", 
    count_mnemonics(mnemonics), 
    Math.log(count_mnemonics(mnemonics))/lg2, 
    count_instructions(mnemonics),
    Math.log(count_instructions(mnemonics))/lg2
  )
  
#  puts 'searching for disassembler strategy ...'
#  s = find_dasm_strategy(mnemonics,lh[1], 0.15)
  puts 'generating disassembler primary table ...'
#  dt = auto_build_dasm_table(mnemonics, lh[1])
#  print_dasm_table_infos(dt)

  dt = DasmTable.new
  dt.add_instructionset(mnemonics)
  dt.print
#  puts insblock.class
#  puts insblock.inspect
end


