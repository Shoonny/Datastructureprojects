#include <iostream>
#include <stack>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <queue>
#include <math.h>

using namespace std;
int Cal(int moptwo,int mopone,char opertaor){
    switch(opertaor){
        case '+':
            return mopone + moptwo;

        case '-':
            return mopone - moptwo;
        case '*':
            return mopone * moptwo;
        case '/':
            return mopone / moptwo;
        case '%':
            return mopone % moptwo;
        case '^':
            return pow(mopone,moptwo);
    }
}

int ReversePoCal(string instance){
    int i = 0;
    stack <int> Operand;
    string ForNum;
    while(i < instance.size()){

        if(instance[i] <= '9' && instance[i] >= '0') {
            while (instance[i] <= '9' && instance[i] >= '0') {
                ForNum += instance[i];
                i++;

            }
            istringstream is(ForNum);
            int temp;
            is >> temp;
            Operand.push(temp);
            ForNum.clear();
        }
       
        switch(instance[i]){
            case '-':

            case '+':



            case '*':

            case '/':
            case '%':

            case '^':
                    int one ;
                    one = Operand.top();
                    Operand.pop();
                    int two;
                    two = Operand.top();
                    Operand.pop();
                    Operand.push(Cal(one,two,instance[i]));
                    i ++;
                    break;
            default:
                i ++;
                break;

        }
    }
    return Operand.top();

}
void Processing(string instance,bool iffileout,ofstream &out){
    stack <char>Operator;
    queue <string>ReversePo;
    stack <int>Operand;
    string ForNum;


        cout << "输入的中缀表达式为" << instance << endl;

        int i = 0;
        while(i < instance.size()){

            if(instance[i] <= '9' && instance[i] >= '0') {
                while (instance[i] <= '9' && instance[i] >= '0') {
                    ForNum += instance[i];
                    i++;

                }
                ReversePo.push(ForNum);
                istringstream is(ForNum);
                int temp;
                is >> temp;
                Operand.push(temp);
                ForNum.clear();
            }
            if(Operator.empty()){

                Operator.push(instance[i]);
                i ++;
                continue;
            }

            switch(instance[i]){
                case '-':
                case '+':
                    if(Operator.top() == '*' || Operator.top() == '/' || Operator.top() == '+' || Operator.top() == '-'|| Operator.top() == '%'|| Operator.top() == '^')
                    {   string temp  ;
                        temp += Operator.top();
                        if(!Operand.empty()){
                            int mopone = Operand.top();
                            Operand.pop();
                            int moptwo = Operand.top();
                            Operand.pop();
                            Operand.push(Cal(mopone,moptwo,Operator.top()));
                        }
                        ReversePo.push(temp);
                        Operator.pop();
                    }

                    Operator.push(instance[i]);
                    i ++;
                    break;

                case '*':
                case '/':
                case '%':
                    if(Operator.top() == '*' || Operator.top() == '/' || Operator.top() == '%'|| Operator.top() == '^'){
                        string temp ;
                        temp  += Operator.top();
                        if(!Operand.empty()){
                            int mopone = Operand.top();
                            Operand.pop();
                            int moptwo = Operand.top();
                            Operand.pop();
                            Operand.push(Cal(mopone,moptwo,Operator.top()));
                        }
                        ReversePo.push(temp);
                        Operator.pop();
                    }
                    Operator.push(instance[i]);
                    i ++;
                    break;
                case '^':if(Operator.top() == '^'){
                    string temp;
                    temp += Operator.top();
                        if(!Operand.empty()){
                            int mopone = Operand.top();
                            Operand.pop();
                            int moptwo = Operand.top();
                            Operand.pop();
                            Operand.push(Cal(mopone,moptwo,Operator.top()));
                        }
                    ReversePo.push(temp);
                    Operator.pop();
                }
                    Operator.push(instance[i]);
                    i ++;
                    break;
                case '(':
                    Operator.push(instance[i]);
                    i ++;
                    break;
                case ')':
                    while(Operator.top() != '('){
                        string temp;
                        temp += Operator.top();
                        if(!Operand.empty()){
                            int mopone = Operand.top();
                            Operand.pop();
                            int moptwo = Operand.top();
                            Operand.pop();
                            int test = Cal(mopone,moptwo,Operator.top());
                            Operand.push(Cal(mopone,moptwo,Operator.top()));
                        }
                        ReversePo.push(temp);
                        Operator.pop();
                    }
                    Operator.pop();
                    i ++;
                    break;
                default:
                    i ++;
                    break;

            }
        }


        if(!ForNum.empty()){
            ReversePo.push(ForNum);
            istringstream is(ForNum);
            int temp;
            is >> temp;
            Operand.push(temp);
            ForNum.clear();
        }



        while(!Operator.empty()){
            string temp ;
            temp += Operator.top();
            if(!Operand.empty()){
                int mopone = Operand.top();
                Operand.pop();
                int moptwo = Operand.top();
                Operand.pop();
                Operand.push(Cal(mopone,moptwo,Operator.top()));
            }
            ReversePo.push(temp);
            Operator.pop();
        }



            cout << "转换后的的后缀表达式为";
                string temp;
                string tempforcal;
            while (!ReversePo.empty()) {
                temp = temp + ReversePo.front() + " ";
                tempforcal = temp;
                cout << ReversePo.front() << " ";
                ReversePo.pop();

            }
            if(iffileout) {

                     out << temp << endl;
                }

            cout << endl;
            cout << "中缀运算结果为" << Operand.top() << endl;
            cout << "后缀运算结果为" << ReversePoCal(tempforcal) << endl;
            cout << endl;



}


int main() {
    while(true) {
        cout << "**************************************" << endl;
        cout << "*  计算中缀表达式并且转换为后缀表达式      *" << endl;
        cout << "*  1.用文件输入  2.用终端输入   3.退出   *" << endl;
        cout << "**************************************" << endl;
        int choice;

        ofstream out;
        cin >> choice;
        switch(choice){
            case 1:
                {
                    cout << "请输入文件的绝对路径：" << endl;
                    string Path;
                    string Line;
                    cin >> Path;
                    ifstream infile;
                    infile.open(Path);
                    cout << "请输入保存文件的绝对路径" << endl;
                    string address;
                    cin >> address;
                out.open(address);

                while (getline(infile, Line) && Line != "") {
                        Processing(Line,true,out);
                    }

                infile.close();
                break;
            }
            case 2:
                {
                cout << "请输入中缀表达式不用输入等号" << endl;
                string TLine;
                cin >> TLine;
                Processing(TLine ,false,out);
                break;
            }
            case 3:
                return 0;
            default:
                cout << "请输入存在的选项" << endl;
                break;
        }
    }

}

