#!/usr/bin/env ruby

#  apt-get install libopenssl-ruby
require "openssl"
require "pgpcard.rb"
include OpenSSL

# Values from joern Card
e=0xbe0df275
n=0xba82f2abc9ac0495dbecbfb953b14bf2e4030e904d74332dcdcc6cc54010b29320c704865026bf94723b15cf40f8208d5249712912081a52c29862716b74c34e0f79cc8ac59ffd17760c347531f583ac1f7da32702c7973a3621cb9a311eb312d00b7ce479b9e6b96587b9c7b03a432e0dad8d5e53e2fac6787d9a338b6bd11d

val2=0x8723D6BEC9201B35ADB39131E74C6E0DF339C6F379E4F890885550293F0D51C9FA2F2D5693B963F83DAFA61ABDCA57F1B47094732220F7935C4DE6E9057E8872FCE808367E55DF55BE902E0EF76904F9A39F76B854FBF616C8F50B24BCAAC4C022DDA7893B43BA4F5AC42D498EDF7220AD8267600C3233B6E9FC2A0E91082500;
val4=0x6895681E10C8B3D2893F8B08FEA84FA870BE754020A036BB9B5452758DB79A796E14B67A24DB8E4598DECF945216A94793C65658066DE63FF7BD3B18ABCFDCB1367A9B7E400BA7E7424AD5CC418D4184396B85AF30F1E2D54E1F87F57F2E069CC7A9C5C785EA341F2DE7E90CEE75F2FB5E63B0B9B320BBE594A8B75E06BBFDB3;


card = PgpCard.new;
card.gen_key(16);
pad = 0x00;

#card.n = n;
#card.e = e;
puts card

# printf "%x", card.verify(val4)

m1   = 2;
sig1 = card.sign(pad + m1)
mo1  = card.verify(sig1) - pad
printf "Signing message m=%x, sig(m)=%x, verify(sig)=%x\n", m1, sig1, mo1

xe = XEuclid.new(sig1, card.n);
pad1 = xe.s % card.n;




exit 1



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

