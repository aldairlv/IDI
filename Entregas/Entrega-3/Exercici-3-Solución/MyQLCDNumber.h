#include <QLCDNumber>
#include <QTimer>
#include <QTime>


class MyQLCDNumber : public QLCDNumber {
    Q_OBJECT

    public:
        MyQLCDNumber(QWidget *parent);
    public slots:
        void setTime();
        void start();
        void reStart();
        void showTime();
        
    signals:
        void sendRecord(QString s);
    protected:
        QTimer *timer; 
        QTime time;
        QTime record_value;
        bool running;
    	
};
