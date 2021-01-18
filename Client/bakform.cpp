#include "bakform.h"
#include "ui_bakform.h"

BakForm::BakForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BakForm)
{
    ui->setupUi(this);
    name = "";
    code = "";
    price = "";
    id = "";
}

BakForm::~BakForm()
{
    delete ui;
}

void BakForm::setImage(QByteArray arr)
{
    imageArr = arr;
    QPixmap pixi = QPixmap();
    int w = ui->ImageLabel->width();
    int h = ui->ImageLabel->height();
    pixi.loadFromData(arr);
    ui->ImageLabel->setPixmap(pixi.scaled(w,h,Qt::KeepAspectRatio));
}

void BakForm::setName(QString str)
{
    name = str;
    ui->NameLabel->setText(name);
}

void BakForm::setCode(QString str)
{
    code = str;
    ui->CodeLabel->setText("Code: "+code);
}
void BakForm::setPrice(QString str)
{
    price = str;
    ui->PriceLabel->setText("Price: "+price);
}

void BakForm::on_DeletePushButton_clicked()
{
    emit deleteSignal(id);
}
