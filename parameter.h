#ifndef PARAMETER_H
#define PARAMETER_H

#include <QDialog>

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
    //获取设置中的读取文件类型列表
    QStringList getFormatList()
    {
        return formatList;
    }

private slots:
    //点击全选
    void selectAllItems(int);

    //点击列表选择状态
    void setItemChecked(QListWidgetItem *);

    //确认设置
    void setDefineFormat(bool);

    //取消设置
    void cancelFormat(bool);

private:
    //初始化槽函数
    void initConnect();

    //初始化提供给用户选择的格式列表
    void initFormats();

    //初始化读取文件类型
    void initListWidget();

    //确定具体的格式内容
    void setFormats(int);

    Ui::Parameter *ui;

    //文件类型列表
    QStringList formatList;

    //预加载文件类型列表
    QStringList formats;
};

#endif // PARAMETER_H
