#ifndef LL_1_H
#define LL_1_H

#include<QString>
#include<QStringList>
#include<QMessageBox>
#include<QMap>
#include<QVector>
#include<QDebug>
class LL_1
{
public:
    LL_1(QStringList rule,QString startSymbol,QString str);
    bool createLLlist();
private:
    QStringList ruleList;//输入文法
    QString str;//输入字符串
    QMap<QChar,QStringList> ruleMap;//文法格式化存储
    QVector<QChar> terminatorVector;//所有终结符

    
    void analysis();
    QChar startSymbol;
    bool getFIRST(QChar symbol,QVector<QChar> &returnFirst);
    bool getFOLLOW(QChar symbol,QVector<QChar> &returnFollow);
    QMap<QChar,bool> readyFIRST;
    QMap<QChar,bool> readyFOLLOW;

    void createFIRST();
    void createFOLLOW();
    QMap<QChar,QVector<QChar>> firstMap;
    QMap<QChar,QVector<QChar>> followMap;

};

#endif // LL_1_H
