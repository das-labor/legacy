require 'rubygems'
require 'mechanize'

def google_search
  agent = WWW::Mechanize.new
  agent.user_agent_alias = 'Mac Safari'
  page = agent.get("http://google.com/")
  search_form = page.forms.with.name("f").first
  search_form.q = "ruby"
  search_results = agent.submit(search_form)
  puts search_results.body  
end

def leo_search
  str = gets[0]
  agent = WWW::Mechanize.new
  page = agent.get("http://dict.leo.org/?searchLoc=0&relink=off&spellToler=standard&sectHdr=off&cmpType=relaxed&lang=en&search=#{str.gsub(/ /,"+")}")
  t = page.search("//table#results")
  
  unless t.empty?
    trs =  t.search("//tr")
    x = trs[0].search("//th")
    p "#{x[1].innerHTML} ::: #{x[3].innerHTML}"
  
    trs[2..trs.size].each do |tr|
      result = tr.search("//td")
      p "#{result[1].innerHTML.gsub(/(<.*?>)/m,"")} ::: #{result[3].innerHTML.gsub(/(<.*?>)/m,"")}"
    end
  end  
end