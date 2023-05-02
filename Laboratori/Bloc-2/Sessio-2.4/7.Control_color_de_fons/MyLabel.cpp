#include <MyLabel.h>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {
        colorRed = 0;
        colorGreen = 0;
        colorBlue = 0;
}

void MyLabel::saveRed(int r) {
    colorRed = r;
    setBackgroundColor();
}
void MyLabel::saveGreen(int g) {
    colorGreen = g;
    setBackgroundColor();
}
void MyLabel::saveBlue(int b) {
    colorBlue = b;
    setBackgroundColor();
}
void MyLabel::setBackgroundColor() {
    //setStyleSheet("background-color: red;");
    setStyleSheet(QString("background-color: rgb(%1,%2,%3);").arg(colorRed).arg(colorGreen).arg(colorBlue));
    //background-color: rgb(246, 97, 81);
}

