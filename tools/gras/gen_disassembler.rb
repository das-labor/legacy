=begin
    gen_disassembler.rb is part of the GeneRic ASsembler (GRAS)
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
require 'gen_assembler.rb'
require 'analyze.rb'

strategy_thesh = 0.15 # 15%

class Bucket < Array
end

class DasmStrategy
  attr_accessor :strategy
  def initialize(s)
    @strategy = Array.new
    @strategy = s
  end
  def pcode_iter(pcode,&block)
    s = Array.new
    x = -1
    @strategy.each{|i|
      s << ((pcode[i]<0)?(pcode[i]):(x+=1))
    }
    x += 1
    (2**(x)).times{|i|
      b = sprintf '%0'+x.to_s+'b', i
      q = String.new
      q = ''
      s.each{|i|
        q << '0' if i==-2
        q << '1' if i==-1
        q << (b[i]-'0'[0]).to_s if i>=0
      }
      p = q.to_i(2)
      block.call(p)
    }
  end
end

def find_dasm_strategy(iset, length, thresh)
  h = gen_parampositionhistogram(iset)
  strategy= Array.new
  (0...length).each{|i|
    if (h[0][i].to_f/(h[0][i]+h[1][i]).to_f) <= thresh
      strategy << i
    end
  }
  strategy
end

def build_dasm_table(iset, strategy)
  s = DasmStrategy.new(strategy)
  table = Array.new(2**strategy.length){|i| Array.new}
  iset.each_pair{|mnem,instrs|
    instrs.instructions.each_pair{|params,iblock|
      s.pcode_iter(iblock.pcode){ |i|
#        puts mnem +'.'+params+': pcode= '+iblock.pcode.join(',')+'  code:'+i.to_s
	table[i] = Array.new if table[i]==nil
        table[i] << iblock
      }
      
    }
  }
  table
end

def print_dasm_table_infos(table)
  table.each_with_index{|bucket,index|
    if bucket!=nil
      printf '[%03d](% 5d): ', index, bucket.length
      s = bucket.collect{|b| b.name+'.'+b.parameters.join(',')}
      puts s.join(';')
    end
  }
  nil
end













