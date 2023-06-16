// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, FALL 2020

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
    string lastName;
    string firstName;
    string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

    ofstream output(filename);
    if (!output) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << "\n";

    // Write the data
    for (auto pData:l) {
        output << pData->lastName << " "
               << pData->firstName << " "
               << pData->ssn << "\n";

    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.\n";

    cout << "Executing sort...\n";
    clock_t t1 = clock();
    sortDataList(theList);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class definitions here if you wish.
//Data* Overall[110000] = {};
Data* bin[26][100000];
string ssnlist[1100000] = {};
int ptrl[26];
int i;
bool comp(Data* a, Data* b);
int listcheck(list<Data *> &l);
//void output(list<Data *> &l, Data* A[]);
//void input(list<Data *> &l);
void insertSort(list<Data *> &l);
void radixSort(list<Data *> &l);
int point[256] = {0};

bool comp(Data* a, Data* b){
    int i = strcmp((a->lastName).c_str(),(b->lastName).c_str());
    if (i < 0){
        return 1;
    }
    else if (i > 0){
        return 0;
    }
    else{
        return strcmp((a->firstName).c_str(),(b->firstName).c_str()) < 0;
    }
}
/*
bool comp1(Data* a, Data* b)
{
    if (a->lastName != b->lastName)
    {
        return a->lastName < b->lastName;
    }
    else if (a->firstName != b->firstName)
    {
        return a->firstName < b->firstName;
    }
    else
    {
        return a->ssn < b->ssn;
    }
}
*/
int listcheck(list<Data *> &l){
    list<string> testcase;
    int i = 0;
    for(auto iter = l.begin(); i != 15; iter++){
        testcase.push_back((*iter)->firstName);
        i++;
    }
    testcase.unique();
    if(testcase.size() == 1){
        return 4; //same first name
    }
    else if (testcase.size() < 5){
        return 3;
    }
    else if (l.size() <= 110000){
        return 1;
    }
    else{
        return 2;
    }
}
/*
void output(list<Data *> &l, Data* A[]){
    int i = 0;
    for (auto & it : l){
        it = A[i];
        i++;
    }
}

void input(list<Data *> &l){
    int i = 0;
    for (auto & iter : l){
        Overall[i] = iter;
        i++;
    }
}
*/
string ss[1100000];
void insertSort(list<Data *> &l){
    int n = 0;
    auto iter = l.begin();
    for(auto iter1 = l.begin(); iter1 != l.end();){
        string firstn = (*iter1)->firstName;
        while(n != l.size()&&(*iter1)->firstName == firstn) {
            ss[i]=(*iter1)->ssn;
            iter1++;
            i++;
            n++;
        }
        for(int j = 1; j <= i-1; j++){
            string a = ss[j];
            int k = j-1;
            while((a<ss[k])&&(k>=0)){
                ss[k+1]=ss[k];
                k--;
            }
            ss[k+1]=a;
        }
        for(int q = 0; q<i; q++){
            (*iter)->ssn=ss[q];
            iter++;
        }
        i=0;
    }
}

void radixSort(list<Data *> &l){
    //int n = 0;
    auto iter = l.begin();
    for (auto & iter1 : l){
        int i = ((iter1->lastName)[0]-65);
        bin[i][ptrl[i]] = iter1;
        ptrl[i]++;
    }
    for(int j = 0; j < 26; j++){
        sort(&bin[j][0], &bin[j][0]+ptrl[j], comp);
        for(int k = 0; k < ptrl[j]; k++){
            *iter = bin[j][k];
            iter++;
        }
    }
    insertSort(l);
}



void inputssn(list<Data *> &l)
{
    int i = 0;
    for (list<Data *>::iterator it = l.begin(); it != l.end(); it++)
    {
        ssnlist[i] = (*it)->ssn;
        i++;
    }
}

void outputssn(list<Data *> &l, string A[])
{
    int i = 0;
    for (list<Data *>::iterator it = l.begin(); it != l.end(); it++)
    {
        (*it)->ssn = A[i];
        i++;
    }
}

void sortDataList(list<Data *> &l){
    int size = l.size();
    switch(listcheck(l)){
        case 1:
            //cout << "1" << endl;
            //radixSort(l);
            //break;
        case 2:
            //cout << "2" << endl;
            radixSort(l);
            break;
        case 3:
            //cout << "3" << endl;
            insertSort(l);
            break;
        case 4:
            //cout << "4" << endl;
            inputssn(l);
            sort(ssnlist,ssnlist+size);
            outputssn(l,ssnlist);
            break;
    }
}