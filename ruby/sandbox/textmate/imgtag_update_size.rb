require 'rubygems'
require 'hpricot'
require 'open-uri'

def image_size(file)
  wh = `sips -g pixelWidth -g pixelHeight "#{file}" | awk '/pixelWidth/ { printf("%d-", $2) } /pixelHeight/ { printf("%d", $2) }'`.split("-")
  return { :width => wh[0], :height => wh[1] }
end

doc = Hpricot(STDIN.readlines.join)
#doc = Hpricot(open("laender.html"))
(doc/"img").each { |i|
  e = image_size(i.attributes['src'])
  i.attributes['width'] = "#{e[:width]}"
  i.attributes['height'] = "#{e[:height]}"
}

print doc.to_html