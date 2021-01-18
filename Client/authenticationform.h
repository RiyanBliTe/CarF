#ifndef AUTHENTICATIONFORM_H
#define AUTHENTICATIONFORM_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class AuthenticationForm;
}

class AuthenticationForm : public QWidget
{
    Q_OBJECT

public:
    explicit AuthenticationForm(QTcpSocket *socket, QWidget *parent = nullptr);
    ~AuthenticationForm();

    void writeToFile();
    void setIncorrect();
    bool correct;

signals:
    void prevWindow(QString);
    void regWindow();

public slots:
    void on_LogInPushButton_clicked();
    void on_RegistrationPushButton_clicked();

private slots:
    void on_EmailLineEdit_textChanged(const QString &arg1);
    void on_PasswordLineEdit_textChanged(const QString &arg1);

private:
    Ui::AuthenticationForm *ui;
    QTcpSocket *socket;
    QByteArray Data;
    QString email;
    QString password;

};

#endif // AUTHENTICATIONFORM_H
