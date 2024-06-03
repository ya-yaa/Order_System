#ifndef TOKEN_OK_H
#define TOKEN_OK_H

#include <QWidget>
#include <QSqlQueryModel>

#include "token_pay.h"

namespace Ui {
class Token_ok;
}

class Token_ok : public QWidget
{
    Q_OBJECT

public:
    explicit Token_ok(QWidget *parent = nullptr);
    ~Token_ok();

    void acceptid(QString o,QString c){
        order_id = o;
        custom_id = c;
    }

    QString order_id;  // 订单编号
    QString custom_id;  // 顾客编号
    QString bill_id;  // 收银单编号
    QString cashier_id;  //收银员编号

    Token_pay* pay = NULL;

private slots:
    // 结账界面
    void on_pay_clicked();

    void on_lookButton_clicked();

private:
    Ui::Token_ok *ui;

    // 当前插入好的订单
    QSqlQueryModel* ordernow = new QSqlQueryModel();

};

#endif // TOKEN_OK_H
