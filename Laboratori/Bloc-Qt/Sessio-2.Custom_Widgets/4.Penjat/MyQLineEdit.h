#include <QLineEdit>

class MyQLineEdit : public QLineEdit {
    Q_OBJECT

    public:
        MyQLineEdit(QWidget *parent);
    public slots:
    	void tratarText(QString s);
        void reset();
    signals:
    	void enviarErrors(int e);
    	void enviarText(QString t);
    protected:
    	int numErrors;
    	QString palabra;
    	QString aux;
};
