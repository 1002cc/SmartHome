#ifndef ADMIN_H
#define ADMIN_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QModelIndexList>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTimer>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QStringListModel>
namespace Ui {
class admin;
}

class admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_clicked();

    void on_pushButton_sig_clicked();

    void on_gotoButton_clicked();

    void on_startButton_clicked();

    void reviecefacedata(bool flag);
    void on_tryButton_clicked();

    void on_deleteButton_clicked();

    void on_facetableView_sql_clicked(const QModelIndex &index);

    void on_deleteuserButton_clicked();

    void on_tableView_sql_clicked(const QModelIndex &index);

private:
    Ui::admin *ui;
    QSqlQuery* query;
    QSqlQueryModel* querydel;
    QSqlQueryModel* facequerydel;
    QStringListModel *Modeldill;
    QModelIndex indexdel;
    QModelIndex userindexdel;
};

#endif // ADMIN_H
