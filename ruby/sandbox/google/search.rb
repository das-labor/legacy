# simple google query without the crap
def google_query(s)
  uri = "http://www.google.com/search?q=#{URI.escape(s)}"
  (Hpricot(open(uri))/"div.g").map do |ele|
    {:title => ele.at("a").inner_text,
     :link  => ele.at("a")['href'],
     :description => (ele/("font".."font/br")).to_html}
  end
end