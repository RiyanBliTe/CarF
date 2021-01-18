#ifndef ITEM_H
#define ITEM_H

#include <QWidget>

namespace Ui {
class Item;
}

class Item : public QWidget
{
    Q_OBJECT

public:
    Item(QWidget *parent = nullptr);
    ~Item();
    QString name;
    QString code;
    QString price;
    QByteArray imageArr;
    int row;
    int column;
    QString id;

    void setName(QString str);
    void setPrice(QString str);
    void setCode(QString str);
    void setImage(QByteArray arr);
    void setEnabledItem();

private slots:
    void on_pushButton_2_clicked();

signals:
    void clickedSent(QString str);
private:
    Ui::Item *ui; 
};

#endif // ITEM_H
