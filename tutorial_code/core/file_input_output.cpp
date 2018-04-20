//file input output 
//文件的输出和输入

//头文件
#include <opencv2/core.hpp>     //Core functionality， 核心函数，包含着核心的数据结构
#include <iostream>
#include <string>

//命名空间
using namespace cv;
using namespace std;

//输出程序相关信息
static void help(char** av)
{
    /**
     * 显示 OpenCV 序列化功能的用法
     * 输出文件可以是 xml (xml) 或 YAML (yml/YAML)。你甚至可以压缩它通过在其扩展名中指定此类, 如 xml.gz yaml.gz 等。
     * 通过文件存储, 可以使用<<和>>运算符在 OpenCV 中序列化对象
    */
    cout << endl
        << av[0] << " shows the usage of the OpenCV serialization functionality."         << endl
        << "usage: "                                                                      << endl
        <<  av[0] << " outputfile.yml.gz"                                                 << endl
        << "The output file may be either XML (xml) or YAML (yml/yaml). You can even compress it by "
        << "specifying this in its extension like xml.gz yaml.gz etc... "                  << endl
        << "With FileStorage you can serialize objects in OpenCV by using the << and >> operators" << endl
        << "For example: - create a class and have it serialized"                         << endl
        << "             - use it to read and write matrices."                            << endl;
}

//数据类的定义
class MyData
{
public:
    MyData() : A(0), X(0), id()                 //构造函数
    {}
    explicit MyData(int) : A(97), X(CV_PI), id("mydata1234") // explicit to avoid implicit conversion，避免隐式转换
    {}
    //数据输出
    void write(FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "A" << A << "X" << X << "id" << id << "}";
    }
    //数据输入
    void read(const FileNode& node)                          //Read serialization for this class
    {
        A = (int)node["A"];
        X = (double)node["X"];
        id = (string)node["id"];
    }
public:   // Data Members
    //数据成员
    int A;
    double X;
    string id;
};

//These write and read functions must be defined for the serialization in FileStorage to work
//这些写和读函数必须定义为文件存储中的序列化才能工作
static void write(FileStorage& fs, const std::string&, const MyData& x)
{
    //对象x的数据输出到fs文件
    x.write(fs);
}
static void read(const FileNode& node, MyData& x, const MyData& default_value = MyData()){
    //读取node文件数据到xd对象
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

// This function will print our custom class to the console
// 此函数将将我们的自定义类打印到控制台
static ostream& operator<<(ostream& out, const MyData& m)
{
    //重载"<<"运算符,完成数据序列化 形式 {id = "id", x = x, A = A}
    out << "{ id = " << m.id << ", ";
    out << "X = " << m.X << ", ";
    out << "A = " << m.A << "}";
    return out;
}

int main(int ac, char** av)
{
    if (ac != 2)
    {
        help(av);
        return 1;
    }

    string filename = av[1];
    { //write
        //写数据，输出数据到文件
        //创建数据
        Mat R = Mat_<uchar>::eye(3, 3),
            T = Mat_<double>::zeros(3, 1);
        
        //实例化一个对象m
        MyData m(1);

        //以写入方式打开文件
        FileStorage fs(filename, FileStorage::WRITE);

        //iterationNr:100形式输出到文件
        fs << "iterationNr" << 100;                         
        //strings:[string1, ..., stringn]形式输出到文件
        fs << "strings" << "[";                              // text - string sequence，
        fs << "image1.jpg" << "Awesomeness" << "../data/baboon.jpg";
        fs << "]";                                           // close sequence

        //mapping:{m1, ... , mn}形式输出到文件
        fs << "Mapping";                              // text - mapping
        fs << "{" << "One" << 1;
        fs <<        "Two" << 2 << "}";

        fs << "R" << R;                                      // cv::Mat
        fs << "T" << T;

        fs << "MyData" << m;                                // your own data structures

        fs.release();                                       // explicit close
        cout << "Write Done." << endl;
    }

    {//read
        //从文件读取数据到对象

        cout << endl << "Reading: " << endl;

        //文件对象
        FileStorage fs;
        //以读取打开文件
        fs.open(filename, FileStorage::READ);

        int itNr;
        //fs["iterationNr"] >> itNr;
        itNr = (int) fs["iterationNr"];
        cout << itNr;
        if (!fs.isOpened())
        {
            cerr << "Failed to open " << filename << endl;
            help(av);
            return 1;
        }

        //FileNode变量获取string字段
        FileNode n = fs["strings"];                         // Read string sequence - Get node
        if (n.type() != FileNode::SEQ)
        {
            cerr << "strings is not a sequence! FAIL" << endl;
            return 1;
        }

        FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node, 遍历节点node
        for (; it != it_end; ++it)
            cout << (string)*it << endl;


        n = fs["Mapping"];                                // Read mappings from a sequence， 读取序列中的mapping

        cout << "Two  " << (int)(n["Two"]) << "; ";
        cout << "One  " << (int)(n["One"]) << endl << endl;


        MyData m;
        Mat R, T;

        fs["R"] >> R;                                      // Read cv::Mat， 读取Mat数据
        fs["T"] >> T;
        fs["MyData"] >> m;                                 // Read your own structure_ 读取自己的数据结构

        cout << endl
            << "R = " << R << endl;
        cout << "T = " << T << endl << endl;
        cout << "MyData = " << endl << m << endl << endl;

        //Show default behavior for non existing nodes
        cout << "Attempt to read NonExisting (should initialize the data structure with its default).";
        fs["NonExisting"] >> m;
        cout << endl << "NonExisting = " << endl << m << endl;
    }

    cout << endl
        << "Tip: Open up " << filename << " with a text editor to see the serialized data." << endl;

    return 0;
}

/**
 * 要点总结
 * 构造函数避免隐式转换
 * ：、 ：[]、: {}三种序列化数据格式
 * 重载运算符
 */