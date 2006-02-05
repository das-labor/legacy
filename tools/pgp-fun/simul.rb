#!/usr/bin/env ruby

# Values from joern Card
e=0xbe0df275
n=0xba82f2abc9ac0495dbecbfb953b14bf2e4030e904d74332dcdcc6cc54010b29320c704865026bf94723b15cf40f8208d5249712912081a52c29862716b74c34e0f79cc8ac59ffd17760c347531f583ac1f7da32702c7973a3621cb9a311eb312d00b7ce479b9e6b96587b9c7b03a432e0dad8d5e53e2fac6787d9a338b6bd11d

val2=0x8723D6BEC9201B35ADB39131E74C6E0DF339C6F379E4F890885550293F0D51C9FA2F2D5693B963F83DAFA61ABDCA57F1B47094732220F7935C4DE6E9057E8872FCE808367E55DF55BE902E0EF76904F9A39F76B854FBF616C8F50B24BCAAC4C022DDA7893B43BA4F5AC42D498EDF7220AD8267600C3233B6E9FC2A0E91082500;
val4=0x6895681E10C8B3D2893F8B08FEA84FA870BE754020A036BB9B5452758DB79A796E14B67A24DB8E4598DECF945216A94793C65658066DE63FF7BD3B18ABCFDCB1367A9B7E400BA7E7424AD5CC418D4184396B85AF30F1E2D54E1F87F57F2E069CC7A9C5C785EA341F2DE7E90CEE75F2FB5E63B0B9B320BBE594A8B75E06BBFDB3;


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
  [2].inject(true) { |is_prime,w| 
    is_prime && (modpow(w,p-1,p)==1)
  }
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
end


#####
#  Simulated OpenPGP Card
class PgpCard 
  attr_writer :d, :e, :n, :p, :q

  def gen_key(bits)
     max = (2**bits) - 1;
     @p = get_prime(max);
     @q = get_prime(max);

     euler = (@p-1) * (@q-1);

     begin
       e = rand(euler-1) + 1;
       xe = XEuclid.new( -euler, e );
       puts "1=#{xe.d} , d=#{xe.t}";
     end while( xe.d!=1)

     @n = @p * @q;
     @d = xe.t;
     @e = e;
  end

  def sign(m)
    return modpow(m, @d, @n);
  end

  def verify(s)
    return modpow(s, @e, @n);
  end

  def prepend(m)
    return 0x234200000000 + m;
  end

  def to_s
    return "n=#{@n} (p=#{@p} q=#{@q}) , d=#{@d} , e=#{@e}";
  end
end


30.times {
  puts get_prime(20);
}




card = PgpCard.new;
card.gen_key(3);
puts card;

#card.n = 23*17;
#card.e = 5;
#card.d = 5;

#m1 = 6;
#sig = card.sign(m1)
#puts card.verify(sig) 



#puts power(val2, e, n);
#puts power(11,3,1000);
#
#n = 3*7
#euler = 2*6
#
#e = 5
#d = 5
#
#
#sig = power(3, d, n)
#puts "Result #{sig}"

