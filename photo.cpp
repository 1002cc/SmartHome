#include "photo.h"
#include "ui_photo.h"

Photo::Photo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Photo)
{
    ui->setupUi(this);
    this->setGeometry(this->parentWidget()->x(),this->parentWidget()->y(),this->parentWidget()->width(),this->parentWidget()->height());
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(ui->videowidget);
    videoWidget->resize(ui->videowidget->width(),ui->videowidget->height());
    flag=0;
    dir.setPath("../SmartHome/image/");
    QStringList ImageList;

    ImageList << "*.bmp" << "*.jpg" << "*.png" <<"*.mp4"<<"*.avi";

    dir.setNameFilters(ImageList);

    ImageCount = dir.count();

    for (int i=0;i < ImageCount;i++)
    {
        strlist.append(dir[i]);
    }
    std::sort(strlist.begin(),strlist.end());
    Modeldill = new QStringListModel(strlist);
    ui->listViewImage->setModel(Modeldill);
    ui->listViewImage->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Photo::~Photo()
{
    delete ui;
}

void Photo::showimage(const int &index)
{
    if(dir[index].contains(".jpg")||dir[index].contains(".png")||dir[index].contains(".bmp")){
        player->pause();
        ui->imagelabel->setPixmap(QPixmap("../SmartHome/image/"+dir[index]));
        ui->imagelabel->show();
    }else{
    player->setVideoOutput(videoWidget);	// 指定视频输出窗口(一个播放器)
    player->setMedia(QUrl::fromLocalFile(QFileInfo("../SmartHome/image/"+dir[index]).absoluteFilePath()));
    player->play();}
}


void Photo::on_listViewImage_clicked(const QModelIndex &index)
{  
    flag=index.row();
    showimage(flag);
}

void Photo::on_pushButtonnext_clicked()
{
    flag-=1;
    if(flag<0)
        flag=ImageCount-1;
    showimage(flag);
}

void Photo::on_pushButtonprev_clicked()
{
    flag+=1;
    if(flag>=ImageCount)
        flag=0;
     showimage(flag);
}

void Photo::on_returnButton_clicked()
{
    this->parentWidget()->show();
    this->close();
}
