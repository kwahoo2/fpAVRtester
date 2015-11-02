#include "serialreader.h"

SerialReader::SerialReader(QObject *parent) :
    QObject(parent)
{
    serial = new QSerialPort(this);
    serial->setPortName("/dev/ttyUSB0");
}

void SerialReader::refreshPorts()
{
    ports = QSerialPortInfo::availablePorts();
}

void SerialReader::setPort(const int val)
{
    QString portName = ports[val].portName();
    serial->setPortName("/dev/"+portName); //linux only!
    qDebug() << "Port set to " << serial->portName();
}

void SerialReader::openSerial()
{

    serial->setBaudRate(9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->open(QIODevice::ReadWrite);

    serial->clear();
    QObject::connect(serial, SIGNAL(readyRead()),
                     this, SLOT(readSerial()));

}

void SerialReader::sendByte(const qint8 val)
{
    char str[] = {static_cast<char>(val)};
    serial->write(str, sizeof(str));
    qDebug() << "Sended " << str;
}

void SerialReader::readSerial()
{
    serialBuffer += serial->readAll();
    if (serialBuffer.length() >= 2)
    {
       // qDebug() << serialBuffer;
        emit sendResult(serialBuffer);
        serialBuffer.clear();
    }
}

void SerialReader::closeSerial()
{
    QObject::disconnect(serial, SIGNAL(readyRead()),
                     this, SLOT(readSerial()));
    serial->close();
}

bool SerialReader::isOpened()
{
    return serial->isOpen();
}

SerialReader::~SerialReader()
{
    serial->close();
}


