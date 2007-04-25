require 'stringio'

module Piping
    def | proc; proc.call self; end
end
class IO
    include Piping
    def self.[](obj)
        if obj.respond_to? :read
            return obj
        else
            StringIO.new(obj.to_s)
        end
    end
end
class StringIO
    include Piping
end

wc = proc { |io| IO[io.inject(0) { |i, line| i += 1 }] }
grep = proc { |exp| proc { |io| IO[io.grep(exp)] } }

count = open("ruby-pipe.rb") | grep[/io/] | wc
p count.read
