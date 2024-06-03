#ifndef DINEIN_PAY_H
#define DINEIN_PAY_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class Dinein_pay;
}

class Dinein_pay : public QWidget
{
    Q_OBJECT

public:
    explicit Dinein_pay(QWidget *parent = nullptr);
    ~Dinein_pay();

    // 接收订单编号、顾客编号、收银单编号
    void acceptid(QString o,QString c,QString b,QString t){
        order_id = o;
        custom_id = c;
        bill_id = b;
        table_id = t;
    }

    QString order_id;  // 订单编号
    QString custom_id;  // 顾客编号
    QString bill_id;  // 收银单编号
    QString table_id; //餐桌编号

private slots:
    void on_pushButton_clicked();

    void on_lookButton_clicked();

private:
    Ui::Dinein_pay *ui;

    // 点菜单
    QSqlQueryModel* order = new QSqlQueryModel();
    // 结账单
    QSqlQueryModel* bill = new QSqlQueryModel();
};

#endif // DINEIN_PAY_H
