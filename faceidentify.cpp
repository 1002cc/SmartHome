#include "faceidentify.h"
#include <fstream>

#define OPENCV_EYE 0

//构造函数处理，清空容器
faceidentify::faceidentify()
{
    eyeimages.clear();
    eyelabels.clear();
}
/**
 * @brief faceidentify::saveimage  保存图片，写入scv文件
 * @param facepath      scv文件路基
 * @param outpath       输出文件路径
 * @param facenumber    脸部编号
 * @param imagecount    训练图片数量
 */
void faceidentify::saveimage(const std::string& facepath,const std::string& outpath,const int&facenumber,const int & imagecount)
{
    //打开摄像头，电脑自带的摄像头选择-1或0，外接摄像头选择2；
    cv::VideoCapture capture(0);
    if(!capture.isOpened()){
        std::cout << "camera open failed" << std::endl;
        return;
    }else{
        std::cout << "camera open success!" << std::endl;
    }

    // 加载检测人脸的xml文件
    if(!faceCascade.load(facexml))
    {
        std::cout << "facexml load error!" << std::endl;
        return;
    }else{
        std::cout << "facexml load success!" << std::endl;
    }
#if OPENCV_EYE
    //加载检查眼睛的xml文件
    if(!eyeCascade.load(eyexml))\{
        cout << "eyexml load error!" << endl;
        return;
    }else{
        cout << "eyexml load success!" << endl;
    }
    std::ofstream outeyeFile;
    //打开文件
    outeyeFile.open(eyepath,std::ios::out|std::ios::binary|std::ios::app);
    outeyeFile << "../opencv/image/eye_0.pgm;1" <<endl;
    std::vector<cv::Rect> eyes;
#endif
    //实例化文件流
    std::ofstream outFile;
    //打开文件
    outFile.open(facepath,std::ios::out|std::ios::binary|std::ios::app);

    std::string str;
    std::vector<cv::Rect> faces;

    int count = 0;

    while(capture.read(frame))
    {
        //翻转图像
        cv::flip(frame, frame, 1);
        // 检测人脸
        faceCascade.detectMultiScale(frame,faces,1.1,3,0,cv::Size(150,150),cv::Size(390,390));
#if OPENCV_EYE
        eyeCascade.detectMultiScale(frame,eyes,1.1,3,0,Size(0,0),Size(200,200));
#endif
        // 判断图像人数个数
        if((int)faces.size()==1){
            for(int i = 0;i<(int)faces.size();i++){
                    cv::resize(frame(faces[i]), dst, cv::Size(200,200));
                    cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
                    str=outpath+"myface_"+std::to_string(count)+".pgm";
                    //cout << str << endl;
                    cv::imwrite(str,dst);
                    str +=";"+std::to_string(facenumber);
                    //cout << str << endl;
                    outFile << str <<std::endl;
                //画框
                cv::rectangle(frame, faces[i], cv::Scalar(0,0,255), 2,8);
            }
#if OPENCV_EYE
            for(int i=0;i<(int)eyes.size();i++){
                if(i==0){
                    cv::resize(frame(eyes[0]), dst, Size(50,50));
                    cv::imwrite(format("../opencv/image/eyel_%d.pgm",count),dst);
                    str ="../opencv/image/eyel_"+std::to_string(count)+".png;"+to_string(facenumber+100);
                    outeyeFile << str <<endl;
                }if(i==1){
                    cv::resize(frame(eyes[1]), dst, Size(50,50));
                    cv::imwrite(format("../opencv/image/eyer_%d.pgm",count),dst);
                    str ="../opencv/image/eyer_"+std::to_string(count)+".png;"+to_string(facenumber+100);
                    outeyeFile << str <<endl;
                }
              cv::rectangle(frame,eyes[i].tl(),eyes[i].br(),cv::Scalar(255,255,255),2);
            }
#endif
            count++;
        }
        //显示图像
        imshow("camera-demo", frame);
        //检查数量
        if(count>imagecount)
            break;
        char c = cv::waitKey(10);
        if(c == 27)
        {
            break;
        }
    }
    outFile.close();
#if OPENCV_EYE
    outeyeFile.close();
#endif
    capture.release();
}


void faceidentify::read_csv(const std::string& filename, std::vector<cv::Mat>& images,  std::vector<int>& labels, char separator)
{
    std::ifstream file(filename.c_str(), std::ifstream::in);
    if (!file)
    {
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    std::string line, path, classlabel;
    while (getline(file, line,'\n')) //从文本文件中读取一行字符，未指定限定符默认限定符为“/n”
    {
        std::stringstream liness(line);//这里采用stringstream主要作用是做字符串的分割
        getline(liness, path, separator);//读入图片文件路径以分好作为限定符
        getline(liness, classlabel);//读入图片标签，默认限定符
        if (!path.empty() && !classlabel.empty()) //如果读取成功，则将图片和对应标签压入对应容器中
        {
           cv::Mat img=cv::imread(path, 0);
           if(img.data!=0){
                images.push_back(img);
                labels.push_back(atoi(classlabel.c_str()));
           }
        }
    }
}

int faceidentify::opencvtrain(const std::string& filename,const std::string& outxmlfile)
{
    std::vector<cv::Mat> images;
    std::vector<int> labels;
    images.clear();
    labels.clear();
    //读取人脸csv
    try{
        read_csv(filename,images,labels);; //从csv文件中批量读取训练数据
    }
    catch (cv::Exception& e){
       std::cerr << "Error opening file \"" << filename << "\". Reason: " << e.msg << std::endl;
       return -1;
    }
    //查看容器数量
    std::cout << "labels.size() " << images.size() << "  labels: " << labels.size() <<  std::endl;

    if (images.size() <= 1) {
        std::string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    //查看照片宽高
     std::cout << "height:" << images[0].rows << "    width:" << images[0].cols <<  std::endl;

    testSample = images[images.size()-1];
    int testLable = labels[labels.size()-1];
    images.pop_back();
    labels.pop_back();

    model = cv::face::EigenFaceRecognizer::create();
    model->train(images, labels);
    model->save(outxmlfile);

    int predictedLabel = model->predict(testSample);
    std::cout <<"测试标签值："<< testLable <<",预测标签值："<< predictedLabel <<std::endl;
    if(testLable == predictedLabel){
        return 1;
    }else{
        return 0;
    }
    //读取眼睛csv
#if OPENCV_EYE
    try{
        read_csv(eyepath,eyeimages,eyelabels);; //从csv文件中批量读取训练数据
    }
    catch (cv::Exception& e){
       cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
    }
    cout << "eyeimages.size() " << eyeimages.size() << "  eyelabels : " << eyelabels.size() << endl;
    cout << "height:" << eyeimages[0].rows << "    width:" << eyeimages[0].cols << endl;
    eyeSample = eyeimages[eyelabels.size()-1];
    int eyeLable = eyelabels[eyelabels.size()-1];
    eyeimages.pop_back();
    eyelabels.pop_back();
    cout << "eyeLable  " << eyeLable << endl;
    eyemodel = face::EigenFaceRecognizer::create();
    eyemodel->train(eyeimages,eyelabels);
    eyemodel->save("../opencv/MyeyePCAModel.xml");
    //cout << format("Predicted class = %d / Actual class = %d.",  predictedLabel, testLable) <<endl;
#endif

//显示容器中图像
#if 0
    for(int i=0;i<(int)images.size();i++)
    {
        cout << images[i] << endl;
        imshow("new"+std::to_string(i) ,images[i]);
    }
    waitKey(0);
    for(int i=0;i<(int)labels.size();i++)
    {
        cout << labels[i] << endl;
    }
    waitKey(0);


#endif

#if 0
    Ptr<face::LBPHFaceRecognizer> model2 = face::LBPHFaceRecognizer::create();
    model2->train(images, labels);
    model2->save("/home/chen/SmartHome/SmartHome/MyFaceLBPHModel.xml");
    int predictedLabel2 = model2->predict(testSample);
    cout <<"测试标签值："<< testLable <<",预测标签值："<< predictedLabel2 <<endl;
#endif
    //显示图像，调试时使用
#if 0
    int height = images[0].rows;
    imshow("Mean Face1", testSample);
    int predictedLabel = model->predict(testSample);
    cout <<"测试标签值："<< testLable <<",预测标签值："<< predictedLabel <<endl;

    Mat vals = model->getEigenValues();//
    printf("%d,%d\n", vals.rows, vals.cols);
    Mat vecs = model->getEigenVectors();
    printf("%d,%d\n", vecs.rows, vecs.cols);
    Mat mean = model->getMean();//1*10304
    printf("%d,%d\n", mean.rows, mean.cols);

    //显示平均脸
    string win_title;
    Mat meanFace = mean.reshape(1, height);//第一个参数为通道数，第二个参数为多少行
    normalize(meanFace, meanFace, 0, 255, NORM_MINMAX, CV_8UC1);
    imshow("Mean Face", meanFace);
    //显示前部分特征脸
    for (int i = min(10, vals.rows); i <min(61, vals.rows); i+=10) {
         Mat vecs_space = Mat(vecs, Range::all(), Range(0, i));
         Mat projection = LDA::subspaceProject(vecs_space, mean, images[0].reshape(1, 1));//投影到子空间
         Mat reconstruction = LDA::subspaceReconstruct(vecs_space, mean, projection);//重建
         Mat result = reconstruction.reshape(1, height);
         normalize(result, result, 0, 255, NORM_MINMAX, CV_8UC1);
         win_title="imageshoew"+std::to_string(i);
         imshow(win_title, result);
     }
    //显示进行人脸重建
    for (int i = min(10, vals.rows); i <min(61, vals.rows); i+=10) {
        Mat vecs_space = Mat(vecs, Range::all(), Range(0, i));
        Mat projection = LDA::subspaceProject(vecs_space, mean, images[0].reshape(1, 1));
        Mat reconstruction = LDA::subspaceReconstruct(vecs_space, mean, projection);
        Mat result = reconstruction.reshape(1, height);
        normalize(result, result, 0, 255, NORM_MINMAX, CV_8UC1);
        //char wintitle[40] = {};
        win_title="imageshoewbuild"+std::to_string(i);
        imshow(win_title, result);
    }
    waitKey(0);
#endif

#if 0
    model = face::EigenFaceRecognizer::create();
    cout << "XZCZXCZX" << endl;
    model->save("../opencv/MyFacePCAModel.xml");
    model->train(images, labels);

    Ptr<face::BasicFaceRecognizer> model1 = face::FisherFaceRecognizer::create();
    model1->train(images, labels);
    model1->save("../opencv/MyFaceFisherModel.xml");

    Ptr<face::LBPHFaceRecognizer> model2 = face::LBPHFaceRecognizer::create();
    model2->train(images, labels);
    model2->save("../opencv/MyFaceLBPHModel.xml");

    int predictedLabel = model->predict(testSample);//加载分类器
    int predictedLabel1 = model1->predict(testSample);
    int predictedLabel2 = model2->predict(testSample);


    //cout << "actual label:" << testLable << "predict label:" << predictLabel << endl;
    string result_message  = format("Predicted class = %d / Actual class = %d.",  predictedLabel, testLable);
    string result_message1 = format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLable);
    string result_message2 = format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLable);
    cout << result_message << endl;
    cout << result_message1 << endl;
    cout << result_message2 << endl;
#endif
}


void faceidentify::identify()
{
    // 打开摄像头,开始识别
    cv::VideoCapture cap(-1);
    if(!cap.isOpened()){
         std::cout << "camera open failed!" <<  std::endl;
    }else{
     std::cout << "camera open success!" <<  std::endl;}

    if(!faceCascade.load(facexml)){
         std::cout << "load savefacexml filed!" <<  std::endl;
    }
#if OPENCV_EYE
    if(!eyeCascade.load(eyexml)){
         std::cout << "eyexml load error!" <<  std::endl;
        return;
    }
        std::vector<cv::Rect> eyes;
        int eyelabel=0;
#endif
    std::vector<cv::Rect> faces;
    int label=0;
    std::string text;
    cv::Ptr<cv::face::EigenFaceRecognizer> facemodel = cv::face::EigenFaceRecognizer::create();
    facemodel->read("../SmartHome/install/MyFacePCAModel.xml");
    while(cap.read(frame))
    {
        // 颠倒图像
        flip(frame,frame,1);
        // 设置显示视频中的信息尺寸
        faceCascade.detectMultiScale(frame, faces,1.1,3,0,cv::Size(150,150),cv::Size(390,390));
#if OPENCV_EYE
        eyeCascade.detectMultiScale(frame,eyes,1.1,3,0,cv::Size(0,0),cv::Size(200,200));
#endif
        if(faces.size()==1){
            for(int i = 0; i < (int)faces.size();i++)
            {
                cv::Mat roi = frame(faces[i]);
                // 设置灰度图像，防止颜色不一致
                cvtColor(roi, dst, cv::COLOR_BGR2GRAY);
                // 设置大小，与窗口一致
                resize(dst,testSample,cv::Size(200,200));
                // 保存识别的结果
                label = facemodel->predict(testSample);
                // 使用红色的框框起来
                rectangle(frame, faces[i], cv::Scalar(0,0,255), 3,8,0);
            }
#if 0
            for(int i = 0; i < (int)eyes.size();i++)
            {
                cv::Mat roi = frame(eyes[i]);
                // 设置灰度图像，防止颜色不一致
                cvtColor(roi, dst, COLOR_BGR2GRAY);
                // 设置大小，与窗口一致
                resize(dst,eyeSample,cv::Size(50,50));
                // 保存识别的结果
                eyelabel =eyemodel->predict(eyeSample);
                // 使用红色的框框起来
                rectangle(frame, eyes[i], cv::Scalar(0,0,255), 3,8,0);
            }
#endif
             std::cout << label <<  std::endl;
            if(label == 1)
            {
                text = "chen";
            }else if(label == 2){
                text = "huge";
            }else if(label == 3){
                text = "pyuyan";
            }else{
                text = "no know!";
            }
            label=0;
            putText(frame, text, cv::Point(faces[0].x,faces[0].y-10),cv::FONT_HERSHEY_PLAIN,
                    2.0,cv::Scalar(255,0,0),3);
        }
        imshow("face-recognition", frame);
        if(cv::waitKey(50) == 27)
        {
            break;
        }
   }
}
