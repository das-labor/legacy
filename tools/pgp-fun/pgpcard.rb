
####
# Exponentation x^e modulo n 
def modpow(x,e,n)
  result = 1
  while (e != 0)
     if (e.modulo(2) == 1)
         result = result * x % n
         e = e-1
     end

     if (e > 0) then
         x = x * x % n;
         e >>= 1; 
     end
  end
  return result
end

####
# Primzahlentest
def prime?(p)
  bn = BN.new(p.to_s);
  return bn.prime?(100);
#  [2,3,5,7].inject(true) { |is_prime,w| 
#    is_prime && (modpow(w,p-1,p)==1)
#  }
end

####
# generate Prime [0, max]
def get_prime(max)
  n = rand(max-1) + 2;
  until prime?(n) do
    n = rand(max-1) + 2;
  end
  return n
end

####
# GCD eben
def gcd(a,b)
  while(b != 0) 
    r = a % b;
    a = b;
    b = r;
  end
  return a;
end

####
# d = s*a + t*b
class XEuclid
  attr_reader :a, :b, :d, :t, :s;

  def initialize(a,b)

    m=a; n=b;
    s=1; t=0;
    u=0; v=1;

    while(n>0) 
      (q,r) = m.divmod(n);

      m=n; n=r;

      tmp=t; t=s-q*t; s=tmp;
      tmp=v; v=u-q*v; u=tmp;
    end

    @a = a;
    @b = b;
    @d = m;
    @t = u;
    @s = s;
  end
  
  def to_s
    return sprintf( "%x * %+x + %x * %+x = %x", @a, @s, @b, @t, @d );
  end
end


#####
#  Simulated OpenPGP Card
class PgpCard 
  attr_reader :d, :e, :n, :p, :q, :pad
  attr_writer :d, :e, :n, :p, :q, :pad

  def initialize()
    @pad = 0;
  end

  # bits == sqrt(keysize)
  def gen_key(bits)
     min = (2**(bits/2).ceil) - 1;
     @p = @q = 0;

     while ((@p == @q) or (@p*@q < (min*min)) ) do
       @p = get_prime(2*min);
       @q = get_prime(2*min);
       #printf "%x * %x = %x\n", @p, @q, @p*@q;
     end

     euler = (@p-1) * (@q-1);

     begin
       e = rand(euler-1) + 1;
       xe = XEuclid.new(e, euler);
     end while(xe.d!=1)

     @n = @p * @q;
     @d = xe.s % euler;
     @e = e;
  end

  def sign(m)
    return modpow(@pad + m, @d, @n);
  end

  def verify(s)
    return modpow(s, @e, @n) - @pad
  end

  def to_s
    return sprintf( "PgpCard: n=%X (p=%X q=%X), d=%X, e=%X", @n, @p, @q, @d, @e );
  end
end


