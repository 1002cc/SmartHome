#include "camera.h"
#include "ui_camera.h"
#include "photo.h"
#include <QDateTime>
#include <QMessageBox>

Camera::Camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);
    this->setGeometry(this->parentWidget()->x(),this->parentWidget()->y(),this->parentWidget()->width(),this->parentWidget()->height());
    ui->pushButtonV->setIcon(QIcon(":/icon/image/iocn/Records.png"));
    ui->pushButtonV->setIconSize(QSize(64,64));
#if QT_CAMERA
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    //使用遍历的方便，将容器中的摄像头都一个个读取出来
    QByteArray cameradata;
    for (const QCameraInfo &cameraInfo : cameras) {
        //将读到的数据存放到字节数组中，只适用一个摄像头的情况，默认程序运行开启摄像头
        cameradata=cameraInfo.deviceName().toUtf8();
    }
    //创建一个摄像头的类
    camera = new QCamera(cameradata);

    //2.再QCameraViewfinder类来进行一个实时画面显示
    viewfinder = new QCameraViewfinder(ui->widgetView); //如果没有给这个取景器设置一个父控件，那么，他就是一个独立窗口界面

    //设置显示的大小跟父控件一样大
    viewfinder->resize(ui->widgetView->width(),ui->widgetView->height());
    viewfinder->show();
     //给摄像头设置一个取景器
     camera->setViewfinder(viewfinder);

     //3.开启摄像头
     camera->start();
#else
    if(cap.open(-1)){
        Recordtime.setHMS(0,0,0,0);
        Recordtimer=new QTimer();
         ui->timelabel->hide();
        connect(Recordtimer, SIGNAL(timeout()), this, SLOT(update()));
        timer = new QTimer(this);
        image = new QImage();
        connect(timer,SIGNAL(timeout()),this,SLOT(readFarme()));
        timer->start(33);
        camerains=true;
    }else{
        QMessageBox::information(this,"摄像机","摄像机打开失败");
        camerains=false;
        qDebug()<< "打开失败";
    }
#endif

}

Camera::~Camera()
{
    delete ui;
    delete timer;
    delete image;
    delete Recordtimer;
    cap.release();
    timer->stop();
}

bool Camera::getcamerastate()
{
    return camerains;
}

void Camera::deletecamera()
{
    timer->stop();
    cap.release();
}

void Camera::on_pushButtonC_clicked()
{
#if QT_CAMERA
    imageCapture = new QCameraImageCapture(camera);

    //建立void imageCaptured(int id, const QImage &preview)的槽连接
    //使用匿名函数，拼接照片名
    connect(imageCapture,&QCameraImageCapture::imageCaptured,this,[=](int id, const QImage &preview){
        QString str="../SmartHome/image/"+QString::number(id)+".jpg";
        preview.save(str);
    });

    camera->setCaptureMode(QCamera::CaptureStillImage);

    camera->searchAndLock();

     //on shutter button pressed
     imageCapture->capture(); //当进行拍照时，它会发射一个信号imageCaptured() ，所以，要保存图片，就可以建立该信号的槽

     //on shutter button released
     camera->unlock();
#else
    if(ui->timelabel->isHidden()!=true)
    {
        ui->timelabel->hide();
    }
    cv::Mat src2;
    cvtColor(src_image, src2, CV_BGR2RGB);
    QImage Qtemp1= QImage((const unsigned char*)(src2.data), src2.cols, src2.rows, src2.step, QImage::Format_RGB888);
    QDateTime datatime = QDateTime::currentDateTime();
    QString imagename = "../SmartHome/image/opencv"+datatime.toString("yyyyMMddhhmmss")+".jpg";
    qDebug()<<imagename;
    Qtemp1.save(imagename);
#endif

}

void Camera::on_pushButtonL_clicked()
{
    Photo*p=new Photo(this);
    p->show();
    this->hide();
}

void Camera::on_pushButton_clicked()
{
    this->parentWidget()->show();
    this->close();
        delete this;
}

void Camera::readFarme()
{
#if !QT_CAMERA
    cap.read(src_image);
    cv::Mat src2;
    cvtColor(src_image, src2, CV_BGR2RGB);
    QImage Qtemp1= QImage((const unsigned char*)(src2.data), src2.cols, src2.rows, src2.step, QImage::Format_RGB888);
    if(flag){
            vwriter<<src_image;
            cv::waitKey((int)cap.get(cv::CAP_PROP_FPS));
    }
    ui->cameralabel->setPixmap(QPixmap::fromImage(Qtemp1));
    ui->cameralabel->setScaledContents(true);
    ui->cameralabel->show();
#endif
}

void Camera::on_pushButtonV_clicked()
{
    if(flag){
        flag=false;
        ui->pushButtonV->setIcon(QIcon(":/icon/image/iocn/Records.png"));
        ui->pushButtonV->setIconSize(QSize(64,64));
        Recordtimer->stop();
    }
    else
    {
        flag=true;
        ui->timelabel->show();
        Recordtimer->start(1000);
        Recordtime.setHMS(0,0,0,0);
        ui->timelabel->setText(Recordtime.toString("mm:ss"));
        ui->pushButtonV->setIcon(QIcon(":/icon/image/iocn/Recording.png"));
        ui->pushButtonV->setIconSize(QSize(32,32));
        QDateTime datatime = QDateTime::currentDateTime();
        QString videoname="../SmartHome/image/"+datatime.toString("yyyyMMddhhmmss")+".avi";
        cv::Size S = cv::Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
                                 (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
        vwriter.open(videoname.toStdString(), //视频文件名
            cv::VideoWriter::fourcc('X','V','I','D'),//编码类型
            cap.get(cv::CAP_PROP_FPS), //帧率
            S,//尺寸大小
            true);//彩色格式为true，灰度为false
        if (!vwriter.isOpened()){
                qDebug() << "fail to open!";
        }
    }
}

void Camera::update()
{
    static quint32 time_out=0;
    time_out++;
    Recordtime=Recordtime.addSecs(1);
    ui->timelabel->setText(Recordtime.toString("mm:ss"));
}
