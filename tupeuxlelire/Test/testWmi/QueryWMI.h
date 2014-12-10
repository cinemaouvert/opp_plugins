#ifndef QUERYWMI_H
#define QUERYWMI_H

#include "AbstractQuery.h"
#include "QuerySink.h"
#include <vector>
#include <string>
#include <QString>

//TEST

using std::string;
using std::vector;

class QueryWMI: public AbstractQuery
{

public:
    QueryWMI();
    QueryWMI(char* sqlString, LPCWSTR property, string outType); //Envoi d'une requête à la construction
    ~QueryWMI();

    void query(char* sqlString, LPCWSTR property, string outType); //Envoi d'une requete au format WQL
    vector<QString>* queryAll(char* sqlString, LPCWSTR property, string dataType);
    void setReponse(string reponse);
    void setCallback(void (*pt2func)(QString));

private:
    string errorMsg(long flag, string fnName);

    //Methodes qui permettent d'initialiser l'interface WMI
    void initializeCom();
    void comSecurityLevels();
    void initialLocator();
    void connectWmi();
    void proxySecurityLevels();

    void initializeWMI(); //Initialisation de l'interface
    void cleanUp(); // Termine les requetes et nettoie

    //ATTRIBUTS
    QuerySink* m_pResponseSink;
    HRESULT m_output;
    IWbemLocator *pLoc;
    IWbemServices *pSvc;
    IEnumWbemClassObject *pEnumerator;
    IWbemClassObject *pclsObj;
    QString m_reponse;
    void (*m_callback)(QString);

    //ATTR STATIC
    static bool notInitializedWMI;
};

#endif // QUERYWMI_H
