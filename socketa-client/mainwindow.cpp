/*   客户端   */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "qhostinfo.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setWindowTitle(QString("客户端程序 "));
    //setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    socket = new QTcpSocket();



    QLabel *NEW_statusbar = new QLabel(this);
    NEW_statusbar->setText("想要了解更多请点击：www.qimstar.cn");
    ui->statusBar->addWidget(NEW_statusbar,1);

    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("8765");



}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()//连接
{
    if(ui->pushButton_Connect->text() == tr("连接"))
    {
        QString IP;
        int port;


        //获取IP地址
        IP = ui->lineEdit_IP->text();
        //获取端口号
        port = ui->lineEdit_Port->text().toInt();

        //取消已有的连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);

        //等待连接成功
        if(!socket->waitForConnected(30000))
        {
            qDebug() << "Connection failed!";
            message_erro.information(NULL, "温馨提示", "错误", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        qDebug() << "Connect successfully!";

        //发送按键使能
        ui->pushButton_Send->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connect->setText("断开连接");
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton_Connect->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }
}

void MainWindow::on_pushButton_Send_clicked()//发送
{
    qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
     //获取文本框内容并以ASCII码形式发送

    //socket->bind()
    socket->write(ui->textEdit_Send->toPlainText().toLatin1());
    socket->flush();//这个函数尽可能地从内部写缓冲区到底层网络套接字，而不阻塞。如果有任何数据被写入，该函数将返回true;否则返回假。
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->textEdit_Recv->toPlainText();
        str+=tr(buffer);
        //刷新显示
        ui->textEdit_Recv->setText(str+"   ");
    }
}

void MainWindow::socket_Disconnected()//连接失能
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    //修改按键文字
    ui->pushButton_Connect->setText("连接");
    qDebug() << "Disconnected!";
}

void MainWindow::on_action_2_triggered()
{
    QMessageBox box;
    box.information(NULL, "关于作者y", "黄淮学院 ", QMessageBox::NoButton, QMessageBox::NoButton);
}
void MainWindow::checkNetWorkOnline()
{


        //QHostInfo::lookupHost("www.baidu.com"，this,SLOT(onLookupHost(QHostInfo)));   //通过静态函数QHostInfo::lookupHost访问指定网络后会将结果返回到槽里面，然后再将结果发送信号出去


}
bool MainWindow::onLookupHost(QHostInfo host)
{
    if (host.error() != QHostInfo::NoError) {
        qDebug() << "Lookup failed:" << host.errorString();
        //网络未连接，发送信号通知
        return 0;
    }
    else{
        return 1;
    }



}
