//discrete fourier tranform, 离散傅里叶变换

//头文件
#include "opencv2/core.hpp"     //Core functionality，核心函数相关
#include "opencv2/imgproc.hpp"  //Image processing， 图像处理相关
#include "opencv2/imgcodecs.hpp"//Image file reading and writing， 图像的加载和写出相关
#include "opencv2/highgui.hpp"  //High-level GUI，图形界面GUI相关

#include <iostream>

/**
 * 程序流程
 * 1、加载图像，格式为灰度图
 * 2、获取图片dft变换的最佳大小
 * 3、边框加0的方式填充图片，即非0部分为dft变换的最佳大小 
 * 4、创建数组储存图像实部虚部，且合并到complexI
 * 5、傅里叶变换 dft(complexI, complexI)
 * 6、重新分离实部虚部，并且计算幅度
 * 7、将幅度映射到对数域
 * 8、以图像中心为原点划分象限，每个象限创建一个ROI
 * 9、对角象限互换
 * 10、显示结果
 */

//命名空间
using namespace cv;
using namespace std;

//帮助函数，输出程序的信息
static void help(void)
{
    cout << endl
        <<  "This program demonstrated the use of the discrete Fourier transform (DFT). " << endl   //离散傅里叶变换示例
        <<  "The dft of an image is taken and it's power spectrum is displayed."          << endl   //离散傅里叶变换后显示功率谱
        <<  "Usage:"                                                                      << endl
        <<  "./discrete_fourier_transform [image_name -- default ../data/lena.jpg]"       << endl;  //默认加载图片路径
}

int main(int argc, char ** argv)
{
    help();

    //获取图像路径（文件名），命令行输入否则默认
    const char* filename = argc >=2 ? argv[1] : "../data/lena.jpg";

    //加载图像，方式为加载灰度图
    Mat I = imread(filename, IMREAD_GRAYSCALE);
    //检查是否成功加载
    if( I.empty()){
        cout << "Error opening image" << endl;
        return -1;
    }

//! [expand]
    Mat padded;                            
    //expand input image to optimal size， 将输入图像扩展到最佳大小
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); 

    // on the border add zero values，在边框上添加零值，使用copyMakeBorder函数
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
//! [expand]

//! [complex_and_real] 实部和虚部
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)}; //Mat 数组储存图像的实部和虚部
    Mat complexI; 
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros，用零添加到扩展的另一平面

//! [complex_and_real]

//! [dft]
    //离散傅里叶变换
    dft(complexI, complexI);            // this way the result may fit in the source matrix，这种方式的结果可能适合在源矩阵

//! [dft]

    // compute the magnitude and switch to logarithmic scale，计算幅度并映射到对数刻度
    //公式 => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)) 
//! [magnitude] 幅度
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I)),分离实部和虚部
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude，计算幅度且存放到planes[0]
    Mat magI = planes[0]; //幅度 
//! [magnitude]

//! [log]
    magI += Scalar::all(1);                    // switch to logarithmic scale，映射到对数刻度
    log(magI, magI);
//! [log]

//! [crop_rearrange]裁剪重新排列
    // crop the spectrum, if it has an odd number of rows or columns， 裁剪频谱, 如果它有奇数行或列数
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    //重新排列傅立叶图像的象限, 使原点位于图像中心
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    //每个象限新建一个ROI
    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant， 左上，第二象限
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right， 右上，第一象限
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left，左下，第三象限
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right， 右下，第四象限

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)，交换左上和右下象限
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)，交换右上和左下象限
    q2.copyTo(q1);
    tmp.copyTo(q2);
//! [crop_rearrange]

//! [normalize]
    //归一化，像素值都映射到[0,1]之间
    normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).
//! [normalize]

    //显示结果
    imshow("Input Image"       , I   );    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey();

    return 0;
}

/**
 * 要点总结：
 * 加载图片格式为灰度图
 * getOptimalDFTSize()函数获取最佳大小
 * copyMakeBorder()加框函数
 * 实部虚部
 * merge()合并函数
 * dft()函数
 * 幅度公式sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
 * split()分离函数
 * magnitude()计算幅度
 * log()对数函数
 * normalize()归一化函数
 * /