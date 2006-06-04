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

    #ifndef CLIENT_H
    #define CLIENT_H

    #include <QObject>
    #include <Qt/QTcpSocket>

#include "CanPacket.h"

    class QLabel;
    class QLineEdit;
    class QPushButton;
    class QTcpSocket;

    class CanSocket : public QObject
    {
        Q_OBJECT

    public:
        CanSocket(QObject *parent = 0);
		
		void connectToCand(QString hostname, int port);
	
	signals:
		void gotPacket(CanPacket);
		
    private slots:
		void readTcp();
        void displayError(QAbstractSocket::SocketError socketError);
	
    public slots:
		void sendPacket(CanPacket);

	private:
		QByteArray *receiveBuffer;
        QTcpSocket *tcpSocket;

    };

    #endif
