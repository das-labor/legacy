class Super
  def inherited
    puts "Super's inherited"
    bounce
    puts "Back to Super's inherited"
    slam
    puts "Super returns from inherited"
  end

  def bounce
    puts "Super's bounce"
  end
end

class Sub < Super
  def slam
    puts "Sub's slam"
  end
end
  
=begin
Try this:

        Sub.new.inherited

=end
