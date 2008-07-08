
#include <QObject>
#include <QThread>
#include <QByteArray>

#include "UsbDevice.h"


class UsbEventDevice : public QThread, public UsbDevice 
{
	Q_OBJECT;
	
public:
	UsbEventDevice(int vid, int pid);
	~UsbEventDevice();

	void startEventThread(int endPoint, unsigned int bufSize, unsigned int readTimeout);
	void stopEventThread();

	//not necessary
	//bool reqeuestRead(int request, int value, int index, QByteArray bytes, int size, int timeout);
	//bool reqeuestWrite(int request, int value, int index, QByteArray bytes, int size, int timeout);

signals:
	void interruptReceived(QByteArray &bytes);
	void connectChanged(bool state);

private:
	void eventThread(int endPoint, unsigned int bufSize, unsigned int eventTimeout);
	virtual void run();

	bool running;

	int _endPoint;
	unsigned int _bufSize;
	unsigned int _readTimeout;
};
