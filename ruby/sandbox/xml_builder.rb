require 'rubygems'
require 'builder'

def sample_1
  x = Builder::XmlMarkup.new(:target => $stdout, :indent => 1)
  
  x.instruct!
  x.declare! :DOCTYPE, :html, :PUBLIC, "-//W3C//DTD XHTML 1.0 Strict//EN", "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
  x.html( "xmlns" => "http://www.w3.org/1999/xhtml" ) { 
   x.head { 
    x.title "XHTML 1.0 example"  
    x.style( "type" => "text/css" ) { x.text! "h1 {font-family:tahoma,sans-serif;font-size:18pt;color:blue} body{font-family:georgia,serif}"}
   }
   x.body {
    x.h1 "Hello from Builder"
    x.p "A Ruby library that facilitates the programatic generation of XML."
    x.p { |y| y << "Methods of interest from <code<Builder::XmlMarkup</code>" }
  
    x.ul {
      x.li "cdata!",
      x.li "comment!",
      x.li "declare!",
      x.li "instruct!",
      x.li "new"
    }
  
    x.p { |y| y << "Methods of interest from "; x.code "Builder::XmlBase"; y << ":" }
  
    x.ul {
      x.li "<<"
      x.li "new"
      x.li "text!"
    }
    
   }
  }
end

def sample_2
  favorites = { 'candy' => 'Neccos', 'novel' => 'Empire of the Sun', 'holiday' => 'Easter' }
  xml = Builder::XmlMarkup.new( :target => $stdout, :indent => 2 )
  xml.instruct! :xml, :version => "1.1", :encoding => "US-ASCII"
  xml.favorites do
    favorites.each { | name,choice| xml.favorite( choice, :item => name ) }
  end
end