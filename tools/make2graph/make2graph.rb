#ruby
#
# License: GPLv3
# Author: Daniel Otte (daniel.otte@rub.de)
# Date: 2008-11-11

$obj_list = Hash.new
$current_id = 0
$out_file
$in_file

def new_obj(id, name)
  $out_file.printf("  %d [label=\"%s\"]\n",id, name)
end

def new_connection(ida, idb)
  $out_file.printf("  %d -> %d\n",ida, idb)
end

def get_id(name)
  if($obj_list[name]==nil)
    new_obj($current_id, name)
    $obj_list[name] = $current_id
    $current_id += 1
    return  $current_id -1
  else
    return $obj_list[name]
  end
end

def handle_line(line)
  rid = nil
  if(m = /^([\w_][\w._]*):(.*)/.match(line))
    rid = get_id(m[1])
    if(m[2])
      s = m[2].split
      s.each do |x|
        lid = get_id(x)
        new_connection(rid, lid)
      end
    end  
  end
end

###############
# MAIN
##############

if ARGV.length==0
  $in_file = $stdin
  $out_file = $stdout
else
  if ARGV.length==1
    $out_file = $stdout
    $in_file = File.open(ARGV[0],"r")
  end
  if ARGV.length==2
    $in_file = File.open(ARGV[0],"r")
    $out_file = File.open(ARGV[1],"w+")
  end
  if ARGV.length>2
    puts('$usage')
  end
end

$out_file.puts('digraph G {')

while line=$in_file.gets do
  handle_line(line)
end

$out_file.puts('}')

$in_file.close
$out_file.close



