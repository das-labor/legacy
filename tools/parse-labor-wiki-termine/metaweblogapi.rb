require 'xmlrpc/client'

module MetaWebLogAPI
  class Client
    def initialize(server, urlPath, blogid, username, password)
      @client = XMLRPC::Client.new(server, urlPath)
      @blogid = blogid
      @username = username
      @password = password
    end
   
    def newPost(content, publish)
      @client.call('metaWeblog.newPost', @blogid, @username,
          @password, content, publish)
    end

    def getPost(postid)
      @client.call('metaWeblog.getPost', postid, @username,
          @password)
    end

    def editPost(postid, content, publish)
      @client.call('metaWeblog.editPost', postid, @username,
          @password, content, publish)
    end
  end
end


