def factorial(n)
  if n == 1
    n
  else
    n * factorial(n-1)
  end
end

=begin
(The above line begins documentation that's ignored by Ruby.)

After loading this file into irb (or cutting and pasting the above
definition), try this:

     factorial(5)



NOTES ON RUBY: 

(These notes will appear with many of the examples. You only need to
read them if you plan on varying the examples, or if you want a deeper
understanding of Ruby syntax and behavior.)

The general form of calling a function is this:
  function_name(argument1, argument2, ...)
The parentheses can often be omitted. So you can write either
    
  factorial(5)

or 

  factorial 5

When in doubt, use parentheses. For example, what does this mean?

  factorial 5-3

Is it 

  factorial(5-3)

or

  factorial(5) - 3 


Multi-line Ruby constructs usually begin with a keyword like "def"
or "if" and end with "end". Indentation is just a convenience for
the reader. The ends of lines are significant. For example, while you
can write an "if" expression that's all on one line, you have to know
some additional formatting rules. There are also variant forms of "if"
more suited to a single line. In this book, I'll always use the
verbose form.

Unlike some languages, all Ruby expressions return a value. So the
value of the "if" expression in factorial is the value of either "n"
or "n * factorial(n-1)", whichever is selected. The value of an entire
function is the value of the last expression executed. So the value of
factorial is the value of the "if" expression.

=end
