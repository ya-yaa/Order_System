#include "widget.h"
#include "ui_widget.h"

#include "token_order.h"
#include <QSqlError>
QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    t = new Token_order;
    d = new Dinein_order;
    m = new Manage;

    connect(t,SIGNAL(Token_back()),this,SLOT(receive_Token()));
    connect(d,SIGNAL(Dinein_back()),this,SLOT(receive_Dinein()));

    // 连接数据库
    db.setDatabaseName("order_system");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("Shi20040428");

    //连接提示
    if(!db.open())  //连接失败
    {
        QMessageBox::information(this,"Database","Fail.Check please.");
    }

}

Widget::~Widget()
{
    delete ui;
}

// 去外带顾客点餐界面
void Widget::on_Token_clicked()
{

    // 创建订单编号与外带顾客编号
    QSqlQuery query;

    // 订单编号
    query.prepare(QString("SELECT COUNT(*) FROM orders;"));
    if (!query.exec()) {
        qDebug() << "查询失败: " << query.lastError();
        return;
    }
    if (query.next()) {
        QVariant sumResult = query.value(0);
        int temp = sumResult.toInt() + 1;

        order_id = QString::number(temp);
        qDebug() << " order_id: " << order_id;
    }



    // 顾客编号
    query.prepare(QString("SELECT COUNT(*) FROM customers;"));
    if (!query.exec()) {
        qDebug() << "查询失败: " << query.lastError();
        return;
    }
    if (query.next()) {
        QVariant sumResult = query.value(0);
        int temp = sumResult.toInt() + 1;

        custom_id = QString::number(temp);
         qDebug() << " custom_id: " << custom_id;
    }


    t ->acceptid(order_id,custom_id);
    this->hide();
    t->show();
}

// 去堂食顾客点餐界面
void Widget::on_Dinein_clicked()
{

    // 创建订单编号与堂食顾客编号
    QSqlQuery query;

    // 订单编号
    query.prepare(QString("SELECT COUNT(*) FROM orders;"));
    if (!query.exec()) {
        qDebug() << "查询失败: " << query.lastError();
        return;
    }
    if (query.next()) {
        QVariant sumResult = query.value(0);
        int temp = sumResult.toInt() + 1;

        order_id = QString::number(temp);
        qDebug() << " dinein order_id: " << order_id;
    }



    // 顾客编号
    query.prepare(QString("SELECT COUNT(*) FROM customers;"));
    if (!query.exec()) {
        qDebug() << "查询失败: " << query.lastError();
        return;
    }
    if (query.next()) {
        QVariant sumResult = query.value(0);
        int temp = sumResult.toInt() + 1;

        custom_id = QString::number(temp);
         qDebug() << "dinein custom_id: " << custom_id;
    }


    d->acceptid(order_id,custom_id);
    this->hide();
    d->show();
}

// 外带点餐返回主窗口
void Widget::receive_Token()
{
    this->t->hide();
    this->show();
}

// 堂食点餐返回主窗口
void Widget::receive_Dinein()
{
    this->d->hide();
    this->show();
}

// 餐厅管理界面
void Widget::on_pushButton_clicked()
{
    if (m == NULL){
        m = new Manage;
    }

    this->hide();
    m->show();
}
