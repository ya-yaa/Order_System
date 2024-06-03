#ifndef CASHIER_MANAGE_H
#define CASHIER_MANAGE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>

namespace Ui {
class Cashier_manage;
}

class Cashier_manage : public QWidget
{
    Q_OBJECT

public:
    explicit Cashier_manage(QWidget *parent = nullptr);
    ~Cashier_manage();

private:
    Ui::Cashier_manage *ui;

    QSqlQueryModel* c;

private slots:
    void on_back_clicked();
    void on_exit_clicked();


    void on_Insert_button_clicked();

    void on_Delete_button_clicked();

    void on_Update_button_clicked();

    void on_Select_button_clicked();

    void on_cashiertable_pressed(const QModelIndex &index);

signals:
    void cashier_back();
};

#endif // CASHIER_MANAGE_H
