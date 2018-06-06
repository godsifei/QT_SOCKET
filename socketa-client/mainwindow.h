//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "qmessagebox.h"
#include "qlabel.h"
#include "qhostinfo.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_Connect_clicked();

    void on_pushButton_Send_clicked();

    void socket_Read_Data();

    void socket_Disconnected();

    void on_action_2_triggered();
    void checkNetWorkOnline();//检查网络是不是连接
    bool onLookupHost(QHostInfo host);//检查网络是不是有网络

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QMessageBox message_erro;
    QLabel LABEL;
};

#endif // MAINWINDOW_H
