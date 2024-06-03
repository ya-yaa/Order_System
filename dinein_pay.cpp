#include "dinein_pay.h"
#include "ui_dinein_pay.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

Dinein_pay::Dinein_pay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dinein_pay)
{
    ui->setupUi(this);
}

Dinein_pay::~Dinein_pay()
{
    delete ui;
}

void Dinein_pay::on_pushButton_clicked()
{
    // 恢复餐桌状态
    QSqlQuery query;
    QString updateQuery = QString("UPDATE tables SET 餐桌状态 = '空闲' WHERE 餐桌编号 = %1;")
                          .arg(table_id);
    query.exec(updateQuery);

    updateQuery = QString("UPDATE tables SET 收银单编号 = NULL  WHERE 餐桌编号 = %1;")
                  .arg(table_id);
    query.exec(updateQuery);

    this->close();
}

// 查看
void Dinein_pay::on_lookButton_clicked()
{
    // 点菜单
    QString select = QString(
        "SELECT 菜品编号, 菜品名称, 菜品分类, 折后价格, 份数, 口味 "
        "FROM OrderDetails "
        "WHERE 订单编号 = %1;"
    ).arg(order_id);

    // QMessageBox::information(this, "执行语句", select);

    order->setQuery(select);
    ui->ordertable->setModel(order);


    qDebug() << " bill_id: " << bill_id;
    // 结账单
    select = "select * from bills where 收银单编号 = " + bill_id + ";";
    //QMessageBox::information(this, "执行语句", select);
    bill->setQuery(select);
    ui->billtable->setModel(bill);

}
