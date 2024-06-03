#include "manage.h"
#include "ui_manage.h"

Manage::Manage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manage)
{
    ui->setupUi(this);

    m = new dish_manage;
    c = new Cashier_manage;

    connect(m,SIGNAL(menu_back()),this,SLOT(receive_menu()));
    connect(c,SIGNAL(cashier_back()),this,SLOT(receive_cashier()));
}

Manage::~Manage()
{
    delete ui;
}

// 菜单管理界面
void Manage::on_tomenu_clicked()
{
    if (m == NULL)
    {
        m = new dish_manage;
    }

    this->hide();
    this->m->show();
}

// 接收菜单管理返回主管理信号
void Manage::receive_menu()
{
    this->m->hide();
    this->show();
}

// 收银员管理界面
void Manage::on_tocashier_clicked()
{
    if (c == NULL)
    {
        c = new Cashier_manage;
    }
    this->hide();
    this->c->show();
}

// 接收收银员管理返回主管理信号
void Manage::receive_cashier()
{
    this->c->hide();
    this->show();
}
