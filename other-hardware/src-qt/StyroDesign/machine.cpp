
#include "machine.h"

MachineDialog::MachineDialog(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	
	connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(reject()));
	
	
	
}
