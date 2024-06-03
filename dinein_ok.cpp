#include "dinein_ok.h"
#include "ui_dinein_ok.h"

#include <QSqlError>

Dinein_ok::Dinein_ok(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dinein_ok)
{
    ui->setupUi(this);

    pay = new Dinein_pay;
}

Dinein_ok::~Dinein_ok()
{
    delete ui;
}

// 发送返回点菜界面的信号，加菜
void Dinein_ok::on_backtoorder_clicked()
{
    emit this->Dineinorder_back();
}

// 查看
void Dinein_ok::on_lookButton_clicked()
{
    QString select = QString(
        "SELECT 菜品编号, 菜品名称, 菜品分类, 折后价格, 份数, 口味 "
        "FROM OrderDetails "
        "WHERE 订单编号 = %1;"
    ).arg(order_id);

    // QMessageBox::information(this, "执行语句", select);

    ordernow->setQuery(select);
    ui->order_now->setModel(ordernow);

}

// 结账界面
void Dinein_ok::on_pay_clicked()
{
    // 检查是否输入收银员编号
    if(ui->cashierid->text() == ""){
        QMessageBox::information(this, "稍等一下", "请输入值班的收银员编号哦~");
        return;
    }

    // 已输入
    QSqlQuery query;
    // 获取收银单编号
    query.prepare(QString("SELECT COUNT(*) FROM bills;"));
    if (!query.exec()) {
        qDebug() << "查询失败: " << query.lastError();
        return;
    }
    if (query.next()) {
        QVariant sumResult = query.value(0);
        int temp = sumResult.toInt() + 1;

        bill_id = QString::number(temp);
        qDebug() << " bill_id: " << bill_id;
    }

    // 计算收款金额
    double result = 0;
    // 获取模型
    QAbstractItemModel* model = ui->order_now->model();
    // 遍历每一行
    for (int row = 0; row < model->rowCount(); ++row) {
        bool ok;
        // 菜品编号0, 菜品名称1, 菜品分类2, 折后价格3, 份数4, 口味5
        // 获取折后价格，份数
        QModelIndex price_index = model->index(row, 3);
        double price = model->data(price_index).toDouble(&ok);
        if (!ok) {
            qWarning() << "Failed to convert price to double.";
            continue;
        }

        QModelIndex num_index = model->index(row, 4);
        double num = model->data(num_index).toDouble(&ok);
        if (!ok) {
            qWarning() << "Failed to convert num to double.";
            continue;
        }

        // 进行运算
        double temp = price * num;
        result += temp;
    }
    // 最终金额保留两位小数
    QString cost = QString::number(result, 'f', 2);

    // 获取收银员编号
    cashier_id = ui->cashierid->text();

    // 插入收银单
    QString insert = QString("insert into bills(收银单编号,金额,收银员编号,餐桌编号) values(%1,%2,%3,%4);")
                     .arg(bill_id,cost,cashier_id,table_id);
    // 如果收银员编号错误
    if (!query.exec(insert)) {
        QMessageBox::critical(this, "数据库操作错误", query.lastError().text());
        return;
    }


    //QMessageBox::information(this, "执行语句", insert);


    // 更新餐桌的收银单,并更新顾客状态
    // 准备存储过程调用语句
    QString callProc = QString("CALL update_table_and_customer(%1, %2, %3);")
                       .arg(bill_id)
                       .arg(table_id)
                       .arg(custom_id);

    // 执行存储过程
    if (!query.exec(callProc)) {
        QMessageBox::critical(this, "数据库操作错误", query.lastError().text());
        return;
    }
    else {
        qDebug() << "Stored procedure executed successfully.";

    }




    pay->acceptid(order_id,custom_id,bill_id,table_id);

    this->hide();
    this->pay->show();

}


