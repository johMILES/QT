#include "codewidget.h"
#include "ui_codewidget.h"
#include <stdlib.h>
#include <statisticalthread.h>
#include <parameter.h>

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QTextCodec>
#include <QProcess>
#include <QFile>
#include <QDataStream>
#include <QTextStream>

#include <QDebug>

CodeWidget::CodeWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeWidget)
{
    ui->setupUi(this);
    initConnect();
    initUiStatus();
    initToolTips();
    initData();
}

//初始化槽函数链接
void CodeWidget::initConnect()
{
    connect(ui->selectFile,SIGNAL( clicked(bool) ),
            this,SLOT( selectFile(bool) ));
    connect(ui->selectDir,SIGNAL( clicked(bool) ),
            this,SLOT( selectDir(bool) ));
    connect(ui->start,SIGNAL( clicked(bool) ),
            this,SLOT( startStatiscal(bool) ));
    connect(ui->lookCodeContents,SIGNAL( clicked(bool) ),
            this,SLOT( lookFileContents(bool) ));
    connect(ui->set,SIGNAL( clicked(bool) ),
            this,SLOT( setParameters(bool) ));
    connect(ui->quit,SIGNAL( clicked(bool) ),
            this,SLOT( quitThis(bool) ));
    connect(ui->makeForFile,SIGNAL( clicked(bool) ),
            this,SLOT( makeForFile(bool) ));
    connect(ui->fileTableList,SIGNAL( itemClicked(QTableWidgetItem*) ),
            this,SLOT( tableItemClicked(QTableWidgetItem*) ));

}

//初始化界面的控件可操作状态
void CodeWidget::initUiStatus()
{

}

//初始化控件的提示信息
void CodeWidget::initToolTips()
{

}

//初始化数据成员
void CodeWidget::initData()
{
    paraDialog = new Parameter();
    format = paraDialog->getFormatList();

    stThread = NULL;

    files_TotleLines = 0;
    files_CodeLines = 0;
    files_NoteLines = 0;
    files_SpaceLines = 0;
    file_Numbers =0;
}

//选择需要统计的单个文件
void CodeWidget::selectFile(bool)
{

    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);

    QString currPath = QDir::currentPath();

    filesList.clear();
    filesList = QFileDialog::getOpenFileNames(
                                        this,
                                        tr("选择文件"),
                                        currPath,
                                        "CPP (*.cpp);;H (*.h )");
    if(filesList.count()>0)
    {
        ui->showFileList->setText(tr("共选中")+QString::number(filesList.count()) +tr("个文件 ：")+filesList.first());
    }
    else
    {
        ui->showFileList->setText("");
    }

}

//选择需要统计的文件夹目录
void CodeWidget::selectDir(bool)
{
    QString currPath = QDir::currentPath();
    QString path = QFileDialog::getExistingDirectory(this,
                                                 tr("选择文件夹"),
                                                 currPath);
    ui->showFileList->setText("选中文件夹目录："+path);

    if(path == NULL)
    {
        return;
    }
    filesList.clear();

    lookForFile(path);
}

//开始统计
void CodeWidget::startStatiscal(bool)
{   
    if(stThread == NULL)
    {
        stThread = new StatisticalThread();
        connect(stThread,SIGNAL( emitStatisticalStatus(QString,int,int,int,int) ),
                this,SLOT( updateTable(QString,int,int,int,int) ));
    }
    stThread->setFilesList(filesList);
    clearAllTableItem();
    clearTotleLineEdit();
    stThread->run();
}

//查看选中文件的本地源代码
void CodeWidget::lookFileContents(bool)
{
    if(ui->fileTableList->rowCount()<=0)
    {
        ui->statusBar->showMessage("没有可以显示的文件");
        return;
    }
    int row = ui->fileTableList->currentRow();

    if(row<0)
    {
        ui->statusBar->showMessage("未选择可以操作的文件目录");
        return;
    }
    QString fileName = ui->fileTableList->item(row,0)->toolTip();

    //打开额外的本地记事本程序，打开预览文件
    QProcess *icemProcess=new QProcess;

#ifdef Q_OS_MAC
    QStringList arguments;
    QString bash="open";
    arguments<<"-e"<<fileName;
    icemProcess->startDetached(bash,arguments);
#else
    QString notepadPath = "notepad.exe";
    icemProcess->start(notepadPath);    //此处还未在Windows下测试
#endif
}

//设置参数
void CodeWidget::setParameters(bool)
{
    if(paraDialog == NULL)
    {
        paraDialog = new Parameter();
    }

    if(paraDialog->exec())
    {
        format = paraDialog->getFormatList();
    }
}

//关闭软件
void CodeWidget::quitThis(bool)
{
    this->close();
}

//更新表格中的数据
void CodeWidget::updateTable(QString fileName,int totleLines,int codeLines,int noteLines,int spaceLines)
{
    file_Numbers++;
    files_TotleLines += totleLines;
    files_CodeLines += codeLines;
    files_NoteLines += noteLines;
    files_SpaceLines += spaceLines;

    FileResults *fr = new FileResults;
    fr->filePath = fileName;
    fr->totleLines = totleLines;
    fr->codeLines = codeLines;
    fr->noteLines = noteLines;
    fr->spaceLines = spaceLines;
    fileResults.append(fr);

    ui->totleLines->setText(QString::number(files_TotleLines)+"(共"+QString::number(file_Numbers) +"个文件)");
    ui->totleCodeLines->setText(QString::number(files_CodeLines)+"("+QString::number((float)files_CodeLines/files_TotleLines*100) +"%)");
    ui->totleNoteLines->setText(QString::number(files_NoteLines)+"("+QString::number((float)files_NoteLines/files_TotleLines*100) +"%)");
    ui->totleSapceLines->setText(QString::number(files_SpaceLines)+"("+QString::number((float)files_SpaceLines/files_TotleLines*100) +"%)");

    addNewItem();
    QFileInfo info(fileName);
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,0)->setText(info.fileName());
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,0)->setToolTip(fileName);

    ui->fileTableList->item(ui->fileTableList->rowCount()-1,1)->setText(QString::number(totleLines));
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,2)->setText(QString::number(codeLines));
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,3)->setText(QString::number(noteLines));
    ui->fileTableList->item(ui->fileTableList->rowCount()-1,4)->setText(QString::number(spaceLines));

    ui->statusBar->showMessage(fileName+" 统计完成");

}

//添加一个新的条目
void CodeWidget::addNewItem()
{
    ui->fileTableList->insertRow(ui->fileTableList->rowCount());

    for(int i=0;i<ui->fileTableList->columnCount();i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        if(i == 0)
        {
            item->setTextColor(QColor(0,0,255,255));
        }
        ui->fileTableList->setItem(ui->fileTableList->rowCount()-1,i,item);
    }
}

//删除一个条目
void CodeWidget::deleteItem()
{

}

//清空表格中显示的信息
void CodeWidget::clearTable()
{

}

//清空表格条目
void CodeWidget::clearAllTableItem()
{
    for(int i=ui->fileTableList->rowCount()-1;i>=0;i--)
    {
        ui->fileTableList->removeRow(i);
    }
}

//清空界面下方总统计显示的数据
void CodeWidget::clearTotleLineEdit()
{
    files_TotleLines = 0;
    files_CodeLines = 0;
    files_NoteLines = 0;
    files_SpaceLines = 0;
    file_Numbers = 0;
    foreach (FileResults* fr, fileResults)
    {
        delete fr;
        fr = NULL;
    }
    fileResults.clear();

    ui->totleLines->setText("");
    ui->totleCodeLines->setText("");
    ui->totleNoteLines->setText("");
    ui->totleSapceLines->setText("");
}

//遍历指定文件目录下的文件
void CodeWidget::lookForFile(const QString &path)
{
    QDir dir(path);

    foreach(QString file,dir.entryList(format,QDir::Files))
    {
        file = path + QDir::separator() + file;
        filesList.append(file);
    }

    foreach(QString subDir,dir.entryList(QDir::Dirs
                                         | QDir::NoDotAndDotDot))
    {
        lookForFile(path + QDir::separator() +subDir);
    }

    ui->showFileList->setText("选中文件夹目录："+path +"共添加文件"+QString::number(filesList.count())+"个");
}

//导出统计结果
void CodeWidget::makeForFile(bool)
{
    QString contents ;          //导出的文件内容->需要写入到文件的字符串
    int index=0;
    contents += tr("代码统计工具统计结果\n");
    contents += tr("共文件")+QString::number(fileResults.count())+tr("个\n");
    foreach(FileResults *fr,fileResults)
    {
        contents+=QString::number(++index)+tr(".文件名：")+fr->filePath+" ;\n";
        contents+=tr("文件总行数：")+QString::number(fr->totleLines) + " ;";
        contents+=tr("代码总行数：")+QString::number(fr->codeLines) + " ;";
        contents+=tr("注释行：")+QString::number(fr->noteLines) + " ;";
        contents+=tr("空行：")+QString::number(fr->spaceLines) + " ;";
        contents+="\n\n";
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                 tr("保存文件"),
                                 tr("代码统计结果.txt"));
    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        return;
    }

    QTextStream writer(&file);
    writer.setCodec(QTextCodec::codecForName("UTF-8"));
    writer<<contents;

    file.close();
}

//响应表格点击事件
void CodeWidget::tableItemClicked(QTableWidgetItem *item)
{
    if(item->column() == 0)
    {
        ui->statusBar->showMessage(item->toolTip());
    }
    else
    {
        ui->statusBar->showMessage(ui->fileTableList->item(item->row(),0)->toolTip());
    }
}

CodeWidget::~CodeWidget()
{
    if(stThread)
    {
        stThread->quit();
        stThread->wait();
        stThread->deleteLater();
    }

    if(paraDialog)
    {
        delete paraDialog;
        paraDialog = NULL;
    }

    foreach (FileResults* fr, fileResults)
    {
        delete fr;
        fr = NULL;
    }
    fileResults.clear();

    delete ui;
}
