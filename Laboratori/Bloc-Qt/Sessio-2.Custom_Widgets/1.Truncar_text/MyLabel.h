#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

    public:
        MyLabel(QWidget *parent);
    public slots:
        void SaveText(const QString &);
        void SaveNumCharacters(int);
    protected:
        void TruncateAndSetText();

        QString text;
        int numCharacters;
};
