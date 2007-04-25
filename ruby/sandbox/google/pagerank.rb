#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
# $Id: google-pr.rb,v b205c14e4ef5 2007-01-15 13:53 +0300 $
# (C) 2006-2007 under terms of LGPL v2.1 
# by Vsevolod S. Balashov <vsevolod@balashov.name>
# based on 3rd party code snippets (see comments)

require 'uri'
require 'open-uri'

module SEO

  # http://blog.outer-court.com/archive/2004_06_27_index.html#108834386239051706
  class GooglePR

    def initialize(uri)
      @uri = uri
    end

    M=0x100000000 # modulo for unsigned int 32bit(4byte)

    def m1(a,b,c,d)
      (((a+(M-b)+(M-c))%M)^(d%M))%M # mix/power mod
    end

    def i2c(i)
      [i&0xff, i>>8&0xff, i>>16&0xff, i>>24&0xff]
    end

    def c2i(s,k=0)
      ((s[k+3].to_i*0x100+s[k+2].to_i)*0x100+s[k+1].to_i)*0x100+s[k].to_i
    end

    def mix(a,b,c)
      a = a%M; b = b%M; c = c%M
      a = m1(a,b,c, c >> 13); b = m1(b,c,a, a <<  8); c = m1(c,a,b, b >> 13)
      a = m1(a,b,c, c >> 12); b = m1(b,c,a, a << 16); c = m1(c,a,b, b >>  5)
      a = m1(a,b,c, c >>  3); b = m1(b,c,a, a << 10); c = m1(c,a,b, b >> 15)
      [a, b, c]
    end

    def old_cn(iurl = 'info:' + @uri)
      a = 0x9E3779B9; b = 0x9E3779B9; c = 0xE6359A60
      len = iurl.size 
      k = 0
      while (len >= k + 12) do
        a += c2i(iurl,k); b += c2i(iurl,k+4); c += c2i(iurl,k+8)
        a, b, c = mix(a, b, c)
        k = k + 12
      end
      a += c2i(iurl,k); b += c2i(iurl,k+4); c += (c2i(iurl,k+8) << 8) + len
      a,b,c = mix(a,b,c)
      return c
    end

    def cn
      ch = old_cn
      ch = ((ch/7) << 2) | ((ch-(ch/13).floor*13)&7)
      new_url = []
      20.times { i2c(ch).each { |i| new_url << i }; ch -= 9 }
      ('6' + old_cn(new_url).to_s).to_i
    end

    def request_uri
      # http://www.bigbold.com/snippets/posts/show/1260 + _ -> %5F
      "http://toolbarqueries.google.com/search?client=navclient-auto&hl=en&ch=#{cn}&ie=UTF-8&oe=UTF-8&features=Rank&q=info:#{URI.escape(@uri, /[^-.!~*'()a-zA-Z\d]/)}"
    end
 
    def page_rank(uri = @uri)
      @uri = uri if uri != @uri
      open(request_uri) { |f| return $1.to_i if f.string =~ /Rank_1:\d:(\d+)/ }
      nil
    end

    private :m1, :i2c, :c2i, :mix, :old_cn
    attr_accessor :uri
  end

end

if __FILE__ == $0 and 1 == ARGV.size
  puts SEO::GooglePR.new(ARGV[0]).page_rank
end
