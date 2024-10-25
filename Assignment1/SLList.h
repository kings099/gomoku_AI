#pragma once
#pragma once
#include <iostream>
#include <cstring>
using namespace std;
/* 自定义字符串类型*/
class MyString
{
private:
    char* str; // 字符串指针
    size_t length;
public:
    /*默认构造函数*/
    MyString() :str(nullptr), length(0) {}
    /*带参构造函数*/
    MyString(const char* str) {
        if (str == nullptr) {
            this->str = new char[1];
            this->str[0] = '\0';
            length = 0;
        }
        else {
            length = strlen(str);
            this->str = new char[length + 1];  // 分配内存，包括 '\0'
            strcpy(this->str, str);  // 复制字符串内容
        }
    }
    /*拷贝构造函数，避免用现有的MyString对象初始化时共用同一个内存*/
    MyString(const MyString& obj) {
        if (obj.str == nullptr) {
            this->str = new char[1];
            this->str[0] = '\0';
            length = 0;
        }
        else {
            length = obj.length;
            this->str = new char[length + 1];
            strcpy(this->str, obj.str);
        }
    }
    /*析构函数*/
    ~MyString() {
        delete[] str;  // 释放动态分配的内存
    }
    MyString& operator=(const MyString& obj);
    MyString& operator=(const char* str);
    MyString operator+(const MyString& obj);
    bool operator==(const MyString& obj);
    bool operator!=(const MyString& obj);
    friend istream& operator>>(istream& cin, MyString& obj) {
        char buffer[1024];  // 临时缓冲区，用于存储输入的字符串
        cin >> buffer;  // 读取输入，遇到空格或换行符停止
        delete[] obj.str;  // 释放之前的内存
        obj.length = strlen(buffer);  // 计算字符串的长度
        obj.str = new char[obj.length + 1];  // 为字符串分配新内存
        strcpy(obj.str, buffer);  // 将输入的内容复制到 obj.str
        return cin;
    }
    friend ostream& operator<<(ostream& cout, const MyString& obj) {
        if (obj.str) {
            cout << obj.str;
        }
        return cout;
    }

    char& operator[](size_t index) {
        return str[index];  // 返回字符的引用
    }
    int getLength() {
        return int(length);
    }
};
/*赋值运算符重载*/
MyString& MyString::operator=(const MyString& obj) {
    if (this != &obj) {  // 防止自我赋值
        delete[] this->str;  // 释放原有内存
        if (obj.str == nullptr) {
            this->str = new char[1];
            this->str[0] = '\0';
            length = 0;
        }
        else {
            length = obj.length;
            this->str = new char[length + 1];
            strcpy(this->str, obj.str);
        }
    }
    return *this;
}
/*将字符串赋值给MyString*/
MyString& MyString::operator=(const char* str)
{
    length = strlen(str);
    delete[] this->str;
    this->str = new char[length + 1];
    strcpy(this->str, str);
    return *this;
}
/*拼接两个MyString*/
MyString MyString::operator+(const MyString& obj) {
    MyString result;
    result.length = length + obj.length;
    result.str = new char[result.length + 1];
    strcpy(result.str, str);  // 复制第一个字符串
    strcat(result.str, obj.str);  // 拼接第二个字符串
    return result;
}
/*比较两个MyString*/
bool MyString::operator==(const MyString& obj)
{
    return strcmp(str, obj.str) == 0;
}
bool MyString::operator!=(const MyString& obj) {
    return !(*this == obj);
}
/***************************************************************************
从此处开始为构建单链表库
***************************************************************************/

template <typename T>
class SLList
{
private:
    struct Node
    {
        T item;
        Node* next;
        Node(const T& item, Node* next = nullptr) :item(item), next(next) {}
    };
    Node* first;
    int size;
    Node* Find(int position)const;
public:
    SLList() : first(nullptr), size(0) {}
    ~SLList() { clear(); }
    void clear();
    int Search(const T& search_item)const;
    T& Visit(int position);
    void Insert(const T& insert_item);
    void InsertAt(const T& insert_item, int position);
    void RemoveAt(int position);
    int Size() { return size; }

};
/*找到第position个节点的地址*/
template <typename T>
typename  SLList<T>::Node* SLList<T>::Find(int position)const
{
    if (position < 0 || position >= size) {  // 检查是否越界
        return nullptr;
    }
    Node* p = first;
    for (int i = 0; i < position; i++) {
        p = p->next;
    }
    return p;
}
/*找到某元素的位置*/
template <typename T>
int SLList<T>::Search(const T& search_item)const
{
    Node* p = first;
    int position = 0;
    while (p != nullptr) {
        if (p->item == search_item) {
            return position;
        }
        p = p->next;
        position++;
    }
    return -1;
}
/*访问某位置的元素*/
template <typename T>
T& SLList<T>::Visit(int position) {
    if (position < 0 || position >= size) { //检查 position 是否在合法范围内 
        cout << "无效位置：" << position << endl;
        throw out_of_range("位置错误");
    }
    Node* p = Find(position);
    if (p == nullptr) {
        throw runtime_error("该位置不存在的节点");
    }
    return p->item;
}
/*插入节点在链表末尾*/
template <typename T>
void SLList<T>::Insert(const T& instert_item) {
    Node* p = new Node(instert_item);
    if (!first) {
        first = p;
    }
    else {
        Node* q = first;
        while (q->next) {
            q = q->next;
        }
        q->next = p;
    }
    size++;
}
/*在position位置插入item*/
template <typename T>
void SLList<T>::InsertAt(const T& insert_item, int position)
{
    if (position<0 || position>size) {
        cout << "无效位置" << endl;
        return;
    }
    Node* p;
    if (position == 0) {
        p = new Node(insert_item, first);
        first = p;
    }
    else {
        p = Find(position - 1); // 找到指向插入位置前一个节点的指针
        p->next = new Node(insert_item, p->next); // 将前一个位置的后记指针指向新节点，新节点指针指向原在position位置的节点
    }
    size++;
}
/*删除position位置元素*/
template <typename T>
void SLList<T>::RemoveAt(int position) {
    if (position < 0 || position >= size) {
        cout << "无效位置" << endl;
        return;
    }
    Node* p, * delete_position;
    if (position == 0) {
        p = Find(0);
        first = p->next;
        delete p;
        size--;
    }
    else {
        p = Find(position - 1);
        delete_position = p->next;//要删除的节点
        p->next = delete_position->next;
        delete delete_position;
        size--;
    }
}
template <typename T>
void SLList<T>::clear() {
    Node* p, * q;
    p = first;
    q = nullptr;
    while (p != nullptr) {
        q = p->next;
        delete p;
        p = q;
    }
    first = nullptr;
    size = 0;
}