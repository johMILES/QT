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

    //设置需要统计的一个文件
    bool setFile(QString fileName);

    //获取当前统计的文件名
    QString getFileName();

    //统计方法
    //统计的核心区功能
    void Statistical();

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
    //需要操作统计的文件
    QFile *file;

    //文件代码总行数
    int fileLines;

    //代码行数
    int codeLines;

    //注释行数量
    int noteLines;

    //空格数量
    int spaceLines;

};

#endif // STATISTICALFUNCTION_H
