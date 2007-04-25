require "rubygems"
require "inline"

class Fixnum
  def prime?
    ('1' * self) !~ /^1?$|^(11+?)\1+$/
  end
end

class Primes
 inline do |builder|
   builder.c "
   int prime(int num) {
     int x; 
     for (x = 2; x < (num - 1) ; x++) {
       if (num == 2) {
         return 1;
       }
       if (num % x == 0) {
         return x;
       }
     }
  return 1;
 }"
 end
end

p = Primes.new

for num in 2..100_000 do
  is_prime = p.prime(num)
  
  if is_prime == 1
    puts "#{num} is a prime number"
  else
    #puts "#{num} equals #{is_prime} * #{num/is_prime}"
  end
end

class Wupp
  inline do |builder|
    builder.c "
      long factorial(int max) {
        int i=max, result=1;
        while (i >= 2) { result *= i--; }
        return result;
      }"
  end
end
p Wupp.new.factorial(20)

## pure ruby
#for num in 1..10_000 do
# is_prime = 1
# for x in 2..(num - 1) do
#   if (num % x == 0) 
#     is_prime = x
#     break
#   end
# end
# 
# if is_prime == 1
#   puts "#{num} is a prime number"
#  else
#    puts "#{num} equals #{is_prime} * #{num/is_prime}"
# end
#end