$stderr << "Dropping into irb shell. Don't forget to call setup_db.\n"
exec "irb", "-I", File.dirname(__FILE__), "-rcommon"