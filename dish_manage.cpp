#include "dish_manage.h"
#include "ui_dish_manage.h"
#include "widget.h"

dish_manage::dish_manage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dish_manage)
{
    ui->setupUi(this);

    // 初始，展示所有信息
    order->setQuery("select * from dishes;");
    ui->dishtable->setModel(order);

}

dish_manage::~dish_manage()
{
    delete ui;

}

void dish_manage::on_back_clicked()
{
    emit this->menu_back();
}

void dish_manage::on_exit_clicked()
{
    this->close();
}

// 增
void dish_manage::on_Insert_button_clicked()
{
    // 读取查询条件
    QString id = ui->dish_id->text();
    QString name = ui->dish_name->text();
    QString type = ui->dish_type->text();
    QString price = ui->dish_price->text();
    QString sale = ui->dish_sale->text();

    // 创建插入语句
    QSqlQuery query;
    QString str1 = "'" + id +"','" + name + "','" + type +"','"+ price +"','"+ sale +"'";
    QString str = "insert into dishes(`菜品编号`, `菜品名称`, `菜品分类`, `菜品价格`, `优惠信息`) values(" + str1 + ");";

    // 执行
    query.exec(str);

    // 更新视图
    order->setQuery("select * from dishes;");
    ui->dishtable->setModel(order);

}

// 删
void dish_manage::on_Delete_button_clicked()
{
    // 读取选中行
    int currentrow = ui->dishtable->currentIndex().row();

    if(currentrow != -1)//若选中一行
    {
        int ok  =  QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除当前行吗？"), QMessageBox::Yes,QMessageBox::No);
        //如确认删除
        if(ok == QMessageBox::Yes)
        {
            // 获取菜品编号
            QString id = order->data(order->index(currentrow,0)).toString();
            // 创建删除语句
            QSqlQuery query;
            QString del = "delete from dishes where 菜品编号=" + id + ";";

            // 执行删除命令
            query.exec(del);

            // 更新视图
            order->setQuery("select * from dishes");
            ui->dishtable->setModel(order);

            QMessageBox::information(this, "删除成功", "所选信息删除成功");
            }
        }
        else
            QMessageBox::information(this, "提示", "请选择你要删除的信息行");


}

//改
void dish_manage::on_Update_button_clicked()
{
    // 创建修改语句
    QSqlQuery query;
    QString update = "update dishes set 菜品名称='" + ui->dish_name->text()
                   + "',菜品分类='" + ui->dish_type->text()
                   + "',菜品价格=" + ui->dish_price->text()
                   + ",优惠信息=" + ui->dish_sale->text()
                   + " where 菜品编号=" + ui->dish_id->text() + ";";

    // 执行修改语句
    query.exec(update);
    QMessageBox::information(this, "修改成功", "表信息修改成功");

    // 更新视图
    order->setQuery("select * from dishes;");
    ui->dishtable->setModel(order);

}

// 查
void dish_manage::on_Select_button_clicked()
{
    QSqlQuery query;
    bool front = 0;  //前面是否有其它条件，无0有1

    QString select = "select * from dishes where ";

    // 编号
    if( ui->dish_id->text() != "")
    {
        select = select + "(菜品编号=" + ui->dish_id->text()+ ")";
        front = 1;
    }

    // 分类，模糊
    if( ui->dish_type->text() != "")
    {
        if(front)
        {
            select += " and ";
        }

        select = select + "(菜品分类 like '%" + ui->dish_type->text() +"%')";
        front = 1;

    }

    // 名称，模糊
    if( ui->dish_name->text() != "")
    {
        if(front)
        {
            select += " and ";
        }

        select = select + "(菜品名称 like '%" + ui->dish_name->text() +"%')";
        front = 1;

    }

    // 价格
    if( ui->dish_price->text() != "")
    {
        if(front)
        {
            select += " and ";
        }

        select = select + "(菜品价格=" + ui->dish_price->text() + ")";
        front = 1;

    }

    // 优惠
    if( ui->dish_sale->text() != "")
    {
        if(front)
        {
            select += " and ";
        }

        select = select + "(CAST(优惠信息 AS CHAR) = '" + ui->dish_sale->text()+ "')";
        front = 1;

    }

    // 没有条件，查所有
    if(front==0){
        select = "select * from dishes";
    }

    select += ';';
    //QMessageBox::information(this, "执行语句", select);

    // 执行条件搜索，更新视图
    order->setQuery(select);
    ui->dishtable->setModel(order);


}

void dish_manage::on_dishtable_pressed(const QModelIndex &index)
{
    QString id = order->data(order->index(index.row(), 0)).toString();
    QString name = order->data(order->index(index.row(), 1)).toString();
    QString type = order->data(order->index(index.row(), 2)).toString();
    QString price = order->data(order->index(index.row(), 3)).toString();
    QString sale = order->data(order->index(index.row(), 4)).toString();

    ui->dish_id->setText(id);
    ui->dish_name->setText(name);
    ui->dish_type->setText(type);
    ui->dish_price->setText(price);
    ui->dish_sale->setText(sale);

}
