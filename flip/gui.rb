###############################################################################
# Ncurses GUI for flip.rb 

require 'ncurses'

include Ncurses
include Ncurses::Form

#
# open debug.log and specify command for including information into the logfile
#

def debug(s)
	begin
		File.open("debug.log","a") {|f| f.puts s}
	rescue
	end
end

#
# Useful functions for ncurses
#

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
		arr = Array.new;
		while s.length>maxlen
			i=s[0..maxlen].rindex(" ")
			arr.push s.slice!(0..(i.nil? ? maxlen : i))
		end
		arr.push s if s.length>0
		return arr
	end

	#
	# show a message in the center of the viewarea and kill it with a pressed button
	#

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

	#
	# return y and x coordinates 
	#

	def myGetYX(w)
		x=[]; y=[]; getyx(w,y,x)
		return [y[0],x[0]]
	end

	#
	# return maximal y and x coordinates
	#

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
		@win=(win.kind_of?(BoxedWin) ? win.content : win)
		@showSelected=true # visually highlight the selected row
		empty
		@win.clear; @win.refresh
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


