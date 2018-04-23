//彩色图像的阈值操作，inRange()，在目标检测上面的应用

//头文件
#include "opencv2/imgproc.hpp" //图像处理相关
#include "opencv2/highgui.hpp" //GUI相关

#include <iostream>
#include <stdlib.h>

//命名空间
using namespace std;
using namespace cv;

/** Function Headers */
//回掉函数声明，格式为void function_name(int, void*)
void on_low_r_thresh_trackbar(int, void *);
void on_high_r_thresh_trackbar(int, void *);
void on_low_g_thresh_trackbar(int, void *);
void on_high_g_thresh_trackbar(int, void *);
void on_low_b_thresh_trackbar(int, void *);
void on_high_b_thresh_trackbar(int, void *);

/** Global Variables */
//全局变量，三个通道的六个不同阈值
int low_r=30, low_g=30, low_b=30;
int high_r=100, high_g=100, high_b=100;

/** @function main */
//主函数
int main()
{
    //! [mat]
    //图像变量声明
    Mat frame, frame_threshold;
    //! [mat]
    //! [cap]
    //打开摄像头， 0默认电脑设备的摄像头
    VideoCapture cap(0);
    //! [cap]
    //! [window]
    //创建窗口
    namedWindow("Video Capture", WINDOW_NORMAL);
    namedWindow("Object Detection", WINDOW_NORMAL);
    //! [window]
    //! [trackbar]
    //-- Trackbars to set thresholds for RGB values
    //创建滑动条，六个滑动条，控制三个B、G、R通道的两个阈值
    createTrackbar("Low R","Object Detection", &low_r, 255, on_low_r_thresh_trackbar);
    createTrackbar("High R","Object Detection", &high_r, 255, on_high_r_thresh_trackbar);
    createTrackbar("Low G","Object Detection", &low_g, 255, on_low_g_thresh_trackbar);
    createTrackbar("High G","Object Detection", &high_g, 255, on_high_g_thresh_trackbar);
    createTrackbar("Low B","Object Detection", &low_b, 255, on_low_b_thresh_trackbar);
    createTrackbar("High B","Object Detection", &high_b, 255, on_high_b_thresh_trackbar);
    //! [trackbar]
    while((char)waitKey(1)!='q'){ //按下q退出
        //! [while]
        cap>>frame; //从cap对象获取一帧图像
        if(frame.empty())
            break;
        //-- Detect the object based on RGB Range Values
        // 基于RGB颜色空间的目标检测
        inRange(frame,Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),frame_threshold);
        /*
        //检查数组元素是否位于两个其他数组的元素之间
        void cv::inRange	(	InputArray 	src,//输入原图
            InputArray 	lowerb,//阈值上限
            InputArray 	upperb,//阈值上限
            OutputArray 	dst //输出图像， CV_8U类型的二值图
            )		
        */
        //! [while]
        //! [show]
        //-- Show the frames
        imshow("Video Capture",frame);  //显示原图
        imshow("Object Detection",frame_threshold);//显示检测结果
        //! [show]
    }
    return 0;
}

//六个滑动条的回掉函数，分别控制三个通道的六个阈值
//! [low]
/** @function on_low_r_thresh_trackbar */
void on_low_r_thresh_trackbar(int, void *)
{
    low_r = min(high_r-1, low_r); //区分确定low、high
    setTrackbarPos("Low R","Object Detection", low_r);
}
//! [low]
//! [high]
/** @function on_high_r_thresh_trackbar */
void on_high_r_thresh_trackbar(int, void *)
{
    high_r = max(high_r, low_r+1);
    setTrackbarPos("High R", "Object Detection", high_r);
}
//![high]
/** @function on_low_g_thresh_trackbar */
void on_low_g_thresh_trackbar(int, void *)
{
    low_g = min(high_g-1, low_g);
    setTrackbarPos("Low G","Object Detection", low_g);
}

/** @function on_high_g_thresh_trackbar */
void on_high_g_thresh_trackbar(int, void *)
{
    high_g = max(high_g, low_g+1);
    setTrackbarPos("High G", "Object Detection", high_g);
}

/** @function on_low_b_thresh_trackbar */
void on_low_b_thresh_trackbar(int, void *)
{
    low_b= min(high_b-1, low_b);
    setTrackbarPos("Low B","Object Detection", low_b);
}

/** @function on_high_b_thresh_trackbar */
void on_high_b_thresh_trackbar(int, void *)
{
    high_b = max(high_b, low_b+1);
    setTrackbarPos("High B", "Object Detection", high_b);
}


/**
 * 要点总结
 * inRange函数
 *    void cv::inRange	(	InputArray 	src,//输入原图
            InputArray 	lowerb,//阈值上限
            InputArray 	upperb,//阈值上限
            OutputArray 	dst //输出图像， CV_8U类型的二值图
            )		
  * 
  * 彩色阈值操作
  * inRange函数：作用检查数组元素是否位于两个其他数组的元素之间， 对应输出二值图里面的白色区域
*/