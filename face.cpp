#include "face.h"
#include "ui_face.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include "control.h"
#include "faceidentify.h"
#include "sql.h"

#define FACE_IDENTIFY_TIMEOUT 30

Face::Face(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Face)
{
    ui->setupUi(this);
    if(cap.open(CAMERA_FLAG))
    {
        connect(this,SIGNAL(sendData_face(bool)),this->parentWidget(),SLOT(reviecefacedata(bool)));
        //使用opencv打开摄像头
        timer = new QTimer(this);
        image = new QImage();
        connect(timer,SIGNAL(timeout()),this,SLOT(readFarme()));
        timer->start(33);
        faceins=true;
        qDebug()<< "打开成功";
        faceCascade.load(facexml);
        eyeCascade.load(eyexml);
        if (faceCascade.empty()){
            qDebug()<< "can't load classifier\n";
        }
        if (eyeCascade.empty()){
            qDebug()<< "can't load eyeCascade\n";
        }
        model = cv::face::FisherFaceRecognizer::create();
        model->read(myfacexml);
        //eyemodel= cv::face::FisherFaceRecognizer::create();
        //eyemodel->read("../SmartHome/MyeyePCAModel.xml");
//        ft2 = cv::freetype::createFreeType2();
//        //下面的字库要自己下载并拷贝到需要的位置
//        ft2->loadFontData( "../install/fangping.ttf", 0 );
    }else{
        QMessageBox::information(this,"摄像机","摄像机打开失败");
        faceins=false;
        qDebug()<< "打开失败";
    }

#if 0
    faceins=true;
    cv::Mat img=cv::imread("../SmartHome/image/p.png");
    if (img.empty()) {
             qDebug()<<"打开失败";
        }
    cvtColor(img, img, CV_RGB2GRAY);
    //faceCascade.load("../SmartHome/install/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml");
    faceCascade.load("../SmartHome/data/haarcascades_cuda/haarcascade_frontalface_alt.xml");
    if(faceCascade.empty()){
        qDebug()<< "打开haarcascade_frontalface_default失败";
    }
    vector<cv::Rect>faces;

    faceCascade.detectMultiScale(img,faces, 1.2, 6, 0, cv::Size(0, 0));
    qDebug()<<(int)faces.size();
    for(int i=0;i<(int)faces.size();i++)
    {
      cv::rectangle(img,faces[i].tl(),faces[i].br(),cv::Scalar(255,255,255),5);
    }
    cv::imshow("image",img);
    cv::waitKey(0);
#endif
}

Face::Face(const QString &name, const int &num, QWidget *parent):
    QWidget(parent),
    ui(new Ui::Face)
{
    ui->setupUi(this);
    if (!cap.open(CAMERA_FLAG)) {
        QMessageBox::information(this,"摄像机","摄像机打开失败");
        faceins=false;
        qDebug("打开失败");
        this->close();
        return ;
    }
    qDebug("打开成功");
    this->num = num;
    this->name = name;
    imagepath="../image/faceimage/"+name;
    if(!QDir(imagepath).exists()){
        QDir().mkdir(imagepath);
    }
    image = new QImage();
    timer = new QTimer(this);
    timer->start(FACE_IDENTIFY_TIMEOUT);
    connect(timer,SIGNAL(timeout()),this,SLOT(trainModel()));
    connect(this,SIGNAL(sendData_flag(bool)),this->parentWidget(),SLOT(reviecefacedata(bool)));

    faceins=true;

    faceCascade.load(facexml);
    if (faceCascade.empty()) {
        qDebug("can't load classifier");
    }
    eyeCascade.load(eyexml);
    if (eyeCascade.empty()) {
        qDebug("can't load eyeCascade");
    }
    outFile.open(facepath,std::ios::out|std::ios::binary|std::ios::app);
//        ft2 = cv::freetype::createFreeType2();
//        //下面的字库要自己下载并拷贝到需要的位置
//        ft2->loadFontData( "../SmartHome/install/fangping.ttf", 0 );
}

Face::Face(const int &num, QWidget *parent):
    QWidget(parent),
    ui(new Ui::Face)
{
    ui->setupUi(this);
    if(cap.open(CAMERA_FLAG)&&num==1)
    {
        timer = new QTimer(this);
        image = new QImage();
        connect(timer,SIGNAL(timeout()),this,SLOT(tryModel()));
        faceins=true;
        faceCascade.load(facexml);
        eyeCascade.load(eyexml);
        if (faceCascade.empty()){
            qDebug()<< "can't load classifier\n";
        }
        if (eyeCascade.empty()){
            qDebug()<< "can't load eyeCascade\n";
        }
        //将sqlite数据取出放到数组里
        QString sql=QString("select * from facedata");
        QSqlQuery query(sql);/*创建执行语句对象*/
        query.first();
        do
        {
            strvec.push_back(query.value(1).toString());
            qDebug()<< query.value(1).toString();
        }while(query.next());
//        ft2 = cv::freetype::createFreeType2();
//        //下面的字库要自己下载并拷贝到需要的位置
//        ft2->loadFontData( "../SmartHome/install/fangping.ttf", 0 );
        model = cv::face::FisherFaceRecognizer::create();
        model->read(myfacexml);
        timer->start(FACE_IDENTIFY_TIMEOUT);
    }else{
        QMessageBox::information(this,"摄像机","摄像机打开失败");
        faceins=false;
        this->close();
        qDebug()<< "打开失败";
    }
}

Face::~Face()
{
    delete timer;
    delete ui;
}

bool Face::facenew()
{
    return faceins;
}

void Face::deleteface()
{
    if (cap.isOpened()) {
        if (timer->timerType()) {
            timer->stop();
        }
        cap.release();
    }
}

void Face::on_pushButton_2_clicked()
{
    this->close();
    this->deleteface();
    //delete this;
}

void Face::readFarme()
{
    cap.read(src_image);
    flip(src_image, src_image, 1);
    if (src_image.channels() == 3) {
        cvtColor(src_image, imgGray, CV_RGB2GRAY);
    } else {
        imgGray = src_image;
    }
    faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, cv::Size(0, 0));
    eyeCascade.detectMultiScale(imgGray, eyes, 1.2, 6, 0, cv::Size(0, 0));
    //qDebug() << "脸部数量" <<faces.size();
    //qDebug() << "眼睛数量" << eyes.size();
    cv::Mat dst,testSample;
    //cv::Mat dsteye,eyeSample;
    QString str;
    std::string text;
    if (faces.size() > 0) {
        for(int i=0;i<(int)faces.size();i++) {
            cv::Mat src=src_image(faces[i]);
            // 保存识别的结果
            // 设置灰度图像，防止颜色不一致
            cvtColor(src, dst, cv::COLOR_BGR2GRAY);
            // 设置大小，与窗口一致
            cv::resize(dst,testSample,cv::Size(200,200));
            // 保存识别的结果
            int label = model->predict(testSample);
            //qDebug() <<label;
            if(flaglabel==-1){
                flaglabel=label;
            }
            labelnum++;
            if(labelnum>=5&&flaglabel==label){
                labelnum=0;
                flaglabel=-1;
                QString sql=QString("select * from facedata where number='%1'")
                        .arg(label);
                /*创建执行语句对象*/
                QSqlQuery query(sql);
                /*判断执行结果*/
                if(!query.next()){
                    QMessageBox::information(this,"登录认证","登录失败,账户或者密码错误");
                    this->close();
                    emit sendData_face(false);
                }else{
                    str=query.value(1).toString();
                    QMessageBox::information(this,"登录认证","登录成功,欢迎你! "+str);
                    this->close();
                    emit sendData_face(true);
                }
            }
            label=0;
            //cv::Size textSize = ft2->getTextSize(text,fontHeight,thickness,&baseline);
            //putText(src_image,text, cv::Point(faces[0].x,faces[0].y-10),cv::FONT_HERSHEY_PLAIN,2.0,cv::Scalar(255,255,255),2);
            if(labelnum==1){
                text="正在检测.";
            }else if(labelnum==2){
                text="正在检测..";
            }else if(labelnum==3){
                text="正在检测...";
            }else if(labelnum==4){
                text="正在检测....";
            }else if(labelnum==5){
                text="正在检测.....";
            }else{
                text="正在检测";
            }
//            ft2->putText(src_image, text, cv::Point(faces[0].x,faces[0].y-10), 30,
//                         cv::Scalar(255,255,255), 2, 8, true );
//            //putText(src_image, "Detecting", cv::Point(faces[0].x,faces[0].y-10),cv::FONT_HERSHEY_PLAIN,2.0,cv::Scalar(255,255,255),2);
            cv::rectangle(src_image,faces[i].tl(),faces[i].br(),cv::Scalar(255,255,255),2);
        }
        for(int i=0;i<(int)eyes.size();i++){
          //cv::Mat src=src_image(faces[i]);
          //cvtColor(src, dsteye, COLOR_BGR2GRAY);
          //cv::resize(dsteye,eyeSample,cv::Size(50,50));
          //int label = eyemodel->predict(eyeSample);
          //qDebug() <<label;
          //cv::Mat src=src_image(eyes[i]);
          //cv::imwrite("../SmartHome/image/faceimage/eye"+to_string(facesute)+".png",src);
          cv::rectangle(src_image,eyes[i].tl(),eyes[i].br(),cv::Scalar(255,255,255),2);
        }

    }
    cv::Mat src2;
    cv::cvtColor(src_image, src2, CV_BGR2RGB);
    QImage Qtemp1= QImage((const unsigned char*)(src2.data), src2.cols, src2.rows, src2.step, QImage::Format_RGB888);
    ui->facelabel->setPixmap(QPixmap::fromImage(Qtemp1));
    ui->facelabel->setScaledContents(true);
    ui->facelabel->show();
}

void Face::trainModel()
{
    cap.read(src_image);
    flip(src_image, src_image, 1);
    if (src_image.channels() == 3){
        cvtColor(src_image, imgGray, CV_RGB2GRAY);
    }
    else{
        imgGray = src_image;
    }
    faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, cv::Size(0, 0));
    eyeCascade.detectMultiScale(imgGray, eyes, 1.2, 6, 0, cv::Size(0, 0));
    std::string str;
    cv::Mat dst;
    //打开文件
    if((int)faces.size()==1){
        for(int i = 0;i<(int)faces.size();i++){
            cv::resize(src_image(faces[i]), dst, cv::Size(200,200));
            cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
            str=imagepath.toStdString()+"/myface_"+std::to_string(count)+".pgm";
            cv::imwrite(str,dst);
            str +=";"+std::to_string(num);
            outFile << str <<std::endl;
            //画框
            cv::rectangle(src_image, faces[i], cv::Scalar(0,0,255), 2,8);
            count++;
        }
    }
    cv::Mat src2;
    cv::cvtColor(src_image, src2, CV_BGR2RGB);
    QImage Qtemp1= QImage((const unsigned char*)(src2.data), src2.cols, src2.rows, src2.step, QImage::Format_RGB888);
    ui->facelabel->setPixmap(QPixmap::fromImage(Qtemp1));
    ui->facelabel->setScaledContents(true);
    ui->facelabel->show();
    if(count>20){
        timer->stop();
        faceidentify f;
        if(f.opencvtrain(facepath)==1){
            QString sql=QString("insert into facedata(username,number) values('%1','%2');")
                    .arg(name).arg(num);
            /*创建执行语句对象*/
            QSqlQuery query;
            /*判断执行结果*/
            if(!query.exec(sql)){
                //qDebug()<<"insert into error";
                QMessageBox::information(this,"注册认证","注册失败！");
                emit sendData_flag(false);
            }else{
                //qDebug()<<"insert into success";
                QMessageBox::information(this,"注册认证","注册成功！");
                emit sendData_flag(true);
            }
            this->deleteface();
            this->close();
        }
    }
    char c = cv::waitKey(10);
    if(c == 27)
    {
        timer->stop();
        this->deleteface();
        this->close();
    }
}

void Face::tryModel()
{
    cap.read(src_image);
    flip(src_image, src_image, 1);
    if (src_image.channels() == 3){
        cvtColor(src_image, imgGray, CV_RGB2GRAY);
    }
    else{
        imgGray = src_image;
    }
    faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, cv::Size(0, 0));
    eyeCascade.detectMultiScale(imgGray, eyes, 1.2, 6, 0, cv::Size(0, 0));
    cv::Mat dst,testSample;
    std::string text;
    if (faces.size() > 0){
        for(int i=0;i<(int)faces.size();i++){
            cv::Mat src=src_image(faces[i]);
            // 保存识别的结果
            // 设置灰度图像，防止颜色不一致
            cvtColor(src, dst, cv::COLOR_BGR2GRAY);
            // 设置大小，与窗口一致
            cv::resize(dst,testSample,cv::Size(200,200));
            // 保存识别的结果
            int label = model->predict(testSample);
            std::cout << label << std::endl;
            if(label-1>=strvec.size()){
                text="未识别";
            }else{
               text=strvec[label-1].toStdString()+std::to_string(label);
               std::cout << text << std::endl;
            }
            //cv::Size textSize = ft2->getTextSize(text,fontHeight,thickness,&baseline);
            //putText(src_image,text, cv::Point(faces[0].x,faces[0].y-10),cv::FONT_HERSHEY_PLAIN,2.0,cv::Scalar(255,255,255),2);
//            ft2->putText(src_image, text, cv::Point(faces[0].x,faces[0].y-10), 30,
//                         cv::Scalar(255,255,255), 2, 8, true );
            cv::rectangle(src_image,faces[i].tl(),faces[i].br(),cv::Scalar(255,255,255),2);
        }
        for(int i=0;i<(int)eyes.size();i++){
          cv::rectangle(src_image,eyes[i].tl(),eyes[i].br(),cv::Scalar(255,255,255),2);
        }
    }
    cv::waitKey(10);
    cv::Mat src2;
    cv::cvtColor(src_image, src2, CV_BGR2RGB);
    QImage Qtemp1= QImage((const unsigned char*)(src2.data), src2.cols, src2.rows, src2.step, QImage::Format_RGB888);
    ui->facelabel->setPixmap(QPixmap::fromImage(Qtemp1));
    ui->facelabel->setScaledContents(true);
    ui->facelabel->show();
}































#if 0
    imageCapture = new QCameraImageCapture(camera);

    //建立void imageCaptured(int id, const QImage &preview)的槽连接
    connect(imageCapture,&QCameraImageCapture::imageCaptured,this,[=](int id, const QImage &preview){
       /* */QByteArray array ;
        QBuffer buff(&array);
        preview.save(&buff,"png");
        QByteArray img = array.toBase64();
        //QByteArray img = Image::imageToBase64("../image/1.jpg");
        QByteArray imgData = "image=" + img; //body

        //获取access_token
        QByteArray replyData; //保存回复信息
        QString url = QString(baiduTokenUrl).arg(client_id).arg(secret_id);

        QMap<QString,QString> header; //封装头部信息
        header.insert(QString("Content-Type"),QString("application/x-www-form-urlencoded"));


        bool result = Http::post_syns(url,header,imgData,replyData);
        if(result)
        {
            QJsonObject obj = QJsonDocument::fromJson(replyData).object();

            accessToken = obj.value("access_token").toString();
        }


        replyData.clear();
        QString imgurl = baiduImageUrl.arg(accessToken);
        result = Http::post_syns(imgurl,header,imgData,replyData);
        if(result)
        {
            qDebug()<<"replyData:"<<replyData;
        }
#endif
        // 设置APPID/AK/SK

#if 0
        std::string app_id = "26639298";
        std::string api_key = "X1fRXg8vz0M6ORVHOYlYos1d";
        std::string secret_key = "jzz9LEAiFHZ9sFOglAfo5NIsI27DAt4P";

        //调用人脸检测
        Json::Value result;
        //创建一个请求的对象
         aip::Face client(app_id, api_key, secret_key);

        QByteArray array ;
        QBuffer buff(&array);
        preview.save(&buff,"png");
        std::string image = array.toBase64().toStdString();
        std::string image_type = "BASE64";
        buff.close();
        // 调用人脸检测
        result = client.detect(image, image_type, aip::null);

        // 如果有可选参数
        std::map<std::string, std::string> options;
        options["face_field"] = "age";
            options["max_face_num"] = "2";
        options["face_type"] = "LIVE";
        options["liveness_control"] = "LOW";

        // 带参数调用人脸检测
        result = client.detect(image, image_type, options);

        //输出结果
        //qDebug << result.toStyledString();

        //输出结果
        QString msg;
        msg = msg.fromStdString(result.toStyledString());
        qDebug() << msg;

    });

    camera->setCaptureMode(QCamera::CaptureStillImage);

    camera->searchAndLock();

     //on shutter button pressed
     imageCapture->capture(); //当进行拍照时，它会发射一个信号imageCaptured() ，所以，要保存图片，就可以建立该信号的槽

     //on shutter button released
     camera->unlock();

#endif
