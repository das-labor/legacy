Dir.pwd

Dir.chroot("#{Dir.pwd}/public")

Dir.chdir("/tmp")

Dir["config.?"]
Dir["*.[a-z][a-z]"]
Dir["*.[^r]*"]
Dir["*.{rb,h}"]
Dir["*"]

Dir.foreach("_css") {|x| puts("Got " + x) }

def create_if_missing *names
	names.each do |name| Dir.mkdir(name) unless File.directory?(name) end 
end

def sort_and_move
  Dir.glob("*") do |file| 
    letter = file[0..0].upcase
    if letter!=file then
      todir = '#'
      todir = letter if ('A'..'Z').include? letter
      Dir.mkdir todir unless File.exists? todir
      File.rename(file, todir + '/' + file)
    end
  end  
end