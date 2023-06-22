#include <MyQLCDNumber.h>

MyQLCDNumber::MyQLCDNumber(QWidget *parent) : QLCDNumber(parent) {
    setSegmentStyle(Filled);
    setDigitCount(12);
    running = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setTime()));
    reStart();
}

void MyQLCDNumber::setTime() {
    time = time.addMSecs(1);
    showTime();   
}

void MyQLCDNumber::start() {
    running = !running;
    if (running) timer->start(1);
    else timer->stop();
}

void MyQLCDNumber::reStart() {
     if (record_value.isNull()) {
        record_value = time;
    	emit sendRecord(record_value.toString("hh:mm:ss:zzz"));
    }
    else {
    	if (time < record_value) {
    	    record_value = time;
    	    emit sendRecord(record_value.toString("hh:mm:ss:zzz"));
    	}
    }
    time = QTime(0,0,0,0);
    showTime();
}

void MyQLCDNumber::showTime() {
    QString text = time.toString("hh:mm:ss:zzz");
    if (not record_value.isNull()) {
        if (time < record_value) setStyleSheet("color:green");
        else if (time > record_value) setStyleSheet("color:red");
        else setStyleSheet("color:black");
    }
    display(text);
}
