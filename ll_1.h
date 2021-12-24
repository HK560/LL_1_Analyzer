/*
 * @Author: HK560
 * @Date: 2021-12-22 10:20:16
 * @LastEditTime: 2021-12-24 09:50:50
 * @LastEditors: HK560
 * @Description: LL1头文件，定义声明LL_1类
 * @FilePath: \LL_1_Analyzer\ll_1.h
 * My Blog: https://blog.hk560.top
 */


#ifndef LL_1_H
#define LL_1_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QDebug>
#include <QMap>
#include <QMessageBox>
#include <QObject>
#include <QSet>
#include <QStack>
#include <QString>
#include <QStringList>
#include <QVector>
#include <algorithm>
class LL_1 : public QObject {
    Q_OBJECT
   public:
    LL_1(QStringList rule, QString startSymbol, QString str);
    bool createLLlist();
    void buildLLMaps();
    void analysis(QString inputString);

   signals:
    void sendStepInfo(QString info);  // QT信号

   private:
    QStringList ruleList;                //输入文法
    QString str;                         //输入字符串
    QMap<QChar, QStringList> ruleMap;    //文法格式化存储
    QVector<QChar> nonTerminatorVector;  //所有非终结符
    QChar startSymbol;                   //文法开始符号
    QMap<QChar, bool> readyFIRST;        //标记是否生成FIRST集
    QMap<QChar, bool> readyFOLLOW;
    QMap<QChar, QVector<QChar>> firstMap;  //存储FIRST集
    QMap<QChar, QVector<QChar>> followMap;
    QMap<QChar, QMap<QChar, QString>> LL_Maps;  // LL分析表
    int step = 0;                               //步骤数量

    void createFIRST();
    void createFOLLOW();
    bool getFIRST(QChar symbol, QVector<QChar> &returnFirst);
    bool getFOLLOW(QChar symbol, QVector<QChar> &returnFollow);
    QVector<QChar> deduplicationVectorQChar(QVector<QChar> &vec);  //去除重复
    void outputStep(QChar stackTopChar, QString nowInputStrChar);
    void outputFIRST(QChar sym);
    void outputFOLLOW(QChar sym);
};

#endif  // LL_1_H
