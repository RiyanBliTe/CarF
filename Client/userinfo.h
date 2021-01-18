#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QByteArray>

class UserInfo
{
    QString first_name;
    QString last_name;
    QString email;
    QString password;
    QString country;
    QString city;
    QString address;
    QByteArray imageArr;

public:
    UserInfo();

    void setEmail(QString _email);
    void setPassword(QString _password);
    void setFirstName(QString _first_name);
    void setLastName(QString _last_name);
    void setCountry(QString _country);
    void setCity(QString _city);
    void setAddress(QString _address);
    void setImageFile(QByteArray _imageArr);

    QString getEmail();
    QString getPassword();
    QString getFirst_name();
    QString getLast_Name();
    QString getCountry();
    QString getCity();
    QString getAddress();
    QByteArray getImageFile();
};

#endif // USERINFO_H
