#!/usr/bin/ruby

#
# flip.rd 
#

require 'ap.rb'
require 'gui.rb'
require 'ftools'

PATH = "tmp/aps";

include Ncurses
include Ncurses::Form

END {
	Ncurses.endwin
}

# This is the user interface of the program.
# Abstract class, use TxtUI or CursesUI
class UI
end

# CursesUI: Ncurses-based user interface.
# After calling getCommand(), it returns only when the program is to be terminated.
# Instance variables:
# @apListBox: holds items of current dir, associated with the attrs of the items
# @entryListBox: hold the selected dir's attrs
# @active: the active listbox (@apListBox or @entryListBox)
# @entryOrig: when editing an entry, this one holds the original attrs for fast reverting
# @apPanel, @entryPanel, @statusRow, @commandWin: windows of the UI
class CursesUI <UI
	include Util
	include NcursesEx
	def initialize()
		for i in 1 .. 15  # SIGHUP .. SIGTERM
		  if trap(i, "SIG_IGN") != 0 then  # 0 for SIG_IGN
		    trap(i) {|sig| onsig(sig) }
		  end
		end
		@apList = APList.new(PATH);

		@state=:MAIN
		Ncurses.initscr; 
		Ncurses.cbreak; 
		Ncurses.noecho; 
		Ncurses.nonl; 
		Ncurses.stdscr.intrflush(false); 
		Ncurses.stdscr.keypad(true)
	end


	# getCommand: main loop of execution. Returns only on the end request of the program (Quit).
	def getCommand
		mkWins if ! defined?(@apListBox)
		updateCommandRow "Commands==> (I)nfo (C)lients (Q)uit exec-(l)ocal exec-(r)emote (B)uild-config"
		refreshAps;
		finished=false; processed=false
		while (true)
			refreshAps;
			break if finished
			processed=false
			#updateStatusRow(@active.key)
			c=@active.getch
			c,processed,finished=global_hook_keypressed(c) if defined?(global_hook_keypressed)
			break if finished
			next if processed
			processed=navigate(@active,c)
			next if processed
			case c
			when ?\C-L # ^L
				refreshUI; next
			when ?q, ?Q
				finished=true; next
			end
			if (@active==@apListBox)
				# Directory
				case c
				when 9 # TAB
					next if @entryListBox.items.length==0
					selectListBox(@entryListBox)
				when ?i, ?I
					showAPInfo; 
				when ?l
					execute_local
				when ?L
					execute_local_all
				when ?r 
					execute_remote
				when ?R 
					execute_remote_all
				when ?e
					@apListBox.value.enable( !@apListBox.value.enabled? );
					#puts "Enabled: #{@apListBox.value.enabled?}"
				when ?b, ?B
					build_config_tgz
					refreshAps
				when KEY_ENTER, 13 # Enter
					debug("Enter. Selected==#{@apListBox.selected}")
				end
				refreshUI
			else (@active==@entryListBox)
				case c
				when 9 # TAB 
					next if @apListBox.items.length==0
					selectListBox(@apListBox)
				when KEY_ENTER, 13 # Enter
					debug("Enter. Selected==#{@apListBox.selected}")
				end
			end
		end
		Ncurses.endwin
		exit(0)
	end

	# get a string from the user
	def getVal(prompt,default="")
		width=[[prompt.length+80,Ncurses.COLS-4].min,40].max
		h = 7;
		w=BoxedWin.new(nil,h,width,(Ncurses.LINES-h)/2,(Ncurses.COLS-width)/2,ATTR_NORMAL)
		w.title( " "+prompt+" ");
		w.refresh
		edt=BoxedWin.new(w,3,width-4,2,1)
		#~ sub.bkgd(?A)
		ret=getLine(edt,default).chomp
		#edt.destroy
		w.destroy
		#~ refresh
		return ret
	end

	def error(s); showMessage(s); refreshUI if defined? @apPanel end
 private

	#
	# redraw the complete window frameset 
	#

	def refreshUI; @apPanel.redrawwin; @entryPanel.redrawwin; @entryListBox.refresh; @apListBox.refresh end
	def finalize; endwin end
	def onsig(sig); endwin; exit(sig) end

	#
	# complete window framset that comes up initialy
	#

	def mkWins
		freerows=4
		@apPanel=BoxedWin.new(nil,Ncurses.LINES-freerows,Ncurses.COLS/2,0,0,ATTR_NORMAL)
		@apListBox=ListBox.new(@apPanel)
		@entryPanel=BoxedWin.new(nil,Ncurses.LINES-freerows,Ncurses.COLS-Ncurses.COLS/2,0,Ncurses.COLS/2,ATTR_NORMAL)
		@entryListBox=ListBox.new(@entryPanel)
		@entryListBox.showSelected(false)
		@statusRow=WINDOW.new(1,Ncurses.COLS,Ncurses.LINES-freerows,0)
		@statusRow.bkgd(ATTR_NORMAL); @statusRow.refresh
		@commandWin=BoxedWin.new(nil,freerows-1,Ncurses.COLS,Ncurses.LINES-freerows+1,0,ATTR_NORMAL)
		@active=@apListBox

		@apPanel.title( "Available Accesspoints", :LEFT );
	end
	
	#
	# fill the status row with a string 
	#

	def updateStatusRow(s)
		s="" if s.nil?
		@statusRow.mvaddstr(0,0,s); @statusRow.clrtoeol; @statusRow.refresh
	end

	#
	# fill the command row with a string
	#

	def updateCommandRow(s)
		s="" if s.nil?
		@commandWin.mvaddstr(0,0,s); @commandWin.clrtoeol; @commandWin.refresh
	end

	#
	# navigate through the selected listbox and aktivate selected item
	#

	def navigate(listbox,ch)
		processed=false
		(listbox.next; processed=true) if ch==KEY_DOWN
		(listbox.prev; processed=true) if ch==KEY_UP
		return processed
	end

	#	
	# change selected listbox (to TAB key) 
	# 

	def selectListBox(panel)
		return if panel==@active
		@active.showSelected(false); @active.refresh
		@active=panel; @active.showSelected
		dirCmds="Commands==> (I)nfo (C)lients (Q)uit exec-(l)ocal exec-(r)emote (B)uild-config"
		entryCmds="Commands==> (Q)uit"
		updateCommandRow(@active==@apListBox ? dirCmds : entryCmds)
	end

	######################################################################
	# FUNCTIONALITY

	#
	# build the config.tgz for the currently selected accesspoint
	# 

	def build_config_tgz
		curAp = @apListBox.value;
		@entryListBox.empty
		ret = curAp.build_config_tgz( "/tftpboot" ).split( "\n" );
		@entryListBox.add( " Building config.tgz for #{curAp.mac} in /tftpboot" )
		

		ret.each { |line|
			@entryListBox.add( "-> #{line}", {});
		}
		refreshUI;
		
	end

	def refreshAps
		@apList.refresh

		apHash = @apList.apHash;
		apHash.each_value { |ap|
			if !@apListBox.has_value?(ap) then
				@apListBox.add( ap.status_line, ap );
			else 
				@apListBox.change( ap.status_line, ap)
			end
		}
		@apListBox.refresh
	end	

	#
	# execute a command in the local selected accesspoint directory 
	#

	def execute_local
		curAp = @apListBox.value;
		cmd = getVal("exec-local in #{curAp.mac}"); 
		return if cmd.nil? or cmd == "";

		a = curAp.execute_local(cmd).split( "\n" );
		@entryListBox.empty
		a.each { |line|
			@entryListBox.add( "#{line}", {});
		}
	end

	#
	# execute a command remote on the selected accesspoint
	#

	def execute_remote
		curAp = @apListBox.value;
		cmd = getVal("exec-remote on #{curAp.ip}");
		return if cmd.nil? or cmd == "";

		a = curAp.execute_remote(cmd).split( "\n" );
		@entryListBox.empty
		a.each { |line|
			@entryListBox.add( "#{line}", {});
		}
	end

	#
	# execute a command for all accesspoints locally
	# 

	def execute_local_all
		cmd = getVal("exec-local-all"); 
		return if cmd == ""
		output = @apList.execute_local_all(cmd);
		@entryListBox.empty
		output.each_key { |mac|
			lines = output[mac].split("\n");
			@entryListBox.add( "===>> #{mac} <<===" );
			lines.each { |line|
				@entryListBox.add( "#{line}", {});
			}
		}
		refreshUI; 
	end

	#
	# execute a command for all accesspoints remote on the accesspoints
	#

	def execute_remote_all
		cmd = getVal("exec-remote-all");
		return if cmd == ""
		output = @apList.execute_remote_all(cmd);
		@entryListBox.empty
		output.each_key { |ip|
			lines = output[ip].split("\n");
			@entryListBox.add( "===>> #{ip} <<===" );
			lines.each { |line|
				@entryListBox.add( "#{line}", {});
			}
		}
		refreshUI;
	end

	#
	# show a basic inofrmation about the selected accesspoints
	#

	def showAPInfo

		curAP = @apListBox.value;
		@entryListBox.empty

		curAP.load_statistics;
		a = curAP.info.split( "\n" );
		a.each { |line|
			@entryListBox.add( "#{line}", {});
		}
		@entryListBox.refresh;
	end
end


# Program initialization. The real work is passed to @ui.
class Program
	def initialize(uitype)
		@ui=uitype==:CURSES ? CursesUI.new() : TxtUI.new()
		cmd=nil
		# XXX Datastructs XXX 
		while cmd!=:QUIT
			cmd=@ui.getCommand
		end
	end
end

##################################################
# Main
# One arg: UI type (curses or txt). Default: curses
#
# Possible improvements: cmdline args for authenticating.

require 'getoptlong'

if __FILE__==$0
	opts=GetoptLong.new(
		["--host", 		GetoptLong::REQUIRED_ARGUMENT],
		["--name", "-n",		GetoptLong::REQUIRED_ARGUMENT],
		["--passwd", "-p",	GetoptLong::REQUIRED_ARGUMENT],
		["--base", "-b",		GetoptLong::REQUIRED_ARGUMENT]
	)
	host=nil; name=nil; pass=nil; base=nil
	opts.each {|opt,arg|
		case opt
		when "--host"; host=arg
		when "--name"; name=arg
		when "--passwd"; pass=arg
		when "--base"; base=arg
		else
			$stderr.puts "Usage: ldap_browser.rb [--host <hostname:port>] [--name <root dn>] [--passwd <root pw>] [--base <basedir>]"
			exit 1
		end
	}
	p=Program.new(ARGV[0].nil? ? :CURSES : ARGV[0].upcase.intern)
end
