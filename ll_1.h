/*
 * @Author: your name
 * @Date: 2021-12-22 10:20:16
 * @LastEditTime: 2021-12-23 15:47:14
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \LL_1_Analyzer\ll_1.h
 */

#ifndef LL_1_H
#define LL_1_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QDebug>
#include <QMap>
#include <QMessageBox>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>
#include <algorithm>
#include<QStack>

#include <QObject>
class LL_1 :public QObject{
    Q_OBJECT
 public:
  LL_1(QStringList rule, QString startSymbol, QString str);
  bool createLLlist();
  void buildLLMaps();
  void analysis(QString inputString);

signals:
 void sendStepInfo(QString info);
 private:
  QStringList ruleList;                //输入文法
  QString str;                         //输入字符串
  QMap<QChar, QStringList> ruleMap;    //文法格式化存储
  QVector<QChar> nonTerminatorVector;  //所有非终结符

  QChar startSymbol;
  bool getFIRST(QChar symbol, QVector<QChar> &returnFirst);
  bool getFOLLOW(QChar symbol, QVector<QChar> &returnFollow);
  QMap<QChar, bool> readyFIRST;
  QMap<QChar, bool> readyFOLLOW;

  void createFIRST();
  void createFOLLOW();
  QMap<QChar, QVector<QChar>> firstMap;
  QMap<QChar, QVector<QChar>> followMap;

  QMap<QChar, QMap<QChar, QString>> LL_Maps;
  int step=0;
  QVector<QChar> deduplicationVectorQChar(QVector<QChar> &vec);
  void outputStep(QChar stackTopChar,QString nowInputStrChar);
  void outputFIRST(QChar sym);
  void outputFOLLOW(QChar sym);
};

#endif  // LL_1_H
