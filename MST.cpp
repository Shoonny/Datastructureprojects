#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef struct Edge{
    int v1;
    int v2;
    int weight;
}Edge;

Edge OriTree[50][50];

bool isAkin(int mv1,int mv2,vector<int> vSet[50]){
    for(int i = 0;i < 49;i ++){
        for(int l = 0;l < vSet[i].size();l ++){
            if(vSet[i][l] == mv1){
                for(int k = 0;k < vSet[i].size();k ++){
                    if(vSet[i][k] == mv2){ return true;  }
                }
            }
        }
    }
    return false;
}

void fileR(ifstream &filein){
    int v1;
    int v2;
    int weight;

    for(int i = 0;i < 49; i ++){
        for(int k = 0;k < 49;k ++){ OriTree[i][k].weight = 0; OriTree[i][k].v1 = i;OriTree[i][k].v2 = k; }
    }
    while(!filein.eof()){
        filein >> v1 >> v2 >> weight;
        OriTree[v1][v2].weight = weight;
    }
}

vector<Edge> Merge(vector<Edge> es1,vector<Edge> es2){
    vector<Edge> out;
    for(int k = 0;k < es1.size();k ++){   out.push_back(es1[k]);  }
    for(int l = 0;l < es2.size();l ++){   out.push_back(es2[l]);  }
    return out;
}

int PosOf(int v1,int v2,vector<vector<Edge>> EdgeSet) {
    if (v2 == -1) {
        for (int a = 0; a < EdgeSet.size(); a++) {
            for (int b = 0; b < EdgeSet[a].size(); b++) {
                if (EdgeSet[a][b].v2 == v1 && EdgeSet[a][b].v1 == v1) { return a; }
            }
        }
    }
    for (int a = 0; a < EdgeSet.size(); a++) {
        for (int b = 0; b < EdgeSet[a].size(); b++) {
            if (EdgeSet[a][b].v1 == v1 && EdgeSet[a][b].v2 == v2) { return a; }
        }
    }
    return -1;
}

int Find(int mv,vector<int> vSet[50]){
    for(int i = 0;i < 49;i ++){
        for(int j = 0;j < vSet[i].size();j ++){ if(vSet[i][j] == mv) return i;}
    }
    return -1;
}

vector<int> GetCluster(int mv,vector<int> vSet[50]){
    for(int i = 0;i < 49;i ++){
        for(int j = 0;j < vSet[i].size();j ++){
            if(vSet[i][j] == mv){
                vector<int> out;
                for(int o = 0;o < vSet[i].size();o ++){
                    out.push_back(vSet[i][o]);
                }
                return out;
            }
        }
    }
}

bool SearchHelp(int tar,vector<Edge> Edges){
    for(int i = 0;i < Edges.size();i ++){
        if(Edges[i].v1 == tar || Edges[i].v2 == tar){ return true;}
    }
    return false;
}

int GetCluPos(vector<int> mclu,vector<vector<Edge>> EdgeSet){
    bool found = false;
    for(int i = 0;i < EdgeSet.size(); i ++){
        for(int k = 0;k < mclu.size();k ++){
                if(SearchHelp(mclu[k],EdgeSet[i])){ found = true; }
                else { found = false; break;}
            }
            if(found){ return i; }
    }

    return -1;
}

void Kruskal() {
    vector<vector<Edge>> EdgeSet;
    vector<int> vSet[50];
    for (int o = 0; o < 49; o ++) {
        for (int h = 0; h < 49; h ++) {
            if (OriTree[o][h].weight != 0) {
                vector<Edge> temp;
                temp.push_back(OriTree[o][h]);
                EdgeSet.push_back(temp);
            }
        }
    }
    for(int p = 0;p < 49;p ++){ vSet[p].push_back(p);}

    while(true) {
        int smallest = 6000;
        int mv1 ;
        int mv2 ;
        for (int i = 0; i < 49; i++) {
            for (int k = 0; k < 49; k++) {
                if (OriTree[i][k].weight != 0 && OriTree[i][k].weight < smallest) {
                    smallest = OriTree[i][k].weight;
                    mv1 = i;
                    mv2 = k;
                }
            }
        }
        if(smallest == 6000){  break;  }
        Edge temp = OriTree[mv1][mv2];
        bool Akin;
        OriTree[mv1][mv2].weight = 0;
        if(isAkin(mv1,mv2,vSet) && isAkin(mv2,mv1,vSet)){ Akin = true; }
        else Akin = false;
        if(!Akin){ //将这条边连接的两个树连接起来
            int pos1 = Find(mv1,vSet);
            int pos2 = Find(mv2,vSet);
            vector<int> mv1clu = GetCluster(mv1,vSet);
            vector<int> mv2clu = GetCluster(mv2,vSet);
            for(int p = 0;p < vSet[pos1].size();p ++){ vSet[pos2].push_back(vSet[pos1][p]);}
            vSet[pos1].clear();
            if(vSet[pos2].size() < 3){ continue ;}
            int currentE = PosOf(temp.v1,temp.v2,EdgeSet);
            EdgeSet[currentE].clear();
            //先找mv1mv2各属哪个组，然后在边集把组对于位置下标找到，融合
            int m1E = GetCluPos(mv1clu,EdgeSet);
            int m2E = GetCluPos(mv2clu,EdgeSet);

            EdgeSet[m1E] = Merge(EdgeSet[m1E],EdgeSet[m2E]);
            EdgeSet[m1E].push_back(temp);
            EdgeSet[m2E].clear();
        }
        else{  continue; }
    }
    int counter = 0;
    int largest = 0;
    for(int l = 0; l < EdgeSet.size();l ++){
        if(EdgeSet[l].size() > largest){ largest = EdgeSet[l].size(); }
    }
    for(int i = 0 ;i < EdgeSet[largest].size();i ++){ cout << EdgeSet[largest][i].v1 << " "<< EdgeSet[largest][i].v2 << " " << EdgeSet[largest][i].weight << endl; counter += EdgeSet[largest][i].weight;}
    cout << counter;
}

int main(){
    string addr;
    cout << "*************简单的MST生成程序*************" << endl;
    cout << "请输入来源文件的绝对地址:";
    cin >> addr;
/*
 * 如果给节点的总个数再说
 */
    ifstream filein;
    filein.open(addr);
    fileR(filein);
    Kruskal();
}