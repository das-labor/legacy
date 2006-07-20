#!/usr/bin/env ruby


WIKIURL     = "http://wiki.das-labor.org/wiki/index.php?title=Spezial:Export"
WIKIPAGE    = "Vorlage:Termine"
WIKIPREFIX  = "http://wiki.das-labor.org/wiki/index.php?title="

BLOGSERVER  = "www.das-labor.org.de"
BLOGPATH    = "/weblog/xmlrpx.php"
BLOGID      = "LABOR"
BLOGUSER    = "termine"
BLOGPASSWD  = '%%termin&&'

POSTTITLE   = "SCHEDULE"
POSTHEAD    = <<EOF

Schedule

EOF
POSTLINE    = " * LALA"
POSTTAIL    = <<EOF

 (k)allisti
EOF

WIKILINES = [
#  /\W+(?:<DATE>\d{2}.\d{2}.\d{2})\W+(?:<DAY>\w\w)\W+(?:<TEXT>.*)/,
  /^\s*\*\W+(?:<DAY>\d{2}).(?:<MONTH>\d{2}).(?:<YEAR>\d{2})\W+(?:<WDAY>\w\w)\W+(?:<TIME>[\d:,.]+)[ ,;-]+(?:<TEXT>.*)/,
  /^\s*\*\W+(?:<WDAY>\w\w)\W+(?:<DAY>\d{2}).(?:<MONTH>\d{2}).(?:<YEAR>\d{2})\W+(?:<TIME>[\d:,.]+)[ ,;-]+(?:<TEXT>.*)/
]

#############################################################################
# Starts Here
require 'http-access2'
require 'rexml/document'
require 'htmlentities/lib/htmlentities.rb'
require 'metaweblogapi.rb'
require 'namedregexp.rb'

class LaborTermine 
  attr_reader "termine"

  def initialize()
     @termine = Array.new;
  end

  def getWikiContent
    proxy = ENV['HTTP_PROXY']
    clnt  = HTTPAccess2::Client.new(proxy)
    txt   = clnt.get(WIKIURL, { "pages" => WIKIPAGE, "action" => "submit", "curonly" => "true" }).content
    return txt.decode_entities;
  end

  def parseLine(line)
    line.gsub!("'''", "");
    WIKILINES.each { |re|
      nre = NamedRegexp.new(re);
      m = nre.match(line);
      return m if !m.nil?
    }
    return nil
   end

  def parseWikiContent(wikiXml)
    doc = REXML::Document.new(wikiXml)

    text = doc.elements["/mediawiki/page/revision"];
    text.to_s.each_line { |line|
      m = parseLine(line);
      if m then
        aterm = Hash.new;
	m.each { |a|
		aterm[a[0].downcase] = a[1];
	}
	lr = /\[\[\s*(.*?)\s*\]\]/;
	if lr.match( aterm["text"] ) then 
		wikipage=$1;
		wikipage.gsub!( " ", "_" );
		aterm["link"] = WIKIPREFIX+wikipage;
		aterm["text"].sub!("[[", "");
		aterm["text"].sub!("]]", "");
	elsif  /^(.*?)\[(http:\S+)\s+(.*)\](.*)$/.match(aterm["text"]) then	 # comment
		aterm["text"] = $1+" "+$3+$4;
		aterm["link"] = $2;
	end
	@termine.push aterm;
      end
    }
  end

  # Main
  def run
    wikiXml = getWikiContent
    parseWikiContent(wikiXml)
  end
end



####
# MAIN
#

class Array
  def to_h
    Hash[*self]
  end
end



lt = LaborTermine.new;
lt.run;


lt.termine.each { |termin|
	puts "<termin>"
	termin.each { |key,val|
		puts "  <#{key}>#{val}</#{key}>"
	}
	puts "</termin>"
}

exit



client = MetaWebLogAPI::Client.new(BLOGSERVER, BLOGPATH,
    BLOGID, BLOGUSER, BLOGPASSWD)

blogpost = {
  'title' => 'New post!',
  'description' => 'This is the body of my new post',
  'pubDate' => Time.gm(2005,05,31,15,0,0,0) # May 31, 2005 at 3:00 PM
}

client.newPost(blogpost, true)


