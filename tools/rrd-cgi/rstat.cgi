#!/usr/bin/env ruby

##########################################################################
# Global Configuration

COLLECTD_HOME="/home/collectd"

##########################################################################
# Includes
require 'cgi'

Cgi = CGI.new

class CollectdGraph
  def initialize( host )
    @cmds = Array.new;

    @host  = host;
    @title = "UNTITLED GRAPH";
    @width = 600;
  end

  def cgi(cgi)
    if cgi.has_key?("title") then
      @title = cgi["title"];
    end
    if cgi.has_key?("width") then
      @width = cgi["width"];
    end
  end

  def html_out
    graph
    @cmds.each_index { |i|
      @cmds[i] = '"'+ @cmds[i]+'"';
    }
    png = `rrdtool graph - -iE -a PNG -w #{@width} -t "#{@title}" -v "#{@host}" #{@cmds.join(" ")}`;
    Cgi.out("image/png") { png };
  end

  def rrddef( vname, file, dsname, function )
    @cmds.push( "DEF:#{vname}=#{file}:#{dsname}:#{function}" );
  end

  def rrdarea( vname, color, legend )
    @cmds.push( "AREA:#{vname}#{color}:#{legend}" );
  end

  def rrdline( vname, color, legend )
    @cmds.push( "LINE1:#{vname}#{color}:#{legend}" );
  end

  def rrdsarea( vname, color, legend )
    @cmds.push( "AREA:#{vname}#{color}:#{legend}:STACK" );
  end

  def rrdsline( vname, color, legend )
    @cmds.push( "LINE1:#{vname}#{color}:#{legend}:STACK" );
  end
end


class LoadGraph < CollectdGraph
  def initialize( host )
    super(host);
  
    @title = "Load on #{host}";
  end

  def graph
    file = COLLECTD_HOME + "/load.rrd";

    # define variables
    rrddef( "s_avg", file, "shortterm", "AVERAGE" );
    rrddef( "s_min", file, "shortterm", "MIN" );
    rrddef( "s_max", file, "shortterm", "MAX" );

    rrddef( "m_avg", file, "midterm", "AVERAGE" );
    rrddef( "m_min", file, "midterm", "MIN" );
    rrddef( "m_max", file, "midterm", "MAX" );

    rrddef( "l_avg", file, "longterm", "AVERAGE" );
    rrddef( "l_min", file, "longterm", "MIN" );
    rrddef( "l_max", file, "longterm", "MAX" );

    # draw graph
    rrdarea( "s_max", Colors["1l"], "1m average" );
    rrdarea( "m_max", Colors["2l"], "5m average" );
    rrdarea( "l_max", Colors["3l"], "15min average" );

    rrdline( "s_avg", Colors["1"], "" );
    rrdline( "m_avg", Colors["2"], "" );
    rrdline( "l_avg", Colors["3"], "" );

    # draw legend
  end
end

class MemoryGraph < CollectdGraph 
  def initialize( host )
    super(host);
  
    @title = "Load on #{host}";
  end

  def graph
    file = COLLECTD_HOME + "/memory.rrd";

    # define variables
    rrddef( "used_avg"    , file, "used"    , "AVERAGE" );
    rrddef( "free_avg"    , file, "free"    , "AVERAGE" );
    rrddef( "buffers_avg", file, "buffers", "AVERAGE" );
    rrddef( "cached_avg"  , file, "cached"  , "AVERAGE" );

    rrdarea(  "used_avg",     Colors["4l"], "used");
    rrdsarea( "buffers_avg",  Colors["3l"], "buffer cache" );
    rrdsarea( "cached_avg",   Colors["2l"], "page cache" );
    rrdsarea( "free_avg",     Colors["1l"], "free" );

  end
end

##########################################################################
# Color Themes
def brightColors
  colors = Hash.new
  colors["black"] = "#000000";
  colors["white"] = "#ffffff";
  colors["bg"]    = "#000000";
  colors["white"] = "#ffffff";
  colors["1"]     = "#ff0000";
  colors["1l"]    = "#ff8888";
  colors["2"]     = "#00ff00";
  colors["2l"]    = "#88ff88";
  colors["3"]     = "#0000ff";
  colors["3l"]    = "#8888ff";
  colors["4"]     = "#ff00ff";
  colors["4l"]    = "#ff88ff";
  return colors;
end



##########################################################################
# Main

Colors = brightColors;


if Cgi.params['graph'] then
  # Return html
  graph = LoadGraph.new("localhost");
  graph.cgi(Cgi);
  graph.html_out;
else 
  # return graph
  graph = MemoryGraph.new("localhost");
  graph.cgi(Cgi);
  graph.html_out;
end


