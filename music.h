#ifndef MUSIC_H
#define MUSIC_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
//����������ͷ�ļ�
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

//���JSON������ص�ͷ�ļ�����Ϊ�Ӹ÷����������������������json����ʽ�ظ���
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
    QMediaPlayer *player;   //������
    bool isplay=false;      //bool���͵Ĳ����жϱ���
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
