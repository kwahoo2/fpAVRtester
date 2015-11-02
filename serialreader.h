#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
//#include <memory> //needed for RAII (unique_ptr)

using namespace std;

class SerialReader : public QObject
{
    Q_OBJECT
public:
    explicit SerialReader(QObject *parent = 0);
    bool isOpened();
    QList <QSerialPortInfo> ports;

protected:

signals:
    void sendResult(const QByteArray data);

private:
    static const qint64 maxBytes = 2; //small buffer for serial size
    QByteArray serialBuffer;
    QSerialPort *serial;
    ~SerialReader();

private slots:
    void readSerial();

public slots:
    void sendByte(const qint8);
    void openSerial();
    void closeSerial();
    void refreshPorts();
    void setPort(const int val);
};


#endif // SERIALREADER_H

