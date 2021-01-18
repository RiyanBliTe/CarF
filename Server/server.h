#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QList>

class Server: public QTcpServer
{
public:
    Server();
    ~Server();
private slots:
    void newConnection();
    void smbDisconnected();
    void readyRead();
private:
    QList<QTcpSocket*> clients;
    QSqlDatabase db;
    QJsonDocument doc;
    QJsonParseError docError;
    QByteArray Data;

    bool emailExist(QString email);
};

#endif // SERVER_H
