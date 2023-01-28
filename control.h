#ifndef CONTROL_H
#define CONTROL_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QMediaPlayer>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>


#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSslSocket>
#include <QDebug>
#include "music.h"
namespace Ui {
class CONTROL;
}

class CONTROL : public QMainWindow
{
    Q_OBJECT

public:
    explicit CONTROL(QWidget *parent = nullptr);
    ~CONTROL();
private slots:
    void on_cameraButton_clicked();

    void on_photoButton_clicked();

    void on_controlButton_clicked();

    void on_adminButton_clicked();

    void on_musicButton_clicked();

    void on_calendarButton_clicked();

    void replyDone(QNetworkReply*reply);

    void voicereplyDone(QNetworkReply*reply);

    void hotreplyDone(QNetworkReply*reply);

    void aireplyDone(QNetworkReply*reply);

    void on_clearButton_clicked();

    void on_weatherButton_clicked();

    void on_pushButton_clicked();

    void on_aipushButton_clicked();

    void reviecedata(bool flag);

private:
    Ui::CONTROL *ui;
    QNetworkAccessManager*manager;
    QNetworkAccessManager*voicemanager;
    QNetworkAccessManager*hotmanager;
    QNetworkAccessManager*aimanager;
    QString weatherid;
    QString weather;
    unsigned int r=0;
    Music*m;
    QString city="guangzhou";
    QString weatherurl;
    QMediaPlayer*player;
    bool musicnumflag=false;
};

#endif // CONTROL_H
