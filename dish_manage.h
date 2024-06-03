#ifndef DISH_MANAGE_H
#define DISH_MANAGE_H

#include <QWidget>
#include <QSqlQueryModel>


namespace Ui {
class dish_manage;
}

class dish_manage : public QWidget
{
    Q_OBJECT

public:
    explicit dish_manage(QWidget *parent = nullptr);
    ~dish_manage();

private:
    Ui::dish_manage *ui;

    QSqlQueryModel* order = new QSqlQueryModel();

signals:
    // 返回主管理界面
    void menu_back();
private slots:
    void on_back_clicked();
    void on_exit_clicked();
    void on_Insert_button_clicked();
    void on_Delete_button_clicked();
    void on_Update_button_clicked();
    void on_Select_button_clicked();
    void on_dishtable_pressed(const QModelIndex &index);
};

#endif // DISH_MANAGE_H

