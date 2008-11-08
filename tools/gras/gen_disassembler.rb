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
  attr_accessor :strategy

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

def find_dasm_strategy(iset, length, thresh, strategy_mask=[])
  h = gen_parampositionhistogram(iset)
  strategy= Array.new
  (0...length).each{|i|
    if (h[0][i].to_f/(h[0][i]+h[1][i]).to_f) <= thresh
      strategy << i
    end
  }
  strategy_mask.each { |mitem|
    strategy.delete(mitem)
  }
  strategy
end

def build_dasm_table(iset, strategy)
  s = DasmStrategy.new(strategy)
  table = Bucket.new(2**strategy.length){|i| Bucket.new}
  table.strategy= strategy
  iset.each_pair{|mnem,instrs|
    instrs.instructions.each_pair{|params,iblock|
      s.pcode_iter(iblock.pcode){ |i|
#        puts mnem +'.'+params+': pcode= '+iblock.pcode.join(',')+'  code:'+i.to_s
	table[i] = Bucket.new if table[i]==nil
        table[i] << iblock
      }
      
    }
  }
  table
end

def auto_build_dasm_table(iset, length, thresh=0.15, per_bucket_limit=10)
  s = find_dasm_strategy(iset, length, thresh)
#  puts 'strategy: ' + s.join(',')
  t = build_dasm_table(iset, s)
  t.each_with_index{|bucket,index|
    if bucket.length >= per_bucket_limit
      bucket_iset = Hash.new
      bucket.each{|iblock|
        bucket_iset[iblock.name] = Mnemonic.new(iblock.name)
        bucket_iset[iblock.name].add_instruction_direct(iblock)
      }
      subs = find_dasm_strategy(bucket_iset, length, thresh, s)
#      puts 'substrategy: ' + subs.join(',')
      t[index] = Bucket.new
      t[index].strategy = subs
      t[index] = build_dasm_table(bucket_iset, subs)
    end
  }
  t
end

def print_dasm_table_infos(table,spacing=0)
  puts ' '*spacing + 'strategy: ' + table.strategy.join(',')
  table.each_with_index{|bucket,index|
    if bucket!=nil
      printf ' '*spacing+'[%03d](% 3d): ', index, bucket.length  if bucket.length>0
      if bucket[0].class == Bucket
        puts 'subcluster' 
        print_dasm_table_infos(bucket,spacing+3)
      else
        s = bucket.collect{|b| b.name+'.'+b.parameters.join(',')}
        puts s.join('; ') if bucket.length>0
      end
    end
  }
  nil
end













