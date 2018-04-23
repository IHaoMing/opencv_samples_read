/**
 * @file LinearTransforms.cpp
 * @brief Simple program to change contrast and brightness
 * @date Mon, June 6, 2011
 * @author OpenCV team
 */

/**
 * 线性变换
 * 改变对比度和亮度的简单示例程序
*/

/**
 * 程序流程
 * 1、加载图像，默认路径或者命令行参数输入
 * 2、初始化参数和创建窗口
 * 3、创建滑动条
 * 4、调用显示
*/

//头文件
#include "opencv2/imgcodecs.hpp"        //图像文件读取和写入相关
#include "opencv2/highgui.hpp"          //GUI相关

//命名空间
using namespace cv;

//全局变量
/** Global Variables */
const int alpha_max = 5;
const int beta_max = 125;
int alpha; /**< Simple contrast control */ //控制对比度的参数，称增益参数
int beta;  /**< Simple brightness control*///控制亮度的参数， 称偏置参数 

/** Matrices to store images */
// 储存图像变量
Mat image;

/**
 * @function on_trackbar
 * @brief Called whenever any of alpha or beta changes
 */
//滑动条回掉函数， 偏置和增益参数变化是调用
static void on_trackbar( int, void* )
{
    Mat new_image = Mat::zeros( image.size(), image.type() );

    for( int y = 0; y < image.rows; y++ )
        for( int x = 0; x < image.cols; x++ )
            for( int c = 0; c < 3; c++ )
                //线性变换公式 dst = alpha*src + beta
                new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );

    imshow("New Image", new_image);
}


/**
 * @function main
 * @brief Main function
 */
//主函数 
int main( int argc, char** argv )
{
   /// Read image given by user
   String imageName("../data/lena.jpg"); // by default，图像默认途径
   if (argc > 1)
   {    
    //用户输入的图像路径
      imageName = argv[1];
   }
   image = imread( imageName );

   /// Initialize values
   //初始化两个参数
   alpha = 1;
   beta = 0;

   /// Create Windows
   /// 创建窗口
   namedWindow("Original Image", 1);
   namedWindow("New Image", 1);

   /// Create Trackbars
   /// 创建滑动条
   createTrackbar( "Contrast", "New Image", &alpha, alpha_max, on_trackbar );
   createTrackbar( "Brightness", "New Image", &beta, beta_max, on_trackbar );

   /// Show some stuff
   /// 调用显示
   imshow("Original Image", image);
   imshow("New Image", image);

   /// Wait until user press some key
   waitKey();
   return 0;
}

/**
 * 要点总结
 * 线性变换公式 dst = alpha*src + beta
 * alpha增益参数，控制图像的对比度
 * beta偏置参数， 控制图像的亮度
*/