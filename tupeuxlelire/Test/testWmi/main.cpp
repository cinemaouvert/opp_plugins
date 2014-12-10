#include <iostream>
#include <QObject>
#include <string>
#include "QueryWMI.h"

using std::cout;
using std::endl;
using std::string;

void cllback(QString reponse){
    cout << "Le callback marche, la preuve: " << reponse.toStdString() << endl;
}

int main(int argc, char *argv[])
{
    //On défini un pointeur de fonction
    void (*pt2fn)(QString);

    //On assigne au pointeur de fonction la fonction de callback
    pt2fn = cllback;

    QueryWMI* cpuTime1 = new QueryWMI;
    cpuTime1->setCallback(pt2fn);
    cpuTime1->query("SELECT PercentPrivilegedTime FROM Win32_PerfFormattedData_Counters_ProcessorInformation WHERE Name = '_total'", L"PercentPrivilegedTime", "string");

    //QueryWMI* cpuTime2 = new QueryWMI("SELECT PercentPrivilegedTime FROM Win32_PerfFormattedData_Counters_ProcessorInformation WHERE Name = '_total'", L"PercentPrivilegedTime", "string");
    cout << "Salut\n";

    Sleep(7000); //Pour que ça fonctionne le programme ne doit pas se terminer tout de suite.
    return 0;
}


