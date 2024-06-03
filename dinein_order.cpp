#include "dinein_order.h"
#include "ui_dinein_order.h"

#include "widget.h"

#include <QSqlError>

Dinein_order::Dinein_order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dinein_order)
{
    ui->setupUi(this);

    ok = new Dinein_ok;

    // 接受确认订单界面的返回信号，返回点餐界面
    connect(ok,SIGNAL(Dineinorder_back()),this,SLOT(receive_ok()));

    // 初始化当前订单表头
    order.setHorizontalHeaderLabels({"编号", "名称", "分类","价格","份数","要求"});
}

Dinein_order::~Dinein_order()
{
    delete ui;
}

// 发送回到主界面信号
void Dinein_order::on_backtomain_clicked()
{
    QSqlQuery query;

    // 启动事务
    if (!db.transaction()) {
        qDebug() << "Error: Unable to start transaction!";
        return ;
    }

    // 执行删除操作
    // 删除订单信息
    QString del = "delete from orders where 订单编号 = " + order_id + ";";
    query.prepare(del);

    // 删除失败处理
    if (!query.exec()) {
        qDebug() << "Error: Unable to delete from Orders table!" << query.lastError().text();
        db.rollback();
        return;
    }

    // 删除顾客信息
    del = "delete from customers where 顾客编号 = " + custom_id + ";";
    query.prepare(del);

    // 删除失败处理
    if (!query.exec()) {
        qDebug() << "Error: Unable to delete from Orders table!" << query.lastError().text();
        db.rollback();
        return;
    }

    // 询问是否确认退出
    int ok = QMessageBox::warning(this,tr("退出"),tr("确定退出吗？您的顾客与订单信息将被删除！"),
                                  QMessageBox::Yes,QMessageBox::No);
    //如确认
    if(ok == QMessageBox::Yes){
        // 提交事务
        if (!db.commit()) {
            qDebug() << "Error: Unable to commit transaction!";
        }
        emit this->Dinein_back();
        return;
    }
    // 否则回滚
    else{
        db.rollback();
        return;
    }


}

// 接受确认订单界面的返回信号，返回点餐界面
void Dinein_order::receive_ok()
{
    // 加菜，不再是第一次下单
    isfirst = false;

    this->ok->hide();
    this->show();
}

// 按照名称进行查询
void Dinein_order::on_nameselect_button_clicked()
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
    ui->dinein_menu->setModel(menu);
}

// 按照分类进行查询
void Dinein_order::on_typeselect_button_clicked()
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
    ui->dinein_menu->setModel(menu);
}

// 选中菜单时显示信息
void Dinein_order::on_dinein_menu_pressed(const QModelIndex &index)
{
    //"菜品编号", "菜品名称", "菜品分类","菜品价格"
    QString id = menu->data(menu->index(index.row(), 0)).toString();
    QString name = menu->data(menu->index(index.row(), 1)).toString();
    QString type = menu->data(menu->index(index.row(), 2)).toString();
    QString price = menu->data(menu->index(index.row(), 3)).toString();

    ui->dinein_dishid->setText(id);
    ui->dinein_dishname->setText(name);
    ui->dinein_dishtype->setText(type);
    ui->dinein_dishprice->setText(price);
}

// 选中当前订单时显示信息
void Dinein_order::on_order_now_pressed(const QModelIndex &index)
{
    //"菜品编号", "菜品名称", "菜品分类","菜品价格","份数","要求"
    QString id = order.data(order.index(index.row(), 0)).toString();
    QString name = order.data(order.index(index.row(), 1)).toString();
    QString type = order.data(order.index(index.row(), 2)).toString();
    QString price = order.data(order.index(index.row(), 3)).toString();
    QString num = order.data(order.index(index.row(), 4)).toString();
    QString dem = order.data(order.index(index.row(), 5)).toString();

    ui->dinein_dishid->setText(id);
    ui->dinein_dishname->setText(name);
    ui->dinein_dishtype->setText(type);
    ui->dinein_dishprice->setText(price);
    ui->dinein_dishnum->setText(num);
    ui->Demand->setText(dem);
}

// 将菜品加入当前订单显示
void Dinein_order::on_Insert_button_clicked()
{
    QString id = ui->dinein_dishid->toPlainText();  //当前菜品编号
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
    //检查是否写了份数
    if(ui->dinein_dishnum->text() != ""){
        rowData << new QStandardItem(ui->dinein_dishid->toPlainText())
                << new QStandardItem(ui->dinein_dishname->toPlainText())
                << new QStandardItem(ui->dinein_dishtype->toPlainText())
                << new QStandardItem(ui->dinein_dishprice->toPlainText())
                << new QStandardItem(ui->dinein_dishnum->text())
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
void Dinein_order::on_Update_button_clicked()
{
    // 要更改菜品的编号
    QString id = ui->dinein_dishid->toPlainText();

    QString newnum = ui->dinein_dishnum->text();  // 新的份数
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
void Dinein_order::on_Delete_button_clicked()
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

// 转到确认订单界面
void Dinein_order::on_okorder_clicked()
{
    // 看订单是否为空
    if(order.rowCount() == 0){
        QMessageBox::information(this, "订单为空", "您还没有点餐哦~");
        return;
    }
    // 看填桌号没有
    if(ui->tableid->text() == ""){
         QMessageBox::information(this, "桌号为空", "请输入您的用餐桌号~");
         return;
    }

    // 看是否为第一次下单，不是第一次下单就删掉contain中原本
    // 是第一次下单
    if (isfirst){
        table_id = ui->tableid->text();

        // 插入顾客
        // 创建插入语句
        QSqlQuery query;
        QString str = custom_id + ",'就餐'";
        QString insert = "insert into customers(顾客编号, 顾客状态) values(" + str + ");";
        // 执行
        query.exec(insert);
        // 插入堂食顾客
        str = custom_id + "," + table_id;
        insert = "insert into dinein_customers(堂食顾客编号,餐桌编号) values(" + str + ");";
        // 执行
        query.exec(insert);


        // 插入订单,桌号下单
        str = order_id + "," + table_id;
        insert = "insert into orders(订单编号,餐桌编号) values(" +str+ ");";
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
    }
    else{
        QSqlQuery query;

        // 启动事务
        if (!db.transaction()) {
            qDebug() << "Error: Unable to start transaction!";
            return ;
        }

        // 执行删除操作
        // 删除原本contain中对应订单的菜品
        QString del = "delete from contain where 订单编号 = " + order_id + ";";
        query.prepare(del);


        // 删除失败
        if (!query.exec()) {
            qDebug() << "Error: Unable to delete from Orders table!" << query.lastError().text();
            db.rollback();
            return;
        }

        int ok = QMessageBox::warning(this,tr("删除原订单"),tr("确定删除原订单并改为新订单吗？"),
                                      QMessageBox::Yes,QMessageBox::No);
        //如确认
        if(ok == QMessageBox::Yes){
            // 提交事务
            if (!db.commit()) {
                qDebug() << "Error: Unable to commit transaction!";
                return;
            }

            // 重新插入，将所有菜品插入包含
            int rowCount = order.rowCount();  // 获取行数
            // 循环获取order_now中信息进行插入
            for (int i = 0; i < rowCount; ++i)
            {
                //"菜品编号", "菜品名称", "菜品分类","菜品价格","份数","要求"
                QString id = order.item(i, 0)->text(); // 获取第 i 行第一列,菜品编号
                QString num = order.item(i, 4)->text(); // 获取第 i 行第五列,份数
                QString dem = order.item(i, 5)->text(); // 获取第 i 行第六列,口味

                QString str = order_id + "," + id + "," + num + ",'" + dem + "'";
                QString insert = "insert into contain(订单编号,菜品编号,份数,口味) values(" +str+ ");";

                query.exec(insert);
            }

        }
        //否则回滚
        else{
            db.rollback();
        }
    }

    // 传递order_id、custom_id、table_id
    this->ok->acceptid(order_id,custom_id,table_id);

    this->hide();
    this->ok->show();
}
