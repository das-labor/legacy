#include "UsbEventDevice.h"

UsbEventDevice::UsbEventDevice(int vid, int pid) : UsbDevice(vid, pid)
{
	running = false;
}


UsbEventDevice::~UsbEventDevice()
{
	if(!this->isFinished())
	{
		stopEventThread();
		while(!this->isFinished());
	}
}


void UsbEventDevice::startEventThread(int endPoint, unsigned int bufSize, unsigned int readTimeout)
{
	running = true;
	_endPoint = endPoint;
	_bufSize = bufSize;
	_readTimeout = readTimeout;

	this->start();
}


void UsbEventDevice::stopEventThread()
{
	running = false;
}


void UsbEventDevice::run()
{
	eventThread(_endPoint, _bufSize, _readTimeout);
}


void UsbEventDevice::eventThread(int endPoint, unsigned int bufSize, unsigned int eventTimeout)
{
	QByteArray ba;
	ba.resize(bufSize);
	int recSize;

	while(running)
	{
		recSize = fetchInterrupt(endPoint, ba.data(), (int)bufSize, (int)eventTimeout);
		
		if(recSize > 0)
		{
			QByteArray &bab = *new QByteArray(ba.data(), recSize);
			emit interruptReceived(bab);
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
