#include "userinfo.h"

UserInfo::UserInfo()
{
    this->email = "";
    this->password = "";
    this->first_name = "";
    this->last_name = "";
    this->country = "";
    this->city = "";
    this->address = "";
}

void UserInfo::setEmail(QString _email) { this->email = _email; }
void UserInfo::setPassword(QString _password) { this->password = _password; }
void UserInfo::setFirstName(QString _first_name) { this->first_name = _first_name; }
void UserInfo::setLastName(QString _last_name) { this->last_name = _last_name; }
void UserInfo::setCountry(QString _country) { this->country = _country; }
void UserInfo::setCity(QString _city) { this->city = _city; }
void UserInfo::setAddress(QString _address) { this->address = _address; }
void UserInfo::setImageFile(QByteArray _imageArr) { this->imageArr = _imageArr; }

QString UserInfo::getEmail() { return email; }
QString UserInfo::getPassword() { return password; }
QString UserInfo::getFirst_name() { return first_name; }
QString UserInfo::getLast_Name() { return last_name; }
QString UserInfo::getCountry() { return country; }
QString UserInfo::getCity() { return city; }
QString UserInfo::getAddress() { return address; }
QByteArray UserInfo::getImageFile() { return imageArr; }
