#!/usr/bin/env python
#Gtk VNC Viewer
#http://launchpad.net/gtkvncviewer
#(c) Clement Lorteau <northern_lights@users.sourceforge.net>

version = "0.4"

import sys
try:
 	import pygtk
  	pygtk.require("2.0")
except:
  	pass
try:
	import gtk
  	import gtk.glade
	import gtkvnc
	import gconf
	import gnomekeyring
	from optparse import OptionParser
	import gobject
except:
	sys.exit(1)
try:
	import LaunchpadIntegration
	launchpad = True
except:
	launchpad = False

import os

#internat'
import locale
import gettext
APP = 'gtkvncviewer'
DIR = 'locale'
locale.setlocale(locale.LC_ALL, '')
gettext.bindtextdomain(APP, DIR)
gettext.textdomain(APP)
gtk.glade.bindtextdomain(APP, DIR)
gtk.glade.textdomain(APP)
gettext.install(APP, DIR, unicode=1)
#lang = gettext.translation(APP, DIR, languages=['ar'])
#lang.install()
_ = gettext.gettext

def wait():
   while gtk.events_pending():
         gtk.main_iteration()

class GtkVncViewer:

	def __init__(self):
                self.parse_options()	
		#build GUI
		self.gladefile = "data/gtkvncviewer.glade"  
	        self.wTree = gtk.glade.XML(self.gladefile) 
		self.dialog = self.wTree.get_widget("connectDialog")
		self.about = self.wTree.get_widget("aboutDialog")
		self.about.set_version(version)
		self.window = self.wTree.get_widget("window")
		self.window_label = self.wTree.get_widget("window_label")
		self.window_toolbar_note = self.wTree.get_widget("toolbar_note")
		self.window_toolbar = self.wTree.get_widget("toolbar")
		self.layout = self.wTree.get_widget("viewport1")
		self.scrolledwindow = self.wTree.get_widget("scrolledwindow1")
		self.fullscreenButton = self.wTree.get_widget("fullscreenButton")
		self.fullscreenButton.set_active(False)
		self.iconview = self.wTree.get_widget("iconview1")
		self.keysButton = self.wTree.get_widget("keysButton")
		self.keysMenu = self.wTree.get_widget("keysMenu")
		self.keysMenu.attach_to_widget(self.keysButton, None)
		self.vnc=gtkvnc.Display()
		self.model = gtk.ListStore (str,str,str,gtk.gdk.Pixbuf)
		self.iconview.set_model(self.model)
		self.iconview.set_text_column(0)
		self.iconview.set_pixbuf_column(3)
		self.helpMenu = self.wTree.get_widget("helpMenu")
		self.helpButton = self.wTree.get_widget("helpButton")
		self.helpMenu.attach_to_widget(self.helpButton, None)
		if (launchpad):
			LaunchpadIntegration.set_sourcepackagename("gtkvncviewer")
			LaunchpadIntegration.add_items(self.helpMenu,0,False,False)
		else:
			self.helpButton.set_sensitive(False)
		
		if (self.dialog):
			self.window.connect("destroy", gtk.main_quit)
			#Create our dictionay and connect it
			dic = { "on_quitButton_clicked" : gtk.main_quit,
				"on_aboutButton_clicked" : self.show_about_dialog,
				"on_aboutDialog_response" : self.handle_about_dialog_answer,
				"on_aboutDialog_delete_event" : self.hide_about_dialog,
				"on_connectButton_clicked" : self.vncconnect,
				"on_window_delete_event" : self.close_window,
				"on_disconnectButton_clicked" : self.disconnect,
				"on_addButton_clicked" : self.add_server,
				"on_iconview1_selection_changed" : self.selected,
				"on_iconview1_item_activated" : self.activated,
				"on_delButton_clicked" : self.delete_clicked,
				"on_screenshotButton_clicked" : self.screenshot,
				"on_helpButton_clicked" : self.helpMenuPop,
				"on_togglebutton1_toggled" : self.fullscreen,
				"on_toolbar_note_entered" : self.show_hide_toolbar,
				"on_window_motion_notify_event" : self.mouse_moved_in_window,
				"on_desktopIconButton_clicked" : self.icon_on_desktop,
				"on_CtrlAltDelmenuitem_activate": self.send_cad,
				"on_CtrlAltBackmenuitem_activate": self.send_cab,
				"on_CtrlEscmenuitem_activate": self.send_ce,
				"on_CtrlAltF1menuitem_activate": self.send_caf1,
				"on_CtrlAltF2menuitem_activate": self.send_caf2,
				"on_CtrlAltF3menuitem_activate": self.send_caf3,
				"on_CtrlAltF4menuitem_activate": self.send_caf4,
				"on_CtrlAltF5menuitem_activate": self.send_caf5,
				"on_CtrlAltF6menuitem_activate": self.send_caf6,
				"on_CtrlAltF7menuitem_activate": self.send_caf7,
				"on_CtrlAltF8menuitem_activate": self.send_caf8,
				"on_CtrlAltF9menuitem_activate": self.send_caf9,
				"on_CtrlAltF10menuitem_activate": self.send_caf10,
				"on_CtrlAltF11menuitem_activate": self.send_caf11,
				"on_CtrlAltF12menuitem_activate": self.send_caf12,
				"on_AltRlessitem_activate": self.send_arless,
				"on_AltRqitem_activate": self.send_arq,
				"on_AltRszligitem_activate": self.send_arszlig,
				"on_AltRplusitem_activate": self.send_arplus,
				"on_AltR7item_activate": self.send_ar7,
				"on_AltR8item_activate": self.send_ar8,
				"on_AltR9item_activate": self.send_ar9,
				"on_AltR0item_activate": self.send_ar0,
				"on_keysButton_clicked" : self.keysMenuPop}
			self.wTree.signal_autoconnect(dic)
			self.dialog.show()
		
		#read stored credentials
		GCONF_AUTH_KEY = "/apps/gtkvncviewer"
		keyring = gnomekeyring.get_default_keyring_sync()
		gconfclient = gconf.client_get_default()
		gconfclient.add_dir (GCONF_AUTH_KEY,
			gconf.CLIENT_PRELOAD_RECURSIVE)
		entries = gconfclient.all_entries(GCONF_AUTH_KEY)
		if len(entries) == 0:
			return #found nothing
		for i in range(len(entries)):
			server = entries[i].get_key()
			server = server.replace (GCONF_AUTH_KEY+"/", "")
			auth_token = entries[i].get_value().get_int()
			if auth_token > 0:
				try:
					secret = gnomekeyring.item_get_info_sync(keyring, auth_token).get_secret()
				except gnomekeyring.DeniedError:
					username = None
					password = None
					auth_token = 0
					print _("gnome-keyring access denied")
				else:
					username, password = secret.split('\n')
			else:
				username = None
				password = None
			pixbuf = self.iconview.render_icon(gtk.STOCK_NETWORK, gtk.ICON_SIZE_BUTTON)
			self.model.append([server, username, password, pixbuf])

		#if a server was specified at startup, connect to it
		if self.startup_options.server:
			res = self.find_server(self.startup_options.server)
			if (res): #found, connect
				iter = res[3]
				self.iconview.select_path(iter)
				wait()
				self.vncconnect(self.window)
			else: #not found, just fill the server name in the dialog
				server_textbox = self.wTree.get_widget("serverEntry")
				user_textbox = self.wTree.get_widget("usernameEntry")
				server_textbox.set_text(self.startup_options.server)
				user_textbox.grab_focus()
	
	def fullscreen (self, data):
		if (self.fullscreenButton.get_active()):
			self.window.fullscreen()
			self.window_toolbar_note.show_all()
			self.window_toolbar.hide_all()
		else:
			self.window.unfullscreen()
			self.window_toolbar_note.hide_all()
			self.window_toolbar.show_all()
		return False

	def helpMenuPop (self, data):
		self.helpMenu.popup(None, None, None, 0, 0, gtk.get_current_event_time())

	def keysMenuPop (self, data):
		self.keysMenu.popup(None, None, None, 0, 0, gtk.get_current_event_time())
	
	def screenshot (self, data):
		homeDir = os.environ.get('HOME', None) #=> can't work on Windows
		pix = self.vnc.get_pixbuf()
        	pix.save(homeDir+"/vnc.png", "png", { "tEXt::Generator App": "gtkvncviewer" })
		dialog = gtk.MessageDialog (self.window,
			gtk.MESSAGE_INFO,
			gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
			gtk.BUTTONS_OK,
			_("Screenshot saved in")+" "+homeDir+"/vnc.png")
		dialog.run()
		dialog.destroy()
		return False
	
	def send_cad (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "Delete"])
		print _("Sent Ctrl+Alt+Delete")
		self.vnc.grab_focus()

	def send_cab (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "BackSpace"])
		print _("Sent Ctrl+Alt+BackSpace")
		self.vnc.grab_focus()

	def send_ce (self, data):
		self.vnc.send_keys(["Control_L", "Escape"])
		print _("Sent Ctrl+Escape")
		self.vnc.grab_focus()

	def send_caf1 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F1"])
		print _("Sent Ctrl+Alt+F1")
		self.vnc.grab_focus()

	def send_caf2 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F2"])
		print _("Sent Ctrl+Alt+F2")
		self.vnc.grab_focus()

	def send_caf3 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F3"])
		print _("Sent Ctrl+Alt+F3")
		self.vnc.grab_focus()

	def send_caf4 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F4"])
		print _("Sent Ctrl+Alt+F4")
		self.vnc.grab_focus()

	def send_caf5 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F5"])
		print _("Sent Ctrl+Alt+F5")
		self.vnc.grab_focus()

	def send_caf6 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F6"])
		print _("Sent Ctrl+Alt+F6")
		self.vnc.grab_focus()

	def send_caf7 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F7"])
		print _("Sent Ctrl+Alt+F7")
		self.vnc.grab_focus()

	def send_caf8 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F8"])
		print _("Sent Ctrl+Alt+F8")
		self.vnc.grab_focus()

	def send_caf9 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F9"])
		print _("Sent Ctrl+Alt+F9")
		self.vnc.grab_focus()

	def send_caf10 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F10"])
		print _("Sent Ctrl+Alt+F10")
		self.vnc.grab_focus()

	def send_caf11 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F11"])
		print _("Sent Ctrl+Alt+F11")
		self.vnc.grab_focus()

	def send_caf12 (self, data):
		self.vnc.send_keys(["Control_L", "Alt_L", "F12"])
		print _("Sent Ctrl+Alt+F12")
		self.vnc.grab_focus()

	def send_arless (self, data):
		self.vnc.send_keys(["Alt_R", "less"])
		print _("Sent AltR+<")
		self.vnc.grab_focus()

	def send_arq (self, data):
		self.vnc.send_keys(["Alt_R", "Q"])
		print _("Sent AltR+Q")
		self.vnc.grab_focus()

	def send_arszlig (self, data):
		self.vnc.send_keys(["Alt_R", "ssharp"])
		print _("Sent AltR+Szlig")
		self.vnc.grab_focus()

	def send_arplus (self, data):
		self.vnc.send_keys(["Alt_R", "plus"])
		print _("Sent AltR++")
		self.vnc.grab_focus()

	def send_ar7 (self, data):
		self.vnc.send_keys(["Alt_R", "7"])
		print _("Sent AltR+7")
		self.vnc.grab_focus()

	def send_ar8 (self, data):
		self.vnc.send_keys(["Alt_R", "8"])
		print _("Sent AltR+8")
		self.vnc.grab_focus()

	def send_ar9 (self, data):
		self.vnc.send_keys(["Alt_R", "9"])
		print _("Sent AltR+9")
		self.vnc.grab_focus()

	def send_ar0 (self, data):
		self.vnc.send_keys(["Alt_R", "0"])
		print _("Sent AltR+0")
		self.vnc.grab_focus()

	def delete_clicked (self, data):		
		select = self.iconview.get_selected_items()
		if len(select) == 0:
			print _("nothing to delete")
			return

		i = select[0][0]
		iter = self.model.get_iter(i)
		s = self.model.get(iter,0,1,2)
		server = s[0]
		
		#ask confirmation
		dialog = gtk.MessageDialog (self.window,
			gtk.MESSAGE_QUESTION,
			gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
			gtk.BUTTONS_YES_NO,
			_("Are you sure you want remove the server \"%(server)s\"?") %{"server": server})
		r = dialog.run()
		dialog.destroy()
		if (r == gtk.RESPONSE_NO):
			print _("deletion canceled")
		else:
			GCONF_AUTH_KEY = "/apps/gtkvncviewer"	
			gconfclient = gconf.client_get_default()
			gconfclient.add_dir (GCONF_AUTH_KEY,
				gconf.CLIENT_PRELOAD_RECURSIVE)
			entry = gconfclient.get_entry(GCONF_AUTH_KEY+"/"+server,
				"", False)
			server = entry.get_key()
			server = server.replace (GCONF_AUTH_KEY+"/", "")
			auth_token = entry.get_value().get_int()
			if auth_token > 0:
				try:
					#delete gnome-keyring entry
					keyring = gnomekeyring.get_default_keyring_sync()
					gnomekeyring.item_delete_sync(keyring, auth_token)
					#delete gconf entry
					gconfclient.unset(GCONF_AUTH_KEY+"/"+server)
					#remove from icon view
					select = self.iconview.get_selected_items()
					if len(select) == 0:
						return
					i = select[0][0]
					iter = self.model.get_iter(i)
					self.model.remove(iter)
					print server+" deleted"
				except gnomekeyring.DeniedError:
					auth_token = 0
					print _("gnome-keyring access denied, could not delete")
			else:
				print _("gconf read error, could not delete")

	def activated (self, widget, data):
		self.vncconnect(self.window)

	def selected (self, data):
		username = self.wTree.get_widget("usernameEntry")
		password = self.wTree.get_widget("pwdEntry")
		server = self.wTree.get_widget("serverEntry")
		select = self.iconview.get_selected_items()
		if len(select) == 0:
			return
		i = select[0][0]
		iter = self.model.get_iter(i)
		s = self.model.get(iter,0,1,2)
		server.set_text(s[0])
		if (s[1] != None):
			username.set_text(s[1])
		else:
			username.set_text("")
		if (s[2] != None):
			password.set_text(s[2])
		else:
			password.set_text("")

	#finds the server 'name', and return its credentials and its iconview path
	#if not found, return False
	def find_server (self, name):
		iter = self.model.get_iter_first()
		while (iter != None):
			row = self.model.get(iter, 0, 1, 2)  #server, username, pwd
			if row[0] == name:
				row += ( self.model.get_path(iter), )
				return row
			iter = self.model.iter_next(iter)
                return False
	
	def add_server (self, data):
		#add it to the iconview
		pixbuf = self.iconview.render_icon(gtk.STOCK_NETWORK, gtk.ICON_SIZE_BUTTON)
		username = self.wTree.get_widget("usernameEntry").get_text()
		password = self.wTree.get_widget("pwdEntry").get_text()
		server = self.wTree.get_widget("serverEntry").get_text()
		if server == "":
			return
		self.model.append([server, username, password, pixbuf])

		#now save it in gnome-keyring
		GCONF_AUTH_KEY = "/apps/gtkvncviewer/"+server
		keyring = gnomekeyring.get_default_keyring_sync()
		auth_token = gnomekeyring.item_create_sync(
                	keyring,
                	gnomekeyring.ITEM_GENERIC_SECRET,
                	"gtkvncviewer VNC server credentials ("+server+")",
                	dict(appname="gtkvncviewer, credentials of vnc server \""+server+"\""),
			"\n".join((username, password)), True)
           	gconf.client_get_default().set_int(GCONF_AUTH_KEY, auth_token)

	def quit():
		self.vnc.close()
		print _("Bye.")
		if (self.window):
			self.window.destroy()

	def close_window(self, widget, data):
		quit()
		return False
	
	def disconnect(self, data):
		quit()

	def show_hide_toolbar(self, widget, data):
		print "show_hide_toolbar"
		return False

	#if in fullscreen and mouse on top, show toolbar
	def mouse_moved_in_window(self, widget, data):
		coords = self.window.window.get_pointer()
		y = coords[1]
		if y <= 5 and self.fullscreenButton.get_active():
			self.window_toolbar.show_all()
			#setup timer that will hide toolbar when ended
			gobject.timeout_add(2000, self.window_toolbar.hide_all) #2 sec.

	def icon_on_desktop(self, data):
		server = self.current_server
		comment = _("Connect to the remote desktop: %s" % (server))
		text = """
[Desktop Entry]
Name=%s
Comment=%s
Exec=gtkvncviewer -s %s
Icon=/usr/share/gtkvncviewer/data/gtkvncviewer_64.png
StartupNotify=true
Terminal=false
Type=Application
Categories=Network;
""" % ("VNC: "+server, comment, server)
		open(os.path.join(os.environ['HOME']+"/Desktop",
				  server+".desktop"),"w").write(text)
			

	def handle_about_dialog_answer(self, widget, data):
		if(data==-6):
			self.about.hide()

	def show_about_dialog(self, widget):
		self.about.show()

	def hide_about_dialog(self, widget, data):
		self.about.hide()
		return True

	def vncconnect(self, window):
		self.dialog.window.set_cursor(gtk.gdk.Cursor(gtk.gdk.WATCH))
		username = self.wTree.get_widget("usernameEntry").get_text()
		password = self.wTree.get_widget("pwdEntry").get_text()
		server = self.wTree.get_widget("serverEntry").get_text()
		self.vnc.set_credential(gtkvnc.CREDENTIAL_USERNAME, username)
		self.vnc.set_credential(gtkvnc.CREDENTIAL_PASSWORD, password)
		self.vnc.set_credential(gtkvnc.CREDENTIAL_CLIENTNAME, "gtkvncviewer")
		print _("Connecting to %s...") % server 
		serverport = server.split(':')
		server = serverport[0]
		try:
			port = serverport[1]
		except:
			port = "5900"
		self.vnc.open_host(server, port)
		#vnc.connect("vnc-auth-credential", self.vnc_auth_cred)
		self.vnc.connect("vnc-connected", self.vnc_connected, self)
		self.vnc.connect("vnc-initialized", self.vnc_initialized, username, server, self)
		self.vnc.connect("vnc-disconnected", self.vnc_disconnected, self.dialog, self)

	def vnc_initialized (src, vnc, username, server, self):
		print _("Connection initialized")
		title = "%s@%s - gtkvncviewer" % (username, server)
		self.current_server = server
		self.dialog.hide()
		self.window.set_title(title)
		self.window_label.set_markup ("<big><b>%s@%s</b></big>" % (username, server))
		self.window.show_all()
		self.window_toolbar_note.hide_all()
		self.window.resize (vnc.get_width(), vnc.get_height())
		vnc.grab_focus()
		
	def vnc_disconnected(src, vnc, window, self):
		print _("Disconnected")
		dialog = gtk.MessageDialog (window,
			gtk.MESSAGE_INFO,
			gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
			gtk.BUTTONS_OK,
			_("You have been disconnected"))
		dialog.run()
		dialog.destroy()
		self.vnc.destroy()
		self.vnc=gtkvnc.Display()
		window.window.set_cursor(gtk.gdk.Cursor(gtk.gdk.ARROW))

	def vnc_connected(src, data, self):
		print _("Connected")
		self.layout.add(self.vnc)
		self.vnc.realize()

        def parse_options(self):
		parser = OptionParser()
		parser.add_option("-s", "--server", dest="server",
				  action="store", type="string",
				  help=_("if server is known by gtkvncviewer, immediately connect to it"),
				  metavar=_("server"))
		(options, args) = parser.parse_args()
		self.startup_options = options

if __name__ == "__main__":
	instance = GtkVncViewer()
	gtk.main()
