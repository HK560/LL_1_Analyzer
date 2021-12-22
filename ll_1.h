#ifndef LL_1_H
#define LL_1_H

#include <QDebug>
#include <QMap>
#include <QMessageBox>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>
#include <algorithm>
#include<QStack>
class LL_1 {
 public:
  LL_1(QStringList rule, QString startSymbol, QString str);
  bool createLLlist();
  void buildLLMaps();
  void analysis();


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

  QVector<QChar> deduplicationVectorQChar(QVector<QChar> &vec);
  void outputStep(QChar stackTopChar,QString nowInputStrChar);
};

#endif  // LL_1_H
