#include "token_ok.h"
#include "ui_token_ok.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

Token_ok::Token_ok(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Token_ok)
{
    ui->setupUi(this);
    pay = new Token_pay;

}

Token_ok::~Token_ok()
{
    delete ui;
}

// 查看
void Token_ok::on_lookButton_clicked()
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
void Token_ok::on_pay_clicked()
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
    //外带加收一元打包费
    result++;
    // 最终金额保留两位小数
    QString cost = QString::number(result, 'f', 2);

    // 获取收银员编号
    cashier_id = ui->cashierid->text();

    // 插入收银单
    QString insert = QString("insert into bills(收银单编号,金额,收银员编号,顾客编号) values(%1,%2,%3,%4);")
                     .arg(bill_id,cost,cashier_id,custom_id);
    // 如果收银员编号错误
    if (!query.exec(insert)) {
        QMessageBox::critical(this, "数据库操作错误", query.lastError().text());
        return;
    }
    //QMessageBox::information(this, "执行语句", insert);

    // 更新外带顾客的收银单,并更新顾客状态
    QString updateQuery = QString("UPDATE token_customers SET 收银单编号 = %1 WHERE 外带顾客编号 = %2;")
                          .arg(bill_id,custom_id);
    query.exec(updateQuery);

    updateQuery = QString("UPDATE customers SET 顾客状态 = '结账' WHERE 顾客编号 = %1;")
                  .arg(custom_id);
    query.exec(updateQuery);

    pay->acceptid(order_id,custom_id,bill_id);
    this->hide();
    this->pay->show();
}
