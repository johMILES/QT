//**************************************************
//版权：   RenGu Company
//文件名： statisticalfunction.h
//作者：   尚超     Version：1.0    Date：2016.XX.XX
//描述：   统计方法集合
//其它：
//修改历史：
//2016.12.05:sc:    完善在Windows平台下的空行统计功能
//                  完善在Mac下解读Windows下创建的文件统计功能
//**************************************************

#ifndef STATISTICALFUNCTION_H
#define STATISTICALFUNCTION_H

#include <QObject>
class QFile;

class StatisticalFunction : public QObject
{
    Q_OBJECT
public:
    explicit StatisticalFunction(QObject *parent = 0);
    ~StatisticalFunction();
    bool setFile(QString fileName); //设置需要统计的一个文件
    QString getFileName();          //获取当前统计的文件名
    void Statistical();             //统计的核心区功能

    //获取统计结果:文件总行数
    int  getTotleLines(){return  fileLines;}

    //获取统计结果:文件代码行数
    int  getCodeLines(){return  codeLines;}

    //获取统计结果:注释行数
    int  getNoteLines(){return  noteLines;}

    //获取统计结果:空格数量
    int  getSpaceLines(){return  spaceLines;}


signals:

public slots:

private:
    QFile *file;        //需要操作统计的文件
    int fileLines;      //文件代码总行数
    int codeLines;      //代码行数量
    int noteLines;      //注释行数量
    int spaceLines;     //空格数量

};

#endif // STATISTICALFUNCTION_H
