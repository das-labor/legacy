=begin
    instruction.rb is part of the GeneRic ASsembler (GRAS)
    Copyright (C) 2008  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
=end

require 'jcode'

class Parameter
  attr_reader :name, :src_pattern, :bin_pattern, :type, :order, :width
  
  def initialize(name, src_pattern, bin_pattern="", type="", order=:msb)
    @namme = name
    @src_pattern = src_pattern
    @bin_pattern = bin_pattern
    @type = type
   self.set_width() 
   self.analyze()
  end
  
  def match(src_str)
    m=/^#{@prefix}(.*)#{@suffix}$/
    return false if m==nil
    value = m[1]
    if @list
      return @hash[value]!=nil
    end
    value = value.to_i
    return @range.member?(value)
  end
  
  def src_to_bin(src_str)
    m=/^#{@prefix}(.*)#{@suffix}$/
    return nil if m==nil
    value = m[1]
    if @list
      v = @hash[value]
      return nil if v==nil
    else
      v = value.to_i
      v -= @bin_range.first
    end
    # now translating according to convention
    if order==:msb
      return v
    end
    if order==:lsb
      s = v.to_i(2)
      s.reverse!
      return s.to_i(2)
    end
  end
  
  def bin_to_src(value)
    return false if !@bin_range.member(value)
    value += @bin_range.first
    if @list
      return @prefix+@list[value]+@suffix
    end
    return @prefix+value.to_s(10)+@suffix
  end
  
  private
  @@log2 = Math.log(2)
  
  def analyze()
    @prefix = ""
    @postfix = ""
    @src_range=nil
    @list=nil
    m=@src_pattern.match(/^[^\[]*$/)
    if m
      # no range present
      m=@src_pattern.match(/,/)
      if m
        # we have a comma separated list
        @list = @src_pattern.split(',')
        @src_range=0..(@list.length-1)
      else
        # we have only a single value and no range (width should be zero)
        @single_value=true
        @single_value=@src_pattern
        @src_range=0
      end
    else
      m=@src_pattern.match(/^([^\[]*)\[([^\]]*)\](.*)$/)
      @prefix=m[1]
      @suffix=m[3]
      rangetext=m[2]
      if m=rangetext.match(/([-]{0,1}[0-9]*)\.\.([-]{0,1}[0-9]*)/)
        a=m[1].to_i()
        b=m[2].to_i()
        @src_range=a..b
      else
        @list = rangetext.split(',')
        @src_range=0..(@list.length-1)        
      end
    end
    if @list
      @hash=Hash.new
      @hash.default=nil
      index=0
      @list.each{|v| @hash[v]=index; index+=1}
    end
    if @src_range==nil
      printf("Error: source pattern %s for parameter type %s not recognized!\n", @src_pattern, @name)
    end
    if Math.log(@src_range)/@@log2 != @width
      printf("Warning: parameter range for %s does not correspond to output bit width!\n", @name)
    end
  end
  
  def set_width()
    if @bin_pattern==""
      @width=0
      return
    end
    m=@bin_pattern.match(/^([-]{0,1}[0-9]*)\.\.([-]{0,1}[0-9]*)/)
    if m
      a=m[1].to_i()
      b=m[2].to_i()
      @bin_range=a..b
      r=b-a+1
      w=Math.log(r)/@@log2
      if w!=w.ceil()
       printf("Warning: range for %s is not bit aligned (%f bits)!\n", @name, w) 
     end
     @width=w.ceil()
    end
  end
end

class Parameters
  
end