#!/usr/bin/ruby

# ldap_browser.rb: NCurses-based LDAP browser.
#
# Changelog
#
# 2003-06-16: v0.02
#   updated to work with slapd 2.1.x (re-read dn after saving a new entry)

require 'ncurses'

include Ncurses
include Ncurses::Form

END {
	Ncurses.endwin
}


def debug(s)
	begin
		File.open("debug.log","a") {|f| f.puts s}
	rescue
	end
end

# Useful functions for ncurses
module NcursesEx
	include Ncurses
	ATTR_NORMAL=A_NORMAL
	ATTR_SELECTED=A_REVERSE
	# max: max. size of the string to return
	def getLine(win,default="",max=0)
		w=win.kind_of?(BoxedWin) ? win.content : win
		w.clear; w.move(0,0)
		# x: current position in the string, first: first visible char's position in the string
		# x-first: cursor position in the window
		width=myGetMaxYX(w)[1]; s=default.dup; x=s.length
		first=[s.length-width+1,0].max; w.addstr(default[first..first+width-1])
		scrollLeft=proc {
			#~ debug "scrollLeft: x==#{x}, first==#{first}, width==#{width}, s.size==#{s.size}"
			w.move(0,0); w.delch; first+=1
			if (s.size>=width+first)
				#~ debug "char to write at the end: '#{s[width+first-1].chr}'"
				w.mvinsch(0,width-1,s[width+first-1])
			end
			w.move(0,x-first)
		}
		scrollRight=proc {
			break if first==0
			#~ debug "scrollRight: x==#{x}, first==#{first}, width==#{width}, s.size==#{s.size}"
			first-=1
			w.mvinsch(0,0,s[first])
			w.move(0,x-first)
		}
		finished=false; processed=false
		while (true)
			break if finished
			c=w.getch
			c,processed,finished=global_hook_keypressed(c) if defined?(global_hook_keypressed)
			break if finished
			next if processed
			case c
			when KEY_BACKSPACE,8,127
				next if x<=0
				(1..8).each {scrollRight.call} if x-first==0 # cursor at first char
				w.move(0,x-first-1); w.delch(); x-=1; s[x]=""
				if (s.size>=width+first)
					w.mvinsch(0,width-1,s[width+first-1])
					w.move(0,x-first)
				end
			when KEY_LEFT
				next if x<=0
				scrollRight.call if x-first==0 # cursor at first char
				x-=1; w.move(0,x-first)
			when KEY_RIGHT
				next if x>=s.size
				scrollLeft.call if x-first==width-1 # cursor at last char
				x+=1; w.move(0,x-first)
			when ?\C-a, KEY_HOME
				w.move(0,0); w.clrtoeol; w.addstr(s[0..width-1]); w.move(0,0)
				first=0; x=0
			when ?\C-e, KEY_END
				w.move(0,0); w.clrtoeol; len=[width-1,s.size].min
				w.addstr(s[-len..-1])
				x=s.size; first=s.size-len
			when 32..255
				next if s.size>=max && max!=0
				# if x==size -> appending, else inserting
				addIt=(x==s.size) ? proc {w.addch(c)} : proc {w.insch(c); w.move(0,x-first)}
				scrollLeft.call if x-first>=width-1 # doesn't fit: need to scroll
				if x==s.size then s+=c.chr else s[x]=c.chr+s[x].chr end
				x+=1
				addIt.call
			when 13,KEY_ENTER
				finished=true
			end
		end
		return s
	end
	def wrapString(aString,maxlen)
		s=aString.dup
		arr=[]
		while s.length>maxlen
			i=s[0..maxlen].rindex(" ")
			arr.push s.slice!(0..(i.nil? ? maxlen : i))
		end
		arr.push s if s.length>0
		return arr
	end
	def showMessage(msg)
		width=[[msg.length+4,60].min,20].max
		msgarr=wrapString(msg,width-4)
		h=6+msgarr.length
		w=BoxedWin.new(nil,h,width,(Ncurses.LINES-h)/2,(Ncurses.COLS-width)/2,ATTR_NORMAL)
		msgarr.each_index {|i| w.mvaddstr(1+i,1,msgarr[i])}
		s="Press any key..."
		w.mvaddstr(h-3,(width-s.length)/2-1,s)
		w.mvchgat(h-3,(width-s.length)/2-1,s.length,ATTR_SELECTED,0,nil)
		w.getch
		w.destroy
	end
	def myGetYX(w)
		x=[]; y=[]; getyx(w,y,x)
		return [y[0],x[0]]
	end
	def myGetMaxYX(w)
		x=[]; y=[]; w.getmaxyx(y,x)
		return [y[0],x[0]]
	end
end

module Util
	# Needs an error() fn to display the error message.
	def toInt(i,errmsg)
		begin
			Integer("nil") if i.nil?
			return Integer(i)
		rescue
			error(errmsg)
			raise
		end
	end
end

# This is the user interface of the program.
# Abstract class, use TxtUI or CursesUI
class UI
	# instance variables:
	# @browser: object for navigating in the LDAP directory
	# @entry: the entry's object which is currently under edited
	# @state: :MAIN, :ADD (in adding an attr), :EDIT (in editing an attr)
	attr_writer :browser # set by Program
	# Directory handling.
	# dir: first, last are not implemented.
	def dir(first,last)
		@browser.getEntries {|e| yield(e)	}
	end
	def chDir(i); @browser.chDir(i==0 ? ".." : i-1) end
	# passes (key,value) pairs to its block
	def entryDetails(i)
		h=@browser.getEntry(i==0 ? nil : i-1)
		(error("entryDetails error"); return) if h.nil?
		h.each {|key,valarr|
			valarr.each {|val| yield(key,val)}
		}
	end
	def entryDN(i); return @browser.resolveDN(i==0 ? nil : i-1) end
	def addEntry(dn)
		@state=:ADD; @entry=LDAPEntry.new(dn+", "+@browser.curDir,{})
	end
	def editEntry(i)
		@state=:EDIT; e=@browser.getEntry(i-1).dup; e.delete("dn")
		@entry=LDAPEntry.new(@browser.resolveDN(i-1),e)
	end
	def copyEntry(i,newname); @browser.copyEntry(i-1,newname); end
	def renameEntry(i,newname); @browser.renameEntry(i-1,newname) end
	def deleteEntry(i); @browser.deleteEntry(i-1) end
end

# BoxedWin consists of two windows: one for the 1-char wide border (@win), one for the content (@content)
class BoxedWin
	include NcursesEx
	include Ncurses

	attr_reader :win, :content
	# parent: can be nil, BoxedWin or Ncurses::WINDOW
	def initialize(parent,h,w,y,x,bkgd=nil)
		@win = parent
		if parent.nil? then @win = WINDOW.new(h,w,y,x); end
		if parent.kind_of?(BoxedWin) then @win = parent.content; end
		
		@win.derwin(h,w,y,x)
		@win.bkgd(bkgd) if !bkgd.nil?
		@win.box(ACS_VLINE,ACS_HLINE)
		@content=@win.derwin(h-2,w-2,1,1)
		@content.keypad(true)
		@content.bkgd(bkgd) if !bkgd.nil?
		@content.move(0,0)
		@win.refresh; 
	end
	def title(s,align=:CENTER)
		@win.box(ACS_VLINE,ACS_HLINE)
		len=[s.size,myGetMaxYX(@win)[1]-2].min
		case align
			when :LEFT; @win.mvaddstr(0,1,s[0...len])
			when :RIGHT; @win.mvaddstr(0,myGetMaxYX(@win)[1]-1-len,s[0...len])
			when :CENTER; @win.mvaddstr(0,(myGetMaxYX(@win)[1]-len)/2,s[0...len])
			else; error "BoxedWin::title(): Hibás align!"
		end
		@win.refresh
	end
	def x; return myGetYX(@content)[1]; end
	def y; return myGetYX(@content)[0]; end
	def maxX; return myGetMaxYX(@content)[1]; end
	def maxY; return myGetMaxYX(@content)[0]; end
	def refresh; @win.wnoutrefresh; @content.wnoutrefresh; end
	def redrawwin; @win.redrawwin; @content.redrawwin; refresh end
	def destroy; @content.delwin; @win.delwin end
	def method_missing(sym,*args)
		@content.send sym,*args
	end
end

# ListBox: contains and displays an array of items.
# An item contains it string to display (key), and an associated object (value)
class ListBox < BoxedWin
	include NcursesEx
	# items: Array of list items: [[key1, val1],[key2,val2]...]
	# win: ncurses window of listbox
	# first: item number of the first visible item
	# selected: item number of the selected item
	attr_reader :items, :win, :first, :selected
	
	def initialize(win)
		debug "HAAAAALOOO *********** HUHU!!! ***"
		debug win;


		@win=(win.kind_of?(BoxedWin) ? win.content : win)
		@showSelected=true # visually highlight the selected row
		empty
#		@win.clear; @win.refresh
	end
	# add item(s)
	def add(arg, arg2=nil)
		if arg.kind_of? Array then @items=@items+arg
		else @items.push([arg,arg2])
		end
	end
	# selected item's key and value
	def key; return @items.size>0 ? @items[@selected][0] : nil end
	def value; return @items.size>0 ? @items[@selected][1] : nil end
	def empty
		@items=[]; @selected=0; @first=0
	end
	def refresh
		@win.erase
		(@win.refresh; return) if @items.size==0
		@first=@items.size-1 if @first>=@items.size
		@selected=@items.size-1 if @selected>=@items.size
		error "ListBox.refresh internal problem!" if (@selected<@first || @selected>@first+maxY)
		(0..maxY).each {|i| refreshRow(i)}
		@win.refresh
	end
	def next
		return if @selected>=@items.size-1
		scrollUp if @selected-@first>=maxY-1
		@selected+=1; selectChanged(@selected-1)
	end
	def prev
		return if @selected<=0
		scrollDown if @selected-@first<=0
		@selected-=1; selectChanged(@selected+1)
	end
	def find(k)
		debug("Listbox.find(#{k})")
		@items.each_index {|i| (debug("found: #{i}"); select(i); return i) if @items[i][0]==k}
		return nil
	end
	# Select 'i'th row
	def select(i)
		return if i>=@items.size || i<0
		if i>=@first && i<@first+maxY
			old=@selected
			@selected=i
			selectChanged(old)
		else
			@selected=i
			setTop([i,@items.size-maxY].min)
		end
	end
	# Set the first row of the list in the window to i.
	def setTop(i)
		@first=i
		debug("setTop: first==#{@first}, selected==#{@selected}")
		refresh
	end
	# Highlight the selected item or not
	def showSelected(val=true)
		@showSelected=val
		refreshRow(@selected-@first)
	end
 private
	def selectChanged(old)
		refreshRow(old-@first)
		refreshRow(@selected-@first)
	end
	def scrollUp
		return if @first>=@items.size-1
		@win.move(0,0); @win.deleteln
		@first+=1
		select(@first) if @selected<@first
		refreshRow(maxY-1)
	end
	def scrollDown
		return if @first<=0
		@win.move(0,0); @win.insertln
		@first-=1
		select(@first+maxY-1) if @selected>=@first+maxY
		refreshRow(0)
	end
	def refreshRow(screenrow,attr=nil)
		return if screenrow<0 || screenrow>=maxY
		s=screenrow+@first>@items.size-1 ? " "*maxX : @items[screenrow+@first][0]
		@win.mvaddstr(screenrow,0,s[0..maxX-1])
		attr=ATTR_SELECTED if attr.nil? &&
			@selected==screenrow+@first && @showSelected
		attr=ATTR_NORMAL if attr.nil?
		@win.mvchgat(screenrow,0,-1,attr,0,nil)
		@win.touchline(screenrow,1)
	end
	def maxX; return myGetMaxYX(@win)[1]; end
	def maxY; return myGetMaxYX(@win)[0]; end
	def method_missing(sym,*args)
		@win.send sym,*args
	end
end

# CursesUI: Ncurses-based user interface.
# After calling getCommand(), it returns only when the program is to be terminated.
# Instance variables:
# @dirListBox: holds items of current dir, associated with the attrs of the items
# @entryListBox: hold the selected dir's attrs
# @active: the active listbox (@dirListBox or @entryListBox)
# @entryOrig: when editing an entry, this one holds the original attrs for fast reverting
# @dirPanel, @entryPanel, @statusRow, @commandWin: windows of the UI
class CursesUI <UI
	include Util
	include NcursesEx
	def initialize()
		for i in 1 .. 15  # SIGHUP .. SIGTERM
		  if trap(i, "SIG_IGN") != 0 then  # 0 for SIG_IGN
		    trap(i) {|sig| onsig(sig) }
		  end
		end
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
		mkWins if ! defined?(@dirListBox)
		updateCommandRow "(C)opy (R)ename (A)dd (D)elete (Q)uit"
		listDir(0,1000); 
		finished=false; processed=false
		while (true)
			break if finished
			processed=false
			updateStatusRow(@active.key)
			c=@active.getch
			c,processed,finished=global_hook_keypressed(c) if defined?(global_hook_keypressed)
			break if finished
			next if processed
			processed=navigate(@active,c)
			showAPInfo if @active==@dirListBox && processed
			next if processed
			case c
			when ?\C-L # ^L
				refreshUI; next
			when ?q, ?Q
				finished=true; next
			end
			if (@active==@dirListBox)
				# Directory
				case c
				when 9 # TAB
				when ?c, ?C
				when ?r, ?R
				when ?a, ?A
				when ?d, ?D
				when KEY_ENTER, 13 # Enter
					debug("Enter. Selected==#{@dirListBox.selected}")
				end
			else # active == @entryListBox
				# jaja XXX
			end
		end
		Ncurses.endwin
		exit(0)
	end

	# get a string from the user
	def getVal(prompt,default="")
		prompt+=":"
		width=[[prompt.length+4,Ncurses.COLS-4].min,40].max
		promptArr=wrapString(prompt,width)
		h=6+promptArr.length
		w=BoxedWin.new(nil,h,width,(Ncurses.LINES-h)/2,(Ncurses.COLS-width)/2,ATTR_NORMAL)
		promptArr.each_index {|i| w.mvaddstr(i,1,promptArr[i])}
		w.refresh
		edt=BoxedWin.new(w,3,width-4,2,1)
		#~ sub.bkgd(?A)
		ret=getLine(edt,default).chomp
		edt.destroy
		w.destroy
		#~ refresh
		return ret
	end

	def error(s); showMessage(s); refreshUI if defined? @dirPanel end
 private
	def refreshUI; @dirPanel.redrawwin; @entryPanel.redrawwin end
	def finalize; endwin end
	def onsig(sig); endwin; exit(sig) end

	def mkWins
		freerows=4
		@dirPanel=BoxedWin.new(nil,Ncurses.LINES-freerows,Ncurses.COLS/2,0,0,ATTR_NORMAL)
		@dirListBox=ListBox.new(@dirPanel)
		@entryPanel=BoxedWin.new(nil,Ncurses.LINES-freerows,Ncurses.COLS-Ncurses.COLS/2,0,Ncurses.COLS/2,ATTR_NORMAL)
		@entryListBox=ListBox.new(@entryPanel)
		@entryListBox.showSelected(false)
		@statusRow=WINDOW.new(1,Ncurses.COLS,Ncurses.LINES-freerows,0)
		@statusRow.bkgd(ATTR_NORMAL); @statusRow.refresh
		@commandWin=BoxedWin.new(nil,freerows-1,Ncurses.COLS,Ncurses.LINES-freerows+1,0,ATTR_NORMAL)
		@active=@dirListBox
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
		@dirPanel.title("Available Access Points",:LEFT)
		@dirListBox.empty; 
		@dirListBox.add("..", "Ich bin zwei doppelpunkt" )
		@dirListBox.add("Huhu", "Ich bin 4 Buchstabig" )
		@dirListBox.add("Fnord","wissen schon")
		@dirListBox.refresh
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
		dirCmds="(C)opy (R)ename (A)dd (D)elete (Q)uit"
		entryCmds="(A)dd (M)odify (D)elete (S)ave (R)evert (Q)uit"
		updateCommandRow(@active==@dirListBox ? dirCmds : entryCmds)
	end

	def showAPInfo
		@entryListBox.empty
		@entryListBox.add( "Somthin .. again...#{@dirListBox.value}", {});
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

# Holds one editable LDAP entry.
class LDAPEntry
	attr_reader :dn, :entry
	attr_accessor :modified # need to reset when saving...
	# entry: element of a LDAP::Conn.search2(...) array
	def initialize(dn,entry)
		@dn=dn; @attrs=[]
		entry.each {|key,valarr|
			valarr.each {|val| @attrs.push([key,val]) }
		}
		@attrs.sort!; @modified=false
	end

	def count; return @attrs.size end
	def dup; return LDAPEntry.new(@dn,getAttrsHash) end

	def attributes
		return @attrs if !block_given?
		@attrs.each{|arr| yield arr[0],arr[1] }
	end
	
	def addAttribute(attr,val)
		@modified=true
		@attrs.push([attr,val]); @attrs.sort!
	end
	# if called with 1 arg, this is the ordinal of the item needed to delete
	# if called with 2 args, they are the [key, value] of the item needed to delete
	def deleteAttribute(*args)
		@modified=true
		return @attrs.delete_at(args[0]) if args.size==1
		return @attrs.delete([args[0],args[1]])
	end
	def modifyAttribute(i,newVal)
		@modified=true
		key=@attrs[i][0]
		@attrs.delete_at(i)
		@attrs.push([key,newVal]); @attrs.sort!
	end
	def getAttrsHash
		ret={}
		@attrs.each {|key,val|
			ret[key]=[] if !(ret.has_key? key)
			ret[key].push val
		}
		return ret
	end
	def correctDN(dn) # you must not change dn semantically! (called from LDAPBrowser.addEntry)
		@dn=dn
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
