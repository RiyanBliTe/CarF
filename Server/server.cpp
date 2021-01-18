#include "server.h"

Server::Server()
{
    //QHostAddress hostadd("192.168.0.103");
    if (!this->listen(QHostAddress::Any, 5356))
        qDebug() << "Cannot start server";
    else
    {
        qDebug() << "Server started";
        connect(this, &QTcpServer::newConnection, this, &Server::newConnection);
        db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("DRIVER={SQL SERVER};SERVER=DESKTOP-7GLTKJ9\\SQLEXPRESS;DATABASE=carf_db;UID=march;PWD=;Trusted_Connection=Yes;");
        if (!db.open())
        {
            qDebug() << "Cannot connect ot database";
        }
        else
        {
            qDebug() << "Succes connect to database";
        }
    }
    qDebug() << this->serverAddress();
    qDebug() << this->serverPort();
}

Server::~Server()
{
    this->close();
}

void Server::newConnection()
{
    QTcpSocket *clientSocket = this->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::smbDisconnected);

    clients << clientSocket;

    qDebug() << "New connection";
}

void Server::readyRead()
{
    QTcpSocket *clientSocket = (QTcpSocket*)sender();
    Data = clientSocket->readAll();
    qDebug() << Data;

    QString str = Data;
    QStringList list = str.split("|separator|");

    if (list.at(0) == "userupdate")
    {
        qDebug() << "true";
        // 0 - command, 1 - oldEmail, 2 - newEmail, 3 - newPassword, 4 - newFirst_name, 5 - newLast_name, 6 - newCountry, 7 - newCity, 8 - newAddress, 9 - newImage
        QByteArray imagearr = QByteArray::fromBase64(list.at(9).toLatin1());

        QSqlQuery query = QSqlQuery(db);
        query.prepare(QString("update clients set email = '%1', password = '%2', first_name = '%3', last_name = '%4', country = '%5', city = '%6', address = '%7', image = (:imageData) where email = '%8'")
                      .arg(list.at(2))
                      .arg(list.at(3))
                      .arg(list.at(4))
                      .arg(list.at(5))
                      .arg(list.at(6))
                      .arg(list.at(7))
                      .arg(list.at(8))
                      .arg(list.at(1)));
        query.bindValue( ":imageData", imagearr );
        query.exec();

        clientSocket->write("userupdate|separator|succes");
    }
    else if (list.at(0) == "getuserdata")
    {
        // 0 - command, 1 - email
        if (!emailExist("'"+list.at(1)+"'"))
        {
            qDebug() << "Unexist email";
        }
        else
        {
            QSqlQuery query = QSqlQuery(db);
            query.prepare(QString("select first_name, last_name, country, city, address, image from clients where email='%1'").arg(list.at(1)));
            if (!query.exec())
                qDebug() << "Wrong select in email";
            else
            {
                query.first();
                QByteArray imageArr = query.value(5).toByteArray();
                QString arts = imageArr.toBase64();
                // 1 - first_name, 2 - last_name, 3 - country, 4 - city, 5 - address, 6 - image
                QByteArray message = QString("userdata|separator|%1|separator|%2|separator|%3|separator|%4|separator|%5|separator|%6")
                        .arg(query.value(0).toString())
                        .arg(query.value(1).toString())
                        .arg(query.value(2).toString())
                        .arg(query.value(3).toString())
                        .arg(query.value(4).toString())
                        .arg(arts)
                        .toStdString().c_str();

                clientSocket->write(message);
                clientSocket->waitForBytesWritten(500);
            }
        }
    }
    else if (list.at(0) == "checkemail")
    {
        // 1 - email, 2 - password
        QSqlQuery query = QSqlQuery(db);
        query.prepare(QString("select count(client_id) as count from clients where email = '%1' and password = '%2'")
                      .arg(list.at(1))
                      .arg(list.at(2)));
        query.exec();
        query.first();
        if (query.value(0).toInt() != 0)
            clientSocket->write("checkemail|separator|true");
        else
            clientSocket->write("checkemail|separator|false");
        clientSocket->waitForBytesWritten(500);
    }
    else if (list.at(0) == "adduser")
    {
        // 1 - email, 2 - password, 3 - first_name, 4 - last_name
        QSqlQuery query = QSqlQuery(db);
        query.prepare(QString("insert into clients (first_name, last_name, email, password) values('%1', '%2', '%3', '%4');")
                      .arg(list.at(3))
                      .arg(list.at(4))
                      .arg(list.at(1))
                      .arg(list.at(2)));
        query.exec();
        query.clear();
        if (emailExist("'"+list.at(1)+"'"))
            clientSocket->write("registeruser|separator|true");
        else
            clientSocket->write("registeruser|separator|false");
        clientSocket->waitForBytesWritten(500);
    }
    else if (list.at(0) == "emailexist")
    {
        // 1 - email, 2 - password
        QSqlQuery query = QSqlQuery(db);
        query.prepare(QString("select count(client_id) as count from clients where email = '%1' and password = '%2'")
                      .arg(list.at(1))
                      .arg(list.at(2)));
        query.exec();
        query.first();
        if (query.value(0).toInt() != 0)
            clientSocket->write("emailexist|separator|true");
        else
            clientSocket->write("emailexist|separator|false");
        clientSocket->waitForBytesWritten(500);
    }
    else if (list.at(0) == "getchairs")
    {
        // select chair_id, chair_name, chair_price, chair_code, chair_image from chairs where buy = 'no'
        QSqlQuery query = QSqlQuery(db);
        query.prepare(QString("select chair_id, chair_name, chair_price, chair_code, chair_image from chairs where buy = 'no'"));
        query.exec();
        QByteArray message = "chairitems|separator|";
        while (query.next())
        {
        //query.first();
            QByteArray imageArr = query.value(4).toByteArray();
            QString arts = imageArr.toBase64();
            message += QString("chairitem|2separator2|%1|2separator2|%2|2separator2|%3|2separator2|%4|separator|")
                    .arg(query.value(0).toString())
                    .arg(query.value(1).toString())
                    .arg(query.value(2).toString())
                    .arg(query.value(3).toString())
                    //.arg(arts)
                    .toStdString().c_str();
        }
            clientSocket->write(message);
            clientSocket->waitForBytesWritten(500);
    }
    else if (list.at(0) == "chairsupdate")
    {
        for (int i = 1; i < list.size(); i++)
        {
            if (list.at(i) != "")
            {
                QSqlQuery query = QSqlQuery(db);
                query.prepare(QString("update chairs set buy = 'yes' where chair_id = %1").arg(list.at(i)));
                query.exec();
            }
        }
    }
}

void Server::smbDisconnected()
{
    qDebug() << "Have disconnet";
}

bool Server::emailExist(QString email)
{
    QSqlQuery query = QSqlQuery(db);
    query.prepare(QString("select count(client_id) as count from clients where email = %1;")
                  .arg(email));
    query.exec();
    query.first();
    if (query.value(0).toInt() != 0)
    {
        query.clear();
        return true;
    }
    query.clear();
    return false;
}
