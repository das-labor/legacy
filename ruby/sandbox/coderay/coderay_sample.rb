require 'rubygems'
require 'coderay'

ofile = File.open("test.rb").readlines.to_s
tokens = CodeRay.scan ofile, :ruby
page = tokens.html :line_numbers => :inline, :wrap => :page

File.new("test.rb.html","w") do |file|
  file.print page
end