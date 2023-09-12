#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "land.h"
#include "sql.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/image/iocn/icon.ico"));
#if 1
    QFile file(":/qss/qss/style-1.qss"); /*QSS文件所在的路径*/
    /*打开资源文件目录*/
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    a.setStyleSheet(filetext.readAll());
    file.close();
#endif
    // 建立一个用户账户数据表
    sql::getInstance()->create_table();
    // 建立一个人脸识别数据表
    sql::getInstance()->create_facetable();
    land w;
    w.show();
    return a.exec();
}
