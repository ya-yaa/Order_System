#include "cashier_manage.h"
#include "ui_cashier_manage.h"
#include "widget.h"

Cashier_manage::Cashier_manage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cashier_manage)
{
    ui->setupUi(this);

    //初始，展示所有信息
    c = new QSqlQueryModel(this);


}

Cashier_manage::~Cashier_manage()
{
    delete ui;
}

// 发送返回主管理界面信号
void Cashier_manage::on_back_clicked()
{
    emit this->cashier_back();
}

// 退出
void Cashier_manage::on_exit_clicked()
{
    this->close();
}

// 增
void Cashier_manage::on_Insert_button_clicked()
{
    // 读取查询条件
    QString id = ui->cashier_id->text();
    QString name = ui->cashier_name->text();
    QString phone = ui->cashier_phone->text();

    // 创建插入语句
    QSqlQuery query;
    QString str = id +",'" + name + "','" + phone +"'";
    QString insert = "insert into cashiers(收银员编号, 收银员姓名, 联系电话) values(" + str + ");";

    // 执行
    query.exec(insert);

    // 更新视图
    c->setQuery("select * from cashiers;");
    ui->cashiertable->setModel(c);

}

// 删
void Cashier_manage::on_Delete_button_clicked()
{
    // 读取选中行
    int currentrow = ui->cashiertable->currentIndex().row();

    if(currentrow != -1)//若选中一行
    {
        int ok  =  QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除当前行吗？"), QMessageBox::Yes,QMessageBox::No);
        //如确认删除
        if(ok == QMessageBox::Yes)
        {
            // 获取菜品编号
            QString id = c->data(c->index(currentrow,0)).toString();
            // 创建删除语句
            QSqlQuery query;
            QString del = "delete from cashiers where 收银员编号=" + id + ";";

            // 执行删除命令
            query.exec(del);

            // 更新视图
            c->setQuery("select * from cashiers");
            ui->cashiertable->setModel(c);

            QMessageBox::information(this, "删除成功", "所选信息删除成功");
            }
        }
        else
            QMessageBox::information(this, "提示", "请选择你要删除的信息行");
}

// 改
void Cashier_manage::on_Update_button_clicked()
{
    // 创建修改语句
    QSqlQuery query;
    QString update = "update cashiers set 收银员姓名='" + ui->cashier_name->text()
                   + "',联系电话='" + ui->cashier_phone->text()
                   + "' where 收银员编号=" + ui->cashier_id->text() + ";";

    // 执行修改语句
    query.exec(update);
    QMessageBox::information(this, "修改成功", "表信息修改成功");

    // 更新视图
    c->setQuery("select * from cashiers;");
    ui->cashiertable->setModel(c);
}

//查
void Cashier_manage::on_Select_button_clicked()
{
    QSqlQuery query;
    bool front = 0;  //前面是否有其它条件，无0有1

    QString select = "select * from cashiers where ";

    // 编号
    if( ui->cashier_id->text() != "")
    {
        select = select + "(收银员编号=" + ui->cashier_id->text()+ ")";
        front = 1;
    }

    // 电话，模糊
    if( ui->cashier_phone->text() != "")
    {
        if(front)
        {
            select += " and ";
        }

        select = select + "(联系电话 like '%" + ui->cashier_phone->text() +"%')";
        front = 1;

    }

    // 名称，模糊
    if( ui->cashier_name->text() != "")
    {
        if(front)
        {
            select += " and ";
        }

        select = select + "(收银员姓名 like '%" + ui->cashier_name->text() +"%')";
        front = 1;

    }

    // 没有条件，查所有
    if(front==0){
        select = "select * from cashiers";
    }

    select += ';';
    //QMessageBox::information(this, "执行语句", select);

    // 执行条件搜索，更新视图
    c->setQuery(select);
    ui->cashiertable->setModel(c);
}

void Cashier_manage::on_cashiertable_pressed(const QModelIndex &index)
{
    QString id = c->data(c->index(index.row(), 0)).toString();
    QString name = c->data(c->index(index.row(), 1)).toString();
    QString phone = c->data(c->index(index.row(), 2)).toString();

    ui->cashier_id->setText(id);
    ui->cashier_name->setText(name);
    ui->cashier_phone->setText(phone);

}
