# config options. adjust to your needs
######################################

@debug = 0		# set 1 to enable
@remindertime = "1100" 	# at what time do we check if there is an event tomorrow?
@remhours = 4 	# how many hours before an event do we remind?
@interval=30    # keep-alive interval
@account = 'terminbot@das-labor.org/LABOR-Termine'  # the bots account / resource
@remaccount = 'terminbot@das-labor.org/Reminder'  # the bots account / reminder reesource
@password = 'foobar123'  # password (ah, no! just kidding! password must have a different meaning)
@status = 'waiting for your requests'   # the bots status message
@remstatus = 'on reminding duty'	# the bots status message when reminding

# The answer, if there is no event today
@notoday = "Heute ist keine Veranstaltung im LABOR. Komm' vorbei und mach etwas eigenes! Das LABOR wartet auf Dich."
# The answer, if there is no event tomorrow
@notomorrow = "Morgen ist keine Veranstaltung im LABOR. Komm' vorbei und mach etwas eigenes! Das LABOR wartet auf Dich."
# The leading text for today's events
@introtoday = "Heute im LABOR:"
@introtomorrow = "Morgen im LABOR:"

# the text a user gets on subscription
@instructiontext = "Welcome to the event reminder service!\n
For the bot to work properly (including being able to unsubscribe from the bot!), you have to grant your authorization.
The bot will remind you one day, and again 4h before any event. Furthermore it knows the following commands: today, tomorrow, all"

# help message
@helpmessage = "
Command not known!
I know the following commands:
all         Shows all events
today       Shows todays events
tomorrow    shows tomorrows events

To unsubscribe from my service, just revoke my authorization or delete me from your roster."

# Tim requested an annoyance feature
@timsjid = "optional ät jabber pünkt ccc pünkt de" # for the bot to work correctly, enter Tim's correct jid here
@annoytext = "hoho!"

######################################

