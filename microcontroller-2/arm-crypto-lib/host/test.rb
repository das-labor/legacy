def a(n)
  return (-n-65+512)%512
end

def b(n)
  return (n+a(n)+65)%512
end

for i in (0..512)
  puts("") if(i%32==0)
  printf("%3d", b(i))
end

irb
