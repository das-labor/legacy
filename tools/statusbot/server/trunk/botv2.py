#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import re
import sleekxmpp
import feedparser

# Python versions before 3.0 do not use UTF-8 encoding
# by default. To ensure that Unicode is handled properly
# throughout SleekXMPP, we will set the default encoding
# ourselves to UTF-8.
if sys.version_info < (3, 0):
	reload(sys)
	sys.setdefaultencoding('utf8')
else:
	raw_input = input

class commands:
    
	def give(args):
		if args == "":
			connect.help(g)
		else:
			split = args.partition(" ")
			nick = split[0]
			split = split[2].partition(" ")
			cmd = split[0]
			args = split[2]
		return nick + ": " + commands.run[cmd][0](args)

	def help(args):
		if args == "":
			cmds = ""
			for k in commands.run.keys():
				cmds = cmds + " " + k
			return "Available commands:" + cmds
		else:
			return "help: " + commands.run[args][1]

	def google(args):
		if args == "":
			connect.help(g)
		else:
			return "https://www.google.com/search?q=" + args.replace(" ", "+")

	def yt(args):
		if args == "":
			connect.help(yt)
		else:
			return "https://www.youtube.com/results?search_query=" + args.replace(" ", "+")

	def translate(args):
		if args == "":
			connect.help(trans)
		else:
			tmp = args.partition(" ")
			word = tmp[0]
			if tmp[2] == "":
				lang = "en"
			else:
				lang = tmp[2]
			return "https://dict.leo.org/" + lang + "de?lp=" + lang + "de&search=" + word

	def wiki(args):
		if args == "":
			connect.help(wiki)
		else:
			tmp = args.partition(" ")
			term = tmp[0]
			if tmp[2] == "":
				lang = "de"
			else:
				lang = tmp[2]
		return "https://" + lang + ".wikipedia.org/w/?search=" + term

	def cve(args):
		if args == "":
			commands.help(cve)
		else:
			p = re.compile('^(?:cve-)?[0-9]{4}-[0-9]{4}$', re.I)
			if p.match(args):
				return "https://cvedetails.com/cve-details.php?t=1&cve_id=" + args
			else:
				return "https://www.google.com/search?q=site:cvedetails.com+" + args

	def mensa(args):
		feed = feedparser.parse("http://www.akafoe.de/module/speiseplan/rss_simple.php?id=1")
		spam = ""
		if args == "-v":
			for f in feed["entries"]:
				spam = spam + f["title"] + ":\n" + \
					f["summary"] + "\n--------\n"
		else:
			return feed["entries"][0]["link"]
		return spam

	run = {
		'help'  : (help      , "help [<cmd>] | Get help on cmd"         ),
		'give'  : (give      , "give <user> <cmd> | Redirect output of" +
					"<cmd> to <user>"                       ),
		'g'     : (google    , "g <search string> | google <string>"    ),
		'yt'    : (yt        , "yt <search string> | search youtube"    ),
		'dict'  : (translate , "trans <word> <{en|es|rf|it|ch|ru}>"     +
					"| Translate <word> @ dict.leo.org"     ),
		'wiki'  : (wiki      , "wiki <term> [<lang>] | Search wikipedia"),
		'cve'   : (cve       , "cve <cve#|term> | Search given CVE or"  +
					"term @ cvedetails.com"                 ),
		'mensa' : (mensa     , "mensa [-v] | Spam todays [-verbose]"    +
					"menu @ rub mensa"                      )
	}

class MUCBot(sleekxmpp.ClientXMPP):

	def __init__(self, jid, password, room, nick):
		sleekxmpp.ClientXMPP.__init__(self, jid, password)

		self.room = room
		self.nick = nick

		# The session_start event will be triggered when
		# the bot establishes its connection with the server
		# and the XML streams are ready for use. We want to
		# listen for this event so that we we can initialize
		# our roster.
		self.add_event_handler("session_start", self.start)

		# The groupchat_message event is triggered whenever a message
		# stanza is received from any chat room. If you also also
		# register a handler for the 'message' event, MUC messages
		# will be processed by both handlers.
		self.add_event_handler("groupchat_message", self.muc_message)

	def start(self, event):
		"""
		Process the session_start event.

		Typical actions for the session_start event are
		requesting the roster and broadcasting an initial
		presence stanza.

		Arguments:
			event -- An empty dictionary. The session_start
			event does not provide any additional
			data.
		"""
		self.get_roster()
		self.send_presence()
		self.plugin['xep_0045'].joinMUC(self.room,
																		self.nick,
																		# If a room password is needed, use:
																		# password=the_room_password,
																		wait=True)

	def muc_message(self, msg):
		"""
		Process incoming message stanzas from any chat room. Be aware
		that if you also have any handlers for the 'message' event,
		message stanzas may be processed by both handlers, so check
		the 'type' attribute when using a 'message' event handler.

		IMPORTANT: Always check that a message is not from yourself,
			otherwise you will create an infinite loop responding
			to your own messages.

		Arguments:
			msg -- The received message stanza. See the documentation
			for stanza objects and the Message stanza to see
			how it may be used.
		"""
		if msg['mucnick'] != self.nick and msg['body'][0] == "!":
			try:
				tmp = msg['body'].partition(" ")
				cmd = tmp[0][1:]
				args = tmp[2]
				self.send_message(mto=msg['from'].bare,
							mbody="%s" % commands.run[cmd][0](args),
							mtype='groupchat')
			except (KeyError, TypeError):
				self.send_message(mto=msg['from'].bare,
							mbody="%s" % commands.help(""),
							mtype='groupchat')

if __name__ == '__main__':
	jid = "status@das-labor.org"
	password = "password"
	room = "labor@conference.das-labor.org"
	nick = "ChanServ"

	# Setup the MUCBot and register plugins. Note that while plugins may
	# have interdependencies, the order in which you register them does
	# not matter.
	xmpp = MUCBot(jid, password, room, nick)
	xmpp.register_plugin('xep_0030') # Service Discovery
	xmpp.register_plugin('xep_0045') # Multi-User Chat
	xmpp.register_plugin('xep_0199') # XMPP Ping

	# Connect to the XMPP server and start processing XMPP stanzas.
	if xmpp.connect(use_ssl=False, use_tls=True):
		# If you do not have the dnspython library installed, you will need
		# to manually specify the name of the server if it does not match
		# the one in the JID. For example, to use Google Talk you would
		# need to use:
		#
		# if xmpp.connect(('talk.google.com', 5222)):
		#     ...
		xmpp.process(block=True)
		print("Done")
	else:
		print("Unable to connect.")
