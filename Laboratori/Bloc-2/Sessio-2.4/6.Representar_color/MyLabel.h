#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

    public:
        MyLabel(QWidget *parent);
    public slots:
        void saveRed(int r);
        void saveGreen(int g);
        void saveBlue(int b);
        void setBackgroundColor();
    protected:
        int colorRed;
        int colorGreen;
        int colorBlue;
};
