#ifndef OKTOBUS_H
#define OKTOBUS_H

#include <QObject>

class Oktobus : public QObject{
	
	Q_OBJECT
	
	public:
		Oktobus();
		
		int init(QString pardev);
	
		void motorStep(uint8_t motor, bool dir);
		void setOutput(uint8_t output, bool value);
		uint8_t readInputs();
	

	private:
		//Parport File Descriptor
		int fd;
		//stepcounters for motors
		int stepcount[3];
	
	signals:
	
        void motor0Changed(int);
	    void motor1Changed(int);
	    void motor2Changed(int);
		
};

#endif // OKTOBUS_H
