#include <QLCDNumber>

class MyQLCDNumber : public QLCDNumber {
    Q_OBJECT

    public:
        MyQLCDNumber(QWidget *parent);
    public slots:
        void SetZero();
        void SetValueAndColor(int);
    signals:
    protected:
};
