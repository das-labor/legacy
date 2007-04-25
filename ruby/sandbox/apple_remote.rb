## remote bundled version
require 'remote'
a = OSX::AppleRemote.alloc.init
d = AppleRemoteDelegate.alloc.init
a.setDelegate d
a.startListening 0
OSX::NSApplication.sharedApplication.run

 
## pure cocoa version
#require 'osx/cocoa'
#include OSX
#OSX::NSBundle.bundleWithPath("/Library/Frameworks/AppleRemote.framework").load
#OSX.ns_import :AppleRemote
#
#class AppleRemoteDelegate < OSX::NSObject
#  addRubyMethod_withType('appleRemoteButton:pressedDown:', 'v@:ii')  
#  def appleRemoteButton_pressedDown(buttonIdentifier, pressedDown)
#    puts "button #{buttonIdentifier}, pressed #{pressedDown}"
#  end
#end
