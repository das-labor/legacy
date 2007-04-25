require "open3"
stdin, stdout, stderr = Open3.popen3('sh')
stdin.puts "ls /"
stdin.close
puts stderr
puts stdout