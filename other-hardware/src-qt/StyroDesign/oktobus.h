


class oktobus : public QObject{
	public:
		oktobus();
		
		int init(QString pardev);
	
		void motorStep(int motor, bool dir);
		void setOutput(uint8_t output, bool value);
		uint8_t readInputs();
	

	private:
		//Parport File Descriptor
		int fd;
		//stepcounters for motors
		int stepcount[2];
	
	public slots:
		
}
