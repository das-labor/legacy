require 'rubygems'
require 'ferret'

require 'find'

include Ferret

@index = Index::Index.new(:path => 'ferret_index')

def update_index(rootpath)
  Find.find(rootpath) do |path|
    if FileTest.file? path
      File.open(path) { |file|
        @index.add_document(:file => path, :content => file.readlines)
      }
    end
  end
end

def create_index
  @index << "This is a new document to be indexed"
  @index << {:title => "Programming Ruby", :content => "blah blah blah"}
  @index << {:title => "Programming Ruby", :content => "yada yada yada"}
  0.upto(100) { @index << ["#{Time.now.tv_sec}","This is a new document to be indexed"] }
end


def search
  @index.search_each('content:"blah"') do |doc, score|
    p "Document #{doc} found with a score of #{score}"
    p [@index[doc]['title'],@index[doc]['content']]
  end  
end