require 'osx/cocoa'

# magic to prevent a Cocoa warning when this is run from the command line
# is this a "bug" (Apple's problem) or "abuse of the framework" (mine)?
OSX::NSApplication.sharedApplication

# use Ruby's open classes to add some new features to NSImage
class OSX::NSImage
  def writeJPEG(filename)
    bits = OSX::NSBitmapImageRep.alloc.initWithData(self.TIFFRepresentation)
    data = bits.representationUsingType_properties(OSX::NSTIFFFileType, nil)
    data.writeToFile_atomically(filename, false)
  end

  def copyAndScale(factor)
    newWidth, newHeight = size.width*factor, size.height*factor
    newImage = OSX::NSImage.alloc.initWithSize [newWidth, newHeight]
    newImage.lockFocus
    drawInRect_fromRect_operation_fraction(
      [0, 0, newWidth, newHeight],
      [0, 0, size.width, size.height],
      OSX::NSCompositeSourceOver,
      1.0)
    newImage.unlockFocus
    newImage
  end
end

def resizeImage(sourceFileName, scaledFileName)
  sourceImage = OSX::NSImage.alloc.initWithContentsOfFile(sourceFileName)
  scaledImage = sourceImage.copyAndScale 0.5
  scaledImage.writeJPEG scaledFileName
end

resizeImage("0.jpg", "half-sized.tif")