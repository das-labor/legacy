require 'fileutils'

@time = Time.now.strftime('%d.%m.%y_%H-%M')

def dump(rep)
  backup_path = rep.gsub('svn/repositories','svn/backup')
  FileUtils.mkdir(File.dirname(backup_path)) unless FileTest.directory?(File.dirname(backup_path))
  FileUtils.mkdir(backup_path) unless FileTest.directory?(backup_path)
  `svnadmin dump #{rep} > #{backup_path}/#{File.basename(rep)}_#{@time}.back`
end

puts "dumping repositories.."

dirs = Dir['/home/svn/repositories/*'].select{|d| File.directory? d}

dirs.each do |rep|
  if FileTest.directory?("#{rep}/conf")
   dump(rep)
  else
    
    
    subdirs = Dir["#{rep}/*"].select{|d| File.directory? d}
    subdirs.each do |subrep|
      if FileTest.directory?("#{subrep}/conf")
        dump(subrep)
      else
        subdirs = Dir["#{subrep}/*"].select{|d| File.directory? d}      
        subdirs.each { |subrep| dump(subrep) if FileTest.directory?("#{subrep}/conf") }
      end
    end
  end
end