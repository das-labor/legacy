if ARGV.size >= 1
  ARGV.each { |arg|
    case arg
      when '-high'
        @high = true
      when '-all'
        to_flv_all
        exit
    end
  }  
  File.exists?(file) ? to_flv(ARGV[0]) : p "#{file} not found"
else
  p "syntax: ./flv2mp4.rb <filename> [-all] [-high]"; exit
end

def to_flv_high(file)
  `ffmpeg -i #{file} -s 320x240 -f mp4 #{file}00.mp4` if @high
end
def to_flv_base(file)
  `ffmpeg -i #{file} -s 320x240 -r 15.00 -b 195 -f mp4 #{file}.mp4`
end
def to_flv_all
  Dir['/*.flv'].each { |file| to_flv(file) }
end
def to_flv(file)
  @high ? to_flv_high(file) : to_flv(file)
end