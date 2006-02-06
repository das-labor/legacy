#!/usr/bin/env ruby

#  apt-get install libopenssl-ruby
require "openssl"
include OpenSSL

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
end


#####
#  Simulated OpenPGP Card
class PgpCard 
  attr_reader :d, :e, :n, :p, :q

  # bits == sqrt(keysize)
  def gen_key(bits)
     max = (2**bits) - 1;
     @p = get_prime(max);
     @q = get_prime(max);
     while (@p == @q) do
       @q = get_prime(max);
     end

     euler = (@p-1) * (@q-1);

     begin
       e = rand(euler-1) + 1;
       xe = XEuclid.new( -euler, e );
     end while(xe.d!=1)
#     puts "#{xe.d} = #{xe.a}*#{xe.s} + #{xe.b}*#{xe.t} , d=#{xe.t}   ";

     @n = @p * @q;
     @d = xe.t;
     @e = e;

     if (@d < 0) then @d = @d % euler; end
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


card = PgpCard.new;
card.gen_key(32);
puts card;

m1   = 7*5*3;
sig1 = card.sign(m1)
mo1  = card.verify(sig1) 
puts "Signing message m=#{m1} ; sig(m)=#{sig1} ; verify(sig)=#{mo1}"

m2   = 7*5*2;
sig2 = card.sign(m2)
mo2  = card.verify(sig2) 
puts "Signing message m=#{m2} ; sig(m)=#{sig2} ; verify(sig)=#{mo2}"

m3   = 7*5;
sig3 = card.sign(m3)
mo3  = card.verify(sig3) 
puts "Signing message m=#{m3} ; sig(m)=#{sig3} ; verify(sig)=#{mo3}"


puts "------------"
puts gcd(m1, m2)
puts gcd(sig1, sig2)
puts gcd(sig2, sig3)

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

