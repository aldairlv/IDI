#include <MyQLCDNumber.h>

MyQLCDNumber::MyQLCDNumber(QWidget *parent) : QLCDNumber(parent) {
    SetZero();
}

void MyQLCDNumber::SetZero() {
    SetValueAndColor(0);
}

void MyQLCDNumber::SetValueAndColor(int n) {
    if (n == 0) setStyleSheet("color:green");
    else {
        if (n%2 == 1) setStyleSheet("color:red");
        else setStyleSheet("color:blue");
    }
    display(n);
}
