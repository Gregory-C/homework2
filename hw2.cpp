#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

struct Qclause{        //一个question
    string normQ;      //标准化表达
    string qu;         //问题的表达
    int ans;          //答案，可以是整形，浮点，或者string
    char oper;         //保存最后一次的操作符
};

class Qgenerator{
private:
    int quesNum;                         //题目的数量
    int operNum;                         //操作数的数量
    vector<char> operClass;              //运算符的种类
    bool negSupp;                        //是否支持负数运算
    bool dicFracSupp;                    //是否支持小数运算
    //bool fracSupp;                     //是否支持分数运算
    //bool expeSupp                      //是否支持指数运算
    pair<int, int> ansRange;             //题目及答案中的数值的范围

    int calculate(int num1, char oper, int num2);
    bool judge(int ans);
    void conbineNormQ(Qclause& clause, Qclause& clause1, Qclause& clause2);
    void combineQu(Qclause&clause, Qclause clause1, Qclause clause2);
    void getQuestion(int num, Qclause& clause);     //获取一道题

public:
    Qgenerator();             //构造函数
    void setting();           //读文件并更改设置
    void generate();          //产生题目并写文件
};

void Qgenerator::getQuestion(int num, Qclause& clause){
    if(num == 0){
        int number;
        int size = 100;
        number = (rand() % size);
        clause.qu = to_string(number);
        clause.ans = number;
        clause.normQ = clause.qu;
        clause.oper = ' ';                  //操作符设置为空格
        return;
    }
    int key = (rand() % num);               //get random number key in range(0,number-1)
    Qclause clause1, clause2;

    getQuestion(key, clause1);              //clause1 = getQuestion(key)
    getQuestion(num - key - 1, clause2);    //clause2 = getQuestion(num - key - 1)
    clause.oper = operClass[rand() % operClass.size()];    //get random operator
    clause.ans = calculate(clause1.ans, clause.oper, clause2.ans);    //calc answer
    bool judge(clause.ans);        //judge whether answer fits in our demands
    combineQu(clause, clause1, clause2);    //form question //通过operator判断是否需要加括号
    conbineNormQ(clause, clause1, clause2); //normalize question//先把运算符左右排序，再在最外面加一个大括号
}

int Qgenerator::calculate(int num1, char oper, int num2){
    int number;
    if (oper == '+'){
        number = num1 + num2;
    }
    else if (oper == '-'){
        number = num1 - num2;
    }
    else if (oper == '*'){
        number = num1*num2;
    }
    else if (oper == '/'){
        number = num1 / num2;
    }
    return number;
}

bool Qgenerator::judge(int ans) {
    //判断是否为需要的
    /*if(sum < 0)//若出现负数则改为加法
    {
        clause.qu = clause1.qu;
        clause.qu += '+';
        clause.qu += clause.qu;
        clause.ans = clause1.ans + clause2.ans;
        clause.oper = '+';
    }*/
    return true;
}

void Qgenerator::combineQu(Qclause&clause, Qclause clause1, Qclause clause2){
    if (clause.oper == '*' or clause.oper == '/'){
        if (clause1.oper == '+' or clause1.oper == '-'){
            clause.qu += '(';
            clause.qu += clause1.qu;
            clause.qu += ')';
        }
        else{
            clause.qu += clause1.qu;
        }
        clause.qu += clause.oper;

        if(clause2.oper != ' '){
            clause.qu += '(';
            clause.qu += clause2.qu;
            clause.qu += ')';
        }
        else{
            clause.qu += clause2.qu;
        }
    }
    //1+2+3 1+(2+3)
    else{
        clause.qu += clause1.qu;
        clause.qu += clause.oper;
        if (clause2.oper == '+' or clause2.oper == '-'){
            clause.qu += '(';
            clause.qu += clause2.qu;
            clause.qu += ')';
        }
        else {
            clause.qu += clause2.qu;
        }
    }
}

void Qgenerator::conbineNormQ(Qclause &clause, Qclause &clause1, Qclause &clause2) {
    if (clause1.normQ > clause2.normQ)      //小的排前面
        clause1.normQ.swap(clause2.normQ);
    clause.normQ = '(';
    clause.normQ += clause1.normQ;
    clause.normQ += clause.oper;
    clause.normQ += clause2.normQ;
    clause.normQ += ')';
}

Qgenerator::Qgenerator(void){
    quesNum = 10;
    operNum = 3;
    operClass.push_back('+');
    operClass.push_back('-');
    operClass.push_back('*');
    operClass.push_back('/');
    negSupp = false;
    dicFracSupp = false;
    ansRange.first = 0;
    ansRange.second = 100;
}

void Qgenerator::setting(){
    //read file
    //set variables
}

void Qgenerator::generate(){
    unordered_map<string, bool> clauseSet;
    for(int i = 0; i < quesNum; i++){
        Qclause temp;
        getQuestion(operNum, temp);             //获取一个问题
        auto iter = clauseSet.find(temp.normQ);
        if(iter == clauseSet.end()){            //若该问题没有重复
            clauseSet[temp.normQ] = true;       //存入map
            cout << temp.qu << " = " << temp.ans << endl;
            //write file
        }
        else{                                   //若已存在这个问题
            i--;                                //这次循环不算数
        }
    }
}

int main() {
    Qgenerator Q;
    Q.generate();
    return 0;
}
