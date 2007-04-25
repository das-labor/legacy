require 'net/http'
require 'tempfile'
 
# Temp and cache folders (defaults are ok)
tmp_dir = "tmp"
cache_dir = "cache"
poster_dir = "posters"
 
# A working host + name to the "kh service" script
# The underscore is a placeholder, roundrobin here
kh_host = "kh_.google.com"
kh_script = "/kh?n=404&v=9&t="
 
# The area tile for the poster
area = "t"
 
# Depth level (will result in 4^d tiles)
depth = 2
tdepth = (2**depth)
 
# The first tile is the deepest one in the upper left corner
first = area + "q" * depth 
length = first.length
 
throw "Area string length + depth may not exceed 18" if length > 18
 
# Encode the position
x = first.gsub(/t|q/, "0").gsub(/r|s/, "1").to_i(2)
y = first.gsub(/r|q/, "0").gsub(/t|s/, "1").to_i(2)
 
# Download the tiles
puts "Resulting image contains #{4**depth} tiles, dimensions will be #{256*tdepth}x#{256*tdepth}"
 
# An empty array for all rows
rows = []
 
# Roundrobin counter
rr = 0
 
puts "Getting #{4**depth} tiles via download (d) or cache (c):"
 
# Each row of the area
tdepth.times do |i|
  # Reset the row
  row = []
 
  print "row #{i+1}: gathering tiles ["
 
  # Each tile of the row
  tdepth.times do |j|
    # Reset the tile string
    tile = ""
 
    # Each digit of the encoded position
    length.times do |k|
      # String representations of the binary encoded positions
      xs = ("%0#{length}b" % x).to_s
      ys = ("%0#{length}b" % y).to_s
 
      # Decode the binary position to the google qrst-format
      c = case [xs[k],ys[k]]
            when [48,48]: "q"
            when [48,49]: "t"
            when [49,48]: "r"
            when [49,49]: "s"
          end
 
      tile << c
    end
 
    # Download the tile if it doenst exist yet
    tile_filename = File.join(cache_dir, tile + ".jpg")
    unless File.exists?(tile_filename)
     print "#{kh_host.sub(/_/, (rr % 6).to_s)}\n"
     p (rr % 6).to_s
     p kh_script + tile
      #File.open(tile_filename, "w") { |f| f.write(Net::HTTP.get_response(kh_host.sub(/_/, (rr % 5).to_s), kh_script + tile).body) }
      p kh_host.sub(/_/, (rr % 5).to_s), kh_script + tile
      rr += 1
      print "d"
    else
      print "c"
    end
 
    # Collect the filenames
    row << tile_filename
 
    # Move position one east
    x += 1
  end
 
  # Create row image
  rtf = File.join(tmp_dir, "row#{i}.jpg")
  print row.inspect
  print "\n] assembling tiles... "
  system "convert #{row.join(" ")} +append #{rtf}"
  puts "completed"
 
  # Collect the row
  rows << rtf
 
  # Reset tile count and move position one south
  x -= tdepth
  y += 1
end
 
# Create poster
poster_filename = File.join(poster_dir, "#{area}_depth#{depth}.jpg")
print "assembling rows... "
system "convert #{rows.join(" ")} -append #{poster_filename}"
puts "completed! result image: #{poster_filename}"