#include "parameter.h"
#include "ui_parameter.h"
#include <QDebug>

Parameter::Parameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameter)
{
    ui->setupUi(this);
    initFormats();
    initListWidget();
    initConnect();
}

//初始化提供给用户选择的格式列表
void Parameter::initFormats()
{
    formats += "*.h";
    formats += "*.cpp";
    formats += "*.idl";

    formatList = formats;
}

//初始化槽函数
void Parameter::initConnect()
{
    connect(ui->selectAllItem,SIGNAL( stateChanged(int) ),
            this,SLOT( selectAllItems(int) ));
    connect(ui->formatListWidget,SIGNAL( itemClicked(QListWidgetItem*) ),
            this,SLOT( setItemChecked(QListWidgetItem*) ));
    connect(ui->decide,SIGNAL( clicked(bool) ),
            this,SLOT( setDefineFormat(bool) ));
    connect(ui->cancel,SIGNAL( clicked(bool) ),
            this,SLOT( cancelFormat(bool) ));
}

//初始化读取文件类型
void Parameter::initListWidget()
{
    int i=0;
    foreach(QString format,formats)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(format);
        item->setCheckState(Qt::Checked);
        ui->formatListWidget->insertItem(i,item);
        i++;
    }
}

//点击全选
void Parameter::selectAllItems(int state)
{
    switch(state)
    {
    case 0:
        for(int i=0;i<ui->formatListWidget->count();i++)
        {
            ui->formatListWidget->item(i)->setCheckState(Qt::Unchecked);
        }
        break;
    case 2:
        for(int i=0;i<ui->formatListWidget->count();i++)
        {
            ui->formatListWidget->item(i)->setCheckState(Qt::Checked);
        }
        break;
    }
}

//点击列表选择状态
void Parameter::setItemChecked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
    {

    }
}

//确认设置
void Parameter::setDefineFormat(bool)
{
    formatList.clear();
    for(int i=0;i<ui->formatListWidget->count();i++)
    {
        if(ui->formatListWidget->item(i)->checkState() == Qt::Checked)
        {
            setFormats(i);
        }
    }
    accept();
}

//确定具体的格式内容
void Parameter::setFormats(int index)
{
    formatList.append(formats.at(index));
}

//取消设置
void Parameter::cancelFormat(bool)
{
    this->hide();
}

//获取设置中的读取文件类型列表
QStringList Parameter::getFormatList()
{
    return formatList;
}

Parameter::~Parameter()
{
    delete ui;
}
