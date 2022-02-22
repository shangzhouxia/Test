#include <iostream>
using namespace std;

//静态多态
int add(int a, int b)
{
    cout<<"in add int()"<<endl;
    return a+b;
}

double add(double a, double b)
{
    cout<<"in add double()"<<endl;
    return a+b;
}

//函数模板（泛型编程）
template <typename T>
T add(T a, T b)
{
    cout<<"in func template"<<endl;
    return a+b;
}

//动态多态最常见的就是声明基类的指针
//该指针指向任意一个子类对象，调用相应的虚函数，根据指向的子类不同，调用不同的方法

class Base {
public:
    virtual void func() {
        cout << "Base::func()"<<endl;
    }
};

class Derived : public Base {
public:
    virtual void func() {
        cout << "Derived::func()"<<endl;
    }
};



int main()
{
    cout<<add(1,1)<<endl;
    cout<<add(1.1, 1.1)<<endl;

    cout<<add<char>('A', ' ')<<endl;

    Base* b = new Derived;          //基类指针指向派生类对象
    b->func();                      //动态绑定派生类成员函数func

    Base & rb = *(new Derived);     //使用引用指向派生类对象
    rb.func();
}