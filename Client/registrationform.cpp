#include "registrationform.h"
#include "ui_registrationform.h"

RegistrationForm::RegistrationForm(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
    this->socket = socket;

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("CARF Registration Form");
    setWindowIcon(QIcon(".\\resources\\icon.png"));

    correct = true;
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

void RegistrationForm::on_RegisterNowPushButton_clicked()
{
    if (ui->PasswordLineEdit->text() != ui->ConfirmPasswordLineEdit->text())
        setIncorrect(2);
    else if (ui->FirstNameLineEdit->text().isEmpty() || ui->LastNameLineEdit->text().isEmpty() ||
             ui->EmailLineEdit->text().isEmpty() || ui->PasswordLineEdit->text().isEmpty())
        setIncorrect(3);
    else
    {
        email = ui->EmailLineEdit->text();
        password = ui->PasswordLineEdit->text();
        first_name = ui->FirstNameLineEdit->text();
        last_name = ui->LastNameLineEdit->text();
        socket->write(QString("emailexist|separator|%1|separator|%2")
                  .arg(ui->EmailLineEdit->text())
                  .arg(ui->PasswordLineEdit->text())
                  .toStdString().c_str());
        socket->waitForBytesWritten(500);
    }
}

void RegistrationForm::on_LogiInPushButton_clicked()
{
    this->hide();
    emit autWindow();
}

void RegistrationForm::setIncorrect(int a)
{
    if (a == 0)
    {
        ui->ExistLabel->setText("This email already exists");
        ui->ExistLabel->setStyleSheet("color: red;");
        correct = false;
    }
    else if (a == 1)
    {
        ui->ExistLabel->setText("Error in user registration in server");
        ui->ExistLabel->setStyleSheet("color: red;");
        correct = false;
    }
    else if (a == 2)
    {
        ui->ExistLabel->setText("Passwords do not match");
        ui->ExistLabel->setStyleSheet("color: red;");
        correct = false;
    }
    else if (a == 3)
    {
        ui->ExistLabel->setText("Fill all fields");
        ui->ExistLabel->setStyleSheet("color: red;");
        correct = false;
    }
}

void RegistrationForm::addUser()
{
    // 1 - email, 2 - password, 3 - first_name, 4 - last_name
    socket->write(QString("adduser|separator|%1|separator|%2|separator|%3|separator|%4")
                  .arg(email)
                  .arg(password)
                  .arg(first_name)
                  .arg(last_name)
                  .toStdString().c_str());
    socket->waitForBytesWritten(500);
}

void RegistrationForm::writeToFile()
{
    QFile toFile(".\\files\\data.json");
    //QFile toFile("C:\\Users\\march\\Desktop\\Client Part\\files\\data.json");
    toFile.open(QIODevice::WriteOnly);
    QTextStream outStream(&toFile);
    outStream << QString("{\"email\":\"%1\",\"password\":\"%2\",\"userStatus\":\"registered\"}").arg(email).arg(password);
    toFile.close();
}

void RegistrationForm::on_EmailLineEdit_textChanged(const QString &arg1)
{
    if (!correct)
    {
        ui->ExistLabel->setText("");
        correct = true;
    }
}

void RegistrationForm::on_PasswordLineEdit_textChanged(const QString &arg1)
{
    if (!correct)
    {
        ui->ExistLabel->setText("");
        correct = true;
    }
}

void RegistrationForm::on_ConfirmPasswordLineEdit_textChanged(const QString &arg1)
{
    if (!correct)
    {
        ui->ExistLabel->setText("");
        correct = true;
    }
}

void RegistrationForm::on_FirstNameLineEdit_textChanged(const QString &arg1)
{
    if (!correct)
    {
        ui->ExistLabel->setText("");
        correct = true;
    }
}

void RegistrationForm::on_LastNameLineEdit_textChanged(const QString &arg1)
{
    if (!correct)
    {
        ui->ExistLabel->setText("");
        correct = true;
    }
}
