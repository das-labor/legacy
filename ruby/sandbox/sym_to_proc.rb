class Symbol
 def to_proc
   lambda { |o| o.send(self) }
 end
end

p [1,/(foo)/,"abc",:hoho].collect(&:to_s)