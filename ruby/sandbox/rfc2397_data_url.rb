require 'base64'

def data_url(filename)
  data = Base64.encode64(File.open(filename).read).select { |line| line.chop! }.join('')
  File.open("#{filename}.txt",'wb') { |file| file.print "data:image/jpeg;base64,#{data}" }
end

data_url(ARGV[0]) if ARGV[0]