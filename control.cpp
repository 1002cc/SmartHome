#include "control.h"
#include "ui_control.h"
#include "camera.h"
#include "photo.h"
#include "admin.h"
#include "equipmentcontrol.h"
#include "land.h"
CONTROL::CONTROL(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CONTROL)
{
    ui->setupUi(this);
    player=new QMediaPlayer(this);
    ui->pushButton->setIconSize(ui->pushButton->size());
    ui->calendwidget->hide();
    manager = new QNetworkAccessManager(this);
    voicemanager = new QNetworkAccessManager(this);
    hotmanager = new QNetworkAccessManager(this);
    aimanager = new QNetworkAccessManager(this);
    connect(aimanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(aireplyDone(QNetworkReply*)));
    connect(hotmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(hotreplyDone(QNetworkReply*)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyDone(QNetworkReply*)));
    connect(voicemanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(voicereplyDone(QNetworkReply*)));
    weatherurl=QString("https://api.seniverse.com/v3/weather/now.json?key=Srhi2-y2LtemJAmOB&location=%1&language=zh-Hans&unit=c").arg(city);
    manager->get(QNetworkRequest(QUrl(weatherurl)));
    hotmanager->get(QNetworkRequest(QUrl("https://zj.v.api.aa1.cn/api/weibo-rs/")));
    QDate date= QDate::currentDate();
    ui->datelcdNumber->display(date.toString("yyyy-MM-dd"));
    QString text = date.toString("hh:mm:ss");
    QTimer *timer = new QTimer(ui->lcdNumber);
    connect(timer, &QTimer::timeout, this, [=](){
            QTime time = QTime::currentTime();
            QString text = time.toString("hh:mm:ss");
            if ((time.second() % 2) == 0)
                text[5] = ' ';
            ui->lcdNumber->display(text);
            r++;
            if(r%300==0){
                manager->get(QNetworkRequest(QUrl(weatherurl)));
                hotmanager->get(QNetworkRequest(QUrl("https://zj.v.api.aa1.cn/api/weibo-rs/")));
            }
    });
    timer->start(1000);
}

CONTROL::~CONTROL()
{
    delete ui;
    delete manager;
    delete voicemanager;
    delete hotmanager;
    delete player;
}

void CONTROL::on_cameraButton_clicked()
{
    Camera*c=new Camera(this);
    if(c->getcamerastate()){
        c->show();
        this->hide();
    }else{
        c->deletecamera();
    }
}

void CONTROL::on_photoButton_clicked()
{
    Photo* p=new Photo(this);
    p->show();
    this->hide();
}

void CONTROL::on_controlButton_clicked()
{
    EquipmentControl*e=new EquipmentControl(this);
    e->show();
    this->hide();
}

void CONTROL::on_adminButton_clicked()
{
    admin*a=new admin(this);
    a->show();
    this->hide();
}

void CONTROL::on_musicButton_clicked()
{
    if(!musicnumflag)
        m=new Music(this);
    m->show();
    this->hide();
}

void CONTROL::on_calendarButton_clicked()
{
    if(ui->calendwidget->isHidden())
    {
        ui->calendwidget->show();
    }else
    {
        ui->calendwidget->hide();
    }
}

void CONTROL::replyDone(QNetworkReply *reply)
{
    QString all = reply->readAll();
    //qDebug()<<all;
    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);
    //qDebug() << err.error;
    if(!json_recv.isNull())
    {
        QJsonObject object = json_recv.object();
        QJsonArray resultArray = object.value("results").toArray();
        QJsonObject resultArrayObject = resultArray.at(0).toObject();
        QJsonObject locationOBJ = resultArrayObject.value("location").toObject();
        QJsonObject weatherOBJ=resultArrayObject.value("now").toObject();
        weatherid="../SmartHome/image/iocn/white/"+weatherOBJ.value("code").toString()+"@1x.png";
        weather=locationOBJ.value("name").toString()+" 天气:"+weatherOBJ.value("text").toString()+" 温度:"+weatherOBJ.value("temperature").toString()+"度";
        //qDebug()<<weather;
    }
    QTime time = QTime::currentTime();
    QString voiceurl=QString("https://zj.v.api.aa1.cn/api/baidu-01/?msg=%0&choose=2&su=100&yd=5").arg("现在是北京时间"+time.toString("hh")+"点"+time.toString("mm")+"分"+weather).remove(QRegExp("\\s"));
    //qDebug()<<voiceurl;
    QNetworkRequest quest;
    quest.setUrl(QUrl(voiceurl));
    quest.setRawHeader("User-Agent","Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:108.0) Gecko/20100101 Firefox/108.0");
    voicemanager->get(quest);
    ui->weatherlabel->setText(weather);
    QPixmap pm=weatherid;
    //qDebug()<<weatherid;
    ui->weathericonlabel->setPixmap(QPixmap(pm));

}

void CONTROL::voicereplyDone(QNetworkReply *reply)
{
    if(reply != nullptr && reply->error() == QNetworkReply::NoError){
        QString all = reply->readAll();
        //qDebug()<<all;
        QJsonParseError err;
        QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);
        //qDebug() << err.error;
        QJsonObject object = json_recv.object();
        //qDebug()<<object.value("download").toString();
        player->setMedia(QUrl(object.value("download").toString()));
        player->setPlaybackRate(0.8);
        player->play();
    }

}

void CONTROL::hotreplyDone(QNetworkReply *reply)
{
    if(reply != nullptr && reply->error() == QNetworkReply::NoError)
    {
        QString all = reply->readAll();
        //qDebug()<<all;
        QJsonParseError err;
        QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);
        QStringList strlist;
        QString str;
        //qDebug() << err.error;
        if(!json_recv.isNull())
        {
            QJsonObject object = json_recv.object();
            QJsonArray resultArray = object.value("data").toArray();
            for(auto i:resultArray)
            {
                QJsonObject object_1=i.toObject();
                //qDebug() <<object_1["index"].toInt();
                //qDebug() <<object_1["title"].toString();
                str=QString::number(object_1["index"].toInt())+"    "+object_1["title"].toString();
                strlist<< str;
            }
            QStringListModel* Model = new QStringListModel(strlist);;
            ui->hotlistView->setModel(Model);
        }
    }
}

void CONTROL::aireplyDone(QNetworkReply *reply)
{
    QString all = reply->readAll();
    //qDebug()<<all;
    QRegExp qreg_exp("[^A-Za-z0-9\u4e00-\u9fa5]");
    ui->ailabel->setText("小爱:"+all);
    QString voiceurl=QString("https://zj.v.api.aa1.cn/api/baidu-01/?msg=%0&choose=2&su=100&yd=5").arg(all.remove(qreg_exp));
    qDebug()<<voiceurl;
    voicemanager->get(QNetworkRequest(QUrl(voiceurl.remove(QRegExp("\\s")))));
}

void CONTROL::on_clearButton_clicked()
{
    ui->calendwidget->hide();
}

void CONTROL::on_weatherButton_clicked()
{
   city = ui->citylineEdit->text();
   weatherurl=QString("https://api.seniverse.com/v3/weather/now.json?key=Srhi2-y2LtemJAmOB&location=%1&language=zh-Hans&unit=c").arg(city);
   manager->get(QNetworkRequest(QUrl(weatherurl)));
}

void CONTROL::on_pushButton_clicked()
{
    land*l=new land();
    l->show();
    this->close();
    delete this;
}

void CONTROL::on_aipushButton_clicked()
{
    QString str=QString("https://v.api.aa1.cn/api/api-xiaoai/talk.php?msg=%0").arg(ui->ailineEdit->text());
    qDebug() << str;
    aimanager->get(QNetworkRequest(QUrl(str)));
    ui->ailineEdit->clear();
}

void CONTROL::reviecedata(bool flag)
{
    musicnumflag=flag;
}

