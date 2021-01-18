#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    customWindow();
    isItems = false;
    total = 0;
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::sockReady);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::sockDisc);
    //socket->connectToHost("176.121.6.94", 5356);
    socket->connectToHost("localhost", 5356);
    user = new UserInfo;
    lay = new QGridLayout();
    lay2 = new QVBoxLayout();
    ui->CatalogWidget->setLayout(lay);
    ui->BakWidget->setLayout(lay2);
    bakCou = 0;
    autForm = new AuthenticationForm(socket);
    regForm = new RegistrationForm(socket);
    items = new Item[20];
    for (int i = 0; i < 20; i++)
    {
        connect(&items[i], &Item::clickedSent, this, &MainWindow::addBak);
        connect(&baks[i], &BakForm::deleteSignal, this, &MainWindow::delBak);
        lay2->addWidget(&baks[i]);
        baks[i].setVisible(false);
    }
    int maxCou = (screenWidth - 250) / 375;
    int rowsCou = 20 / maxCou;
    if (7 % maxCou != 0) rowsCou++;
    int count = 0;
        for (int i = 0; i < rowsCou; i++)
        {
            for (int j = 0; j < maxCou; j++)
            {
                if (count < 20){
                    lay->addWidget(&items[count], i, j);
                    items[count++].setVisible(false);
                }
                else
                    break;

            }
        }
    connect(autForm, &AuthenticationForm::prevWindow, this, &MainWindow::show);
    connect(autForm, &AuthenticationForm::regWindow, regForm, &RegistrationForm::show);
    connect(regForm, &RegistrationForm::autWindow, autForm, &AuthenticationForm::show);
    connect(regForm, &RegistrationForm::mainWindow, this, &MainWindow::show);
    this->hide();
    if (!socket->waitForConnected(3000))
    {
        QMessageBox::critical(this, "Connection info", "Cannot connect to server");
        this->close();
    }
    else
    {
        QFile statusFile(".\\files\\data.json");
        //QFile statusFile("C:\\Users\\march\\Desktop\\Client Part\\files\\data.json");
        statusFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val = statusFile.readAll();
        statusFile.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject json = doc.object();
        QString str = json["userStatus"].toString();
        if (str == "registered")
        {
            user->setEmail(json["email"].toString());
            user->setPassword(json["password"].toString());

            socket->write(QString("getuserdata|separator|%1").arg(json["email"].toString()).toStdString().c_str());
            socket->waitForBytesWritten(500);
            this->show();
        }
        else
        {
            autForm->show();
        }
    }
}

void MainWindow::delBak(QString str)
{
    for (int i = 0; i < 20; i++)
    {
        if (baks[i].id == str)
        {
            baks[i].setVisible(false);
            //lay2->removeWidget(&baks[i]);
            total -= baks[i].price.toInt();
            ui->TotalLabel->setText("Total: "+QString::number(total)+"$");
            bakCou--;
            break;
        }
    }
    for (int i = 0; i < 20; i++)
    {
        if (items[i].id == str)
        {
            items[i].setEnabledItem();
            break;
        }
    }
}

void MainWindow::addBak(QString str)
{
    QStringList listt = str.split("|separator|");
    // 0 - id, 1 - name, 2 - code, 3 - price
    baks[bakCou].id = listt.at(0);
    baks[bakCou].setName(listt.at(1));
    baks[bakCou].setCode(listt.at(2));
    baks[bakCou].setPrice(listt.at(3));
    //QFile file("C:\\Users\\march\\Pictures\\New folder\\"+listt.at(0)+".jpg");
    QFile file(".\\resources\\images\\"+listt.at(0)+".jpg");
    file.open(QIODevice::ReadOnly);
    QByteArray inByteArray = file.readAll();
    baks[bakCou].setImage(inByteArray);
    if (!baks[bakCou].isVisible())baks[bakCou].setVisible(true);
    //lay2->addWidget(&baks[bakCou]);
    bakCou++;
}

void MainWindow::customWindow()
{
    screenWidth = QApplication::desktop()->screenGeometry().width();
    screenHeight = QApplication::desktop()->screenGeometry().height();

    minimumWidth = screenWidth * 0.8;
    minimumHeight = screenHeight * 0.8;

    basketCount = 0;

    setMinimumWidth(minimumWidth);
    setMinimumHeight(minimumHeight);

    showMaximized();

    setWindowTitle("CarF Shop");
    setWindowIcon(QIcon(".\\resources\\icon.png"));

    updateBasketCount();

    QRect rect1(0,0,75,75);
    QRegion region1(rect1, QRegion::Ellipse);
    ui->ProfileIconToolButton->setMask(region1);
    ui->ProfileIconToolButton->setIcon(QIcon(".\\resources\\user64.png"));
    QSize profS = ui->ProfileIconToolButton->size();
    ui->ProfileIconToolButton->setIconSize(profS);

    ui->BasketWidget->setVisible(false);
    ui->CatalogWidget->setVisible(false);
    QFont font("Lucida Sans Typewriter", 12);
    font.setBold(true);
    ui->ProfilePushButton->setFont(font);
    ui->ProfileWidget->setVisible(true);

    QFile countriesFile(".\\files\\countries.txt");
    countriesFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString countries = countriesFile.readAll();
    QStringList countrylist = countries.split(";");
    countrylist.prepend("- Not selected -");
    ui->comboBox->addItems(countrylist);
    ui->comboBox_2->addItems(countrylist);

    ui->AvatarToolButton->setStyleSheet("background-color: white; border-radius: 175px;");
    QRect rect(0,0,350,350);
    QRegion region(rect, QRegion::Ellipse);
    ui->AvatarToolButton->setMask(region);
    ui->AvatarToolButton->setIcon(QIcon(".\\resources\\user256.png"));
    QSize size = ui->AvatarToolButton->size();
    ui->AvatarToolButton->setIconSize(size);

    //ui->AvatarToolButton->setStyleSheet("background-color: white;");
    ui->ProfileIconToolButton->setStyleSheet("background-color: white; border-radius: 75px;");
}

MainWindow::~MainWindow()
{
    if (socket != nullptr) delete socket;
    delete ui;
}

void MainWindow::updateBasketCount()
{
    ui->InTheBasketLabel->setText("In the basket: " + QString::number(basketCount));
}

void MainWindow::on_ProfilePushButton_clicked()
{
    QFont normalFont("Lucida Sans Typewriter", 10);

    ui->BasketWidget->setVisible(false);
    ui->BasketPushButton->setFont(normalFont);

    QFont font("Lucida Sans Typewriter", 12);
    font.setBold(true);
    ui->ProfilePushButton->setFont(font);
    ui->ProfileWidget->setVisible(true);

    ui->CatalogWidget->setVisible(false);
    ui->CatalogPushButton->setFont(normalFont);

    ui->SignOutPushButton->setFont(normalFont);

    QPixmap pixi = QPixmap();
    pixi.loadFromData(user->getImageFile());
    QIcon newImage(pixi);
    ui->AvatarToolButton->setIcon(newImage);
    ui->FirstNameLineEdit->setText(user->getFirst_name());
    ui->LastNameLineEdit->setText(user->getLast_Name());
    ui->comboBox->setCurrentText(user->getCountry());
    ui->CityLineEdit->setText(user->getCity());
    ui->AddressLineEdit->setText(user->getAddress());
    ui->EmailLineEdit->setText(user->getEmail());
}

void MainWindow::on_CatalogPushButton_clicked()
{
    QFont normalFont("Lucida Sans Typewriter", 10);

    ui->BasketWidget->setVisible(false);
    ui->BasketPushButton->setFont(normalFont);

    ui->ProfileWidget->setVisible(false);
    ui->ProfilePushButton->setFont(normalFont);

    ui->CatalogWidget->setVisible(true);
    QFont font("Lucida Sans Typewriter", 12);
    font.setBold(true);
    ui->CatalogPushButton->setFont(font);

    ui->SignOutPushButton->setFont(normalFont);
    if (!isItems)
    {
        socket->write(QString("getchairs").toStdString().c_str());
        isItems = true;
    }
}

void MainWindow::on_BasketPushButton_clicked()
{
    QFont normalFont("Lucida Sans Typewriter", 10);

    QFont font("Lucida Sans Typewriter", 12);
    font.setBold(true);
    ui->BasketWidget->setVisible(true);
    ui->BasketPushButton->setFont(font);

    ui->ProfileWidget->setVisible(false);
    ui->ProfilePushButton->setFont(normalFont);

    ui->CatalogWidget->setVisible(false);
    ui->CatalogPushButton->setFont(normalFont);

    ui->SignOutPushButton->setFont(normalFont);

    ui->FirstNameBakLineEdit->setText(user->getFirst_name());
    ui->LastNameBakLineEdit->setText(user->getLast_Name());
    ui->EmailBakLineEdit->setText(user->getEmail());
    ui->comboBox_2->setCurrentText(user->getCountry());
    ui->CityBakLineEdit->setText(user->getCity());
    ui->AddressBakLineEdit->setText(user->getAddress());

    int totalI = 0;
    for (int i = 0; i < 20; i++)
    {
        if (baks[i].isVisible())
        {
            totalI += baks[i].price.toInt();
        }
    }
    total = totalI;
    ui->TotalLabel->setText("Total: "+QString::number(totalI)+"$");
}

void MainWindow::on_SignOutPushButton_clicked()
{
    //QFile toFile("C:\\Users\\march\\Desktop\\Client Part\\files\\data.json");
    QFile toFile(".\\files\\data.json");
    toFile.open(QIODevice::WriteOnly);
    QTextStream outStream(&toFile);
    outStream << QString("{\"email\":\"\",\"password\":\"\",\"userStatus\":\"\"}");
    toFile.close();
    this->close();
}

void MainWindow::on_SavePushButton_clicked()
{
    //user->setEmail(ui->EmailLineEdit->text());
    if ((ui->NewPasswordLineEdit->text() == ui->VeifyLineEdit->text()) &&
            !ui->NewPasswordLineEdit->text().isEmpty() && !ui->VeifyLineEdit->text().isEmpty())
    {
        user->setPassword(ui->NewPasswordLineEdit->text());
        user->setFirstName(ui->FirstNameLineEdit->text());
        user->setLastName(ui->LastNameLineEdit->text());
        user->setCountry(ui->comboBox->currentText());
        user->setCity(ui->CityLineEdit->text());
        user->setAddress(ui->AddressLineEdit->text());

        QPixmap map;
        map.loadFromData(user->getImageFile());
        ui->ProfileIconToolButton->setIcon(QIcon(map));

        QJsonObject recordObject;
        recordObject.insert("userStatus", QJsonValue::fromVariant("registered"));
        recordObject.insert("email", QJsonValue::fromVariant(user->getEmail()));
        recordObject.insert("password", QJsonValue::fromVariant(user->getPassword()));

        QJsonDocument doc(recordObject);
        QString jsonString = doc.toJson(QJsonDocument::Indented);
        QFile file;
        file.setFileName(".\\files\\data.json");
        file.open(QFile::WriteOnly | QFile::Text);
        file.write(QJsonDocument(doc).toJson());
        file.close();
        QByteArray imarr = user->getImageFile();
        QString img = imarr.toBase64();

        socket->write(QString("userupdate|separator|%1|separator|%2|separator|%3|separator|%4|separator|%5|separator|%6|separator|%7|separator|%8|separator|%9")
                      .arg(user->getEmail())
                      .arg(ui->EmailLineEdit->text())
                      .arg(user->getPassword())
                      .arg(user->getFirst_name())
                      .arg(user->getLast_Name())
                      .arg(user->getCountry())
                      .arg(user->getCity())
                      .arg(user->getAddress())
                      .arg(img)
                      .toStdString().c_str());
        socket->waitForBytesWritten(500);
    }
}

void MainWindow::on_AvatarToolButton_clicked()
{
    QString filter = "All File (*.*) ;; PNG File (*.png) ;; JPG File (*.jpg) ;; JPEG File (*.jpeg)";
    QString fileName = QFileDialog::getOpenFileName(this, "Select a picture", "C://", filter);
    if (fileName.length() != 0)
    {
        ui->AvatarToolButton->setIcon(QIcon(fileName));
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray inByteArray = file.readAll();
        user->setImageFile(inByteArray);
    }
}

void MainWindow::sockReady()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        Data = socket->readAll();

        QString mess = Data;
        QStringList list = mess.split("|separator|");
        if (list.at(0) == "userdata")
        {
            // 1 - first_name, 2 - last_name, 3 - country, 4 - city, 5 - address, 6 - image
            user->setFirstName(list.at(1));
            user->setLastName(list.at(2));
            user->setCountry(list.at(3));
            user->setCity(list.at(4));
            user->setAddress(list.at(5));

            QByteArray imageArr = QByteArray::fromBase64(list.at(6).toLatin1());
            user->setImageFile(imageArr);
            QPixmap pixi = QPixmap();
            pixi.loadFromData(imageArr);
            QIcon newImage(pixi);
            ui->AvatarToolButton->setIcon(newImage);
            ui->FirstNameLineEdit->setText(user->getFirst_name());
            ui->LastNameLineEdit->setText(user->getLast_Name());
            ui->comboBox->setCurrentText(user->getCountry());
            ui->CityLineEdit->setText(user->getCity());
            ui->AddressLineEdit->setText(user->getAddress());
            ui->EmailLineEdit->setText(user->getEmail());
            ui->ProfileIconToolButton->setIcon(newImage);
            ui->NameLabel->setText(QString(user->getFirst_name()).append(" ").append(user->getLast_Name()));
        }
        else if (list.at(0) == "checkemail")
        {
            if (list.at(1) == "true")
            {
                autForm->writeToFile();
                autForm->close();
                this->show();
                //QFile statusFile("C:\\Users\\march\\Desktop\\Client Part\\files\\data.json");
                QFile statusFile(".\\files\\data.json");
                statusFile.open(QIODevice::ReadOnly | QIODevice::Text);
                QString val = statusFile.readAll();
                statusFile.close();
                QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
                QJsonObject json = doc.object();
                QString str = json["userStatus"].toString();
                if (str == "registered")
                {
                    user->setEmail(json["email"].toString());
                    user->setPassword(json["password"].toString());

                    socket->write(QString("getuserdata|separator|%1").arg(json["email"].toString()).toStdString().c_str());
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                autForm->setIncorrect();
            }
        }
        else if (list.at(0) == "emailexist")
        {
            if (list.at(1) == "true")
            {
                regForm->setIncorrect(0);
            }
            else
            {
                regForm->addUser();
            }
        }
        else if (list.at(0) == "registeruser")
        {
            if (list.at(1) == "true")
            {
                regForm->writeToFile();
                regForm->close();
                this->show();
                //QFile statusFile("C:\\Users\\march\\Desktop\\Client Part\\files\\data.json");
                QFile statusFile(".\\files\\data.json");
                statusFile.open(QIODevice::ReadOnly | QIODevice::Text);
                QString val = statusFile.readAll();
                statusFile.close();
                QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
                QJsonObject json = doc.object();
                QString str = json["userStatus"].toString();
                if (str == "registered")
                {
                    user->setEmail(json["email"].toString());
                    user->setPassword(json["password"].toString());

                    socket->write(QString("getuserdata|separator|%1").arg(json["email"].toString()).toStdString().c_str());
                    socket->waitForBytesWritten(500);
                }
            }
            else
            {
                regForm->setIncorrect(1);
            }
        }
        else if (list.at(0) == "chairitems")
        {
            int itemCou = 0;
            for(int i = 1; i < list.size() - 1; i++)
            {
                QStringList list2 = list.at(i).split("|2separator2|");
                items[itemCou].id = list2.at(1);
                items[itemCou].setName(list2.at(2));
                items[itemCou].setPrice(list2.at(3));
                items[itemCou].setCode(list2.at(4));
                //QFile file("C:\\Users\\march\\Pictures\\New folder\\"+items[itemCou].id+".jpg");
                QFile file(".\\resources\\images\\"+items[itemCou].id+".jpg");
                file.open(QIODevice::ReadOnly);
                QByteArray inByteArray = file.readAll();
                items[itemCou].setImage(inByteArray);
                itemCou++;
            }
            for (int i = 0; i < itemCou; i++)
            {
                items[i].setVisible(true);
            }

        }
    }
}

void MainWindow::sockDisc()
{
    socket->deleteLater();
}

void MainWindow::on_OrderPushButton_clicked()
{
    if (ui->FirstNameBakLineEdit->text().isEmpty() || ui->LastNameBakLineEdit->text().isEmpty() ||
            ui->EmailBakLineEdit->text().isEmpty() || ui->CityBakLineEdit->text().isEmpty() ||
            ui->AddressBakLineEdit->text().isEmpty() || ui->CardNumberLineEdit->text().isEmpty() ||
            ui->ValidityLineEdit->text().isEmpty() || ui->CVC2LineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Result", "Fill in all fields!");
    }
    else
    {
        QString messAr = "chairsupdate|separator|";
        for (int i = 0; i < 20; i++)
        {
            if (baks[i].isVisible())
            {
                messAr.append(baks[i].id).append("|separator|");
                for (int j = 0; j < 20; j++)
                {
                    if (baks[j].id == baks[i].id)
                    {
                        baks[j].setVisible(false);
                        //lay2->removeWidget(&baks[i]);
                        bakCou--;
                        break;
                    }
                }
            }
        }
        total = 0;
        ui->TotalLabel->setText("Total: 0$");
        QMessageBox msgBox;
        msgBox.setText("The order has been submitted for processing.");
        msgBox.exec();
        socket->write(messAr.toStdString().c_str());
        socket->waitForBytesWritten(500);
    }
}
