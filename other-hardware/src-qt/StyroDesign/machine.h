#ifndef MACHINE_H
#define MACHINE_H

#include <QTimer>

#include "ui_machine.h"
#include "oktobus.h"
#include "display.h"

class MachineDialog : public QDialog
{
	Q_OBJECT

	public:
		MachineDialog(QWidget *parent, DrawArea *);

	public slots:
		void xPosChanged(int);	
		void yPosChanged(int);
		void moveSpeedChanged(int);	
		void plotSpeedChanged(int);	
	
		void heaterChanged(bool state);
	
		void pressedUp();
		void pressedDown();
		void pressedLeft();
		void pressedRight();
		void pressedPlot();
		void stop();
	
	
		void update();
		void doPlot();
	
	private:
		
		
		DrawArea * drawArea;
		QString chainCode;
	
		Oktobus* oktobus;
		
		bool isUp, isDown, isRight, isLeft;
		QTimer *timer, *plotTimer;
	
		
		Ui::MachineDialog ui;
};


#endif
