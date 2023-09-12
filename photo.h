#ifndef PHOTO_H
#define PHOTO_H

#include <QMainWindow>
#include <QStringListModel>
#include <algorithm>
#include <QModelIndex>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMediaPlayer>
#include <QVideoWidget>
namespace Ui {
class Photo;
}

class Photo : public QMainWindow
{
    Q_OBJECT
public:
    explicit Photo(QWidget *parent = nullptr);
    ~Photo();
    void showimage(const int &index);
private slots:
    void on_listViewImage_clicked(const QModelIndex &index);
    void on_pushButtonnext_clicked();
    void on_pushButtonprev_clicked();
    void on_returnButton_clicked();
private:
    Ui::Photo *ui;
    QDir dir;
    QStringListModel *Modeldill;
    QStringList strlist;
    QString imagename;
    int flag;
    int ImageCount;
    int w,h;
    QPixmap t;
    QMediaPlayer* player;
    QVideoWidget* videoWidget;
};

#endif // PHOTO_H
