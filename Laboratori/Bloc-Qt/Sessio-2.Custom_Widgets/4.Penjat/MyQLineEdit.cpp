#include <MyQLineEdit.h>
#include <iostream>
using namespace std;
MyQLineEdit::MyQLineEdit(QWidget *parent) : QLineEdit(parent) {
    palabra = "DICCIONARI";
    aux = QString (palabra.size(),'*');
    numErrors = 5;
}

void MyQLineEdit::tratarText(QString s){
    bool repetida = false;
    bool found = false;
   
    s = s.toUpper();
    QChar ultimaLetra =  s[s.size()-1];
        
    // comprobar si el usuario ya habia enviado antes la misma letra
    for(int i = 0; i < s.size()-1 and not repetida; i++) {
        if (s[i] == ultimaLetra) repetida = true;
    }
            
    // comprobar si la letra enviada corresponde a alguna de las letras dentro de la palabra
    for (int i = 0; i < palabra.size() and not found and not repetida; i++) {
	if (palabra[i] == ultimaLetra) found = true;
    }
        
    if ((not found or repetida)) numErrors -= 1;
    if (found) {
        for (int i = 0; i < palabra.size(); ++i){
            if (palabra[i] == ultimaLetra) aux[i] = palabra[i];
        }
    } 
    emit enviarText(aux);
    emit enviarErrors(numErrors);
     
    if(numErrors == 0) {
        clear();
	setStyleSheet("background-color: red;");
	setEnabled(false);
    }
    else if(aux == palabra) {
        clear();
	setStyleSheet("background-color: green;");
	setEnabled(false);
    }
}

void MyQLineEdit::reset(){
	aux = QString (palabra.size(),'*');
	emit enviarText(aux); 
	numErrors = 5;
	emit enviarErrors(numErrors);
	clear();
	setStyleSheet("background-color: none;");
	setEnabled(true);
}



