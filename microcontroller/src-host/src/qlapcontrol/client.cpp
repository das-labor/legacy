/****************************************************************************
    **
    ** Copyright (C) 2004-2006 Trolltech AS. All rights reserved.
    **
    ** This file is part of the documentation of the Qt Toolkit.
    **
    ** This file may be used under the terms of the GNU General Public
    ** License version 2.0 as published by the Free Software Foundation
    ** and appearing in the file LICENSE.GPL included in the packaging of
    ** this file.  Please review the following information to ensure GNU
    ** General Public Licensing requirements will be met:
    ** http://www.trolltech.com/products/qt/opensource.html
    **
    ** If you are unsure which license is appropriate for your use, please
    ** review the following information:
    ** http://www.trolltech.com/products/qt/licensing.html or contact the
    ** sales department at sales@trolltech.com.
    **
    ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
    ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
    **
    ****************************************************************************/

#include <QtGui>
#include <QObject>
#include <Qt/QtNetwork>

#include "client.h"

CanSocket::CanSocket(QObject *parent)
        : QObject(parent)
{
	receiveBuffer = new QByteArray();
	tcpSocket = new QTcpSocket(this);

	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readTcp()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void CanSocket::connectToCand(QString host, int port)
{
        tcpSocket->abort();
        tcpSocket->connectToHost(host, port);
}

void CanSocket::sendPacket(CanPacket cp){
	tcpSocket->write(*(cp.toCann()));
}
	
	
void CanSocket::readTcp()
{
	*receiveBuffer += tcpSocket->readAll();
			
	if (receiveBuffer->size() < 2){
		return;	
	}

	uint8_t size;
		
	while (size = (*receiveBuffer)[0], receiveBuffer->size() >= size+2){
		//CAN Packet type
		if ((uint8_t)(*receiveBuffer)[1] == 0x11 ){
			CanPacket cp;
			cp.fromCann(receiveBuffer->mid(2, size));
			emit gotPacket(cp);
		}
		receiveBuffer->remove(0, size+2);
		if (receiveBuffer->size() < 2){
			return;	
		}
	}
}

void CanSocket::displayError(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
	case QAbstractSocket::RemoteHostClosedError:
		break;
	case QAbstractSocket::HostNotFoundError:
		QMessageBox::information(0, tr("Fortune Client"),
				tr("The host was not found. Please check the "
					"host name and port settings."));
		break;
	case QAbstractSocket::ConnectionRefusedError:
		QMessageBox::information(0, tr("Fortune Client"),
			tr("The connection was refused by the peer. "
			"Make sure the fortune server is running, "
			"and check that the host name and port "
			"settings are correct."));
		break;
	default:
		QMessageBox::information(0, tr("Fortune Client"),
			tr("The following error occurred: %1.")
			.arg(tcpSocket->errorString()));
	}
}
