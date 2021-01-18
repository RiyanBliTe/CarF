#include "item.h"
#include "ui_item.h"
#include <QGraphicsDropShadowEffect>

Item::Item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Item)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(this);
    dropShadowEffect->setBlurRadius(25);
    this->setGraphicsEffect(dropShadowEffect);

    name = "";
    code = "";
    price = "";
    QByteArray imageArr;
    row = 0;
    column = 0;
    id = "";
}

Item::~Item()
{
    delete ui;
}

void Item::setName(QString str)
{
    name = str;
    ui->label_2->setText(str);
}

void Item::setPrice(QString str)
{
    price = str;
    ui->label_3->setText("Price: "+str+"$");
    int pr = str.toInt();
    pr /= 12;
    ui->label_5->setText("Credit: "+QString::number(pr).append("$ / 1 month"));
}

void Item::setCode(QString str)
{
    code = str;
    ui->label_4->setText("Code: "+str);
}

void Item::setImage(QByteArray arr)
{
    imageArr = arr;
    QPixmap pixi = QPixmap();
    int w = ui->label->width();
    int h = ui->label->height();
    pixi.loadFromData(arr);
    //pixi.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(pixi.scaled(w,h,Qt::KeepAspectRatio));
}

void Item::on_pushButton_2_clicked()
{
    QString str = id+"|separator|"+name+"|separator|"+code+"|separator|"+price;
    ui->pushButton_2->setEnabled(false);
    ui->widget->setStyleSheet("QWidget{background-color: #CDCDCD;border-radius: 5px;border: medium;}QPushButton{border-radius: 6px;background-color: #28A745;}QPushButton:hover{background-color: #218838;color: #F2F3F7;}QPushButton:pressed{background-color: #1E7E34;border-style: solid;border-width: medium;border-color: #F2F3F7;}");
    emit clickedSent(str);
}

void Item::setEnabledItem()
{
    ui->pushButton_2->setEnabled(true);
    ui->widget->setStyleSheet("QWidget{background-color: white;border-radius: 5px;border: medium;}QPushButton{border-radius: 6px;background-color: #28A745;}QPushButton:hover{background-color: #218838;color: #F2F3F7;}QPushButton:pressed{background-color: #1E7E34;border-style: solid;border-width: medium;border-color: #F2F3F7;}");
}
