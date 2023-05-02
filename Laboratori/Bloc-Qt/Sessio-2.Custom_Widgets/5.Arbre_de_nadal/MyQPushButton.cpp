#include <MyQPushButton.h>
#include <iostream>
#include <random>
#include <unistd.h>
using namespace std;
MyQPushButton::MyQPushButton(QWidget *parent) : QPushButton(parent) {
    srand(time(nullptr));
}

void MyQPushButton::tratarNum(int n) {
    num = rand();
    int d = (num*n)%5;
    if (d == 0) {
        setStyleSheet("background-color: #0ffc03; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
    else if (d == 1) {
  	setStyleSheet("background-color: #fc0303; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
    else if (d == 2) {
  	setStyleSheet("background-color: #fcfc03; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
    else if (d == 3) {
  	setStyleSheet("background-color: #fc03c2; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
}


void MyQPushButton::tratarRand() {
    num = rand();
    int d = (num)%4;
    if (d == 0) {
  	setStyleSheet("background-color: #0ffc03; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
    else if (d == 1) {
        setStyleSheet("background-color: #fc0303; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
    else if (d == 2) {
  	setStyleSheet("background-color: #fcfc03; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
    else if (d == 3) {
  	setStyleSheet("background-color: #fc03c2; border-radius:5px; max-width:10px; max-height:10px; min-width:10px; min-height:10px;");
    }
}



