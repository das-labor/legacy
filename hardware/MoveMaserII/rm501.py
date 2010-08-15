#!/usr/bin/python

# python lib to control the MoveMaster RM-501 robot

import serial
import sys
import time
import random


class RM501:
	axes = [0,0,0,0,0]
	
	def __init__(self, port='/dev/ttyUSB0'):
		self.serial = serial.Serial(
				port     = port,
				baudrate = 9600,
				bytesize = serial.EIGHTBITS,
				parity   = serial.PARITY_EVEN,
				stopbits = serial.STOPBITS_ONE,
				timeout  = 0,
#				xonxoff  = False,
#				rtscts   = False,
				xonxoff  = True,
				rtscts   = True,
				writeTimeout = 0,
				)

		self.axes[0] = 12000 # 12000
		self.axes[1] = 5200 # 5200
		self.axes[2] = -3600 # 3600
		self.axes[3] = -4800 # 4800
		self.axes[4] = -9600 # 9600
				
		self.serial.close()
		self.serial.open()
		if not self.serial.isOpen():
			print "failed to open port " + self.serial.portstr
		self.serial.dsrdtr = 1
		self.serial.xonxoff = 1
		self.serial.write(serial.XON)
		self.serial.write(serial.XOFF)
		self.serial.setRTS(True)
		self.serial.setDTR(True)
		
	def writeCommand(self, cmd):
		print time.ctime(), "CMD: " + cmd
		start_time = time.time()

		
		self.serial.write("%s"  % cmd[0])
		time.sleep(0.2)
		self.serial.flush()
		self.serial.write("%s\r" % cmd[1:])
		self.serial.flush()

		while not self.serial.getCTS():
			#print "waiting to finish ..."
			#self.debugStatus()
			time.sleep(0.05)

		tdiff = time.time() - start_time
		#print time.ctime(),"CMD DONE after %f seconds" % (tdiff)


	def nest(self):
		self.writeCommand("NT")

	def setHome(self):
		self.writeCommand("HO")

	def toOrigin(self):
		self.writeCommand("OG")

	def setSpeed(self, speed):
		self.writeCommand("SP %d" % speed)
		
	def move(self, body=0, shoulder=0, elbow=0, wrist1=0, wrist2=0):
		#self.setSpeed(9)
		self.axes[0] += body
		self.axes[1] += shoulder
		self.axes[2] += elbow
		self.axes[3] += wrist1
		self.axes[4] += wrist2
		print "new pos: (%s)" % ", ".join(["%s" % el for el in self.axes])
		self.writeCommand("MI %d, %d, %d, %d, %d, 0" % (body, shoulder, elbow, wrist1, wrist2))

	def savePos(self, pos, body=0, shoulder=0, elbow=0, wrist1=0, wrist2=0):
		self.writeCommand("PS %d, %d, %d, %d, %d, %d, 0" % (pos, body, shoulder, elbow, wrist1, wrist2))

	def toPos(self, pos):
		self.writeCommand("MO %d" % (pos))
		
	def gripOpen(self):
		self.writeCommand("GO")
	
	def gripClose(self):
		self.writeCommand("GC")
		
	def reset(self):
		self.writeCommand("RS")
		
	def debugStatus(self):
		print "CTS: ",self.serial.getCTS()
		print "DSR: ",self.serial.getDSR()
		print "RI : ",self.serial.getRI()
		print "RI : ",self.serial.getRI()
		print "CD : ",self.serial.getCD()	

	def runTest(self):
		self.reset()
		self.setSpeed(9)
		
		self.testsavedPosition()
		
	def testsavedPosition(self):
		self.nest()
		self.setHome()
		self.savePos(1, -3000, -2000,  1000,  1000, -1000)
		self.savePos(2,  3000,  1000, -1000, -1000, -1000)
		while 1:
			self.toPos(1)
			self.toPos(2)

	def testRandomPosition(self):
		self.nest()
		self.setHome()
		while 1:
			self.randomPosition()

	def testAxis(self):
		self.nest()
		self.setHome()
		# test axes
		print "== step 1"
		self.move(0, 0, 3600)
		self.move(0, 0, 0, 4800)
		self.move(0, -5200)
		self.move(-12000)

		print "== step 2"
		self.move(12000)
		self.move(0, 5200)
		self.move(0, 0, -3600)
		self.move(0, 0, 0, -4800)
	
	def test1(self):
		self.nest()
		self.setHome()
		while 1:
			self.move(-3000, -2000,  1000,  1000, -1000)
			self.move(-3000,  -1000, 1000, 1000, -1000)
			self.move(0,  1000, -1000, -1000, 1000)
			self.toOrigin()
	
	def randomPosition(self):
		target = [0,0,0,0]
		#target[0] = random.randint(0, 12000)
		#target[1] = random.randint(0, 5200)
		#target[2] = random.randint(0, 3600)
		#target[3] = random.randint(0, 4800)
		target[0] = random.randint(0, 12000)
		target[1] = random.randint(3000, 5200)
		target[2] = random.randint(3000, 3600)
		target[3] = random.randint(0, 4800)

		print "target: (%s)" % ", ".join(["%s" % el for el in target])
		
		diff = [0,0,0,0]
		for i in range(0,4):
			diff[i] = target[i] - self.axes[i]
		
		print "diff: (%s)" % ", ".join(["%s" % el for el in diff])
		
		self.move(diff[0], diff[1], diff[2], 0) #diff[2], diff[3])
		
def main():
	#t = RM501(port=int(sys.argv[1]))
	t = RM501()
	t.runTest()

if __name__ == "__main__":
    main()
