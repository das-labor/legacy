class Super
  def super_set(val)
    @val = val
  end

  def super_get
    @val
  end
end

class Sub < Super
  def sub_set(val)
    @val = val
  end

  def sub_get
    @val
  end
end
  
=begin
Try this:

    s = Sub.new
    s.super_set(5)
    s.super_get
    s.sub_get
    
    s.sub_set("dawn")
    s.super_get
    s.sub_get

RUBY NOTE:

Notice that the instance variable @val can hold an object of any class.
It first held an Integer, then it was changed to hold a String. In
languages like Ruby, Smalltalk, and Lisp, *objects* have types, but
variables do not.

=end
