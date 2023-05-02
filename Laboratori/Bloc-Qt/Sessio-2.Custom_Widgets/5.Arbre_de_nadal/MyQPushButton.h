#include <QPushButton>

class MyQPushButton : public QPushButton {
    Q_OBJECT

    public:
        MyQPushButton(QWidget *parent);
    public slots:
        void tratarNum(int n);
        void tratarRand();
    	//void tratarText(QString s);
        //void reset();
    signals:
    	void enviarNum(int e);
    	//void enviarErrors(int e);
    	//void enviarText(QString t);
    protected:
    	//int numErrors;
    	//QString palabra;
    	//QString aux;
    	int num;
};
