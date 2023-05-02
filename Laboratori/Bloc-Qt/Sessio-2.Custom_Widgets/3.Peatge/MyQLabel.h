#include <QLabel>
#include <string>

class MyQLabel : public QLabel {
    Q_OBJECT

    public:
        MyQLabel(QWidget *parent);
    public slots:
    	void controlPas(bool obert);
        void colorSemafor();
        void numCotxes();
        void recaptacioCotxes();
        void numMoto();
        void recaptacioMoto();
        void numCamions();
        void recaptacioCamions();
    protected:
    	bool b = false;
    	int total_cotxes = 0;
    	int recaptacio_cotxes = 0;
    	int total_moto = 0;
    	int recaptacio_moto = 0;
    	int total_camions = 0;
    	int recaptacio_camions = 0;
};
