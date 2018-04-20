/**
 * @file AddingImages.cpp
 * @brief Simple linear blender ( dst = alpha*src1 + beta*src2 )
 * @author OpenCV team
 */
//
/**
 * 叠加图像的.cpp文件
 * 简单的线性混合，混合公式 dst = alpha*src1 +  beta*src2
*/

//头文件
#include "opencv2/imgcodecs.hpp"  // Image file reading and writing ,图片加载和写出操作相关
#include "opencv2/highgui.hpp"    // High-level GUI， GUI图形界面相关
#include <iostream>

//命名空间
using namespace cv;
using namespace std;

/**
 * @function main
 * @brief Main function
 */

/**
 * 程序流程
 * 1、图像、公式系数参数的声明和初始化，输入参数
 * 2、加载图片，检查图片是否加载成功
 * 3、调用addWeighted()函数
 * 4、显示线性混合结果
*/

//主函数
int main( void )
{
  
  //声明和初始化混合公式 dst = alpha*src1 +  beta*src2 中的两个系数
   double alpha = 0.5; double beta; double input;

  //图像容器变量
   Mat src1, src2, dst;

   /// Ask the user enter alpha
   // 输入系数alpha
   cout << " Simple Linear Blender " << endl;
   cout << "-----------------------" << endl;
   cout << "* Enter alpha [0.0-1.0]: ";
   cin >> input;

   // We use the alpha provided by the user if it is between 0 and 1
   // 系数限定在0-1之间
   if( input >= 0 && input <= 1 )
     { alpha = input; }

   //![load]
   /// Read images ( both have to be of the same size and type )
   //  加载两副待混合的原图
   src1 = imread( "../data/LinuxLogo.jpg" );
   src2 = imread( "../data/WindowsLogo.jpg" );
   //![load]

  //检查图片是否成功加载
   if( src1.empty() ) { cout << "Error loading src1" << endl; return -1; }
   if( src2.empty() ) { cout << "Error loading src2" << endl; return -1; }

   //![blend_images]
   // 由alpha计算beta, 两者关系限定为  beta = ( 1.0 - alpha )
   beta = ( 1.0 - alpha );
   //调用addWeighted()函数进行线性混合操作， gamma 参数区0.0
   addWeighted( src1, alpha, src2, beta, 0.0, dst);
   //![blend_images]

   //![display]
   //显示混合结果
   imshow( "Linear Blend", dst );
   waitKey(0);
   //![display]

   return 0;
}

/**
 * 要点总结
 * 线性混合公式 dst = alpha*src1 +  beta*src2
 * 混合公式两个系数取值限定在[0,1], 两者关系为beta = ( 1.0 - alpha )
 * gamma参数不取零混合公式就变为 dst = alpha*src1 +  beta*src2 + gamma
 * 调用示例addWeighted( src1, alpha, src2, beta, 0.0, dst);
*/