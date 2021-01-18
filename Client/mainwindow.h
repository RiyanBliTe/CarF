#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "authenticationform.h"
#include "registrationform.h"
#include <QMainWindow>
#include <QDesktopWidget>
#include <QDebug>
#include <QStringList>
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QIcon>
#include <QFileDialog>
#include "userinfo.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonArray>
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include "item.h"
#include "bakform.h"
#include <QGridLayout>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateBasketCount();

private slots:
    void on_ProfilePushButton_clicked();
    void on_CatalogPushButton_clicked();
    void on_BasketPushButton_clicked();
    void on_SignOutPushButton_clicked();
    void on_SavePushButton_clicked();
    void on_AvatarToolButton_clicked();

    void on_OrderPushButton_clicked();

public slots:
    void sockReady();
    void sockDisc();
    void addBak(QString str);
    void delBak(QString str);

private:
    Ui::MainWindow *ui;
    RegistrationForm *regForm;
    AuthenticationForm *autForm;

    int screenWidth;
    int screenHeight;

    int minimumWidth;
    int minimumHeight;

    int basketCount;
    int total;

    UserInfo* user;
    QTcpSocket *socket;
    QByteArray Data;
    QGridLayout *lay;
    QVBoxLayout *lay2;
    //Item items[20];
    Item *items;
    BakForm baks[20];
    int bakCou;
    bool isItems;

    QJsonDocument doc;
    QJsonParseError docError;

    void customWindow();
};
#endif // MAINWINDOW_H
