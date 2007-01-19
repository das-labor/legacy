# config options. adjust to your needs
######################################

@debug = 1		# set 1 to enable
@interval=30    # keep-alive interval
@statustimeout=4	# how many keep-alive intervals to wait before setting status to NONE (20 might be a good value)
@account = 'statusdaemon@das-labor.org/status'  # the bots account / resource
@password = 'foobar23'  # password (ah, no! just kidding! password must have a different meaning)
@status = 'no status available, please wait...'   # the bots status message

@setaccount = "statsetter@das-labor.org/status" # the account that may set the status

# labor status icon variables
@staticon = 'status.png'
@iconon = 'statusON.png'
@iconoff = 'statusOFF.png'
@iconnone = 'statusNONE.png'

# the text a user gets on subscription
@instructiontext = "This is the LABOR status daemon!\n Go away!
"

######################################

