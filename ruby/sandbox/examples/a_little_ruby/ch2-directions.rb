def descending?(first, second, third)
  first > second && second > third
end

def never_descending?(first, second, third)
  first <= second && second <= third
end

=begin

Try this:
    descending?(3, 2, 1)       # true
    never_descending?("big", "bigger", "biggest")  # true
    never_descending?(3, 2, 1)  # false

=end
