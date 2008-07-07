#include "UsbEventDevice.h"

UsbEventDevice::UsbEventDevice(int vid, int pid) : UsbDevice(int vid, int pid)
{
	running = false;
}


UsbEventDevice::~UsbEventDevice()
{
	if(!this.finished())
	{
		stopEventThread();
		while(!this.finished());
	}
}


void UsbEventDevice::startEventThread(int endPoint, unsigned int bufSize, unsigned int readTimeout)
{
	running = true;
	_endPoint = endPoint;
	_bufSize = bufSize;
	_readTimeout = readTimeout;

	this.start();
}


void UsbEventDevice::stopEventThread()
{
	running = false;
}


void UsbEventDevice::run()
{
	EventThread(_endPoint, _bufSize, _readTimeout);
}


void UsbEventDevice::EventThread(int endPoint, unsigned int bufSize, unsigned int eventTimeout)
{
	QByteArray ba(bufSize);
	int recSize;

	while(running)
	{
		recSize = fetchInterrupt(0, ba.constData(), bufSize, 0);

		if(recSize > 0)
		{
			emit interruptReceived(new QByteArray(ba.constData(), recSize));
		}
	}

	QThread::exit();
}


//not necessary
/*bool UsbEventDevice::reqeuestRead(int requesttype, int request, int value, int index, QByteArray &bytes, int size, int timeout)
{
	bytes.resize(size+1);

	requestRead();
}

bool UsbEventDevice::reqeuestWrite(int requesttype, int request, int value, int index, QByteArray &bytes, int size, int timeout)
{
}
*/