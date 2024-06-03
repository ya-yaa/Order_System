#ifndef DINEIN_ORDER_H
#define DINEIN_ORDER_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlQuery>

#include "dinein_ok.h"

namespace Ui {
class Dinein_order;
}

class Dinein_order : public QWidget
{
    Q_OBJECT

public:
    explicit Dinein_order(QWidget *parent = nullptr);
    ~Dinein_order();

    //接收订单编号与顾客编号
    void acceptid(QString o,QString c){
        order_id = o;
        custom_id = c;
    }

    QString order_id;  // 订单编号
    QString custom_id;  // 顾客编号
    QString table_id; // 桌号

    Dinein_ok *ok = NULL;

private:
    Ui::Dinein_order *ui;

    // 菜单
    QSqlQueryModel* menu = new QSqlQueryModel();
    // 折扣
    QSqlQueryModel* sale = new QSqlQueryModel();
    //当前订单视图
    QStandardItemModel order;  //当前订单视图的数据模型

    // 是否为第一次下单
    bool isfirst = true;


private slots:
    //回到主界面
    void on_backtomain_clicked();
    //确认订单界面
    void on_okorder_clicked();

    // 接收回到点餐界面信号
    void receive_ok();

    void on_nameselect_button_clicked();

    void on_typeselect_button_clicked();

    void on_dinein_menu_pressed(const QModelIndex &index);

    void on_order_now_pressed(const QModelIndex &index);

    void on_Insert_button_clicked();

    void on_Update_button_clicked();

    void on_Delete_button_clicked();

signals:
    // 用于返回主窗口的信号
    void Dinein_back();
};

#endif // DINEIN_ORDER_H
