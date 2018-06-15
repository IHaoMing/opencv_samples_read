/**
 * file Smoothing.cpp
 * brief Sample code for simple filters
 * author OpenCV team
 */
/// 图像滤波的简单示例

///头文件 
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

//命名空间
using namespace std;
using namespace cv;

/// Global Variables
///全局变量
int DELAY_CAPTION = 1500;//延时原图
int DELAY_BLUR = 100;   //延时滤波图
int MAX_KERNEL_LENGTH = 31;//最大

Mat src; Mat dst;
char window_name[] = "Smoothing Demo";

/// Function headers
int display_caption( const char* caption );//显示原图
int display_dst( int delay );//显示效果图


/**
 * function main
 */

///主函数
int main( int argc, char ** argv )
{
    namedWindow( window_name, WINDOW_AUTOSIZE );

    /// Load the source image
    /// 加载原图
    const char* filename = argc >=2 ? argv[1] : "../data/lena.jpg";

    src = imread( filename, IMREAD_COLOR );
    if(src.empty()){
        printf(" Error opening image\n");
        printf(" Usage: ./Smoothing [image_name -- default ../data/lena.jpg] \n");
        return -1;
    }

    //显示原图
    if( display_caption( "Original Image" ) != 0 ) { return 0; }

    dst = src.clone();
    if( display_dst( DELAY_CAPTION ) != 0 ) { return 0; }


    /// Applying Homogeneous blur
    if( display_caption( "Homogeneous Blur" ) != 0 ) { return 0; }

    //![blur]
    /// 调用blur均值滤波函数
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    { blur( src, dst, Size( i, i ), Point(-1,-1) );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
    //![blur]

    /// Applying Gaussian blur
    if( display_caption( "Gaussian Blur" ) != 0 ) { return 0; }

    //![gaussianblur]
    ///调用高斯滤波
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    { GaussianBlur( src, dst, Size( i, i ), 0, 0 );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
    //![gaussianblur]

    /// Applying Median blur
    if( display_caption( "Median Blur" ) != 0 ) { return 0; }

    //![medianblur]
    ///中值滤波
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    { medianBlur ( src, dst, i );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
    //![medianblur]

    /// Applying Bilateral Filter
    if( display_caption( "Bilateral Blur" ) != 0 ) { return 0; }

    //![bilateralfilter]
    ///双边滤波
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    { bilateralFilter ( src, dst, i, i*2, i/2 );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
    //![bilateralfilter]

    /// Done
    display_caption( "Done!" );

    return 0;
}

/**
 * @function display_caption
 */
int display_caption( const char* caption )
{
    dst = Mat::zeros( src.size(), src.type() );
    putText( dst, caption,
             Point( src.cols/4, src.rows/2),
             FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );

    return display_dst(DELAY_CAPTION);
}

/**
 * @function display_dst
 */
int display_dst( int delay )
{
    imshow( window_name, dst );
    int c = waitKey ( delay );
    if( c >= 0 ) { return -1; }
    return 0;
}
