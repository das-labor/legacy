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

class Block
  attr_reader :name, :root 
  attr_accessor :key_value, :sub_blocks
  def initialize(name, root)
    @name = name
    @root = root
    @key_value = Hash.new
    @sub_blocks = Hash.new
  end
end

#strip commands
$in_block_comment = false
def parse_line_l1(line)
  line = /[^#]*/.match(line).to_s
  if $in_block_comment
    if /.*=end/.match(line)
      $in_block_comment = false
      return /.*=end([^#]*)/.match(line)[1]
    else
      return ""
    end
  else
    if /(.*)=begin/.match(line)
      $in_block_comment = true
      return /(.*)=begin/.match(line)[1]
    end
  end
  line
end


$main_block = Block.new("root_block", nil)
$current_block = $main_block

def new_block(root, name)
  $current_block = $current_block.sub_blocks[name] = Block.new(name, $current_block)
end

def close_block(block)
  $current_block = block.root
end

def new_value(block, key, value)
  block.key_value[key] = value
end

$linenumber = 0
$left_expr=""
def parse_l2(line)
  if /^[\s]*$/.match(line)
    return
  end
  if m = /([\w\s,._<>+-]*)([^\{\}=]*)(.*)/.match(line)
    $left_expr += m[1];
    if m[2]!=""
      puts("[l:" + $linenumber + "] Garbage: "+m[2]);
    end
    if m2 = /=([^;]*);(.*)/.match(m[3])
      new_value($current_block, $left_expr, m2[1])
      $left_expr = ""
      parse_l2(m2[2])    
    end
    if m2 = /\{(.*)/.match(m[3])
      new_block($current_block, $left_expr)
      $left_expr = ""
      parse_l2(m2[1])
    end
    if m2 = /\}(.*)/.match(m[3])
      close_block($current_block)
      $left_expr = ""
      parse_l2(m2[1])
    end
  else
    puts("[l:" + $linenumber + "] What???")
  end
end

def loadfile(fname)
  file=File.open(fname)
  while line=file.gets do
    $linenumber += 1
    parse_l2(parse_line_l1(line))
  end
end




