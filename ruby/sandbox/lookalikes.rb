class String
  LOOKALIKES = {'A'=>'a4','B'=>'8','T'=>'7','H'=>'77','S'=>'5','E'=>'3','O'=>'0','I'=>'1','L'=>'1'}
  def l33t
    l33t_val=self.dup.upcase
    LOOKALIKES.each {|k,v| l33t_val.gsub!(k,v)}
    l33t_val
  end
end

puts "hello wie geht es dir".l33t