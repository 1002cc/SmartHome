#ifndef FACE_H
#define FACE_H

#include <QWidget>
#include <QByteArray>
#include <QDebug>
#include <QTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/freetype.hpp>
namespace Ui {
class Face;
}

class Face : public QWidget
{
    Q_OBJECT

public:
    explicit Face(QWidget *parent = nullptr);
    explicit Face(const QString& name="chen",const int& num=3,QWidget *parent = nullptr);
    explicit Face(const int& num=1,QWidget *parent = nullptr);
    ~Face();
    bool facenew();
    void deleteface();
private slots:
    void on_pushButton_2_clicked();

    void readFarme();

    void trainModel();

    void tryModel();
signals:
    void sendData_face(bool flag=false);
    void sendData_flag(bool flag=false);
private:
    Ui::Face *ui;
    cv::VideoCapture cap;
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;
    cv::Ptr<cv::face::FisherFaceRecognizer> model;
    //cv::Ptr<cv::face::FisherFaceRecognizer> eyemodel;
    //cv::Ptr<cv::face::LBPHFaceRecognizer> lbhpmodel;
    std::string facepath="../SmartHome/image/faceimage/myfacedata.txt";

    std::string eyepath="../opencv/image/myeyedata.txt";

    std::string facexml = "../SmartHome/install/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml";

    std::string eyexml = "../SmartHome/install/data/haarcascades/haarcascade_eye.xml";

    std::string savefacexml = "../SmartHome/install/data/haarcascades/haarcascade_frontalface_alt_tree.xml";

    std::string myfacexml="../SmartHome/install/MyFacePCAModel.xml";

    cv::Mat src_image;
    QTimer *timer;
    QImage *image;
    bool faceins=false;
    cv::Mat img1, imgGray;
    std::vector<cv::Rect> faces;
    std::vector<cv::Rect> eyes;
    int facesute=0;
    bool facesuccessflag=false;
    int flaglabel=-1;
    int labelnum=0;
    QString imagepath;
    int num;
    QString name;
    int count = 0;
    std::ofstream outFile;
    QVector<QString> strvec;
     cv::Ptr<cv::freetype::FreeType2> ft2;
};

#endif // FACE_H
