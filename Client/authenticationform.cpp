#include "authenticationform.h"
#include "ui_authenticationform.h"

AuthenticationForm::AuthenticationForm(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthenticationForm)
{
    ui->setupUi(this);
    this->socket = socket;

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("CARF SignIn Form");

    setWindowIcon(QIcon(".\\resources\\icon.png"));

    QPixmap emblem(".\\resources\\emblem.png");
    int width = ui->EmblemLabel->width();
    int height = ui->EmblemLabel->height();
    ui->EmblemLabel->setPixmap(emblem.scaled(width, height, Qt::KeepAspectRatio));

    correct = true;
}

AuthenticationForm::~AuthenticationForm()
{
    delete ui;
}

void AuthenticationForm::on_LogInPushButton_clicked()
{
    if (ui->EmailLineEdit->text().isEmpty() || ui->PasswordLineEdit->text().isEmpty())
        setIncorrect();
    else
    {
        email = ui->EmailLineEdit->text();
        password = ui->PasswordLineEdit->text();
        socket->write(QString("checkemail|separator|%1|separator|%2")
                  .arg(ui->EmailLineEdit->text())
                  .arg(ui->PasswordLineEdit->text())
                  .toStdString().c_str());
        socket->waitForBytesWritten(500);
    }
}

void AuthenticationForm::on_RegistrationPushButton_clicked()
{
    this->hide();
    emit regWindow();
}

void AuthenticationForm::writeToFile()
{
    QFile toFile(".\\files\\data.json");
    //QFile toFile("C:\\Users\\march\\Desktop\\Client Part\\files\\data.json");
    toFile.open(QIODevice::WriteOnly);
    QTextStream outStream(&toFile);
    outStream << QString("{\"email\":\"%1\",\"password\":\"%2\",\"userStatus\":\"registered\"}").arg(email).arg(password);
    toFile.close();
}

void AuthenticationForm::setIncorrect()
{
    ui->StateLabel->setText("Incorrect login or password");
    ui->StateLabel->setStyleSheet("color: red;");
    correct = false;
}

void AuthenticationForm::on_EmailLineEdit_textChanged(const QString &arg1)
{
    if (!correct)
    {
        ui->StateLabel->setText("");
        correct = true;
    }
}

void AuthenticationForm::on_PasswordLineEdit_textChanged(const QString &arg1)
{
    if (!correct)
    {
        ui->StateLabel->setText("");
        correct = true;
    }
}
