def permute(expr, indices=Array.new(expr.length,0), curr=0)
  result = []
  expr[curr].length.times { |i|
    indices[curr] = i
    if curr+1 < expr.length then
      result.push *permute(expr, indices, curr+1)
    else
      one = []; indices.each_with_index { |k,j| one << expr[j][k] }
      result << one
    end
  }
  result
end

puts "Examples -----"
term = [[0,1,2],[0,1,2],[0,1,2]]
perm = permute(term)

p perm
puts "More -----"
newterm = permute([["Hello", "Hi", "Yo"], ["My friend", "old pal", "good buddy"], ["what's up?", "how are you?", "what's new?"]]).each { |sing| puts sing.join(" ") }
