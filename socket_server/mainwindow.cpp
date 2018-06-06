#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_Port->setText("8765");
    ui->pushButton_Send->setEnabled(false);

    server = new QTcpServer();

    //连接信号槽
    connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_pushButton_Listen_clicked()
{
     QMessageBox Listen_box;
    if(ui->pushButton_Listen->text() == tr("侦听"))
    {
        //从输入框获取端口号
        int port = ui->lineEdit_Port->text().toInt();

        //监听指定的端口
        if(!server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            qDebug()<<server->errorString();
            Listen_box.information(NULL, "温馨提示", "错误", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        //修改按键文字
        ui->pushButton_Listen->setText("取消侦听");
        qDebug()<< "Listen succeessfully!";
        Listen_box.information(NULL, "温馨提示", "侦听成功", QMessageBox::Yes, QMessageBox::Yes);

    }
    else
    {


        if(socket_flag == 1)//查看客户端是不是连接了已经
        {
        if(socket->state() == QAbstractSocket::ConnectedState)//如果连接了客户端，先断开客户端
        {
            //关闭连接
            socket->disconnectFromHost();
        }
        //取消侦听
        server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("侦听");
        //发送按键失能
        ui->pushButton_Send->setEnabled(false);
        }

        //取消侦听
        server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("侦听");
        //发送按键失能
        ui->pushButton_Send->setEnabled(false);

     }
}

void MainWindow::on_pushButton_Send_clicked()
{
    qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
    //获取文本框内容并以ASCII码形式发送
    socket->write(ui->textEdit_Send->toPlainText().toLatin1());//向客户端以网络格式发送信息
    socket->flush();
}

void MainWindow::server_New_Connect()
{

    socket_flag = 1;
    socket = server->nextPendingConnection(); //获取客户端socket信息
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    //发送按键使能

    ui->pushButton_Send->setEnabled(true);
    ui->textEdit_Recv->setText("A Client connect!\n");
    socket->write(tr("server:connect server\n").toLatin1());//向客户端以网络格式发送信息
    socket->flush();
    qDebug() << "A Client connect!";
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

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    qDebug() << "Disconnected!";
}
