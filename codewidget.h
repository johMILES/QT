//**************************************************
//版权：   RenGu Company
//文件名： codewidget.h
//作者：   尚超     Version：1.0    Date：2016.10.27
//描述：   软件主界面
//其它：
//修改历史：
//2016.12.04:sc:    添加结果导出功能；添加排序功能；单元格不可编辑；
//                  添加文件类型idl; 添加在Windows下查看文件源码功能；
//2016.12.05:sc:    完善结果文件导出功能；完善表头排序功能；
//                  完善界面比例显示功能；
//**************************************************

#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QMainWindow>
#include <QStringList>
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
    struct FileResults
    {
        QString filePath;   //文件全路径
        int totleLines;     //文件总行数
        int codeLines;      //代码行数
        int noteLines;      //注释行
        int spaceLines;     //空行
    };

private slots:
    void selectFile(bool);
    void selectDir(bool);
    void startStatiscal(bool);
    void lookFileContents(bool);
    void setParameters(bool);
    void quitThis(bool);
    void makeForFile(bool);
    void updateTable(QString fileName,int totleLines, int codeLines, int noteLines, int spaceLines);
    void tableItemClicked(QTableWidgetItem *);
    void mySort(int);

private:
    void initConnect();
    void initUiStatus();
    void initToolTips();
    void initData();
    void lookForFile(const QString &path);
    void addNewItem();
    void deleteItem();
    void clearTable();
    void clearAllTableItem();
    void clearTotleLineEdit();
    QString makeHtmlSontents();

    Ui::CodeWidget *ui;
    StatisticalThread *stThread;    //统计步骤线程
    Parameter *paraDialog;          //设置参数界面
    QList<FileResults*> fileResults;//文件统计结果集合
    QStringList filesList;          //需要统计的文件的路径集合
    QStringList format;             //需要统代码量的文件类型
    int files_TotleLines;           //统计的所有文件的总行数
    int files_CodeLines;            //统计的所有文件的代码行数
    int files_NoteLines;            //统计的所有文件的注释行数
    int files_SpaceLines;           //统计的所有文件的空行行数
    int file_Numbers;               //已经统计的文件

};

#endif // CODEWIDGET_H
