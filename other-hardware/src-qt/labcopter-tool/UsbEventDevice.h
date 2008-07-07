#include <QObject>
#include "UsbDevice.h"

class UsbEventDevice : public QThread, UsbDevice
{
public:
	UsbEventDevice(int vid, int pid);
	~UsbEventDevice();

	void startEventThread(int endPoint, unsigned int bufSize, unsigned int readTimeout);
	void stopEventThread();

	//not necessary
	//bool reqeuestRead(int requesttype, int request, int value, int index, QByteArray bytes, int size, int timeout);
	//bool reqeuestWrite(int requesttype, int request, int value, int index, QByteArray bytes, int size, int timeout);

public signals:
	void interruptReceived(QByteArray &bytes);
	void connectChanged(bool state);

private:
	void eventThread();
	virtual void run();

	bool running;

	int _endPoint;
	unsigned int _bufSize;
	unsigned int _readTimeout;
};
