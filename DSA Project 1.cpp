/*
    Ruxi (Sophia) Xu
    ECE-264 DSA 1 Project 1
    This program manipulates stacks and queues.
    The program ask user for name of input file which contains list of commands.
    The input command should be asking program to create list of stack/queue, push, or pop values.
    The program will then process the commands and ask user for output file name.
    The program will output the results of commands from input file.
    In the output file, the program will tell users the results of input.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <list>
#include <iterator>
#include <cstdlib>
#include <vector>

using namespace std;

ifstream InputFile;
ofstream OutputFile;

//--------------------SIMPLE LIST CLASS--------------------
//Base class
template <typename T>
class SimpleList{
public:
    string GetListName();
    string ListName;
    bool Empty();
    virtual void push(T value) = 0;
    virtual T pop() = 0;
    //Constructor for SimpleList
    explicit SimpleList(string name){
        ListName = name;
        head = NULL;
        tail = NULL;
    }
private:
    class Node{
    public:
        T data;
        Node *next;
        Node(T value, Node* pointer){
            data = value;
            next = pointer;
        }
    };
    Node *head;
    Node *tail;
protected:
    void PushHead(T data);
    void PushTail(T data);
    T PopHead();
};

//List declarations (provided)
list<SimpleList<int> *> listSLi; // all integer stacks and queues
list<SimpleList<double> *> listSLd; // all double stacks and queues
list<SimpleList<string> *> listSLs; // all string stacks and queues

//Get list name
template <typename T>
string SimpleList<T>::GetListName() {
    return ListName;
}

//Check existence of list
template <typename T>
SimpleList<T>* NameCheck(list<SimpleList<T> *> &ListOfList, string ListName){
    for (typename list<SimpleList<T> *>::const_iterator iter = ListOfList.begin(); iter != ListOfList.end(); iter++){
        if ((*iter)->GetListName().compare(ListName) == 0){
            return *iter;
        }
    }
    return NULL;
}

//Check if the list is empty
//List will be empty if the head and tail node are NULL
template <typename T>
bool SimpleList<T>::Empty(){
    return (head == NULL && tail == NULL);
}

//Push value to the head of list
//Create a new Node and put data in, replace the head with new Node
template <typename T>
void SimpleList<T>::PushHead(T data) {
    Node *newN = new Node(data, head);
    head = newN;
}

//Push value to the tail of the list
//If tail is NULL, which means list is empty, replace head with new Node
//Else, let the next one on the node be the new Node, put data there
template <typename T>
void SimpleList<T>::PushTail(T data) {
    Node *newN = new Node(data, NULL);
    if (tail == NULL) {
        head = newN;
    } else {
        tail->next = newN;
    }
    tail = newN;
}

//Pop value from the head of the list
//If head is empty, return empty list
template <typename T>
T SimpleList<T>::PopHead(){
    if (head == NULL){
        return NULL;
    }
    Node *newN = head;
    head = newN->next;
    T data = newN->data;
    if (head == NULL) {
        tail = NULL;
    }
    delete newN;
    return data;
}

//--------------------STACK--------------------
//Stack: LIFO, push and pop from the same end

//Stack class
template <typename T>
class Stack : public SimpleList<T>{
public:
    //Constructor of stack
    explicit Stack(const string& StackName):SimpleList<T>(StackName) {
    }
    void push(T value) override;
    T pop() override;
};

//Push value into stack list
template <typename T>
void Stack<T>::push(T value){
    SimpleList<T>::PushHead(value);
}

//Pop value from head of the stack
template <typename T>
T Stack<T>::pop(){
    return Stack<T>::PopHead();
}

//--------------------QUEUE--------------------
//Queue: Push from the end, pop from the beginning

//Queue Class
template <typename T>
class Queue : public SimpleList<T>{
public:
    //Constructor
    explicit Queue(const string& QueueName):SimpleList<T>(QueueName){
    }
    void push(T value) override;
    T pop() override;
};

//Push value from tail of the queue
template <typename T>
void Queue<T>::push(T value) {
    SimpleList<T>::PushTail(value);
}

//Pop value from head of the queue
template<typename T>
T Queue<T>::pop() {
    return SimpleList<T>::PopHead();
}

//--------------------FUNCTIONS--------------------

//Process the line by checking the first word of command, add them
//to stack or queue, or push or pop the value from existing list
template <typename T>
void ProcessLine(list<SimpleList<T> *> &ListOfList, string command[], T value){
    string ListName;
    if(command[0].compare("create") == 0){
        if(NameCheck(ListOfList, command[1]) != NULL){     			 //Check if name exist before create list
            OutputFile << "ERROR: This name already exists!" << endl;
        }
        else{
            if(command[2].compare("stack") == 0){
                SimpleList<T> *ptr = new Stack<T>(command[1]);
                ListOfList.push_back(ptr);
            }
            else if(command[2].compare("queue") == 0){
                SimpleList<T> *ptr = new Queue<T>(command[1]);
                ListOfList.push_back(ptr);
            }
        }
    }
    else if(command[0].compare("push") == 0){
        SimpleList<T> *ptr = NameCheck(ListOfList, command[1]);     //Check name before push value
        if (ptr == NULL){
            OutputFile << "ERROR: This name does not exist!" << endl;
        }
        else{
            ptr->push(value);
        }
    }
    else if(command[0].compare("pop") == 0){
        SimpleList<T> *ptr = NameCheck(ListOfList, command[1]);     //Check if name exist
        if(ptr == NULL){
            OutputFile << "ERROR: This name does not exist!" << endl;
        }
        else if(ptr->Empty()) {
            OutputFile << "ERROR: This list is empty!" << endl;     //Check if list is empty
        }
        else{
            OutputFile << "Value popped: " << ptr->pop() << endl;
        }

    }
}

//Open input and output file
void OpenFile(ifstream &input, ofstream &output) {
    string InputFileName;
    cout << "Enter input file name:";
    cin >> InputFileName;
    input.open(InputFileName.c_str());
    if (!input.is_open()) {
        cerr << "cannot open" << endl;
    }
    string OutputFileName;
    cout << "Enter output file name:";
    cin >> OutputFileName;
    output.open(OutputFileName.c_str());
}

//Process file to i,d,s list and send them to process line
void ProcessFile(const string& line){
    OutputFile << "PROCESSING COMMAND: " << line << endl;
    stringstream LineStream(line);
    string words;
    string* command = new string[3];
    int i=0;
    while (LineStream >> words){    //Separate input line into three command
        command[i++] = words;
    }
    if (command[1].substr(0,1).compare("i") == 0){
        int value = 0;
        if (!command[2].empty()) {
            value = atoi(command[2].c_str()); 	//Convert string to int
        }
        ProcessLine(listSLi, command, value);
    }
    else if (command[1].substr(0,1).compare("d") == 0){
        double value = 0.0;
        if (!command[2].empty()) {
            value = atof(command[2].c_str());	//Convert string to double
        }
        ProcessLine(listSLd, command, value);
    }
    else if (command[1].substr(0,1).compare("s") == 0){
        string value = command[2];
        ProcessLine(listSLs, command, value);
    }
}

//Main
int main() {
    OpenFile(InputFile, OutputFile);
    string line;
    while(getline(InputFile, line)){
        ProcessFile(line);
    }
    InputFile.close();
    OutputFile.close();
    return 0;
}
