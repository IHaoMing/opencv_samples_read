//How to scan images
//如何扫描图像，如何读取访问像素

//头文件
#include <opencv2/core.hpp>     //核心模块，核心数据结构相关
#include <opencv2/core/utility.hpp> //改为使用设备层
#include "opencv2/imgcodecs.hpp"    //文件输入输出相关
#include <opencv2/highgui.hpp>      //GUI相关
#include <iostream>
#include <sstream>

//命名空间
using namespace std;
using namespace cv;

//输出程序相关信息
static void help()
{
    cout
        << "\n--------------------------------------------------------------------------" << endl
        << "This program shows how to scan image objects in OpenCV (cv::Mat). As use case"
        << " we take an input image and divide the native color palette (255) with the "  << endl
        << "input. Shows C operator[] method, iterators and at function for on-the-fly item address calculation."<< endl
        << "Usage:"                                                                       << endl
        << "./how_to_scan_images <imageNameToUse> <divideWith> [G]"                       << endl
        << "if you add a G parameter the image is processed in gray scale"                << endl
        << "--------------------------------------------------------------------------"   << endl
        << endl;
}


Mat& ScanImageAndReduceC(Mat& I, const uchar* table);               //C语言形式访问像素  p[j] = table[p[j]];
Mat& ScanImageAndReduceIterator(Mat& I, const uchar* table);        //迭代器形式访问 (*it)[0] = table[(*it)[0]];
Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar * table);   //随机访问 _I(i,j)[0] = table[_I(i,j)[0]];

int main( int argc, char* argv[])
{
    //输出帮助信息
    help();
    //判断命令行参数
    if (argc < 3)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }

    //输入图像到I
    Mat I, J;
    if( argc == 4 && !strcmp(argv[3],"G") )
        I = imread(argv[1], IMREAD_GRAYSCALE);
    else
        I = imread(argv[1], IMREAD_COLOR);

    if (I.empty())
    {
        cout << "The image" << argv[1] << " could not be loaded." << endl;
        return -1;
    }

    //! [dividewith]
    int divideWith = 0; // convert our input string to number - C++ style
    //转换输入的字符出为c++风格
    stringstream s;
    s << argv[2];
    s >> divideWith;
    if (!s || !divideWith)
    {
        cout << "Invalid number entered for dividing. " << endl;
        return -1;
    }

    uchar table[256];
    for (int i = 0; i < 256; ++i)
       table[i] = (uchar)(divideWith * (i/divideWith));
    //! [dividewith]

    const int times = 100;
    double t;

    t = (double)getTickCount(); //记录开始时间点

    for (int i = 0; i < times; ++i)
    {
        cv::Mat clone_i = I.clone();
        J = ScanImageAndReduceC(clone_i, table);
    }

    //得出处理耗时
    t = 1000*((double)getTickCount() - t)/getTickFrequency();
    t /= times;

    //输出处理耗时
    cout << "Time of reducing with the C operator [] (averaged for "
         << times << " runs): " << t << " milliseconds."<< endl;

    t = (double)getTickCount(); //记录开始时间点

    for (int i = 0; i < times; ++i)
    {
        cv::Mat clone_i = I.clone();
        J = ScanImageAndReduceIterator(clone_i, table);
    }

    //得出处理耗时
    t = 1000*((double)getTickCount() - t)/getTickFrequency();
    t /= times;
    //输出处理耗时
    cout << "Time of reducing with the iterator (averaged for "
        << times << " runs): " << t << " milliseconds."<< endl;

    //记录开始时间点
    t = (double)getTickCount();

    for (int i = 0; i < times; ++i)
    {
        cv::Mat clone_i = I.clone();
        ScanImageAndReduceRandomAccess(clone_i, table);
    }

    //得出处理耗时
    t = 1000*((double)getTickCount() - t)/getTickFrequency();
    t /= times;

    //输出处理耗时
    cout << "Time of reducing with the on-the-fly address generation - at function (averaged for "
        << times << " runs): " << t << " milliseconds."<< endl;

    //! [table-init]
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for( int i = 0; i < 256; ++i)
        p[i] = table[i];
    //! [table-init]

    t = (double)getTickCount();

    for (int i = 0; i < times; ++i)
        //! [table-use]
        //LUT函数操作 dst(I)←lut(src(I) + d)
        LUT(I, lookUpTable, J);
        //! [table-use]

    t = 1000*((double)getTickCount() - t)/getTickFrequency();
    t /= times;
    //输出处理时间
    cout << "Time of reducing with the LUT function (averaged for "
        << times << " runs): " << t << " milliseconds."<< endl;
    return 0;
}

//! [scan-c]
Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    int channels = I.channels();

    int nRows = I.rows;
    int nCols = I.cols * channels;

    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i,j;
    uchar* p;
    for( i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for ( j = 0; j < nCols; ++j)
        {
            p[j] = table[p[j]];
        }
    }
    return I;
}
//! [scan-c]

//! [scan-iterator]
Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    const int channels = I.channels();
    switch(channels)
    {
    case 1:
        {
            MatIterator_<uchar> it, end;
            //访问形式
            for( it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
                *it = table[*it];
            break;
        }
    case 3:
        {
            MatIterator_<Vec3b> it, end;
            for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
            {
                (*it)[0] = table[(*it)[0]];
                (*it)[1] = table[(*it)[1]];
                (*it)[2] = table[(*it)[2]];
            }
        }
    }

    return I;
}
//! [scan-iterator]

//! [scan-random]
Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    const int channels = I.channels();
    //获取通道
    switch(channels)
    {
    case 1:
        {
            //单通道访问
            for( int i = 0; i < I.rows; ++i)
                for( int j = 0; j < I.cols; ++j )
                    //访问形式  table[I.at<uchar>(i,j)];
                    I.at<uchar>(i,j) = table[I.at<uchar>(i,j)];
            break;
        }
    case 3:
        {
         Mat_<Vec3b> _I = I;
        //三通道访问，使用向量Mat_<Vec3b>
         for( int i = 0; i < I.rows; ++i)
            for( int j = 0; j < I.cols; ++j )
               {
                   //访问形式 table[_I(i,j)[0]];
                   _I(i,j)[0] = table[_I(i,j)[0]];
                   _I(i,j)[1] = table[_I(i,j)[1]];
                   _I(i,j)[2] = table[_I(i,j)[2]];
            }
         I = _I;
         break;
        }
    }

    return I;
}
//! [scan-random]

/**
 * 要点总结
 * c风格、迭代器、随机访问三种访问的方式
 * 色彩空间的减少table[i] = (uchar)(divideWith * (i/divideWith));
 * LUT函数操作 dst(I)←lut(src(I) + d)
 * 计算处理耗时的方式
 */