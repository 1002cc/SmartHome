#include "admin.h"
#include "ui_admin.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include "face.h"
admin::admin(QWidget *parent) : QMainWindow(parent),
                                ui(new Ui::admin)
{
    ui->setupUi(this);
    ui->listWidget->setCurrentRow(0);
    query = new QSqlQuery();
    // 数据库数据显示
    querydel = new QSqlQueryModel();
    querydel->setQuery("select * from userdata");
    // 设置标题栏
    querydel->setHeaderData(0, Qt::Horizontal, "序号");
    querydel->setHeaderData(1, Qt::Horizontal, "账号");
    querydel->setHeaderData(2, Qt::Horizontal, "密码");
    ui->tableView_sql->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sql->setModel(querydel);
    this->ui->tableView_sql->verticalHeader()->hide();
    ui->frame_2->hide();

    facequerydel = new QSqlQueryModel();
    facequerydel->setQuery("select * from facedata");
    // 设置标题栏
    facequerydel->setHeaderData(0, Qt::Horizontal, "序号");
    facequerydel->setHeaderData(1, Qt::Horizontal, "用户名");
    facequerydel->setHeaderData(2, Qt::Horizontal, "编号");
    ui->facetableView_sql->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->facetableView_sql->setModel(facequerydel);
    this->ui->facetableView_sql->verticalHeader()->hide();
}

admin::~admin()
{
    delete ui;
}

void admin::on_listWidget_currentRowChanged(int currentRow)
{
    ui->tabWidget->setCurrentIndex(currentRow);
}

void admin::on_pushButton_clicked()
{
    this->parentWidget()->show();
    this->close();
}

void admin::on_pushButton_sig_clicked()
{
    /*获取账号密码和再次确认密码*/
    QString username = ui->lineEdit_id->text();
    QString password = ui->lineEdit_password_1->text();
    QString surepass = ui->lineEdit_password_2->text();
    /*判断是否为空*/
    if (username != nullptr && password != nullptr && surepass != nullptr)
    {
        /*判断密码是否一致*/
        if (password == surepass)
        {
            QString sql = QString("insert into userdata(username,password) values('%1','%2');")
                              .arg(username)
                              .arg(password);
            /*创建执行语句对象*/
            QSqlQuery query;
            /*判断执行结果*/
            if (!query.exec(sql))
            {
                qDebug() << "insert into error";
                QMessageBox::information(this, "注册认证", "注册失败！");
            }
            else
            {
                qDebug() << "insert into success";
                QMessageBox::information(this, "注册认证", "注册成功！");
                querydel->setQuery("select * from userdata");
            }
        }
        else
        {
            QMessageBox::information(this, "注册认证", "两次密码输入不一致");
        }
    }
    else
    {
        QMessageBox::information(this, "注册认证", "账号密码不能为空");
    }
}

void admin::on_gotoButton_clicked()
{
    if (ui->frame_2->isHidden())
    {
        ui->frame_2->show();
    }
    else
    {
        ui->frame_2->hide();
    }
}

void admin::on_startButton_clicked()
{
    QString name = ui->lineEditname->text();
    QString num = ui->lineEditnumber->text();
    QString sql = QString("select * from facedata where username='%1' OR number='%2'")
                      .arg(name)
                      .arg(num);
    /*创建执行语句对象*/
    QSqlQuery query(sql);
    if (!query.next())
    {
        ui->lineEditname->clear();
        ui->lineEditnumber->clear();
        qDebug() << name << " : " << num << " : " << num.toInt();
        Face *f = new Face(name, num.toInt(), this);
        if (f->facenew())
            f->show();
        else
            f->deleteface();
    }
    else
    {
        QMessageBox::information(this, "注册识别", "该用户已存在");
        ui->lineEditname->clear();
        ui->lineEditnumber->clear();
    }
}

void admin::reviecefacedata(bool flag)
{
    if (flag)
    {
        facequerydel->setQuery("select * from facedata");
        ui->frame_2->hide();
    }
    else
    {
        ui->frame_2->hide();
    }
}

void admin::on_tryButton_clicked()
{
    Face *f = new Face(1, this);
    if (f->facenew())
        f->show();
    else
        f->deleteface();
}

void admin::on_deleteButton_clicked()
{
    QString textd = indexdel.sibling(indexdel.row(), 0).data().toString();
    QString sql = "delete from facedata where id=" + textd;
    query->exec(sql);
    facequerydel->setQuery("select * from facedata");
}

void admin::on_facetableView_sql_clicked(const QModelIndex &index)
{
    /*获取tableview单击的内容，并显示到文本中*/
    // QString text =index.sibling(index.row(),index.column()).data().toString();
    this->indexdel = index;
}

void admin::on_deleteuserButton_clicked()
{
    QString textd = userindexdel.sibling(userindexdel.row(), 0).data().toString();
    QString sql = "delete from userdata where id=" + textd;
    query->exec(sql);
    querydel->setQuery("select * from userdata");
}

void admin::on_tableView_sql_clicked(const QModelIndex &index)
{
    /*获取tableview单击的内容，并显示到文本中*/
    // QString text =index.sibling(index.row(),index.column()).data().toString();
    this->userindexdel = index;
}
