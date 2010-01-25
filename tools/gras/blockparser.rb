=begin
    blockparser.rb is part of the GeneRic ASsembler (GRAS)
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



class Block
  attr_reader :name, :root 
  attr_accessor :key_value, :sub_blocks
  def initialize(name, root)
    @name = name
    @root = root
    @key_value = Hash.new
    @sub_blocks = Hash.new
  end
  def to_xml(indent)
    puts(' '*indent+"<block name=#{@name}>")
    @key_value.each{|k,v|puts(' '*(indent+4)+"<#{k} #{v}/>") } if @key_value
    @sub_blocks.each{|k, v| v.to_xml(indent+4)} if @sub_blocks
    puts(' '*indent+"</block>")
  end
end

#strip comments
$in_block_comment = false
def parse_line_l1(line)
  line.gsub!(/(\\#)/, 0xff.chr)
  line.gsub!(/'([^#']*)#*([^#']*)'/, "\\1\377\\2")
  line = /^[^#]*/.match(line).to_s()
#  line = /([^#]*(\\#)*[^#]*|[^#]*'[^']*'[^#]*|[^#]*"[^"]*"[^#]*|[^#]*)*/.match(line).to_s
  line.gsub!("\377", '#')
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

def new_block(root, name)
  if $current_block.sub_blocks[name] == nil
    $current_block = $current_block.sub_blocks[name] = Block.new(name, $current_block)
  else
    $current_block = $current_block.sub_blocks[name]
  end
end

def close_block(block)
  $current_block = block.root
  if $current_block==nil
    puts($filename +":" + $linenumber.to_s + ": error: too much '}'");
  end
end

def new_value(block, key, value)
  block.key_value[key] = value
end

$filename = ""
$linenumber = 0

def parse_lexpr(expr)
  expr.strip!
  if expr==""
    puts($filename +":" + $linenumber.to_s + ": error: empty lvalue!");
  end
  expr
end

$left_expr=""
def parse_l2(line)
  if /^[\s]*$/.match(line)
    return
  end
  if m = /([\w\s,._<>+@$#\[\]\(\)-]*)([^\{\}=]*)(.*)/.match(line)
    $left_expr += m[1];
    if m[2]!=""
      puts($filename +":"  + $linenumber.to_s + ": Garbage: "+m[2]);
    end
    if m2 = /=([^;]*);(.*)/.match(m[3])
      $left_expr = parse_lexpr($left_expr) 
      new_value($current_block, $left_expr, m2[1].strip)
      $left_expr = ""
      parse_l2(m2[2])    
    end
    if m2 = /\{(.*)/.match(m[3])
      $left_expr = parse_lexpr($left_expr)
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
    puts($filename +":"  + $linenumber.to_s + ": What???")
  end
end

def loadfile(fname)
  $main_block = Block.new("root_block", nil)
  $current_block = $main_block  
  file=File.open(fname)
  $filename = fname
  while line=file.gets do
    $linenumber += 1
    parse_l2(parse_line_l1(line))
  end
  file.close
  return $main_block
end

def writeblock(f, block, deep=0)
  f.puts ' '*(deep*2) + block.name + '{'

  block.key_value.each_pair{|key,value|
    f.puts(' '*((deep+1)*2) + key.to_s() + ' = ' + value.to_s() +';')
  }
  if not block.sub_blocks.empty?
    block.sub_blocks.each_value{|value|
      writeblock(f, value, deep+1)
    }
  end
  f.puts ' '*deep*2 + '}'
end

def savefile(fname, block)
  file=File.open(fname,'w')
  deep = 0;
  writeblock(file,block,0) 
  file.close
end

