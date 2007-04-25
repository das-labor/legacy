# CodeRay dynamic highlighter
# Usage: start this and your browser.
# Go to http://localhost:300/?<path to the file>
# and you should get the highlighted version.

require 'webrick'
require 'rubygems'
require 'coderay'

class CodeRayServlet < WEBrick::HTTPServlet::AbstractServlet
  def do_GET req, res
    file = req.query_string
    res.body =
      case file
      when nil
        'No file given.'
      when /favicon.ico$/
        "I can't highlight icons, stupid browser! ;)"
      else
        CodeRay.scan_file(file).html :tab_width => 2 , :wrap => :page
      end
    res['Content-Type'] = 'text/html'
  end
end

# 300 = ?R + ?a + ?y
s = WEBrick::HTTPServer.new :Port => 3002

s.mount '/', CodeRayServlet

s.mount_proc '/version' do |req, res|
  res.body = 'CodeRay::Version = ' + CodeRay::Version
  res['Content-Type'] = "text/plain"
end

trap("INT") { s.shutdown }
s.start