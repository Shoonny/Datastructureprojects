#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <stack>
using namespace std;


string mtosting(list<string> &text,int strap,bool isdel){
    list<string>::iterator iterator1;
    string out;
    iterator1 = text.begin();
    for(int i = 0;i < strap;i ++){ iterator1 ++;}
    for(int i = strap;i < text.size();i ++){ out += *(iterator1); iterator1 ++;}
    iterator1 = text.end();
    if(isdel){
       for(int i = text.size() - 1;i >= strap;i --){ text.pop_back();}
    }
    return out;

}

list<string> mtolist(string text){
    istringstream input(text);
    input>> noskipws;
    string temp;
    list<string> out;
    char Par;
    int k = text.size();
    while(true){
       if(k == 0){   return out;  }
        temp = "";
        for(int i = 0;i < 80;i ++){
            if(k <= 0){   break;  }
            input >> Par;
            if(Par == '\n'){
                k --;
                temp += Par;
                break;
            }
            temp += Par;
            k --;
        }

        out.push_back(temp);


    }
}

list<string> fileR(ifstream &filein){
    string temp;
    list<string> out;
    char Par;
    while(!filein.eof()){
        temp = "";
        for(int i = 0;i < 80;i ++){
            if(filein.eof()){
                break;
            }
            filein >> Par;
            if(Par == '\n'){
                temp += Par;
                break;
            }
            temp += Par;
        }

        out.push_back(temp);

    }
    return out;
}

void Display(list<string> &text){

    list<string>::iterator itor;
    itor = text.begin();
    for(int i = 0;i < text.size();i ++) {
        cout << *itor ++ << endl;
    }

}

void insert(list<string> &text){
    cout << "输入要插入的部分所在的行（从0开始以此类推）：" << endl;
    int inrow;
    int incol;
    cin >> inrow;
    if(inrow > text.size()){ cout << "数据不合法" << endl;  return; }

    cout << "请输入要插入的部分所在的列" << endl;
    cin >> incol;
    if(incol > 80){ cout << "数据不合法" << endl;  return;  }

    string input;
    cout << "现在输入要插入的字符串" << endl;
    cin >> input;

    string process = mtosting(text,inrow, true);
    process.insert(incol ,input);
    list<string>:: iterator it;
    it = text.begin();
    for(int i = 0;i <= inrow - 1;i ++){ it ++;}

    text.splice(it,mtolist(process));




}

bool Search(list<string> &text,string input){
    string temp = mtosting(text,0,false);

    int act_size = temp.size();
    bool found = false;
    bool notfound = false;
    int counter = 0;
    list<string>:: iterator it = text.begin();

    for(int i = 0;i < act_size;i ++){
        if((counter == (*it).size() && i != act_size - 1) || (*it)[counter] == '\n'){     it ++; counter = 0;    }
        if((*it)[counter] == input[0]){
            list<string>::iterator substi = it;
            int subi = counter;
            found = true;
            int strapback = 0;
            for(int o = 0;o < input.size();o ++){
                if(subi + o == (*substi).size()|| (*substi)[subi] == '\n'){
                    strapback += (*substi).size();
                    substi ++;
                }
                if((*substi)[(subi + o) - strapback] != input[o]){
                    found = false;
                    cout << "mismatch at" << subi + o << endl;
                    break;
                }
            }
            if(found){
                cout << "找到了输入的字符串"<< "位于"
                << distance(text.begin(),it) <<"行"
                << subi <<"列"<< endl;
                notfound = true;
            }
        }
        counter ++;

    }
    if(!notfound){  cout << "未找到输入的字符串" << endl;  return false;}
    else{    return true;   }
}

void Del(list<string> &text){
    cout<< "请输入要删除的字符串" << endl;
    string input;
    cin >> input;
    if(!Search(text,input)){    return;   }
    int choice = 0;
    cout << "请选择是删除第一个还是删除全部出现过的" <<endl;
    cout << "1.删除第一个     2.删除全部" << endl;
    cin >> choice;
    bool found = false;
    bool isALL = false;
    switch(choice){
        case 1:{
            break;
        }
        case 2:{
            isALL = true;
            break;
        }
        default:
        {
            cout << "不合法输入" << endl;
            return;
        }
    }
    string temp = mtosting(text,0,true);
    for(int i = 0;i < temp.size();i ++){
        if(temp[i] == input[0]){
            found = true;
            for(int o = 0;o < input.size();o ++){
                if(temp[i + o] != input[o]){   found = false;  break;}
            }
            if(found){

                temp.erase(i,input.size());
                found = false;
                i --;
                if(!isALL){  text = mtolist(temp);return;  }
            }
        }
    }
    text = mtolist(temp);

}

void Save(list<string> &text,string Address){
    ofstream outprint;
    outprint.open(Address);
    list<string>::iterator iterator1;
    iterator1 = text.begin();
    for(int i = 0;i < text.size();i ++){
        outprint << *iterator1;
        iterator1 ++;
    }
    outprint.close();
}

int main() {
    ifstream filein;
    list<string> text;
    cout << "ASCII文本编辑器" << endl;
    cout << "实现了显示，插入，删除，查找，以及保存的功能" << endl;
    cout << "请输入文件所在的绝对路径" << endl;
    string Address;
    cin >> Address;
	
    filein.open(Address);
    filein >> noskipws;
    text = fileR(filein);
    filein.close();

    while(true) {

        cout << "请选择要执行的操作：" << endl;
        cout << "1 显示  2 插入 3 删除 4 查找 5 保存 6 退出" << endl;
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                Display(text);
                break;
            }
            case 2: {
                insert(text);
                break;
            }
            case 3: {
                Del(text);
                break;
            }
            case 4: {
                cout << "请输入要查找的字符串" << endl;
                string input;
                cin >> input;
                Search(text,input);
                break;
            }
            case 5: {
                Save(text,Address);
                break;
            }
            case 6: {
                return 0;
            }
            default: {
                cout << "请输入合适的选项" << endl;
                break;
            }



        }
    }
}
