#include <iostream>
#include "Boss1_Slum.h"
#include <vector>

using namespace std;

void OutputRandomizedChar(char *Array1,vector<char> v,int size){
    for(int i = 0;i < 3;i++){
        int randint = rand()%size;
       Array1[i] = v[randint];
    }
    
}
void checkCondition(char *CurrentArray){
    if((CurrentArray[0] == CurrentArray[1]) && (CurrentArray[1]== CurrentArray[2]) &&(CurrentArray[0]== CurrentArray[2])){
        cout << endl<<"Won"<<endl;



    }

}

void SpinSlot(int chips, char *RandomizedChar ,vector<char> BaseChar ,int vecsize){
    do{
    OutputRandomizedChar(RandomizedChar,BaseChar,vecsize);
    for(int i = 0;i < 3;i++){
        
        cout << RandomizedChar[i]<<" ";
    }
    checkCondition(RandomizedChar);
    cout << "\n" << "Current chip: " << chips <<endl;
    cin.get();
    chips--;
    }while(chips > 0);
}



int main() {
    //int result = otherFunction();
    //vector<char> BaseChar= {'A','B','C','D','E'};
    vector<char> BaseChar= {'A','B'};
    int chips = 10;
    int vecsize =  BaseChar.size();
    srand(time(0));
    char RandomizedChar[3] = {};

   SpinSlot(chips,RandomizedChar,BaseChar,vecsize);//input 1.chips 2.basechar 3.vecsize
    



    //cout << (BaseChar.at(0) == BaseChar.at(3));
}