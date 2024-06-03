#ifndef DINEIN_OK_H
#define DINEIN_OK_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>

#include "dinein_pay.h"

namespace Ui {
class Dinein_ok;
}

class Dinein_ok : public QWidget
{
    Q_OBJECT

public:
    explicit Dinein_ok(QWidget *parent = nullptr);
    ~Dinein_ok();

    void acceptid(QString o,QString c,QString t){
        order_id = o;
        custom_id = c;
        table_id = t;
    }

    QString order_id;  // 订单编号
    QString custom_id;  // 顾客编号
    QString table_id; //餐桌编号
    QString bill_id;  // 收银单编号
    QString cashier_id;  //收银员编号

    Dinein_pay* pay = NULL;

private slots:
    // 回到点餐界面
    void on_backtoorder_clicked();
    // 结账界面
    void on_pay_clicked();

    void on_lookButton_clicked();

private:
    Ui::Dinein_ok *ui;

    // 当前插入好的订单
    QSqlQueryModel* ordernow = new QSqlQueryModel();

signals:
    // 用于返回点餐界面的信号
    void Dineinorder_back();
};

#endif // DINEIN_OK_H
