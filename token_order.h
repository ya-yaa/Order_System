#ifndef TOKEN_ORDER_H
#define TOKEN_ORDER_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QTableView>

#include "token_ok.h"

namespace Ui {
class Token_order;
}

class Token_order : public QWidget
{
    Q_OBJECT

public:
    explicit Token_order(QWidget *parent = nullptr);

    ~Token_order();

    //接收订单编号与顾客编号
    void acceptid(QString o,QString c){
        order_id = o;
        custom_id = c;
    }

    QString order_id;  // 订单编号
    QString custom_id;  // 顾客编号

    // 确认订单界面
    Token_ok *ok = NULL;

private slots:
    //回到主窗口
    void on_backtomain_clicked();
    //确认订单界面
    void on_okorder_clicked();

    void on_Insert_button_clicked();

    void on_Delete_button_clicked();

    void on_nameselect_button_clicked();

    void on_typeselect_button_clicked();

    void on_token_menu_pressed(const QModelIndex &index);

    void on_order_now_pressed(const QModelIndex &index);

    void on_Update_button_clicked();

private:
    Ui::Token_order *ui;

    QSqlQueryModel* menu = new QSqlQueryModel();
    QSqlQueryModel* sale = new QSqlQueryModel();
    //当前订单视图
    QStandardItemModel order;  //当前订单视图的数据模型



signals:
    // 用于返回主窗口的信号
    void Token_back();
};

#endif // TOKEN_ORDER_H
