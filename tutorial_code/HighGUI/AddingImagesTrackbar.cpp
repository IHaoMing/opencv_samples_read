/**
 * @file LinearBlend.cpp
 * @brief Simple linear blender ( dst = alpha*src1 + beta*src2 )
 * @author OpenCV team
 */

/**
 * 图像线性混合
 * 简单的线性混合公式( dst = alpha*src1 + beta*src2 )
 */

//头文件
#include "opencv2/imgcodecs.hpp"        //文件输入输出相关
#include "opencv2/highgui.hpp"          //GUI相关
#include <stdio.h>

//命名空间
using namespace cv;

/** Global Variables */
//全局变量
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

/** Matrices to store images */
//储存图像的全局Mat变量
Mat src1;
Mat src2;
Mat dst;

//![on_trackbar]
/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
//滑动条的回掉函数声明原型必须为 void function_name(int, void*)
static void on_trackbar( int, void* )
{
    //由滑动条刻度值计算alpha系数
   alpha = (double) alpha_slider/alpha_slider_max ;

    //计算beta
   beta = ( 1.0 - alpha );

    //线性混合，gamma参数取0.0
   addWeighted( src1, alpha, src2, beta, 0.0, dst);

   imshow( "Linear Blend", dst );
}
//![on_trackbar]

/**
 * @function main
 * @brief Main function
 */
int main( void )
{
   //![load]
   /// Read images ( both have to be of the same size and type )
   //加载图像
   src1 = imread("../data/LinuxLogo.jpg");
   src2 = imread("../data/WindowsLogo.jpg");
   //![load]

   //检查图像是否加载成功
   if( src1.empty() ) { printf("Error loading src1 \n"); return -1; }
   if( src2.empty() ) { printf("Error loading src2 \n"); return -1; }

   /// Initialize values
   alpha_slider = 0;    //初始化滑动条的值

   //![window]
   //创建窗口，WINDOW_AUTOSIZE类型
   namedWindow("Linear Blend", WINDOW_AUTOSIZE); // Create Window
   //![window]

   //![create_trackbar]
   char TrackbarName[50];
   sprintf( TrackbarName, "Alpha x %d", alpha_slider_max );         //格式化滑动条名称
   createTrackbar( TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar );    //创建滑动条
   //![create_trackbar]

   /// Show some stuff
   on_trackbar( alpha_slider, 0 );  //调用显示

   /// Wait until user press some key
   waitKey(0);
   return 0;
}

/**
 * 要点总结：
 * 线性混合公式 ( dst = alpha*src1 + beta*src2 ) 或者 ( dst = alpha*src1 + beta*src2 + gamma )
 * 滑动条回掉函数原型 void function_name(int, void*)
 * 创建滑动条createTrackbar()
 * 格式化字符串sprintf(), opencv3转为sprintf_s();安全版本sprintf_s()
 */