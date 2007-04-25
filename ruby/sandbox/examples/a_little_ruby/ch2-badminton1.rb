class Super
  def refined
    puts "Super's refined"
  end

  def unique
    puts "Super's unique"
  end
end

class Sub < Super
  def refined
    puts "Sub's refined"
    super
    puts "Back to Sub's refined"
    unique
    puts "Sub's refined returns"
  end
end
  
=begin
Try this:

        Sub.new.refined

RUBY NOTE:

"puts" is much like "print". One difference is that you don't have to
put "\n" in the String to terminate the line. That's why I use it here.

=end
