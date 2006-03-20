#ifndef MACHINE_H
#define MACHINE_H


#include "ui_machine.h"


class MachineDialog : public QDialog
{
	Q_OBJECT

	public:
		MachineDialog(QWidget *parent = 0);

	private:
		Ui::MachineDialog ui;
};

#endif
