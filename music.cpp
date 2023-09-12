#include "music.h"
#include "ui_music.h"
#include <QFileInfo>
Music::Music(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Music)
{
    ui->setupUi(this);
    musicinit();
    connect(this,SIGNAL(sendData_land(bool)),this->parentWidget(),SLOT(reviecedata(bool)));
}

Music::~Music()
{
    delete ui;
    delete player;
}

void Music::musicinit()
{
    //显示位置
    this->setGeometry(this->parentWidget()->x(),this->parentWidget()->y(),this->parentWidget()->width(),this->parentWidget()->height());
    //实例化一个播放器和播放列表
    player=new QMediaPlayer(this);
    connect(player,&QMediaPlayer::durationChanged,this,[=](qint64 duration){
        ui->nowlabel->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration/1000%60));
        ui->musicSlider->setRange(0,duration);
    });
    connect(player,&QMediaPlayer::positionChanged,this,[=](qint64 pos){
        ui->startlabel->setText(QString("%1:%2").arg(pos/1000/60,2,10,QChar('0')).arg(pos/1000%60));
        ui->musicSlider->setValue(pos);
        if(pos>=player->duration()&&pos!=0)
        {
            flag++;
            flag=flag%playerlist.size();
            ui->musiclistWidget->setCurrentRow(flag);
        }
    });
    connect(ui->musicSlider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);
    musicmanager=new QNetworkAccessManager(this);
    imagemanager=new QNetworkAccessManager(this);
    connect(musicmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(recv_ack(QNetworkReply*)));
    connect(imagemanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(recv_image(QNetworkReply*)));
    player->setVolume(50);//默认音量
    //隐藏
    ui->tableWidget->hide();
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setColumnHidden(5, true);
    ui->tableWidget->setColumnHidden(4, true);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置内容自动调整行宽
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置内容显示居中
    ui->tableWidget->verticalHeader()->setVisible(false);//设置隐藏列头
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);//设置选中一行
    QStringList strList;
    strList <<tr("序号")<< tr("歌曲")<<tr("歌手")<<tr("时长");
    ui->tableWidget->setHorizontalHeaderLabels(strList);
    ui->stopButton->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)63)));
    ui->prevButton->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)67)));
    ui->nextButton->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)66)));
    ui->pushButton->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)35)));
    ui->pushButton_2->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)36)));
    ui->sSlider->setMaximum(100);
    ui->sSlider->setMinimum(0);
    ui->sSlider->setValue(50);
    ui->volumelabel->setNum(50);
    ui->musicSlider->setMinimum(0);
    //获取指定目录下的所有.MP3文件名
    dir.setPath("../music/");
    QStringList ImageList;

    ImageList << "*.mp3";

    dir.setNameFilters(ImageList);
    //获取音乐数量
    musicCount = dir.count();
    //将音乐文件名添加到链表中
    for (int i=0;i < musicCount;i++)
    {
        ui->musiclistWidget->addItem(dir[i]);
        playerlist.append("../music/"+dir[i]);
    }
}


void Music::onlinemusic(int musicId)
{
    QString url;
    url=QString("https://music.163.com/song/media/outer/url?id=%0").arg(musicId);
    qDebug()<<url;
    player->setMedia(QUrl(url));
    QMediaContent a= player->media();
    player->play();
    isplay=true;
}

void Music::playmusic()
{
    flag=flag%playerlist.size();
    if(flag<0)
        flag=playerlist.size();
    qDebug()<<flag;
    player->setMedia(QUrl::fromLocalFile(QFileInfo(playerlist[flag]).absoluteFilePath()));
    player->play();
    ui->musiclistWidget->setCurrentRow(flag);
    ui->musiclabel->setText(dir[flag].left(dir[flag].size()-4));
    isplay=true;
}


void Music::on_prevButton_clicked()
{
    flag--;
    playmusic();
}

void Music::on_stopButton_clicked()
{
    if(isplay)
    {
      this->player->pause();
      this->isplay=false;
      ui->stopButton->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)61)));
    }
    else
    {
      this->player->play();
      this->isplay=true;
      ui->stopButton->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)63)));
    }
}

void Music::on_nextButton_clicked()
{
    flag++;
    playmusic();
}

void Music::recv_ack(QNetworkReply *rely)
{
    data=rely->readAll();
    //qDebug()<< data;
    QJsonParseError err;
    QJsonDocument json=QJsonDocument::fromJson(data.toUtf8(),&err);
    if(err.error!=QJsonParseError::NoError)
    {
        qDebug()<<"err"<<err.errorString();
        return;
    }
    int k=0;
    QJsonObject object=json.object();
    QStringList keys=object.keys();
    if(keys.contains("result"))
    {
        QJsonObject result=object["result"].toObject();
        QStringList rkeys=result.keys();
        if(result.contains("songs"))
        {
            QJsonArray arr=result["songs"].toArray();
            for(auto i:arr)
            {
                QJsonObject object_1=i.toObject();
                musicId=object_1["id"].toInt();
                music_duration=object_1["duration"].toInt();
                musicname=object_1["name"].toString();

                QJsonArray artistsArray = object_1["artists"].toArray();
                for(auto j : artistsArray)
                {
                    QJsonObject object_2 = j.toObject();
                    singername = object_2["name"].toString();         // 歌手名
                    imageid = object_2["img1v1Url"].toString();
                }
                //QJsonObject alArray = object_1["alias"].toObject();

                dd.setMSecsSinceEpoch(music_duration);
#if 0
                qDebug()<<"音乐ID："+QString::number(musicId);
                qDebug()<<"音乐名："+musicname;
                qDebug()<<"歌手名："+singername;
                //时间戳计算音乐时长
                qDebug()<<"总时长："+dd.toString("mm:ss");
                qDebug()<<"图片URL："+imageid;
#endif
                ui->tableWidget->setItem(k,0,new QTableWidgetItem(QString::number(k+1)));
                ui->tableWidget->setItem(k,1,new QTableWidgetItem(musicname));
                ui->tableWidget->setItem(k,2,new QTableWidgetItem(singername));
                ui->tableWidget->setItem(k,3,new QTableWidgetItem(dd.toString("mm:ss")));
                ui->tableWidget->setItem(k,4,new QTableWidgetItem(QString::number(musicId)));
                ui->tableWidget->setItem(k,5,new QTableWidgetItem(imageid));
                k++;
                if(k==10)
                    break;
            }
        }
    }

}

void Music::recv_image(QNetworkReply *rely)
{
    QByteArray data = rely->readAll();
    //更新的UI
    QPixmap map;
    map.loadFromData(data);
    ui->imagelabel->setScaledContents(true);
    ui->imagelabel->setPixmap(map);
}

void Music::on_pushButton_clicked()
{
    QNetworkRequest request;
    request.setUrl(QString("http://iwxyi.com:3000/search?keywords=%0").arg(ui->namelineEdit->text()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36");
    musicmanager->get(request);
    ui->tableWidget->show();
}

void Music::on_tableWidget_cellDoubleClicked(int row, int column)
{
    ui->tableWidget->hide();
    //获取id、图片路径
    QTableWidgetItem* item = ui->tableWidget->item(row,4);
    //qDebug() << item->text();
    onlinemusic(item->text().toInt());
    item = ui->tableWidget->item(row,5);
    QNetworkRequest request(item->text());
    imagemanager->get(request);
    //qDebug() << item->text();
    ui->musicSlider->setMaximum(music_duration);
    item = ui->tableWidget->item(row,3);
    ui->nowlabel->setText(item->text());
    item = ui->tableWidget->item(row,1);
    ui->musiclabel->setText(item->text());

}

void Music::on_pushButton_2_clicked()
{
    //判断对象是否被隐藏
    if(ui->tableWidget->isHidden())
    {
         ui->tableWidget->show();
         ui->pushButton_2->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)28)));
    }
    else
    {
       ui->tableWidget->hide();
       ui->pushButton_2->setIcon(QIcon(style()->standardPixmap((QStyle::StandardPixmap)36)));
    }
}

void Music::on_returnButton__clicked()
{
    emit sendData_land(false);
    this->close();
    this->parentWidget()->show();
    if(isplay==true)
        player->stop();
    delete this;
}

void Music::on_backstageButton_clicked()
{
    emit sendData_land(true);
    this->hide();
    this->parentWidget()->show();
}

void Music::on_sSlider_valueChanged(int value)
{
     player->setVolume(value);
     ui->volumelabel->setText(QString::number(value));
}

void Music::on_musiclistWidget_currentRowChanged(int currentRow)
{
    qDebug()<< "lie";
    flag=currentRow;
    playmusic();
}
