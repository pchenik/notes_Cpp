#include <bits/stdc++.h>

using namespace std;

struct Base {
    void kek() {
        for(int i = 0; i < 5; ++i);
    }
    virtual ~Base() { }
    virtual void print() { cout << "Base" << endl; }
};

struct Derived1 : Base {
    int x, y;
    void print() {cout << "Derived1" << endl; }
};


struct Derived2 : Base {
    void print() {cout << "Derived2" << endl; }
};

struct Derived3 : Derived1, Derived2 {
    int x;
    void print() {cout << "Derived3" << endl;}
};


int main()
{
    Base* b0 = new Base();
    Derived3* b3 = new Derived3();
    Derived1* b1 = new Derived1();
    Derived2* b2 = new Derived2();

    srand(time(NULL));
    int k = rand() % 2;
    cout << k << endl;
    if (k)
        cout << b3 << endl;
    else
        cout << b1 << endl;
    Base* _b = (k) ? (Base*)(Derived2*)(b3) : b1;
    cout << dynamic_cast<void*>(_b) << endl;
    return 0;
}
