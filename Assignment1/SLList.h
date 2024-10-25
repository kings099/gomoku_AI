#pragma once
#pragma once
#include <iostream>
#include <cstring>
using namespace std;
/* �Զ����ַ�������*/
class MyString
{
private:
    char* str; // �ַ���ָ��
    size_t length;
public:
    /*Ĭ�Ϲ��캯��*/
    MyString() :str(nullptr), length(0) {}
    /*���ι��캯��*/
    MyString(const char* str) {
        if (str == nullptr) {
            this->str = new char[1];
            this->str[0] = '\0';
            length = 0;
        }
        else {
            length = strlen(str);
            this->str = new char[length + 1];  // �����ڴ棬���� '\0'
            strcpy(this->str, str);  // �����ַ�������
        }
    }
    /*�������캯�������������е�MyString�����ʼ��ʱ����ͬһ���ڴ�*/
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
    /*��������*/
    ~MyString() {
        delete[] str;  // �ͷŶ�̬������ڴ�
    }
    MyString& operator=(const MyString& obj);
    MyString& operator=(const char* str);
    MyString operator+(const MyString& obj);
    bool operator==(const MyString& obj);
    bool operator!=(const MyString& obj);
    friend istream& operator>>(istream& cin, MyString& obj) {
        char buffer[1024];  // ��ʱ�����������ڴ洢������ַ���
        cin >> buffer;  // ��ȡ���룬�����ո���з�ֹͣ
        delete[] obj.str;  // �ͷ�֮ǰ���ڴ�
        obj.length = strlen(buffer);  // �����ַ����ĳ���
        obj.str = new char[obj.length + 1];  // Ϊ�ַ����������ڴ�
        strcpy(obj.str, buffer);  // ����������ݸ��Ƶ� obj.str
        return cin;
    }
    friend ostream& operator<<(ostream& cout, const MyString& obj) {
        if (obj.str) {
            cout << obj.str;
        }
        return cout;
    }

    char& operator[](size_t index) {
        return str[index];  // �����ַ�������
    }
    int getLength() {
        return int(length);
    }
};
/*��ֵ���������*/
MyString& MyString::operator=(const MyString& obj) {
    if (this != &obj) {  // ��ֹ���Ҹ�ֵ
        delete[] this->str;  // �ͷ�ԭ���ڴ�
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
/*���ַ�����ֵ��MyString*/
MyString& MyString::operator=(const char* str)
{
    length = strlen(str);
    delete[] this->str;
    this->str = new char[length + 1];
    strcpy(this->str, str);
    return *this;
}
/*ƴ������MyString*/
MyString MyString::operator+(const MyString& obj) {
    MyString result;
    result.length = length + obj.length;
    result.str = new char[result.length + 1];
    strcpy(result.str, str);  // ���Ƶ�һ���ַ���
    strcat(result.str, obj.str);  // ƴ�ӵڶ����ַ���
    return result;
}
/*�Ƚ�����MyString*/
bool MyString::operator==(const MyString& obj)
{
    return strcmp(str, obj.str) == 0;
}
bool MyString::operator!=(const MyString& obj) {
    return !(*this == obj);
}
/***************************************************************************
�Ӵ˴���ʼΪ�����������
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
/*�ҵ���position���ڵ�ĵ�ַ*/
template <typename T>
typename  SLList<T>::Node* SLList<T>::Find(int position)const
{
    if (position < 0 || position >= size) {  // ����Ƿ�Խ��
        return nullptr;
    }
    Node* p = first;
    for (int i = 0; i < position; i++) {
        p = p->next;
    }
    return p;
}
/*�ҵ�ĳԪ�ص�λ��*/
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
/*����ĳλ�õ�Ԫ��*/
template <typename T>
T& SLList<T>::Visit(int position) {
    if (position < 0 || position >= size) { //��� position �Ƿ��ںϷ���Χ�� 
        cout << "��Чλ�ã�" << position << endl;
        throw out_of_range("λ�ô���");
    }
    Node* p = Find(position);
    if (p == nullptr) {
        throw runtime_error("��λ�ò����ڵĽڵ�");
    }
    return p->item;
}
/*����ڵ�������ĩβ*/
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
/*��positionλ�ò���item*/
template <typename T>
void SLList<T>::InsertAt(const T& insert_item, int position)
{
    if (position<0 || position>size) {
        cout << "��Чλ��" << endl;
        return;
    }
    Node* p;
    if (position == 0) {
        p = new Node(insert_item, first);
        first = p;
    }
    else {
        p = Find(position - 1); // �ҵ�ָ�����λ��ǰһ���ڵ��ָ��
        p->next = new Node(insert_item, p->next); // ��ǰһ��λ�õĺ��ָ��ָ���½ڵ㣬�½ڵ�ָ��ָ��ԭ��positionλ�õĽڵ�
    }
    size++;
}
/*ɾ��positionλ��Ԫ��*/
template <typename T>
void SLList<T>::RemoveAt(int position) {
    if (position < 0 || position >= size) {
        cout << "��Чλ��" << endl;
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
        delete_position = p->next;//Ҫɾ���Ľڵ�
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