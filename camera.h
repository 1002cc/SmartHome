#ifndef CAMERA_H
#define CAMERA_H

#define MULTIMEDIA_CAMERA 1
#define OPENCV_CAMERA 0
#define QT_CAMERA OPENCV_CAMERA

#include <QMainWindow>
#include <QByteArray>
#include <QDebug>
#include <QTimer>
#include <QTime>
#if QT_CAMERA
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#else
//opencv头文件
#include <opencv2/opencv.hpp>

#endif

namespace Ui {
class Camera;
}

class Camera : public QMainWindow
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    ~Camera();
    bool getcamerastate();

    void deletecamera();
private slots:
    void on_pushButtonC_clicked();

    void on_pushButtonL_clicked();

    void on_pushButton_clicked();

    void readFarme();

    void on_pushButtonV_clicked();

    void update();

private:
    Ui::Camera *ui;
#if QT_CAMERA
    QCamera*camera;
    QCameraViewfinder*viewfinder;
    QCameraImageCapture* imageCapture;
#else
    cv::VideoCapture cap;
    cv::VideoWriter vwriter;
    cv::Mat src_image;
    QTimer *timer;
    QImage *image;
    QTimer *Recordtimer;
    QTime Recordtime;
    bool flag=false;
    bool camerains=false;
#endif

};

#endif // CAMERA_H
