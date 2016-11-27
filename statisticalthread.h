#ifndef STATISTICALTHREAD_H
#define STATISTICALTHREAD_H

#include <QThread>

class StatisticalThread : public QThread
{
    Q_OBJECT

public:
    StatisticalThread();
    void run();

    //设置需要统计代码的文件
    void setFilesList(QStringList files);

signals:
    //发送统计文件的相关信息
    void emitStatisticalStatus(QString,int,int,int,int);

private:
    //准备统计的文件列表
    QStringList filesList;
};

#endif // STATISTICALTHREAD_H
