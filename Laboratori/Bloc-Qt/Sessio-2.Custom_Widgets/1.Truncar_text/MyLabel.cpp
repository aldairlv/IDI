#include <MyLabel.h>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {

}

void MyLabel::SaveText(const QString& t){
    text = t;
    TruncateAndSetText();
}

void MyLabel::SaveNumCharacters(int n) {
    numCharacters = n;
    TruncateAndSetText();
}

void MyLabel::TruncateAndSetText() {
    QString t = text;
    int n = t.length() - numCharacters;
    if (n < 0) n = 0;
    t.truncate(n);

    setText(t);
}
