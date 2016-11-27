#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QMainWindow>
class QTableWidgetItem;
class StatisticalThread;
class Parameter;

namespace Ui {
class CodeWidget;
}

class CodeWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit CodeWidget(QWidget *parent = 0);
    ~CodeWidget();

private slots:
    //选择需要统计的单个文件
    void selectFile(bool);

    //选择需要统计的文件夹目录
    void selectDir(bool);

    //开始统计
    void startStatiscal(bool);

    //查看选中文件的本地源代码
    void lookFileContents(bool);

    //设置参数
    void setParameters(bool);

    //关闭软件
    void quitThis(bool);

    //更新表格中的数据
    void updateTable(QString fileName,int totleLines, int codeLines, int noteLines, int spaceLines);

    //响应表格点击事件
    void tableItemClicked(QTableWidgetItem *);

private:
    //初始化槽函数链接
    void initConnect();

    //初始化界面的控件可操作状态
    void initUiStatus();

    //初始化控件的提示信息
    void initToolTips();

    //初始化数据成员
    void initData();

    //显示状态栏需要显示的信息
    void showStatus(QString);

    //遍历指定文件目录下的文件
    void lookForFile(const QString &path);

    //添加一个新的条目
    void addNewItem();

    //删除一个条目
    void deleteItem();

    //清空表格中显示的信息
    void clearTable();

    //清空表格条目
    void clearAllTableItem();

    //清空界面下方总统计显示的数据
    void clearTotleLineEdit();

    //统计步骤线程
    StatisticalThread *stThread;

    //设置参数界面
    Parameter *paraDialog;

    //需要统计的文件的路径集合
    QStringList filesList;

    //需要统代码量的文件类型
    QStringList format;

    Ui::CodeWidget *ui;

    //统计的所有文件的总行数
    int files_TotleLines;

    //统计的所有文件的代码行数
    int files_CodeLines;

    //统计的所有文件的注释行数
    int files_NoteLines;

    //统计的所有文件的空行行数
    int files_SpaceLines;

};

#endif // CODEWIDGET_H
