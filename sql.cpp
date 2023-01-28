#include "sql.h"

sql::sql()
{
    /*创建sqlite数据库*/
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../SmartHome/db/userdata.db");
    /*创建执行语句对象*/
    query =new QSqlQuery(db);
    if(!db.open())
    {
        qDebug()<<"open error";
        error = db.lastError();
    }
}


sql::~sql()
{
    delete query;
}

void sql::create_table()
{
    /*创建sqlite数据库表*/
    QString createsql=QString("create table if not exists userdata(id integer primary key autoincrement,"
                        "username ntext unique not NULL,"
                        "password ntext not NULL)");
    QSqlQuery query(db);
    if(!query.exec(createsql)){
        qDebug()<<"table create error";
    }
    else{
        qDebug()<<"table create success";
    }
}

void sql::create_facetable()
{
    /*创建sqlite数据库表*/
    QString createsql=QString("create table if not exists facedata(id integer primary key autoincrement,"
                        "username ntext unique not NULL,"
                        "number ntext not NULL)");
    QSqlQuery query(db);
    if(!query.exec(createsql)){
        qDebug()<<"facetable create error";
    }
    else{
        qDebug()<<"facetable create success";
    }
}

QString sql::getError()
{
     return error.text();
}
