#ifndef MANAGE_H
#define MANAGE_H

#include <QWidget>

#include "dish_manage.h"
#include "cashier_manage.h"

namespace Ui {
class Manage;
}

class Manage : public QWidget
{
    Q_OBJECT

public:
    explicit Manage(QWidget *parent = nullptr);
    ~Manage();

    dish_manage* m = NULL;
    Cashier_manage* c = NULL;

private slots:
    ////按钮
    void on_tomenu_clicked();
    void on_tocashier_clicked();

    ////接受返回主管理界面信号
    void receive_menu();
    void receive_cashier();

private:
    Ui::Manage *ui;
};

#endif // MANAGE_H
