default: muelltermine.ics
gelb.ics.snippet: remondis.html.snippet
	cat remondis.html.snippet | ruby gelbkalender_snippetproducer.rb 2010 > gelb.ics.snippet
#muelltermine.ics: USB_Restmuell.ics USB_Blaumuell.ics gelb.ics.snippet
#	ruby icsconverter.rb USB_Restmuell.ics USB_Blaumuell.ics gelb.ics.snippet > muelltermine.ics
muelltermine.ics: USB_Restmuell.ics USB_Blaumuell.ics USB_Gelbmuell.ics
	ruby icsconverter.rb USB_Restmuell.ics USB_Blaumuell.ics USB_Gelbmuell.ics > muelltermine.ics
