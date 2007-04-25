print "#{def r(x);rand(x);end; C = "\033["}2J#{C}0;0f#{C}32m"; w=r(20).to_i+13
h=w-r(10).to_i; h.times { |line| puts " " * ((w / 2) - (line / 2)) + 
(1..line+1).collect { r(rand(12)) == 0 ? "#{C+'5m' if Time.now.usec % 2 == 0}#{C}#{r(7) + 
31}m*#{C}0m#{C}32m":'='}.join}; print "#{C}33m"; 3.times{ puts (("x" * (w / 6)
).center(w)) }; puts "#{C}0m\n", "Merry Christmas From Ruby Inside!"