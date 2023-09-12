#ifndef MUSIC_H
#define MUSIC_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
//添加网络相关头文件
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

//添加JSON解析相关的头文件，因为从该服务器请求的天气数据是以json的形式回复的
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSslSocket>
#include <QDebug>
#include <QStringListModel>
#include <algorithm>
#include <QModelIndex>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QUrl>
#include <QAudioOutput>

namespace Ui {
class Music;
}

class Music : public QMainWindow
{
    Q_OBJECT
public:
    explicit Music(QWidget *parent = nullptr);
    ~Music();
    void musicinit();
    void readMusicfile();
    void onlinemusic(int musicId);
    void playmusic();
private slots:
    void on_prevButton_clicked();
    void on_stopButton_clicked();
    void on_nextButton_clicked();
    void recv_ack(QNetworkReply *rely);
    void recv_image(QNetworkReply *rely);
    void on_pushButton_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_pushButton_2_clicked();
    void on_returnButton__clicked();
    void on_backstageButton_clicked();
    void on_sSlider_valueChanged(int value);
    void on_musiclistWidget_currentRowChanged(int currentRow);
signals:
    void sendData_land(bool flag);
private:
    Ui::Music *ui;
    QMediaPlayer *player;   //播放者
    bool isplay=false;      //bool类型的播放判断变量
    QNetworkAccessManager*musicmanager;
    QNetworkAccessManager*imagemanager;
    QString data;
    int musicId;
    int music_duration;
    QString musicname;
    QString singername;
    QString imageid;
    bool isshow=false;
    QDir dir;
    QStringListModel *Modeldill;
    QStringList strlist;
    QString musicnamepath;
    int musicCount;
    QStringList playerlist;
    QDateTime dd;
    QAudioOutput* audioOutput;
    int flag=0;
};

#endif // MUSIC_H
