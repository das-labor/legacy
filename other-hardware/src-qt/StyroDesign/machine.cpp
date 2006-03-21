
#include "machine.h"

MachineDialog::MachineDialog(QWidget *parent, DrawArea *drawarea)
        : QDialog(parent)
{
	drawArea = drawarea;
	ui.setupUi(this);
	
	connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(reject()));
	timer = new QTimer(this);
	timer->setSingleShot(false);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	
	plotTimer = new QTimer(this);
	plotTimer->setSingleShot(false);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(doPlot()));
	
	
	connect(ui.pushButtonUp, SIGNAL(pressed()), this, SLOT(pressedUp()));
	connect(ui.pushButtonDown, SIGNAL(pressed()), this, SLOT(pressedDown()));
	connect(ui.pushButtonLeft, SIGNAL(pressed()), this, SLOT(pressedLeft()));
	connect(ui.pushButtonRight, SIGNAL(pressed()), this, SLOT(pressedRight()));
	connect(ui.pushButtonUp, SIGNAL(released()), this, SLOT(stop()));
	connect(ui.pushButtonDown, SIGNAL(released()), this, SLOT(stop()));
	connect(ui.pushButtonLeft, SIGNAL(released()), this, SLOT(stop()));
	connect(ui.pushButtonRight, SIGNAL(released()), this, SLOT(stop()));
	connect(ui.sliderMoveSpeed, SIGNAL(valueChanged(int)), this, SLOT(moveSpeedChanged(int)));
	connect(ui.sliderPlotSpeed, SIGNAL(valueChanged(int)), this, SLOT(plotSpeedChanged(int)));
	
	ui.sliderMoveSpeed->setValue(300);
	ui.sliderPlotSpeed->setValue(100);
	
	oktobus = new Oktobus();
	if (oktobus->init("/dev/parport0")) {
		//Fehler
	}
	stop();
	
	connect(oktobus, SIGNAL(motor1Changed(int)), this, SLOT(xPosChanged(int)));
	connect(oktobus, SIGNAL(motor2Changed(int)), this, SLOT(yPosChanged(int)));
		
}

void MachineDialog::xPosChanged(int pos) {
	QString qstr = QString("%1").arg(pos, 10);
	ui.XPosEdit->setText(qstr);
}


void MachineDialog::yPosChanged(int pos) {
	QString qstr = QString("%1").arg(pos, 10);
	ui.YPosEdit->setText(qstr);
}



void MachineDialog::pressedUp() {
	isUp = true;
	timer->start();
}

void MachineDialog::pressedDown() {
	isDown = true;
	timer->start();
}

void MachineDialog::pressedLeft() {
	isLeft = true;
	timer->start();
}

void MachineDialog::pressedRight() {
	isRight = true;
	timer->start();
}


void MachineDialog::moveSpeedChanged(int speed) {
	if(speed){
		timer->setInterval(1000/speed);
	}else{
		timer->setInterval(1000000);
	}
}

void MachineDialog::plotSpeedChanged(int speed) {
	if(speed){
		plotTimer->setInterval(1000/speed);
	}else{
		plotTimer->setInterval(1000000);
	}
}



void MachineDialog::stop() {
	isUp = false;
	isDown = false;
	isRight = false;
	isLeft = false;
	timer->stop();
}

void MachineDialog::update() {
	if (isUp)
		oktobus->motorStep(1, 1);
	if (isDown)
		oktobus->motorStep(1, 0);
	if (isLeft)
		oktobus->motorStep(2, 0);
	if (isRight)
		oktobus->motorStep(2, 1);
	
}

void MachineDialog::doPlot() {
	QString muh;	
	muh = drawArea->getChainCode();
}
