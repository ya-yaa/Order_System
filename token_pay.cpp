#include "token_pay.h"
#include "ui_token_pay.h"

#include <QDebug>
#include <QMessageBox>

Token_pay::Token_pay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Token_pay)
{
    ui->setupUi(this);
}

Token_pay::~Token_pay()
{
    delete ui;
}

// 退出
void Token_pay::on_pushButton_clicked()
{
    this->close();
}

// 查看
void Token_pay::on_lookButton_clicked()
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
