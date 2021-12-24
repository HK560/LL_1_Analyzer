/*
 * @Author: HK560
 * @Date: 2021-12-22 10:20:16
 * @LastEditTime: 2021-12-24 09:46:25
 * @LastEditors: HK560
 * @Description: LL1文法分析实现
 * @FilePath: \LL_1_Analyzer\ll_1.cpp
 * My Blog: https://blog.hk560.top
 */

#include "ll_1.h"

LL_1::LL_1(QStringList rule, QString startSymbol, QString str) {
    this->ruleList = rule;
    this->str = str;
    this->startSymbol = startSymbol.at(0);
}

bool LL_1::createLLlist() {
    try {
        //读取到ruleMap里

        qDebug() << "funtion:createLList";
        for (auto i = ruleList.begin(); i != ruleList.end(); i++) {
            QString nowRule = *i;
            if (nowRule.isEmpty()) continue;
            QStringList leftRight =
                nowRule.split("->", QString::SkipEmptyParts);
            if (leftRight.size() != 2) throw QString("输入文法非法");
            QChar left = leftRight.first().at(0);
            QString right = leftRight.last();
            QStringList rightList = right.split('|', QString::SkipEmptyParts);
            if (ruleList.contains(left) || nonTerminatorVector.contains(left)) {
                throw QString("终结符重复，文法格式非法");
            }
            ruleMap.insert(left, rightList);
            readyFIRST.insert(left, false);
            readyFOLLOW.insert(left, false);
            nonTerminatorVector.append(left);
            qDebug() << left << "->" << ruleMap[left];
        }
        createFIRST();
        createFOLLOW();
    } catch (QString str) {
        QMessageBox::warning(NULL, "Warning", str);
        return false;
    }
    return true;
}

void LL_1::analysis(QString inputString) {
    QStack<QChar> symbolStack;
    step = 0;
    //符号栈
    auto itr_nowInputStrChar = inputString.begin();  //输入字符串的迭代器
    symbolStack.push('#');
    symbolStack.push('E');
    while (!symbolStack.isEmpty()) {
        QChar nowStackTopSymbol = symbolStack.top();
        symbolStack.pop();
        QChar nowInputStrSymbol = *itr_nowInputStrChar;
        int nowInputStrSymbolIndex =
            std::distance(inputString.begin(), itr_nowInputStrChar);
        //判断符号栈顶和当前指向的输入串字符是否相等
        if (nowStackTopSymbol == nowInputStrSymbol) {
            //相等时
            if (nowStackTopSymbol == '#') {  //如果都为#则匹配成功结束
                outputStep(nowStackTopSymbol,
                           inputString.mid(nowInputStrSymbolIndex));
                qDebug() << "Match successfully" << endl;
                emit sendStepInfo(QString("匹配结束"));
                QMessageBox::information(NULL, "Info", "匹配成功!");
                return;
            }
            if (itr_nowInputStrChar != inputString.end()) {
                outputStep(nowStackTopSymbol,
                           inputString.mid(nowInputStrSymbolIndex));
                qDebug() << "Next Char" << endl;
                itr_nowInputStrChar++;  //指向下一个输入串字符
            } else {
                qDebug() << "error!";
                QMessageBox::warning(NULL, "错误", "匹配错误");
                return;
            }
        }
        //符号栈顶和当前指向的输入串字符不相等时
        else {
            auto itr_stackTopSymbol = LL_Maps.find(nowStackTopSymbol);
            auto itr_inputStrSymbol =
                LL_Maps[nowStackTopSymbol].find(nowInputStrSymbol);
            //判断LL表是否存在当前符号栈顶和指向字符串字符对应规则是否存在
            if ((itr_stackTopSymbol != LL_Maps.end() ||
                 LL_Maps.end().key() == nowStackTopSymbol) &&
                (itr_inputStrSymbol != LL_Maps[nowStackTopSymbol].end() ||
                 LL_Maps[nowStackTopSymbol].end().key() == nowStackTopSymbol)) {
                QString nowRule =
                    LL_Maps[nowStackTopSymbol][nowInputStrSymbol];  //得到规则
                if (!nowRule.isEmpty()) {
                    if (nowRule.at(0) != '%') {
                        std::reverse(nowRule.begin(),
                                     nowRule.end());  //逆序字符串
                        //推入栈中
                        for (auto itr_nowRule = nowRule.begin();
                             itr_nowRule != nowRule.end(); itr_nowRule++) {
                            symbolStack.push(QChar(*itr_nowRule));
                        }
                    }
                    outputStep(nowStackTopSymbol,
                               inputString.mid(nowInputStrSymbolIndex));
                }
            }
            //对应规则不存在则报错
            else {
                qDebug() << "error!" << endl;
                QMessageBox::warning(NULL, "错误", "匹配错误");
                return;
            }
        }
    }
    qDebug() << "Error!" << endl;
}

bool LL_1::getFIRST(QChar symbol, QVector<QChar> &returnFirst) {
    Q_ASSERT(ruleMap.contains(symbol));
    QVector<QChar> FIRST;
    // FIRST.clear();
    QStringList nowSymbolRule = ruleMap[symbol];
    try {
        for (auto i = nowSymbolRule.begin(); i != nowSymbolRule.end(); i++) {
            QString nowRule = *i;
            QChar sym;
            // 1.
            sym = nowRule.at(0);
            if (!sym.isUpper()) {
                FIRST.append(sym);
                continue;
            }
            // 3.
            sym = nowRule.at(0);
            if (sym.isUpper()) {
                if (readyFIRST[sym]) {
                    Q_ASSERT(firstMap.contains(sym));
                    QVector<QChar> tmp = firstMap[sym];
                    tmp.removeOne('%');
                    FIRST.append(tmp);
                } else
                    return false;
            }
            // 4.
            for (int k = 1; k < nowRule.size(); k++) {
                QChar lastSym = nowRule.at(k - 1);
                QChar nSym = nowRule.at(k);
                if (!nSym.isUpper()) break;
                if (readyFIRST[lastSym]) {
                    if (firstMap[lastSym].contains('%')) {
                        Q_ASSERT(firstMap.contains(nSym));
                        if (readyFIRST[nSym]) {
                            QVector<QChar> tmp = firstMap[nSym];
                            tmp.removeOne('%');
                            FIRST.append(tmp);
                        } else
                            return false;
                    } else
                        break;
                }
                if (k == nowRule.size() && !FIRST.contains('%')) {
                    FIRST.append('%');
                }
            }
        }
    } catch (QString str) {
        QMessageBox::warning(NULL, "Waring",
                             QString("In getFirst():%1").arg(str));
    }
    // QSet<QVector<QChar>>ss=FIRST;
    returnFirst = deduplicationVectorQChar(FIRST);
    qDebug() << symbol << "'s FIRSET is ready";
    return true;
}

bool LL_1::getFOLLOW(QChar symbol, QVector<QChar> &returnFollow) {
    Q_ASSERT(!firstMap.isEmpty());
    Q_ASSERT(!nonTerminatorVector.isEmpty());
    QVector<QChar> FOLLOW;
    try {
        // 1.
        if (symbol == startSymbol) FOLLOW.append('#');
        // 2.
        for (auto i = ruleMap.begin(); i != ruleMap.end(); i++) {
            if (i.key() == symbol) continue;
            for (auto k = i.value().begin(); k != i.value().end(); k++) {
                QString nowRule = *k;
                if (nowRule.contains(symbol)) {
                    // 2.
                    QChar backSym = nowRule.back();
                    if (backSym != symbol &&
                        nowRule.at(nowRule.size() - 2) == symbol) {
                        if (!nonTerminatorVector.contains(backSym))
                            FOLLOW.append(backSym);
                        else {
                            QVector<QChar> add = firstMap[backSym];
                            add.removeOne('%');
                            FOLLOW.append(add);
                        }
                    }
                    // 3.
                    backSym = nowRule.back();
                    if (backSym == symbol ||
                        ((nowRule.at(nowRule.size() - 2) == symbol &&
                          firstMap[backSym].contains('%')) &&
                         backSym != symbol)) {
                        if (readyFOLLOW[i.key()] == true) {
                            FOLLOW.append(followMap[i.key()]);
                        } else
                            return false;
                    }
                }
            }
        }
    } catch (QString str) {
        QMessageBox::warning(NULL, "Waring",
                             QString("In getFOLLOW:%1").arg(str));
    }
    returnFollow = deduplicationVectorQChar(FOLLOW);
    qDebug() << symbol << "'s FOLLOW is ready";
    return true;
}

void LL_1::createFIRST() {
    Q_ASSERT(!ruleMap.isEmpty());
    Q_ASSERT(!nonTerminatorVector.isEmpty());
    int sum = nonTerminatorVector.size();
    int nowtime = 0;
    while (sum--) {
        for (auto i = nonTerminatorVector.begin();
             i != nonTerminatorVector.end(); i++) {
            if (readyFIRST[*i]) continue;
            QVector<QChar> nowFirst;
            if (getFIRST(*i, nowFirst)) {
                nowtime++;
                firstMap[*i] = nowFirst;
                readyFIRST[*i] = true;
                sum = nonTerminatorVector.size();
                qDebug() << *i << "FIRST:[" << firstMap[*i] << "]";
                outputFIRST(*i);
            }
        }
    }
    if (nowtime != nonTerminatorVector.size())
        QMessageBox::warning(NULL, "warning", "Create FIRST Failed!");
}

void LL_1::createFOLLOW() {
    Q_ASSERT(!ruleMap.isEmpty());
    Q_ASSERT(!nonTerminatorVector.isEmpty());
    Q_ASSERT(!readyFIRST.isEmpty());

    int sum = nonTerminatorVector.size();
    int nowtime = 0;
    while (sum--) {
        for (auto i = nonTerminatorVector.begin();
             i != nonTerminatorVector.end(); i++) {
            if (readyFOLLOW[*i]) continue;
            QVector<QChar> nowFollow;
            if (getFOLLOW(*i, nowFollow)) {
                nowtime++;
                followMap[*i] = nowFollow;
                readyFOLLOW[*i] = true;
                sum = nonTerminatorVector.size();
                qDebug() << *i << "FOLLOW:[" << followMap[*i] << "]";
                outputFOLLOW(*i);
            }
        }
    }
    if (nowtime != nonTerminatorVector.size())
        QMessageBox::warning(NULL, "warning", "Create FOLLOW Failed!");
}

QVector<QChar> LL_1::deduplicationVectorQChar(QVector<QChar> &vec) {
    std::sort(vec.begin(), vec.end());              //去重前需要排序
    auto it = std::unique(vec.begin(), vec.end());  //去除容器内重复元素
    vec.erase(it, vec.end());
    return vec;
}
//输出分析步骤
void LL_1::outputStep(QChar stackTopChar, QString nowInputStrChar) {
    QString outputText;
    outputText.append("步骤:");
    outputText.append(QString::number(++step));
    outputText.append(" 符号栈栈顶:");
    outputText.append(stackTopChar);
    outputText.append(" 字符串:");
    outputText.append(nowInputStrChar);
    qDebug() << outputText;
    emit sendStepInfo(outputText);
}

void LL_1::outputFIRST(QChar sym) {
    QString outputStr;
    outputStr.append(QString("FIRST(%1)=[").arg(sym));
    for (auto k = firstMap[sym].begin(); k != firstMap[sym].end(); k++) {
        outputStr.append(*k);
        outputStr.append(" ");
    }
    outputStr.append("]");
    emit sendStepInfo(outputStr);
}

void LL_1::outputFOLLOW(QChar sym) {
    QString outputStr;
    outputStr.append(QString("FOLLOW(%1)=[").arg(sym));
    for (auto k = followMap[sym].begin(); k != followMap[sym].end(); k++) {
        outputStr.append(*k);
        outputStr.append(" ");
    }
    outputStr.append("]");
    emit sendStepInfo(outputStr);
}

void LL_1::buildLLMaps() {
    // 2.
    for (auto i = ruleMap.begin(); i != ruleMap.end(); i++) {  //遍历每一个规则
        QVector<QChar> nowNonTerminatorFIRST =
            firstMap[i.key()];             //当前非终结符的FIRST集合
        QChar nowNonTerminator = i.key();  //当前非终结符符号
        for (auto k = i.value().begin(); k != i.value().end();
             k++) {  //对每一个当前非终结符的产生式
            QVector<QChar> ownFirstSym;  //拥有的首字符
            QString nowRule = *k;
            QChar firstSym = k->at(0);
            if (!firstSym.isUpper())
                ownFirstSym.append(firstSym);
            else
                ownFirstSym.append(firstMap[firstSym]);
            for (auto t = ownFirstSym.begin(); t != ownFirstSym.end();
                 t++) {  //对于每一个当前产生式拥有的首字符
                QChar nowSym = *t;
                if (nowSym != '%' && nowNonTerminatorFIRST.contains(
                                         nowSym)) {  //首字符与first匹配
                    LL_Maps[nowNonTerminator][nowSym] = nowRule;  //加入LL表
                    qDebug() << "MAP[" << nowNonTerminator << "][" << nowSym
                             << "]=" << LL_Maps[nowNonTerminator][nowSym];
                }
            }
        }
        // 3.
        if (nowNonTerminatorFIRST.contains('%')) {
            QVector<QChar> ownFOLLOW = followMap[nowNonTerminator];
            for (auto t = ownFOLLOW.begin(); t != ownFOLLOW.end(); t++) {
                QChar nowFOLLOWSym = *t;
                LL_Maps[nowNonTerminator][nowFOLLOWSym] = QString("%");
                qDebug() << "MAP[" << nowNonTerminator << "][" << nowFOLLOWSym
                         << "]=" << LL_Maps[nowNonTerminator][nowFOLLOWSym];
            }
        }
    }
}
