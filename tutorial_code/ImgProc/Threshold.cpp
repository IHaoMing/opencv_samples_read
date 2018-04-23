/**
 * @file Threshold.cpp
 * @brief Sample code that shows how to use the diverse threshold options offered by OpenCV
 * @author OpenCV team
 */

/**
 * 阈值
 * 演示如何使用 OpenCV 提供的各种阈值选项的简短示例代码
*/

//头文件
#include "opencv2/imgproc.hpp"      //图像处理相关
#include "opencv2/imgcodecs.hpp"    //图像读取和写入相关
#include "opencv2/highgui.hpp"      //GUI相关

/**
 * 程序流程:
 * 1、加载图像，命令行输入图像路径或者默认
 * 2、初始化变量，创建窗口
 * 3、创建滑动条
 * 4、调用显示
*/


//命名空间
using namespace cv;

/// Global variables
//  全局变量
int threshold_value = 0;      //阈值
int threshold_type = 3;       //阈值分割的类型
int const max_value = 255;    //最大值
int const max_type = 4;       //最大值的类型
int const max_BINARY_value = 255;   //二值化图像的最大值

// 图像变量， 窗口
Mat src, src_gray, dst;
const char* window_name = "Threshold Demo";

//滑动条
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";

/// Function headers
/// 回掉函数声明，原型必须为 void function_name(int, void*)
void Threshold_Demo( int, void* );

/**
 * @function main
 */
// 主函数
int main( int argc, char** argv )
{
  //! [load]
  // 加载图像
  String imageName("../data/stuff.jpg"); // by default， 默认路径
  if (argc > 1)
  {
      imageName = argv[1];//或者命令行参数输入图像路径
  }

  //加载图像
  src = imread( imageName, IMREAD_COLOR ); // Load an image

  if( src.empty() )
    { return -1; }

  //图像转为灰度图
  cvtColor( src, src_gray, COLOR_BGR2GRAY ); // Convert the image to Gray
  //! [load]

  //! [window]
  //创建窗口
  namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
  //! [window]

  //! [trackbar]
  // 创建滑动条
  createTrackbar( trackbar_type,
                  window_name, &threshold_type,
                  max_type, Threshold_Demo ); // Create Trackbar to choose type of Threshold

  createTrackbar( trackbar_value,
                  window_name, &threshold_value,
                  max_value, Threshold_Demo ); // Create Trackbar to choose Threshold value
  //! [trackbar]

  //调用显示
  Threshold_Demo( 0, 0 ); // Call the function to initialize

  /// Wait until user finishes program
  /// 等待按下 esc 键退出
  for(;;)
    {
      char c = (char)waitKey( 20 );
      if( c == 27 )
    { break; }
    }

}

//![Threshold_Demo]
/**
 * @function Threshold_Demo
 */
void Threshold_Demo( int, void* )
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */

  /**
   * 阈值分割的类型
   * 0、二进制，大于阈值， dst = max_value； 小于阈值， dst = 0;
   * 1、反二进制，与二进制相反;
   * 2、阈值截断，大于阈值， dst = thresh; 小于阈值， dst = src;
   * 3、0阈值， 大于阈值， dst = src; 小于阈值， dst = 0;
   * 4、反0阈值，与0阈值相反
  */

  //调用阈值分割函数
  threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );

  imshow( window_name, dst );
}
//![Threshold_Demo]

/**
 * 要点总结
 * 阈值操作的类型，二进制、反二进制、阈值截断、0阈值、反0阈值
*/
