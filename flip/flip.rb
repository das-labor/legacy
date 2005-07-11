#!/usr/bin/ruby

# ldap_browser.rb: NCurses-based LDAP browser.
#
# Changelog
#
# 2003-06-16: v0.02
#   updated to work with slapd 2.1.x (re-read dn after saving a new entry)

require 'ap.rb'
require 'gui.rb'

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
		updateCommandRow "Commands==> (I)nfo (C)lients (Q)uit"
		listDir(0,1000); 
		finished=false; processed=false
		while (true)
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
					showMessage ( "Current selected AP information" ); 
					refreshUI; 
					showAPInfo; 
				when ?c, ?C
					showMessage ( "Current Clients on AP" ); 
					refreshUI;
					showAPClients;
				when ?l
					execute_local;
				when ?L
					cmd = getVal("exec-local-ALL");
					refreshUI;
				when ?r 
					cmd = getVal("exec-remote");
					refreshUI;
				when ?R 
					cmd = getVal("exec-remote-ALL");
					refreshUI;
				when KEY_ENTER, 13 # Enter
					debug("Enter. Selected==#{@apListBox.selected}")
				end
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
#		width=[[prompt.length+4,Ncurses.COLS-4].min,40].max
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
	def refreshUI; @apPanel.redrawwin; @entryPanel.redrawwin; @entryListBox.refresh; @apListBox.refresh end
	def finalize; endwin end
	def onsig(sig); endwin; exit(sig) end

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
	end
	
	def updateStatusRow(s)
		s="" if s.nil?
		@statusRow.mvaddstr(0,0,s); @statusRow.clrtoeol; @statusRow.refresh
	end
	def updateCommandRow(s)
		s="" if s.nil?
		@commandWin.mvaddstr(0,0,s); @commandWin.clrtoeol; @commandWin.refresh
	end

	def listDir(first,last)
		@apPanel.title("Available Access Points",:LEFT)
		@apListBox.empty; 
		@apList.refresh;
		apHash = @apList.apHash;
		apHash.each_value { |ap|
			@apListBox.add( "MACADDR: #{ap.mac} IPADDR: #{ap.ip}", ap );
		}
		@apListBox.refresh
	end

	def navigate(listbox,ch)
		processed=false
		(listbox.next; processed=true) if ch==KEY_DOWN
		(listbox.prev; processed=true) if ch==KEY_UP
		return processed
	end
	
	# change selected listbox (to TAB key)
	def selectListBox(panel)
		return if panel==@active
		@active.showSelected(false); @active.refresh
		@active=panel; @active.showSelected
		dirCmds="Commands==> (I)nfo (C)lients (Q)uit"
		entryCmds="Commands==> (Q)uit"
		updateCommandRow(@active==@apListBox ? dirCmds : entryCmds)
	end
	
	def execute_local
		curAp = @apListBox.value;
		cmd = getVal("exec-local in #{curAp.mac}"); 
		@entryListBox.empty
		a = curAp.execute_local(cmd).split( "\n" ) if cmd != "";

		a.each { |line|
			@entryListBox.add( "#{line}", {});
		}
		
		

		refreshUI; 
	end

	def showAPInfo
		curAP = @apListBox.value;
		@entryListBox.empty
		a = curAP.info.split( "\n" );
		a.each { |line|
			@entryListBox.add( "#{line}", {});
		}
		@entryListBox.refresh;
	end
	
	def showAPClients
		curAP = @apListBox.value;
                @entryListBox.empty
		a = curAP.clients.split( "\n" );
		a.each { |line|
			@entryListBox.add( "#{line}", {});
		}
		@entryListBox.refresh;
	end
end

# TxtUI: text-based interface
# getCommand returns after every processed command.
class TxtUI < UI
	include Util
	def initialize()
		@state=:MAIN
	end

	def getVal(prompt,default="")
		defstr=default.empty? ? "" : " [#{default}]"
		$stdout.write("#{prompt}#{defstr}: ")
		ret=$stdin.gets().chomp
		return ret.empty? ? default : ret
	end

	# Main menu.
	def getCommand
		return entryCommand if @state==:ADD || @state==:EDIT
		cmds=[]
		puts "\nCurrent dir: #{@browser.curDir}"
		cmds.push "(L)ist directory"
		cmds.push "(C)hange dir <num> (0: parent)"
		cmds.push "(S)how entry <num>"
		cmds.push "(D)elete entry <num>"
		cmds.push "(E)dit entry <num>"
		cmds.push "(A)dd entry <relative dn>"
		cmds.push "C(o)py entry <num> <new abs. dn>"
		cmds.push "(R)ename entry <num> <new relative dn>"
		cmds.push "(Q)uit"
		printInColumns(cmds,2)
		printf "> "
		line=$stdin.gets()
		if line !~ /^(.) *([0-9]+)? *([0-9]+)?/
			puts "Invalid command line!"
			return :INVALID
		end
		#~ puts ("cmd: #{$1}, #{$2}, #{$3}, '#{$&}'")
		cmd=$1.upcase
		begin
			case cmd
				when "L"
					i=$2.nil? ? 0 : toInt($2,"Invalid parameters. Example: L 1 5")
					j=$3.nil? ? 100 : toInt($3,"Invalid parameters. Example: L 1 5")
					listDir(i,j)
				when "C"
					i=toInt($2,"Invalid parameter. Example: #{cmd} 3")
					chDir(i)
				when "S"
					i=toInt($2,"Invalid parameter. Example: #{cmd} 3")
					displayEntry(i)
				when "D"
					i=toInt($2,"Invalid parameter. Example: #{cmd} 3")
					deleteEntry(i)
				when "R"
					line=~ /^. ([0-9]+) (.*)$/
					i=toInt($1,"Invalid parameter. Example: #{cmd} 3 ou=ACME")
					renameEntry(i,$2)
				when "O"
					line=~ /^. ([0-9]+) (.*)$/
					i=toInt($1,"Invalid parameter. Example: #{cmd} 3 ou=ACME,dc=foo,dc=org")
					copyEntry(i,$2)
				when "A"
					line=~ /^. (.*)$/
					addEntry($1) if !$1.nil?
					puts "Invalid command line!"
					return :INVALID
				when "E"
					i=toInt($2,"Invalid parameter. Example: #{cmd} 3")
					editEntry(i)
				when "Q"
					return :QUIT
			end
		rescue
			return :INVALID
		end
		return :SUCCESS
	end

	def error(errmsg); puts errmsg end
	def message(msg); puts msg end
 private
	# :ADD or :EDIT state menu
	# The current entry is in @entry
	def entryCommand
		cmds=[]
		puts "\nCurrent entry: #{@entry.dn}"
		cmds.push "(L)ist attributes"
		cmds.push "(A)dd attribute value <attr>=<value>"
		cmds.push "(M)odify attribute value <num> <newval>"
		cmds.push "(D)elete attribute value <num>"
		cmds.push "(S)ave changes and exit"
		cmds.push "(E)xit without saving"
		printInColumns(cmds,2)
		printf "> "
		line=$stdin.gets()
		if line !~ /^(.)/
			error "Invalid command line!"; return :INVALID
		end
		#~ puts ("cmd: #{$1}, #{$2}, #{$3}, '#{$&}'")
		cmd=$1.upcase
		begin
			case cmd
				when "L"
#					listAttrs()
				when "A"
					line=~ /^. ([^=]*)=(.*)$/
					addAttr($1,$2)
				when "M"
					line=~ /^. ([0-9]+) (.*)$/
					i=toInt($1,"Invalid parameter. Example: #{cmd} 3 bar")
					modifyAttr(i,$2)
				when "D"
					line=~ /^. ([0-9]+)/
					i=toInt($1,"Invalid parameter. Example: #{cmd} 3")
					deleteAttr(i)
				when "S"
					@entry=nil; @state=:MAIN
				when "E"
					@entry=nil; @state=:MAIN
			end
		rescue
			return :INVALID
		end
		return :SUCCESS
	end
	
	def displayEntry(i)
		puts "#{entryDN(i)}\n#{'-'*entryDN(i).size}"
		entryDetails(i) {|key,val| printf("%-14s -> %s\n",key,val) }
	end

	# Print ar's elements in colnum columns.
	def printInColumns(ar,colnum)
		rownum=(ar.size-1)/colnum+1
		# res: array of rows, where a row is the array of strings in that row
		res=[]; rownum.times {res.push(Array.new)}
		# lenArr: for each column, holds the array lengths of its items
		lenArr=[]; colnum.times {lenArr.push(Array.new)}
		ar.each_index {|i| 
			res[i%rownum].push(ar[i]); lenArr[i/rownum].push(ar[i])
		}
		colLen=[] # Holds the width of each column
		lenArr.each {|arr|
			colLen.push((arr.max {|i,j| i.size <=> j.size}).size)
		}
		res.each {|arr|
			arr.each_index {|i|
				printf("%*s  ",-colLen[i],arr[i].nil? ? "" : arr[i])
			}
			printf("\n")
		}
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
