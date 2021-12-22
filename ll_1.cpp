#include "ll_1.h"
#pragma execution_character_set("utf-8")
LL_1::LL_1(QStringList rule, QString startSymbol,QString str) {
    this->ruleList = rule;
    this->str = str;
    this->startSymbol = startSymbol.at(0);
}

bool LL_1::createLLlist() {
    try {
        //读取到ruleMap里
        
        qDebug()<<"funtion:createLList";
        for (auto i = ruleList.begin(); i != ruleList.end(); i++) {
            QString nowRule = *i;
            if (nowRule.isEmpty())
                continue;
            QStringList leftRight =
                nowRule.split("->", QString::SkipEmptyParts);
            if (leftRight.size() != 2) throw "leftRight's size error!";
            QChar left = leftRight.first().at(0);
            QString right = leftRight.last();
            QStringList rightList = right.split('|', QString::SkipEmptyParts);
            if (ruleList.contains(left) || terminatorVector.contains(left)) {
                throw QString("终结符重复，文法格式非法");
            }
            ruleMap.insert(left, rightList);
            readyFIRST.insert(left,false);
            readyFOLLOW.insert(left,false);
            terminatorVector.append(left);
            qDebug()<<left<<"->"<<ruleMap[left];
            // debug
        }
        createFIRST();
        createFOLLOW();
        // culi
    } catch (QString str) {
        QMessageBox::warning(NULL, "Warning", str);
        return false;
    }
    return true;
}

void LL_1::analysis() {}

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

            // 2.
            // 1. 中实现
            // sym = nowRule.at(0);
            // if (sym == "%")
            // {
            //     FIRST.append(sym);
            //     break;
            // }

            // 3.
            sym = nowRule.at(0);
            if (sym.isUpper()) {
                if (readyFIRST[sym]) {
                    Q_ASSERT(firstMap.contains(sym));
                    QVector<QChar>tmp = firstMap[sym];
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
                            QVector<QChar>tmp = firstMap[nSym];
                            tmp.removeOne('%');
                            FIRST.append(tmp);
                        }
                        else return false;
                    } else
                        break;
                }
                // if (getFIRST(lastSym).contains('%')) {
                //     FIRST.append(getFIRST(nSym).removeOne('%'));
                // }
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
    qDebug()<<symbol<<"'s FIRSET is ready";
    return true;
}

bool LL_1::getFOLLOW(QChar symbol, QVector<QChar> &returnFollow) {
    Q_ASSERT(!firstMap.isEmpty());
    Q_ASSERT(!terminatorVector.isEmpty());
    QVector<QChar> FOLLOW;
    try {
        // 1.
        if (symbol == startSymbol) FOLLOW.append('#');
        // 2.
        for (auto i = ruleMap.begin(); i != ruleMap.end(); i++) {
            if(i.key()==symbol) continue;
            for (auto k = i.value().begin(); k != i.value().end(); k++) {
                QString nowRule = *k;
                if (nowRule.contains(symbol)) {
                    // 2.
                    QChar backSym = nowRule.back();
                    if (backSym != symbol &&
                        nowRule.at(nowRule.size() - 2) == symbol) {
                        if (!terminatorVector.contains(backSym))
                            FOLLOW.append(backSym);
                        else{
                            QVector<QChar> add=firstMap[backSym];
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
                    // if(backSym!=symbol&&nowRule.at(nowRule.size()-2)==symbol&&getFIRST(backSym).contains('%')){
                }
            }
        }
    } catch (QString str) {
        QMessageBox::warning(NULL, "Waring",
                             QString("In getFOLLOW:%1").arg(str));
    }
    
    returnFollow = deduplicationVectorQChar(FOLLOW);
    qDebug()<<symbol<<"'s FOLLOW is ready";
    return true;
}

void LL_1::createFIRST() {
    Q_ASSERT(!ruleMap.isEmpty());
    Q_ASSERT(!terminatorVector.isEmpty());

    int sum = terminatorVector.size();
    int nowtime = 0;
    while (sum--) {
        for (auto i = terminatorVector.begin(); i != terminatorVector.end();
             i++) {
            
            if(readyFIRST[*i])
                continue;
            QVector<QChar> nowFirst;
            if (getFIRST(*i, nowFirst)) {
                nowtime++;
                firstMap[*i] = nowFirst;
                readyFIRST[*i]=true;
                sum = terminatorVector.size();
                qDebug()<<*i<<"FIRST:["<<firstMap[*i]<<"]";
            }
        }
    }
    if(nowtime!=terminatorVector.size())
        QMessageBox::warning(NULL,"warning","Create FIRST Failed!");
}

void LL_1::createFOLLOW() {
    Q_ASSERT(!ruleMap.isEmpty());
    Q_ASSERT(!terminatorVector.isEmpty());
    Q_ASSERT(!readyFIRST.isEmpty());

    int sum = terminatorVector.size();
    int nowtime = 0;
    while (sum--) {
        for (auto i = terminatorVector.begin(); i != terminatorVector.end();
             i++) {
            if(readyFOLLOW[*i])
                continue;
            QVector<QChar> nowFollow;
            if (getFOLLOW(*i, nowFollow)) {
                nowtime++;
                followMap[*i] = nowFollow;
                readyFOLLOW[*i]=true;
                sum = terminatorVector.size();
                qDebug()<<*i<<"FOLLOW:["<<followMap[*i]<<"]";
            }
        }
    }
    if(nowtime!=terminatorVector.size())
        QMessageBox::warning(NULL,"warning","Create FOLLOW Failed!");
}


QVector<QChar> LL_1::deduplicationVectorQChar(QVector<QChar> &vec){
    std::sort(vec.begin(),vec.end());     //去重前需要排序
    auto it= std::unique(vec.begin(),vec.end());   //去除容器内重复元素
    vec.erase(it,vec.end());
    return vec;
}