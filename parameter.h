//**************************************************
//版权：   RenGu Company
//文件名： parameter.h
//作者：   尚超     Version：1.0    Date：2016.XX.XX
//描述：   参数设置界面
//其它：
//修改历史：
//**************************************************

#ifndef PARAMETER_H
#define PARAMETER_H

#include <QDialog>
#include <QStringList>

class QListWidgetItem;

namespace Ui {
class Parameter;
}

class Parameter : public QDialog
{
    Q_OBJECT

public:
    explicit Parameter(QWidget *parent = 0);
    ~Parameter();
    QStringList getFormatList();

private slots:
    void selectAllItems(int);
    void setItemChecked(QListWidgetItem *);
    void setDefineFormat(bool);
    void cancelFormat(bool);

private:
    void initConnect();
    void initFormats();
    void initListWidget();
    void setFormats(int);

    Ui::Parameter *ui;
    QStringList formatList;     //文件类型列表
    QStringList formats;        //预加载文件类型列表
};

#endif // PARAMETER_H
