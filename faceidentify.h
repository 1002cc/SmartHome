#ifndef FACEIDENTIFY_H
#define FACEIDENTIFY_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>

class faceidentify
{
public:
    faceidentify();

    void saveimage(const std::string& facepath,const std::string& outpath,const int &facenumber=1,const int & imagecount=10);

    void read_csv(const std::string& filename,  std::vector<cv::Mat>& images,  std::vector<int>& labels, char separator = ';');

    int opencvtrain(const std::string& filename,const std::string& outxmlfile="../SmartHome/install/MyFacePCAModel.xml");

    void identify();
private:
    // 级联器
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;
    cv::Ptr<cv::face::EigenFaceRecognizer> model;
    cv::Ptr<cv::face::EigenFaceRecognizer> eyemodel;
    std::vector<cv::Mat> eyeimages;
    std::vector<int> eyelabels;
    cv::Mat frame;      //获取摄像头图像信息
    cv::Mat dst;        //处理图片大小
    cv::Mat testSample;
    cv::Mat eyeSample;

    //std::string eyepath="../opencv/image/myeyedata.txt";

    std::string facexml = "../SmartHome/install/data/haarcascades/haarcascade_frontalface_alt2.xml";

    std::string eyexml = "../SmartHome/install/data/haarcascades/haarcascade_eye.xml";

    std::string savefacexml = "../SmartHome/install/data/haarcascades/haarcascade_frontalface_alt_tree.xml";

};

#endif // FACEIDENTIFY_H
