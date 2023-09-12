#ifndef LAND_H
#define LAND_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSettings>
#include <QMovie>
#include "face.h"

QT_BEGIN_NAMESPACE
namespace Ui { class land; }
QT_END_NAMESPACE

class land : public QMainWindow
{
    Q_OBJECT
private:
    void LoginInit();
    void loginData();
    void loadcfg();
    void ReadInit(QString, QStringList&);
    void WriteInit(QString, QStringList);
public:
    land(QWidget *parent = nullptr);
    ~land();
private slots:
    void on_pushButton_login_clicked();
    void on_checkBox_clicked();
    void setlogint(int);
    void on_pushButton_login_p_clicked();
    void reviecefacedata(bool flag);
private:
    Ui::land *ui;
    QMovie *movie;
    QStringList m_passwdList;//保存密码
    QStringList m_isremberList;
    QStringList m_userNameList;
    QString isrembered;//是否记住密码
    QString username;
    QString password;
    Face *f;
};
#endif // LAND_H
