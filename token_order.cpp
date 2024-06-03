#include "token_order.h"
#include "ui_token_order.h"
#include "widget.h"

Token_order::Token_order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Token_order)
{
    ui->setupUi(this);

    ok = new Token_ok;

    // 初始
    //Widget::order->setQuery();
    //ui->token_menu->setModel(Widget::order);

    order.setHorizontalHeaderLabels({"编号", "名称", "分类","价格","份数","要求"});

}

Token_order::~Token_order()
{
    delete ui;
}

// 回到主界面
void Token_order::on_backtomain_clicked()
{
    emit this->Token_back();
}

// 确认下单界面
void Token_order::on_okorder_clicked()
{
    // 订单为空
    if(order.rowCount() == 0){
        QMessageBox::information(this, "订单为空", "您还没有点餐哦~");
        return;
    }
    else{
        // 插入顾客
        // 创建插入语句
        QSqlQuery query;
        QString str = custom_id + ",'就餐'";
        QString insert = "insert into customers(顾客编号, 顾客状态) values(" + str + ");";
        // 执行
        query.exec(insert);
        // 插入外带顾客
        insert = "insert into token_customers(外带顾客编号) values(" +custom_id+ ");";
        // 执行
        query.exec(insert);

        // 插入订单
        str = order_id + "," +custom_id;
        insert = "insert into orders(订单编号,顾客编号) values(" +str+ ");";
        // 执行
        query.exec(insert);
        // 将所有菜品插入包含
        int rowCount = order.rowCount();  // 获取行数
        // 循环获取order_now中信息进行插入
        for (int i = 0; i < rowCount; ++i)
        {
            //"菜品编号", "菜品名称", "菜品分类","菜品价格","份数","要求"
            QString id = order.item(i, 0)->text(); // 获取第 i 行第一列,菜品编号
            QString num = order.item(i, 4)->text(); // 获取第 i 行第五列,份数
            QString dem = order.item(i, 5)->text(); // 获取第 i 行第六列,口味

            str = order_id + "," + id + "," + num + ",'" + dem + "'";
            insert = "insert into contain(订单编号,菜品编号,份数,口味) values(" +str+ ");";

            query.exec(insert);

        }

        // 传递order_id与custom_id
        this->ok->acceptid(order_id,custom_id);

        this->hide();
        this->ok->show();
    }
}


// 按照名称进行查询
void Token_order::on_nameselect_button_clicked()
{
    QSqlQuery query;
    QString select;

    // 如果有查询条件
    if( ui->selectline->text() != "")
    {
        select = select + "select `菜品编号`, `菜品名称`,`菜品分类`, `菜品价格` "
                                  "from dishes "
                                  "where 菜品名称 like '%" + ui->selectline->text() +"%';";

    }
    // 空查询条件，查所有
    else{
        select = "select `菜品编号`, `菜品名称`,`菜品分类`, `菜品价格` from dishes;";

        // 显示折扣商品
        sale->setQuery("select `菜品编号`, `菜品名称`, `菜品价格`, `优惠信息` "
                       "from dishes "
                       "where 优惠信息<1;");
        ui->sale_today->setModel(sale);
    }

    //QMessageBox::information(this, "执行语句", select);

    menu->setQuery(select);
    ui->token_menu->setModel(menu);
}

// 按照分类进行查询
void Token_order::on_typeselect_button_clicked()
{
    QString select;

    // 如果有查询条件
    if( ui->selectline->text() != "")
    {
        select = select + "select `菜品编号`, `菜品名称`,`菜品分类`, `菜品价格` "
                                  "from dishes "
                                  "where 菜品分类 like '%" + ui->selectline->text() +"%';";

    }
    // 空查询条件，查所有
    else{
        select = "select `菜品编号`, `菜品名称`,`菜品分类`, `菜品价格` from dishes;";

        // 显示折扣商品
        sale->setQuery("select `菜品编号`, `菜品名称`, `菜品价格`, `优惠信息` "
                       "from dishes "
                       "where 优惠信息<1;");
        ui->sale_today->setModel(sale);
    }

    //QMessageBox::information(this, "执行语句", select);

    menu->setQuery(select);
    ui->token_menu->setModel(menu);
}

// 将菜品加入当前订单显示
void Token_order::on_Insert_button_clicked()
{
    QString id = ui->token_dishid->toPlainText();  //当前菜品编号
    int rowCount = order.rowCount();  // 获取行数
    // 在表中查找当前菜品是否已经在订单中
    for (int i = 0; i < rowCount; ++i)
    {
        QStandardItem* item = order.item(i, 0); // 获取第 i 行第一列的项目
        // 已经存在
        if (item && item->text() == id)
        {
            QMessageBox::information(this, "加入失败", "当前菜品已在您的订单中，请直接修改它~");
            return;
        }
    }

    //没有在当前订单中，插入
    QList<QStandardItem*> rowData;
    //"菜品编号", "菜品名称", "菜品分类","菜品价格","份数","要求"
    if(ui->token_dishnum->text() != ""){
        rowData << new QStandardItem(ui->token_dishid->toPlainText())
                << new QStandardItem(ui->token_dishname->toPlainText())
                << new QStandardItem(ui->token_dishtype->toPlainText())
                << new QStandardItem(ui->token_dishprice->toPlainText())
                << new QStandardItem(ui->token_dishnum->text())
                << new QStandardItem(ui->Demand->text());
        order.appendRow(rowData);
        rowData.clear();
    }
    else{
        QMessageBox::information(this, "插入失败", "请输入点餐份数~");
    }

    ui->order_now->setModel(&order);
}

// 修改当前订单行
void Token_order::on_Update_button_clicked()
{
    // 要更改菜品的编号
    QString id = ui->token_dishid->toPlainText();

    QString newnum = ui->token_dishnum->text();  // 新的份数
    QString newdem = ui->Demand->text();  // 新的要求

    int rowCount = order.rowCount();  // 获取行数
    int columnCount = order.columnCount();  // 获取列数

    // 在表中查找要修改的行
    for (int i = 0; i < rowCount; ++i)
    {
        QStandardItem* item = order.item(i, 0); // 获取第 i 行第一列的项目
        // 找到正确的菜品编号
        if (item && item->text() == id)
        {
          QStandardItem* lastColumnItem = order.item(i, columnCount - 1); // 获取该行最后一列(要求)的项目
          if (lastColumnItem)
          {
              lastColumnItem->setText(newdem);
          }

          lastColumnItem = order.item(i, columnCount - 2);// 获取该行倒数第二列(份数)的项目
          if (lastColumnItem)
          {
              lastColumnItem->setText(newnum);
          }
              break; // 找到后退出循环
        }
    }

}

// 从当前订单显示中删除菜品
void Token_order::on_Delete_button_clicked()
{
    // 读取选中行
    int currentrow = ui->order_now->currentIndex().row();

    if(currentrow != -1)//若选中一行
    {
        int ok  =  QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除当前行吗？"), QMessageBox::Yes,QMessageBox::No);
        //如确认删除
        if(ok == QMessageBox::Yes)
        {
            QModelIndexList selected = ui->order_now->selectionModel()->selectedIndexes();
            for (QModelIndex index : selected) {
                int row = index.row();
                order.removeRow(row);
            }
            QMessageBox::information(this, "删除成功", "所选信息删除成功");
        }
     }
     else
            QMessageBox::information(this, "提示", "请选择你要删除的信息行");

    ui->order_now->setModel(&order);
}

// 选中菜单时显示信息
void Token_order::on_token_menu_pressed(const QModelIndex &index)
{
    //"菜品编号", "菜品名称", "菜品分类","菜品价格"
    QString id = menu->data(menu->index(index.row(), 0)).toString();
    QString name = menu->data(menu->index(index.row(), 1)).toString();
    QString type = menu->data(menu->index(index.row(), 2)).toString();
    QString price = menu->data(menu->index(index.row(), 3)).toString();

    ui->token_dishid->setText(id);
    ui->token_dishname->setText(name);
    ui->token_dishtype->setText(type);
    ui->token_dishprice->setText(price);
}

// 选中当前订单时显示信息
void Token_order::on_order_now_pressed(const QModelIndex &index)
{
    //"菜品编号", "菜品名称", "菜品分类","菜品价格","份数","要求"
    QString id = order.data(order.index(index.row(), 0)).toString();
    QString name = order.data(order.index(index.row(), 1)).toString();
    QString type = order.data(order.index(index.row(), 2)).toString();
    QString price = order.data(order.index(index.row(), 3)).toString();
    QString num = order.data(order.index(index.row(), 4)).toString();
    QString dem = order.data(order.index(index.row(), 5)).toString();

    ui->token_dishid->setText(id);
    ui->token_dishname->setText(name);
    ui->token_dishtype->setText(type);
    ui->token_dishprice->setText(price);
    ui->token_dishnum->setText(num);
    ui->Demand->setText(dem);

}


