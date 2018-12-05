#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

typedef struct BitBuffer{
    unsigned char ch;
    unsigned int size;
}BitBuffer;

typedef struct HuffNode{
     char ID;
     int weight;
     HuffNode *rightchd;
     HuffNode *leftchd;
}HuffNode;

typedef struct freqUnit{
    int Appear;
    char ID;
}freqUnit;

typedef struct codeUnit{
    char ID;
    string code;
}codeUnit;

BitBuffer bitbuffer;

FILE* outfp;

FILE* infp;

HuffNode* mainhead;

int Inhelp(){
    if(bitbuffer.size == 0){ bitbuffer.ch = fgetc(infp); bitbuffer.size = 8;}
    int bit = (bitbuffer.ch & 128) >> 7;
    bitbuffer.ch = bitbuffer.ch << 1;
    bitbuffer.size --;
    return bit;
}

void Outhelp(string dataout){
    for(int i = 0;i < dataout.size();i ++){
        if(dataout[i] == '1'){  bitbuffer.size ++; bitbuffer.ch = (bitbuffer.ch << 1) + 1;}
        if(dataout[i] == '0'){ bitbuffer.size ++; bitbuffer.ch = (bitbuffer.ch << 1) + 0;}

        if(bitbuffer.size == 8){
            fputc(bitbuffer.ch,outfp);
            bitbuffer.ch = 0;
            bitbuffer.size = 0;
        }
    }
}

double GetSize(string addr){
    FILE *ori = fopen(addr.c_str(),"rb");
    long prev=ftell(ori);
    fseek(ori, 0L, SEEK_END);
    long sz=ftell(ori);
    fseek(ori,prev,SEEK_SET);
    return sz;
}

void BuildingHelp(string tempcode,HuffNode* huffhead,vector<codeUnit> &huffcodes){
    if(huffhead -> rightchd != nullptr){ BuildingHelp( tempcode + "1",huffhead -> rightchd,huffcodes); }
    if(huffhead -> leftchd != nullptr){ BuildingHelp(tempcode + "0",huffhead -> leftchd,huffcodes); }
    else { codeUnit temp;  temp.ID = huffhead -> ID;  temp.code = tempcode; huffcodes.push_back(temp); }
}

string fileR(ifstream &filein){
    string instance;
    filein >> noskipws;

    while(!filein.eof()){
        char temp = NULL;
        filein >> temp;
        if(temp == NULL){  continue;  }
        instance += temp;
    }
    return instance;
}

void Attentioner(int source){

    unsigned char blaze = 0;
    queue<int> Bseq;
    cout << source << endl;

    while(source != 0){
        int i = source / 2;
        int left = source % 2;
        Bseq.push(left);
        source = i;
    }
    while(!Bseq.empty()){
        int temp = Bseq.front();
        if(temp == 1){blaze = (blaze << 1) + 1;}
        if(temp == 0){blaze = blaze << 1;}
        Bseq.pop();
    }
    fputc(blaze,outfp);

}

void fileOut(string instance,vector<codeUnit> huffcodes){
    outfp = fopen("f2_result.huf","wb+");
    bitbuffer.size = 0;
    bitbuffer.ch = 0;
    string dataout;
    for(int l = 0;l < instance.size();l ++){
        for(int j = 0;j < huffcodes.size(); j ++){  if(huffcodes[j].ID == instance[l]){ dataout = huffcodes[j].code;} }
        if(l == instance.size() - 1){  break; }
        Outhelp(dataout);
    }//如果要求缓存区满8位也要补八位进文件最后一个字符先不要写进去
    if(bitbuffer.size != 0){
        Attentioner(bitbuffer.size);
        for(int p = bitbuffer.size;p < 8;p ++){
            Outhelp("0");
        }
    }
    else{
        Attentioner(0);
        Outhelp(dataout);
    }
    fclose(outfp);
    cout << "文件写入完成 保存在与可执行文件同级的文件夹下" << endl;
}

vector<HuffNode*> SeqCensus(string instance){
    vector<freqUnit> freq;
    freqUnit temp;
    temp.Appear = 1;
    temp.ID = instance[0];
    freq.push_back(temp);
    bool isfound;
    for(int i = 1;i < instance.size();i ++){
        isfound = false;
        for(int o = 0;o < freq.size();o ++){
            if(instance[i] == freq[o].ID){ freq[o].Appear ++;  isfound = true; break;}
        }
        if(!isfound){
            freqUnit temp2;
            temp2.Appear = 1;
            temp2.ID = instance[i];
            freq.push_back(temp2);

        }
    }

    vector<HuffNode*> forest;
    for(int k = 0;k < freq.size();k ++){
        HuffNode* tempNode = (HuffNode*)malloc(sizeof(HuffNode));
        tempNode -> ID = freq[k].ID;
        tempNode -> weight = freq[k].Appear;
        tempNode -> rightchd = nullptr;
        tempNode -> leftchd = nullptr;
        forest.push_back(tempNode);
        if(freq[k].ID == '\n'){cout << "字符" << "\'\\n\'" << "的频数为" << freq[k].Appear << endl; continue;}
        cout << "字符" << '\''<<freq[k].ID << '\''<< "的频数为" << freq[k].Appear << endl;
    }
    return forest;
}

HuffNode* HuffBuilding(vector<HuffNode*> forest){
    HuffNode *temp1;
    HuffNode *temp2;
    while(forest.size() != 1){
        HuffNode *smallest = forest[0];
        vector<HuffNode*>::iterator it1 = forest.begin();
        for(int i = 0;i < forest.size(); i ++){ if(forest[i] -> weight < smallest -> weight){ smallest = forest[i]; it1 = forest.begin(); it1 += i; } }
        temp1 = smallest;
        forest.erase(it1);
        smallest = forest[0];
        it1 = forest.begin();
        for (int j = 0; j < forest.size(); ++j) { if(forest[j] -> weight < smallest -> weight){ smallest = forest[j]; it1 = forest.begin(); it1 += j;  } }
        temp2 = smallest;
        forest.erase(it1);
        HuffNode *temphead = (HuffNode*)malloc(sizeof(HuffNode));
        temphead -> weight = temp1 -> weight + temp2 -> weight;
        temphead -> ID = NULL;
        temphead -> leftchd = temp1;
        temphead -> rightchd = temp2;
        forest.push_back(temphead);
    }
    return forest[0];
}

void Decoding(string hufaddr){
    HuffNode* newhead = mainhead;
    HuffNode* fixhead = newhead;
     int  actsize = (int)GetSize(hufaddr) - 2;
    infp = fopen(hufaddr.c_str(),"rb");
    ofstream outfile;
    outfile.open("/Users/zhuyuanhao/Downloads/result.txt");
    for(int k = 0;k < actsize * 8;k ++){
        int path = Inhelp();
        if( path == 0){  newhead = newhead -> leftchd; if(newhead -> ID != NULL){if(newhead -> ID == '\r'){cout << endl; outfile << endl;}cout << newhead -> ID; outfile << newhead -> ID;newhead = fixhead;  }  continue;}
        if( path == 1){  newhead = newhead -> rightchd; if(newhead -> ID != NULL){if(newhead -> ID == '\r'){cout << endl; outfile << endl;}cout << newhead -> ID; outfile << newhead -> ID;newhead = fixhead;  }  continue;}
    }
    int getValid = fgetc(infp);
    bitbuffer.ch = 0;
    bitbuffer.size = 0;

    for(int o = 0;o < getValid;o ++){
        int path = Inhelp();
        if( path == 0){  newhead = newhead -> leftchd; if(newhead -> ID != NULL){if(newhead -> ID == '\r'){cout << endl; outfile << endl;}cout << newhead -> ID; outfile << newhead -> ID;newhead = fixhead;  }  continue;}
        if( path == 1){  newhead = newhead -> rightchd; if(newhead -> ID != NULL){if(newhead -> ID == '\r'){cout << endl; outfile << endl;}cout << newhead -> ID; outfile << newhead -> ID;newhead = fixhead;  }  continue;}

    }

    cout << endl;
    cout << "**************************************************************" << endl;
    cout << "解码完成" << endl;
}

void Coding(string addr){
    ifstream filein;
    filein.open(addr);
    string instance = fileR(filein);
    filein.close();
    if(instance.empty()){  cout << "输入文件为空" << endl; return ; }
    HuffNode *huffhead = HuffBuilding(SeqCensus(instance));
    mainhead = huffhead;
    vector<codeUnit> huffcodes;
    string temp;
    BuildingHelp(temp,huffhead,huffcodes);
    cout << "下面是字符的编码情况" << endl;
    cout << "**************************************************************" << endl;
    for(int k = 0;k < huffcodes.size();k ++){
        if(huffcodes[k].ID == '\n'){cout << "字符" << "\'"<<"\\n" << "\'" << "的编码为" << huffcodes[k].code << endl; continue;}
        cout << "字符" << "\'"<<huffcodes[k].ID << "\'" << "的编码为" << huffcodes[k].code << endl;
    }
    cout << "**************************************************************" << endl;


    fileOut(instance,huffcodes);

    cout << "文件压缩前大小为:" << GetSize(addr) << "字节" << endl;
    cout << "文件压缩后的大小为:" << GetSize("f2_result.huf") << "字节" << endl;
    cout << "文件压缩率为" << (1.0 - GetSize("f2_result.huf")/GetSize(addr)) * 100 << "%" << endl;
}

int main(){
    while(true) {
        cout << "**************************************************************" << endl;
        cout << "*********************霍夫曼编码树简单压缩程序*********************" << endl;
        cout << "**************************************************************" << endl;
        cout << "************************务必首先进行1号操作**********************" << endl;
        cout << "1.进行霍夫曼编码  2.进行霍夫曼码译码  3.退出" << endl;
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: {
                cout << "请输入待编码文件的绝对地址:";
                string addr;
                cin >> addr;
                cout << "**************************************************************" << endl;
                Coding(addr);
                break;
            }
            case 2: {
                cout << "请输入编码后的.huf文件绝对地址:";
                string hufaddr;
                cin >> hufaddr;
                cout << "**************************************************************" << endl;
                Decoding(hufaddr);
                break;
            }
            case 3:
                return 0;
            default:
                cout << "请输入正确选项" << endl;
                break;
        }
    }
}