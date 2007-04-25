# Brainfuck interpreter... very ugly and nonfunctional! But hey, it freakin'
# works sometimes, SOMETIMES! :D
class Brainfuck
  attr_reader   :buffer, :input, :pointer
  attr_accessor :stdout, :stdin
 
  # Initialize Brainfuck session with a buffer
  def initialize(attr = {})
    initialize_buffer(attr[:input], attr[:bufsize])
    @stdout, @stdin = (attr[:stdout] || $stdout), (attr[:stdin]  || $stdin)
  end
 
  # Clear buffer
  def clear
    @pointer = 0
    @buffer.clear
  end
 
  # Evaluate a line of brainfuck code in the current buffer. Buffer positions
  # are maintained and kept at the last state left by the code.
  def evaluate(code)
    brackets  = 0
    loop_code = ''
 
    code.split(//).each_with_index do |char, i|
      # Skip any unknown characters...
      next unless ['>', '<', '+', '-', '.', ',', '[', ']'].include? char
 
      # Do we need to grab contents for looping?
      if brackets > 0 and char != ']'
        loop_code << char
        next
      end
 
      # Normal BF parsing
      case char
      when '>' then @pointer += 1
      when '<' then @pointer -= 1
      when '+' then @buffer[@pointer] += 1
      when '-' then @buffer[@pointer] -= 1
      when '.' then putchar
      when ',' then getchar
      when '[' then brackets += 1
      when ']'
        brackets -= 1
        evaluate(loop_code) until current.zero? if brackets.zero?
        loop_code = ''
      end
    end
  end
 
private
 
  # Initialize buffer
  def initialize_buffer(input = nil, bufsize = nil)
    if input
      require 'stringio'
      @stdin = StringIO.new(input)
    end
 
    @pointer = 0
    @input   = input || ''
    @bufsize = bufsize || 30_000
    @buffer  = Array.new(@bufsize - @input.size) { 0 }
  end
 
  # Write a character to standard output
  def putchar
    char = current
    raise IOError, "Nothing to write from position #@pointer" unless char
    @stdout.putc(char)
  end
 
  # Read a character from standard input
  def getchar
    @buffer[@pointer] = @stdin.getc
  end
 
  # Get the character at the current pointer position
  def current
    @buffer[@pointer]
  end
end
 
class String
  # Interpret this string as brainfuck code
  def brainfuck
    require 'stringio'
    Brainfuck.new(:stdout => io = StringIO.new).evaluate(self)
    io.rewind
    io.read
  end
 
  alias bf brainfuck
end
 
module Kernel
  # Interpret brainfuck code of a given string or $_, standard input.
  def brainfuck(str = nil)
    (str || $_).bf
  end
 
  alias bf brainfuck
end