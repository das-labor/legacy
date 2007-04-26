require 'osx/cocoa'

OSX::NSBundle.bundleWithPath("/System/Library/Frameworks/IOBluetoothUI.framework").load
OSX.ns_import :IOBluetoothDeviceSelectorController
OSX.ns_import :IOBluetoothDeviceInquiry
#OSX.ns_import :IOBluetoothDevice

bluedevices = OSX::IOBluetoothDeviceSelectorController.deviceSelector
# bluedevices = OSX::IOBluetoothDeviceInquiry #OSX::IOBluetoothDeviceSelectorController.deviceSelector

bluedevices.runModal

p blue_data = bluedevices.getResults