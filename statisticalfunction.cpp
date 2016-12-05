#include "statisticalfunction.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

StatisticalFunction::StatisticalFunction(QObject *parent) : QObject(parent)
{
    file = NULL;
}

bool StatisticalFunction::setFile(QString fileName)
{
    if(file == NULL)
    {
        file = new QFile(fileName);
    }

    if(!file->open(QIODevice::ReadOnly))
    {
        return false;
    }

    return true;
}

QString StatisticalFunction::getFileName()
{
    return file->fileName();
}

void StatisticalFunction::Statistical()
{
    int totleNum = 0;
    int codeNum = 0;
    int noteNum = 0;
    int spaceNum = 0;
    bool startShiftNote = false;
    int startAreaNote = 0;
    int codeAfterNote=0;    // */类型后有代码的清空，计算是不是有代码
    bool isSpaceLine=true;  //当前行是否是空行，默认初始化是空行
    bool isCodeLine = false;//当前行是否有代码，默认为没有代码
    QChar ch;
    QChar preCh;

    if(file->size() == 0){
        fileLines = 0;
        codeLines = 0;
        noteLines = 0;
        spaceLines = 0;
        return;
    }


    QTextStream reader(file);

    while(!reader.atEnd())
    {
        reader>>ch;

        if(ch == '/'){
            isSpaceLine = false;
            if(preCh == '/'){
                if(startShiftNote){
                    noteNum++;
                    startShiftNote = false; //结束行注释
                    reader.readLine();
                    totleNum++;
#ifdef Q_OS_MAC
                    isSpaceLine = true;
                    ch = '\xa';
#else
                    isSpaceLine = true;
                    ch = '\n';
#endif
                    if(codeAfterNote >0){
                        codeNum++;
                        codeAfterNote=0;
                    }
                }
            }
            else if(preCh == '*'){
                noteNum += startAreaNote; //此时为 */ 区域注释结束类型，当前的注释总行数需要加上区域注释计数器
                startAreaNote = 0;          //区域注释计数器清零,结束区域注释
            }
            else{
                if(startAreaNote == 0){     //遇到 '/' 为非区域注释,此时的行注释有效
                    startShiftNote = true;  //开始行注释
                }
            }
        }
        else if(ch == '*'){
            isSpaceLine = false;
            if(preCh == '/'){
                startAreaNote++;    // 此时为 /* 类型，区域注释计数器加一
            }
            if(startShiftNote == true)
            {
                startShiftNote = false;
            }
        }
        else if(ch == '\xa'){
            if(startAreaNote>0){
                startAreaNote++;    // 如果此时的区域注释计数器大于0，则当前的换行是在注释范围内，区域注释计数器加一
            }
            else if(isSpaceLine == true)
            {
                spaceNum++;
            }
            else if(codeAfterNote >0){
                codeNum++;
                codeAfterNote=0;
            }
            totleNum++;
            isSpaceLine = true;
        }
        else if(ch == '\r'){

        }
        else if(ch == '\n'){

            if(startAreaNote>0){
                startAreaNote++;    // 如果此时的区域注释计数器大于0，则当前的换行是在注释范围内，区域注释计数器加一
            }
            else if(isSpaceLine == true)
            {
                spaceNum++;
            }
            else if(codeAfterNote >0 ){
                codeNum++;
                codeAfterNote=0;
            }
            totleNum++;
            isSpaceLine = true;
            isCodeLine = false; //1.8
        }
        else if(ch == '\t'){

        }
        else if(ch == ' '){

        }
        else if(ch == '\0'){

        }
        else{
            if(startAreaNote == 0){
                codeAfterNote++;
                isCodeLine = true;  //1.8
                qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
                       <<"-->"<<ch
                       <<"\n";
            }

            isSpaceLine = false;
//            isCodeLine = true;
        }

        preCh = ch;
    }

    fileLines = totleNum;
    codeLines = codeNum;
    noteLines = noteNum;
    spaceLines = spaceNum;
}

StatisticalFunction::~StatisticalFunction()
{
    file->close();
    delete file;
}
