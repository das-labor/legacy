=begin
    convert_to_xml.rb is part of the GeneRic ASsembler (GRAS)
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

require 'blockparser.rb'

def in_quote(str)
  return str if str.match(/^".*"$/)
  return '"'+str+'"'
end

def e(str) #perform escaping
  str.gsub!('&', '&amp;')
  str.gsub!('<', '&lt;')
  str.gsub!('>', '&gt;')
  return str  
end

def convert(finname, foutname)
#  puts 'Loading ...'
  block=loadfile(finname)
#  puts block.inspect
#  block.to_xml(0)
  if foutname==nil || foutname==  '-'
    fout=STDOUT
  else
    fout = File.open(foutname,"w+")
  end
  fout.puts '<?xml version="1.0" encoding="UTF-8"?>'
  fout.puts '<systeminformation>'
  cpu_block=block.sub_blocks["cpu"]
  fout.puts '  <cpu>'
  cpu_block.key_value.each{|k,v| fout.puts "    <#{e(k)}>#{e(v)}</#{e(k)}>"}   if cpu_block
  fout.puts '  </cpu>'
  param_block=block.sub_blocks["parameters"]
  fout.puts '  <parameters>'
  param_block.key_value.each{|k,v| fout.puts "    <#{e(k)}>#{e(v)}</#{e(k)}>"} if param_block
  param_block.sub_blocks.each do |k,v|
    fout.puts("    <param name=#{e(in_quote(k))}>")
    v.key_value.each{|x,y| fout.puts("      <#{e(x)}>#{e(y)}</#{e(x)}>")}
    fout.puts("    </param>")
  end                                                                 if param_block
  fout.puts '  </parameters>'
  ins_block=block.sub_blocks["instructionset"]
  fout.puts '  <instructionset>'
  ins_block.key_value.each{|k,v| fout.puts "    <#{e(k)}>#{e(v)}</#{e(k)}>"}
  ins_block.sub_blocks.each do |k,v|
    fout.puts("    <instruction name=#{in_quote(k)}>")
    v.key_value.each{|x,y| fout.puts("      <#{e(x)}>#{e(y)}</#{e(x)}>")}
    v.sub_blocks.each do |params, desc|
      fout.puts("      <param_code name=#{e(in_quote(params))}>")
      desc.key_value.each{|x,y| fout.puts "        <#{e(x)}>#{e(y)}</#{e(x)}>"}
      fout.puts("      </param_code>")   
    end
    fout.puts("    </instruction>")
  end
  
  fout.puts '  </instructionset>'
  fout.puts '</systeminformation>'

end

if ARGV.length==0
  puts "usage: #{$0} in_file [out_file]"
  exit(-1)
end

if ARGV.length==2
  foutname=ARGV[1]
else
  foutname=ARGV[0]+".xml"
end

convert(ARGV[0], foutname)