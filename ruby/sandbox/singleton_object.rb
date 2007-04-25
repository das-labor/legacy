# add a method called "singleton" to Object class
class Object
  # return singleton class associated with the current object
  def singleton
    # create a singleton class for "self"
    class <<self
      # note that class is expression in Ruby, we just return 
      # "self". Note: "self" here is the singleton class itself
      self
    end
  end
end

class Person
end

class Employee < Person
end

class Manager < Employee
end

puts "Manager's singleton = #{Manager.singleton}"
puts "Employee's singleton = #{Employee.singleton}"
puts "Person's singleton = #{Person.singleton}"

def print_hierarchy(klass)
  puts "#{klass}'s hierarchy" 
  puts "\t" + klass.ancestors.join("\n\t")
end

# print class hierarchy of each class
print_hierarchy(Manager)
print_hierarchy(Employee)
print_hierarchy(Person)

# print class hierarchy of each singleton class
print_hierarchy(Manager.singleton)
print_hierarchy(Employee.singleton)
print_hierarchy(Person.singleton)