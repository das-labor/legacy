SCONS:=$(shell which scons) 

trousers:
	$(SCONS) trousers=1

microtss:
	$(SCONS) trousers=0

all: trousers

clean:
	$(SCONS) -c
