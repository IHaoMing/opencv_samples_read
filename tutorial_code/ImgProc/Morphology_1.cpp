/**
 * @file Morphology_1.cpp
 * @brief Erosion and Dilation sample code
 * @author OpenCV team
 */

/**
 * 形态学操作
 * 腐蚀和膨胀
*/

/**
 * 程序流程
 * 1、加载图像
 * 2、创建窗口，移动窗口
 * 3、创建滑动条分别控制形态学操作的结构元素类型和形态学操作核的大小（程度）
 * 4、调用默认显示
*/
//头文件
#include "opencv2/imgproc.hpp"  //图像处理相关
#include "opencv2/highgui.hpp"  //GUI相关
#include <iostream>

//命名空间
using namespace cv;
using namespace std;

/// Global variables
/// 全局变量
Mat src, erosion_dst, dilation_dst; //原图、腐蚀效果图、膨胀效果图

int erosion_elem = 0;//腐蚀结构元素
int erosion_size = 0;//腐蚀程度大小
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;//最大卷积核

/** Function Headers */
void Erosion( int, void* ); //腐蚀
void Dilation( int, void* );//膨胀

/**
 * @function main
 */
//主函数
int main( int argc, char** argv )
{
  /// Load an image
  /// 加载图像
  CommandLineParser parser( argc, argv, "{@input | ../data/chicky_512.png | input image}" );
  src = imread( parser.get<String>( "@input" ), IMREAD_COLOR );
  if( src.empty() )
  {
    cout << "Could not open or find the image!\n" << endl;
    cout << "Usage: " << argv[0] << " <Input image>" << endl;
    return -1;
  }

  /// Create windows
  /// 创建窗口
  namedWindow( "Erosion Demo", WINDOW_AUTOSIZE );
  namedWindow( "Dilation Demo", WINDOW_AUTOSIZE );
  moveWindow( "Dilation Demo", src.cols, 0 ); //移动Dilation Demo窗口的位置

  /// Create Erosion Trackbar
  /// 创建控制腐蚀的滑动条
  createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
          &erosion_elem, max_elem,
          Erosion );//腐蚀的结构元素，矩形、交叉十字、椭圆

  createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo",
          &erosion_size, max_kernel_size,
          Erosion );//腐蚀程度大小

  /// Create Dilation Trackbar
  createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
          &dilation_elem, max_elem,
          Dilation );//膨胀结构元素，矩形、交叉十字、椭圆

  createTrackbar( "Kernel size:\n 2n +1", "Dilation Demo",
          &dilation_size, max_kernel_size,
          Dilation );//膨胀程度大小

  /// Default start
  /// 默认的腐蚀膨胀操作
  Erosion( 0, 0 );
  Dilation( 0, 0 );

  waitKey(0);
  return 0;
}

//![erosion]
/**
 * @function Erosion
 */

//腐蚀
void Erosion( int, void* )
{
  //根据erosion_elem取值选择腐蚀操作结构元素方式
  int erosion_type = 0;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  //![kernel]
  //获取操作结构元素
  Mat element = getStructuringElement( erosion_type,
                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                       Point( erosion_size, erosion_size ) );
  //![kernel]

  /// Apply the erosion operation
  ///进行腐蚀操作
  erode( src, erosion_dst, element );
  imshow( "Erosion Demo", erosion_dst );
}
//![erosion]

//![dilation]
/**
 * @function Dilation
 */
void Dilation( int, void* )
{
  //根据dilation_elem取值选择膨胀结构元素的操作方式
  int dilation_type = 0;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  //获取操作的结构元素
  Mat element = getStructuringElement( dilation_type,
                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                       Point( dilation_size, dilation_size ) );

  /// Apply the dilation operation
  ///进行膨胀操作
  dilate( src, dilation_dst, element );
  imshow( "Dilation Demo", dilation_dst );
}
//![dilation]

/**
 * 要点总结：
 * moveWindow()移动窗口
 * getStructuringElement()获取结构元素
 * erode()腐蚀操作
 * dilate()膨胀操作
*/