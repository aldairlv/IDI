#include <MyQLabel.h>
#include <string>
#include <iostream>
using namespace std;

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent) {

}

void MyQLabel::controlPas(bool obert) {
	b = obert;
}
void MyQLabel::colorSemafor() {
	if (b) {
		setText("Obert");
		setStyleSheet("background-color:green");
	}
	else {	
		setStyleSheet("background-color:red");
		setText("Tancat");
	}
}

void MyQLabel::numCotxes() {

	if (b) {
		total_cotxes += 1;
		setText(QString::number(total_cotxes));
	}
}

void MyQLabel::recaptacioCotxes() {
	if (b) {
		recaptacio_cotxes += 1;
		setText(QString::number(recaptacio_cotxes));
	}
}

void MyQLabel::numMoto() {

	if (b) {
		total_moto += 1;
		setText(QString::number(total_moto));
	}
}

void MyQLabel::recaptacioMoto() {
	if (b) {
		recaptacio_moto += 1;
		setText(QString::number(recaptacio_moto));
	}
}

void MyQLabel::numCamions() {

	if (b) {
		total_camions += 1;
		setText(QString::number(total_camions));
	}
}

void MyQLabel::recaptacioCamions() {
	if (b) {
		recaptacio_camions += 4;
		setText(QString::number(recaptacio_camions));
	}
}
