#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#include "../target.desc"

/*POSIX CODE*/
#ifdef _TTY_POSIX_
#include "posix_qextserialport.h"
#define QextBaseType Posix_QextSerialPort 

/*MS WINDOWS CODE*/
#else
#include "win_qextserialport.h"
#define QextBaseType Win_QextSerialPort
#endif

class QextSerialPort: public QextBaseType {
public:
    QextSerialPort();
    QextSerialPort(const char* name);
    QextSerialPort(PortSettings const& s);
    QextSerialPort(const char*name, PortSettings const& s);
    QextSerialPort(const QextSerialPort& s);
    QextSerialPort& operator=(const QextSerialPort&);
    virtual ~QextSerialPort();
};

#endif
