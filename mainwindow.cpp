#include "mainwindow.h"
#include "ui_mainwindow.h"

#define float2fix(a) ((int)((a)*256.0))         //Convert float to fix. a is a float
#define fix2float(a) ((float)(a)/256.0)         //Convert fix to float. a is an int

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mySerRead = new SerialReader(this);
    ui->setupUi(this);

    QObject::connect(mySerRead, SIGNAL(sendResult(QByteArray)),
                     this, SLOT(updateResult(QByteArray)));

    mySerRead ->openSerial();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calcButton_clicked()
{
    float num0 = ui->lineEdit->text().toFloat();
    float num1 = ui->lineEdit_2->text().toFloat();

    int fixed0 = float2fix(num0);
    unsigned char fix00 = (unsigned char)((fixed0 >> 8) & 0xFF); //& 0xFF after or before cast?
    unsigned char fix01 = (unsigned char)(fixed0 & 0xFF);

    int fixed1 = float2fix(num1);
    unsigned char fix10 = (unsigned char)((fixed1 >> 8) & 0xFF);
    unsigned char fix11 = (unsigned char)(fixed1 & 0xFF);

    qDebug("%x %x", fix00, fix01);
    qDebug("%x %x", fix10, fix11);

    mySerRead->sendByte(fix00);
    mySerRead->sendByte(fix01);
    mySerRead->sendByte(fix10);
    mySerRead->sendByte(fix11);
}

void MainWindow::updateResult(QByteArray data)
{
    qDebug() << data;
    int returnVal = (((int)data.at(0)) << 8) + (int)data.at(1);
    qDebug("%x", returnVal);
    float result = fix2float(returnVal);
    ui->lineEdit_result->setText(QString::number(result));
}
