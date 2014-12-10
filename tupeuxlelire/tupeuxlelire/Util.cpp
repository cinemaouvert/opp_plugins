#include "Util.h"

Util::Util()
{
}

double Util::convertKiloOctet(int *octets){
    double result = *octets/1024;
    return result;
}
void Util::convertKiloOctet(qulonglong *octets){
    qulonglong diviseur = 1024;
    *octets = *octets/diviseur;
}

double Util::convertMegaOctet(int *octets){
    double result = (*octets/1024)/1024;
    return result;
}

void Util::convertMegaOctet(qulonglong *octets){
    qulonglong diviseur = 1024;
    *octets = (*octets/diviseur)/diviseur;
}

double Util::convertGigaOctet(int *octets){
    double result = ((*octets/1024)/1024)/1024;
    return result;
}

void Util::convertGigaOctet(qulonglong *octets){
    qulonglong diviseur = 1024;
    *octets = ((*octets/diviseur)/diviseur)/diviseur;
}

double Util::convertTeraOctet(int *octets){
    double result = (((*octets/1024)/1024)/1024)/1024;
    return result;
}

double Util::convertMegaOctetToOctet(double *megaOctets){
    double result = (*megaOctets * 1024)* 1024;
    return result;
}

void Util::convertMegaOctetToOctet(qulonglong *megaOctets){
    qulonglong multiplicateur = 1024;
    *megaOctets = (*megaOctets * multiplicateur)* multiplicateur;
}

/*
 * Converti un vector de QString représentant des int 64bit en megaoctet
*/

void Util::convertMegaOctet(vector<QString>* stringV){
    bool test;
    qulonglong llVar;

    for(int i = 0; i<stringV->size(); i++){
        llVar = stringV[0][i].toULongLong();
        convertMegaOctet(&llVar);

        if(llVar > 1024){
            llVar = stringV[0][i].toULongLong();
            convertGigaOctet(&llVar);
            stringV[0][i] = QString::number(llVar);
            stringV[0][i] = stringV[0][i] + " Go";
        }else if(llVar == 0){
            llVar = stringV[0][i].toULongLong();
            convertKiloOctet(&llVar);
            if(llVar == 0){
                llVar = stringV[0][i].toULongLong();
                stringV[0][i] = QString::number(llVar);
                stringV[0][i] = stringV[0][i] + " o";
            }else{
                stringV[0][i] = QString::number(llVar);
                stringV[0][i] = stringV[0][i] + " Ko";
            }
        }else{
            stringV[0][i] = QString::number(llVar);
            stringV[0][i] = stringV[0][i] + " Mo";
        }
    }
}

string Util::intToString(int Int)
{
    stringstream ss;
    ss << Int;
    return ss.str();
}
