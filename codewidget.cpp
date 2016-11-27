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
    connect(ui->fileTableList,SIGNAL( itemClicked(QTableWidgetItem*) ),
            this,SLOT( tableItemClicked(QTableWidgetItem*) ));

}

void CodeWidget::initUiStatus()
{

}

void CodeWidget::initToolTips()
{

}

void CodeWidget::initData()
{
    paraDialog = new Parameter();
    format = paraDialog->getFormatList();

    stThread = NULL;

    files_TotleLines = 0;
    files_CodeLines = 0;
    files_NoteLines = 0;
    files_SpaceLines = 0;
}

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
        ui->showFileList->setText("共选中"+QString::number(filesList.count()) +"个文件 ："+filesList.first());
    }
    else
    {
        ui->showFileList->setText("");
    }

}

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

void CodeWidget::quitThis(bool)
{
    this->close();
}

void CodeWidget::updateTable(QString fileName,int totleLines,int codeLines,int noteLines,int spaceLines)
{
    files_TotleLines += totleLines;
    files_CodeLines += codeLines;
    files_NoteLines += noteLines;
    files_SpaceLines += spaceLines;

    ui->totleLines->setText(QString::number(files_TotleLines));
    ui->totleCodeLines->setText(QString::number(files_CodeLines));
    ui->totleNoteLines->setText(QString::number(files_NoteLines));
    ui->totleSapceLines->setText(QString::number(files_SpaceLines));

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

void CodeWidget::deleteItem()
{

}

void CodeWidget::clearTable()
{

}

void CodeWidget::clearAllTableItem()
{
    for(int i=ui->fileTableList->rowCount()-1;i>=0;i--)
    {
        ui->fileTableList->removeRow(i);
    }
}

void CodeWidget::clearTotleLineEdit()
{
    files_TotleLines = 0;
    files_CodeLines = 0;
    files_NoteLines = 0;
    files_SpaceLines = 0;

    ui->totleLines->setText("");
    ui->totleCodeLines->setText("");
    ui->totleNoteLines->setText("");
    ui->totleSapceLines->setText("");
}

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

    delete ui;
}
