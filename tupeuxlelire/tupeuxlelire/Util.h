#ifndef UTIL_H
#define UTIL_H

#include <iostream>

#include <string>
#include <sstream>
#include <vector>
#include <QApplication>

using namespace std;

class Util
{
public:
    Util();
    static double convertKiloOctet(int *octets);
    static void convertKiloOctet(qulonglong *octets);

    static double convertMegaOctet(int *octets);
    static void convertMegaOctet(qulonglong *octets);

    static double convertGigaOctet(int *octets);
    static void convertGigaOctet(qulonglong *octets);

    static double convertTeraOctet(int *octets);

    static double convertMegaOctetToOctet(double *megaOctets);
    static void convertMegaOctetToOctet(qulonglong *megaOctets);

    static void convertMegaOctet(vector<QString>* stringV);

    static string intToString (int Int);
};

#endif // UTIL_H
