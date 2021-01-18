#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class RegistrationForm;
}

class RegistrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationForm(QTcpSocket *socket, QWidget *parent = nullptr);
    ~RegistrationForm();

    void setIncorrect(int a);
    void addUser();
    void writeToFile();

signals:
    void autWindow();
    void mainWindow();

private slots:
    void on_RegisterNowPushButton_clicked();
    void on_LogiInPushButton_clicked();
    void on_EmailLineEdit_textChanged(const QString &arg1);
    void on_PasswordLineEdit_textChanged(const QString &arg1);
    void on_ConfirmPasswordLineEdit_textChanged(const QString &arg1);
    void on_FirstNameLineEdit_textChanged(const QString &arg1);
    void on_LastNameLineEdit_textChanged(const QString &arg1);

private:
    Ui::RegistrationForm *ui;
    QTcpSocket *socket;
    QByteArray Data;
    QString email;
    QString password;
    QString first_name;
    QString last_name;
    bool correct;
};

#endif // REGISTRATIONFORM_H
