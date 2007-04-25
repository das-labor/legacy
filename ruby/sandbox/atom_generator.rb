require 'uuidtools'

module AtomGenerator
  STRFTIME = "%Y-%m-%d"

  class StuffBag
    undef_method :id

    def initialize(&block)
      @tags = {}
      instance_eval(&block) if block_given?
    end

    def method_missing(meth, *args, &block)
      self.class.send(:attr_accessor, meth)
      if block_given?
        klass = AtomGenerator.const_get(meth.to_s.capitalize)
        @tags[meth] = instance_variable_set("@#{meth}", klass.new(&block))
      else
        value = args.size == 1 ? args.first : args
        @tags[meth] = instance_variable_set("@#{meth}", value)
      end
    end

    def uuid
      "urn:uuid:#{UUID.timestamp_create.to_s}"
    end

    def each
      @tags.each{|t| yield(t)}
    end

    def map
      @tags.map{|t| yield(t)}
    end

    def to_atom
      [
        '<?xml version="1.0" encoding="utf-8"?>',
        '<feed xmlns="http://www.w3.org/2005/Atom">',
        to_s('  '),
        '</feed>',
      ]
    end

    def to_s(indent = '  ')
      name = self.class.name.split('::').last.downcase
      [
        "#{indent}<#{name}>",
        tags(indent),
        out << "#{indent}</#{name}>",
      ]
    end

    def tags(indent = '  ')
      map do |key, value|
        if key == :link
          %{#{indent}  <#{key} href="#{value}" />}
        else
          if value.is_a?(StuffBag)
            value.to_s(indent + '  ')
          else
            %{#{indent}  <#{key}>#{value}</#{key}>}
          end
        end
      end
    end
  end

  class Feed < StuffBag
  end

  class Author < StuffBag
  end

  class Entry < StuffBag
  end
end

feed =
AtomGenerator::Feed.new do
  title "Example Feed"
  subtitle "A subtitle."
  link "http://example.org/"
  id uuid

  author do
    name "John Doe"
    email "johndoe@example.com"
  end

  entry do
    title "Atom-Powered Robots Run Amok"
    link "http://example.org/2003/12/13/atom03"
    summary "Some text."
    id uuid
  end
end

puts feed.to_atom

=begin
<?xml version="1.0" encoding="utf-8"?>
<feed xmlns="http://www.w3.org/2005/Atom">

 <title>Example Feed</title>
 <subtitle>A subtitle.</subtitle>
 <link href="http://example.org/"/>
 <updated>2003-12-13T18:30:02Z</updated>
 <author>
   <name>John Doe</name>
   <email>johndoe@example.com</email>
 </author>
 <id>urn:uuid:60a76c80-d399-11d9-b91C-0003939e0af6</id>

 <entry>
   <title>Atom-Powered Robots Run Amok</title>
   <link href="http://example.org/2003/12/13/atom03"/>
   <id>urn:uuid:1225c695-cfb8-4ebb-aaaa-80da344efa6a</id>
   <updated>2003-12-13T18:30:02Z</updated>
   <summary>Some text.</summary>
 </entry>
</feed>
=end