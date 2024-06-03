#ifndef TOKEN_PAY_H
#define TOKEN_PAY_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class Token_pay;
}

class Token_pay : public QWidget
{
    Q_OBJECT

public:
    explicit Token_pay(QWidget *parent = nullptr);
    ~Token_pay();

    // 接收订单编号、顾客编号、收银单编号
    void acceptid(QString o,QString c,QString b){
        order_id = o;
        custom_id = c;
        bill_id = b;
    }

    Token_pay* pay = NULL;

    QString order_id;  // 订单编号
    QString custom_id;  // 顾客编号
    QString bill_id;  // 收银单编号

private slots:
    void on_pushButton_clicked();

    void on_lookButton_clicked();

private:
    Ui::Token_pay *ui;

    // 点菜单
    QSqlQueryModel* order = new QSqlQueryModel();
    // 结账单
    QSqlQueryModel* bill = new QSqlQueryModel();
};

#endif // TOKEN_PAY_H
