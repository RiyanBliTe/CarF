#ifndef BAKFORM_H
#define BAKFORM_H

#include <QWidget>

namespace Ui {
class BakForm;
}

class BakForm : public QWidget
{
    Q_OBJECT

public:
    explicit BakForm(QWidget *parent = nullptr);
    ~BakForm();
    QString name;
    QString code;
    QString price;
    QString id;
    QByteArray imageArr;

    void setImage(QByteArray arr);
    void setName(QString str);
    void setCode(QString str);
    void setPrice(QString str);

private slots:
    void on_DeletePushButton_clicked();

signals:
    void deleteSignal(QString);

private:
    Ui::BakForm *ui;

};

#endif // BAKFORM_H
