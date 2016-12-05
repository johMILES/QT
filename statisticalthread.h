//**************************************************
//版权：   RenGu Company
//文件名： statisticalthread.h
//作者：   尚超     Version：1.0    Date：2016.10.27
//描述：   统计线程
//其它：
//修改历史：
//2016.12.05:sc:    添加#include <QStringList>，确保Windows下的编译
//**************************************************

#ifndef STATISTICALTHREAD_H
#define STATISTICALTHREAD_H

#include <QThread>
#include <QStringList>

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
