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

ESC_OPEN_BRACKET   = 0x00.chr
ESC_CLOSE_BRACKET  = 0x01.chr
ESC_COMMA          = 0x02.chr

def unescape(str)
  str.gsub!(ESC_OPEN_BRACKET,  '[')
  str.gsub!(ESC_CLOSE_BRACKET, ']')
  str.gsub!(ESC_COMMA,         ',')
  str
end

def escape_one_pair(str)
  while str.match(/\"[^\"]*\[[^\"]*\"/) do
    str.gsub!(/(\"[^\"]*)\[([^\"]*\")/, '\1'+ESC_OPEN_BRACKET+'\2')
  end
  while str.match(/\"[^\"]*\][^\"]*\"/) do
    str.gsub!(/(\"[^\"]*)\]([^\"]*\")/, '\1'+ESC_CLOSE_BRACKET+'\2')
  end
  while str.match(/\"[^\"]*\,[^\"]*\"/) do
    str.gsub!(/(\"[^\"]*)\,([^\"]*\")/, '\1'+ESC_COMMA+'\2')
  end
  str.gsub!('"', '')
  str
end

def auto_escape(str)
  ret = ''
  while m=str.match(/([^\"]*)(\"[^\"]*\")(.*)/) do
    ret += m[1]+escape_one_pair(m[2])
    str = m[3]
  end
  ret += str
  return ret
end

def get_c_type(length)
  return "uint8_t"   if length <= 8
  return "uint16_t"  if length <= 16
  return "uint32_t"  if length <= 32
  return "uint64_t"  if length <= 64
  puts "Error: no C-type found for length=#{length}"
  return nil
end



class Parameter
  attr_reader :name, :unique_name, :src_pattern, :bin_pattern, :type, :order, :width
  
  def initialize(name, src_pattern, bin_pattern="", type="", order=:msb)
    @name = name
    u = self.object_id*2
    u=u.abs-1 if u<0
    @unique_name = u.to_s(16)
    @src_pattern = src_pattern
    @bin_pattern = (bin_pattern)?(bin_pattern):("")
    @type = (type)?type:""
    if order.class==String
      @order = :msb if order=="msb"
      @order = :lsb if order=="lsb"
    else
      @order=order
    end
   set_width() 
   analyze()
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
  
  def generate_c_decoder(fout)
    fout.printf("/* decoding for type \"%s\" is done by routines/objects with te identifier %s*/\n", @name, @unique_name)
    if @single_value
      fout.printf("void decode_param_%s(char* ouststr){\n", @unique_name, get_c_type(@width))
      fout.printf("    strcpy(outstr, \"#{@prefix}#{@single_value}#{@suffix}\"); \n", @unique_name)
      fout.printf("}\n")
      return
    end
    if @list
      fout.printf("static char** decode_param_%s_list = {\n", @unique_name)
      @list.each {|e| fout.printf("    \"%s\", \n",e)}
      fout.printf("};\n\n")
      fout.printf("void decode_param_%s(char* ouststr, %s value){\n", @unique_name, get_c_type(@width))
      fout.printf("    value -= %d; \n", @bin_range.first)   if @bin_range.first!=0
      fout.printf("    sprintf(outstr, \"#{@prefix}%%s#{@suffix}\", decode_param_%s_list[value]); \n", @unique_name)
      fout.printf("}\n")
      return
    else
      fout.printf("void decode_param_%s(char* ouststr, %s value){\n", @unique_name, get_c_type(@width))
      fout.printf("    value -= %d; \n", @bin_range.first)   if @bin_range && @bin_range.first!=0
      fout.printf("    value += %d; \n", @src_range.first)   if @src_range && @src_range.first!=0
      fout.printf("    sprintf(outstr, \"#{@prefix}%%d#{@suffix}\", value); \n")
      fout.printf("}\n")
      return
    end
  end
  
  private
  @@log2 = Math.log(2)
  
  def analyze()
    @prefix = ""
    @postfix = ""
    @src_range=nil
    @list=nil
    @single_value=false     
    @src_pattern = auto_escape(@src_pattern)
    m=@src_pattern.match(/^[^\[]*$/)
    if m
      # no range present
      m=@src_pattern.match(/,/)
      if m
        # we have a comma separated list
        @list = @src_pattern.split(',')
        @list.collect!{|e| unescape(e)}
        @src_range=0..(@list.length-1)
        @single_value = @list[0] if @width==0
      else
        # we have only a single value and no range (width should be zero)
        @single_value=@src_pattern.match(/\"?([^\"]*)\"?/)[1]
        @src_range=0..0
      end
    else
      m=@src_pattern.match(/^([^\[]*)\[([^\]]*)\](.*)$/)
      @prefix = unescape(m[1].match(/\"?([^\"]*)\"?/)[1])
      @suffix = unescape(m[3].match(/\"?([^\"]*)\"?/)[1])
      rangetext=m[2]
      if m=rangetext.match(/^([-]?[0-9]*)\.\.([-]?[0-9]*)/)
        a=m[1].to_i()
        b=m[2].to_i()
        @src_range=a..b
      else
        @list = rangetext.split(',').collect{|e| unescape(e)}
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
    puts "(#{@name}) src_range: "+@src_range.to_s+" width: "+@width.to_s
    if (x=Math.log(@src_range.last-@src_range.first+1)/@@log2) != @width
      printf("Warning: parameter range for %s (%f bits) does not correspond to output bit width (%f bits)!\n",
             @name, x, @width )
    end
  end
  
  def set_width()
    @bin_range=0..0
    if @bin_pattern=="" || @bin_pattern=='""'
      @width=0
      return
    end
    m=@bin_pattern.match(/([-]?[0-9]*)[\.]{2}([-]?[0-9]*)/)
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
     return
   end
   printf("Error binary pattern (%s) for %s not reconized!\n", @bin_pattern, @name)
  end
  
  
end
