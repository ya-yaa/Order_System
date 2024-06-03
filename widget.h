#ifndef WIDGET_H
#define WIDGET_H

#include "token_order.h"
#include "dinein_order.h"
#include "manage.h"

#include <QWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

// 数据库
extern QSqlDatabase db;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    Token_order *t = NULL;  //外带顾客界面
    Dinein_order *d = NULL;  //堂食顾客界面
    Manage *m = NULL;  //餐厅管理界面

    QString order_id;  // 订单编号
    QString custom_id;  // 顾客编号

private slots:
    ////按钮
    // 外带点餐界面
    void on_Token_clicked();
    // 堂食点餐界面
    void on_Dinein_clicked();
    // 餐厅管理界面
    void on_pushButton_clicked();

    ////接收信号
    // 外带点餐返回
    void receive_Token();
    // 堂食点餐返回
    void receive_Dinein();



private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
