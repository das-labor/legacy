def rb_init
  path = 'path/to/include'
  rbfiles = Dir.entries(path).select {|x| /\.rb\z/ =~ x}
  rbfiles -= [ File.basename(__FILE__) ]
  rbfiles.each { |path| require(File.basename(path)) }
end