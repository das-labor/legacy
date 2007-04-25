#!/usr/bin/env ruby

# == Synopsis
#
# THIS DOES NOT WORK YET ON WIN32.
# Create a new rails project and a new svn repository, importing the rails
# project into the svn repository and telling svn to ignore all the files that
# it should ignore.  The rails project is the trunk; empty branches and tags
# directories are also created.
#
# == Usage
#
#   ruby svnrails.rb projname
# 
# projname::
#   The name of the project.  If omitted, the projname defaults to "myrailsapp".
#   A new svn repository is created called repo_#{projname}.  For example, if
#   you run the command "ruby svnrails.rb myproj" the rails project will be
#   called myproj and the svn repository will be called repo_myproj.
#
# == Author
#   Peter Armstrong
#     Email: peterarmstrong@gmail.com
#     Blog: peterarmstrong.com
#
# == Credits
#   This is based on the steps found at the Rails wiki:
#     http://wiki.rubyonrails.org/rails/pages/HowtoUseRailsWithSubversion
#   which were incorporated into a rake task posted at the blog of a person
#   named Jake in the following blog post:
#     http://blog.unquiet.net/archives/2005/11/06/helpful-rake-tasks-for-using-rails-with-subversion.
#   This rake task gave me the idea to script the whole thing.
#
# == Notes
#   This could probably be written a lot more tersely; however, I'm trying to
#   ensure it is readable rather than short.  I want the script to look like
#   what a user does on the command line, rather than just achieve the same end.
#
# == Copyright
#   Copyright (c) 2006 Peter Armstrong
#   Licensed under the same terms as Ruby.
#
# == Revision History
#   Version 0.1, 2006-06-17 - initial version

require "fileutils"
require "rdoc/usage"

def msgputs(str)
  puts "\n" + str + "...\n"
end

def exit_if_exists(filename)
  if (File.exist? filename)
    msgputs "A file or directory with the name #{filename} already exists; aborting script."
    exit
  end
end

projname = ARGV[0]
if (projname == "-h" || projname == "--help")
  RDoc::usage
end

msgputs "initial setup"
origwd = Dir.getwd
sep = File::SEPARATOR
if (projname.nil?)
  projname = "myrailsapp"
end
reponame = "repo_" + projname
projpath = origwd + sep + projname
repopath = origwd + sep + reponame
repofileurl = "file://" + repopath
trunkurl = repofileurl + sep + "trunk"

msgputs "ensuring there's nothing in the way"


exit_if_exists projname
exit_if_exists reponame
exit_if_exists "trunk"

msgputs "creating the rails project"
system "rails #{projname}"

puts "\n\ncreating the following directory structure for importing into svn:"
puts "  #{projname}"
puts "  #{projname}#{sep}trunk"
puts "  #{projname}#{sep}tags"
puts "  #{projname}#{sep}branches"
puts "(the new rails project is in #{projname}/trunk)"
File.rename(projname, "trunk")
Dir.mkdir projname
FileUtils.mv("trunk", projname)
Dir.chdir projpath
Dir.mkdir "branches"
Dir.mkdir "tags"
Dir.chdir origwd

msgputs "importing the directory structure in #{projpath} into the svn repository identified by #{repofileurl}"
system "svnadmin create #{repopath}"
system "svn import #{projpath} #{repofileurl} -m 'new rails project'"

msgputs "now removing the projpath directory tree we used to do the svn import"
FileUtils.rm_rf projpath

msgputs "checking out a fresh copy of the trunk from our svn repository"
system "svn co #{trunkurl} #{projname}"

msgputs "changing into the freshly checked out project and doing all the svn operations we need to do initally"
Dir.chdir projpath

msgputs "ignoring a type of Mac OSX file"
system "svn propset svn:ignore -R '.DS_Store' ."

msgputs "ignoring log files"
system "svn remove log/*"
system "svn commit -m 'removing all log files from subversion'"
system 'svn propset svn:ignore "*.log" log/'
system "svn update log/"
system "svn commit -m 'Ignoring all files in /log/ ending in .log'"

msgputs "ignoring tmp files"
system "svn propset svn:ignore '*' tmp/"
system "svn propset svn:ignore '*' tmp/cache/"
system "svn propset svn:ignore '*' tmp/sessions/"
system "svn propset svn:ignore '*' tmp/sockets/"
system "svn update"
system "svn commit -m 'ignore temp files'"

msgputs "ignoring db files"
system 'svn propset svn:ignore "*.db" db/'
system "svn update db/"
system "svn commit -m 'Ignoring all files in /db/ ending in .db'"
system "svn move config/database.yml config/database.example"
system "svn commit -m 'Moving database.yml to database.example to provide a template for anyone who checks out the code'"
system 'svn propset svn:ignore "database.yml" config/'
system "svn update config/"
system "svn commit -m 'Ignoring database.yml'"
Dir.chdir "#{projpath}#{sep}config"
FileUtils.cp("database.example", "database.yml")

# woo hoo, we're done
puts "\n\n********************************************************************************"
puts "Congratulations, your rails project #{projname} is created."
puts "Your subversion repository is located at #{repopath}"
puts "and can be accessed via the URL: #{repofileurl}."
puts "The head revision is checked out to #{projpath}."
puts "To run your server, cd to this directory and run 'ruby script/server'"
puts "********************************************************************************\n\n"
