#ifndef SQL_H
#define SQL_H

#include <QSqlDatabase> //数据驱动
#include <QSqlQuery> //数据库执行语句
#include <QMessageBox>//消息盒子
#include <QSqlError>
#include <QDebug>

class sql
{
public:
    sql();
    ~sql();
    void create_table();
    void create_facetable();
    int db_query(QString m_query_sql); //发送数据库语句
    QString getError();
private:
    QSqlDatabase db;
    QSqlError error;
    QSqlQuery *query;
};

#endif // SQL_H
