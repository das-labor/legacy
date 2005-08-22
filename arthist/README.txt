  What is it
    arthist - Perl MPD client for generating/maintaining HTML page with your
    MPD activity. Date stamps as well as colors used in the output can be
    configured. It also generates links to http://www.allmusic.com/ from the
    song tags as well as inline album cover pictures!

  Download
    tar file
    http://www.studiosells.net/msells/arthist/$$VERSION/arthist-0.3.tar

    sample output http://www.studiosells.net/msells/arthist/sample.html

  Usage
    "arthist <output_filename"> will append to output_filename. "arthist
    -help" will show this help message.

  Requirements
    Three packages are required to use arthist.

    * CGI::Enurl  CGI::Enurl can be found at
                  http://jenda.krynicky.cz/perl/Enurl.pm.html A quick way to
                  get & install this is:

                  "perl -MCPAN -e 'install CGI::Enurl'"

                  Assuming you have CPAN which most of the civilized world
                  does

    * A modified MPD.pl
                  The one in SVN still has _one_ bug but it's close...

    * Net::Amazon A quick way to get & install this is:

                  "perl -MCPAN -e 'install Net::Amazon'"

                  Assuming you have CPAN which most of the civilized world
                  does

  Configuration
    It's possible to control the format of the datestamps as well as what
    events are reported. Since you will likley only do this once these are
    stored in the script itself in two hashes:

    * config{mpd_host}, config{mpd_port}
            Specifies which MPD server to connect to.

            *IMPORTANT: MPD_HOST and MPD_PORT in your environment take
            precedence over these.*

    * config{filename}
            Filename to write output to if none given on command line.

    * config{poll_interval}
            How often in seconds to poll MPD

    * config{show_album_covers}
            Set to 1 if you want inline album covers.

    * config{amazon_cache}
            Filename to use as a cache for album cover info

    * config{always_show_albums}
            If set to 0 album covers are only shown if they are different
            than the revious one.

    * config{cover_url}
            Determines the size of inline cover art. Valid values are:
            url_smallcover, url_medcover, url_largecover. I've found medium
            to work nicely so that's the default.

    * config{report_***}
            Controls which events are reported on. The default settings
            don't report changes in bitrate since this is probably noisy and
            not of interest.

    * config{***_datestamp}
            Specifies the format of datestamps. See * man strftime* for
            valid format specs.

            The concept of config{change_datestamp} is to allow a shorter
            datestamp to appear when change_datestamp remains the same. The
            defaults values will report day/date only when they change.

  To Do
    * Make it it's own web server (!TRT?)
    * Somehow put new stuff at the top of the file

  Version History
    v0.2 Initial release Sat Apr 24 23:34:11 EDT 2004

    v0.3 Many bug fixes from #mpd people

         Now using CSS (thanks mackstann)

         Released Sun Apr 25 03:13:47 EDT 2004

