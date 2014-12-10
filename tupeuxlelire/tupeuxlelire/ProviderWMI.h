//N'est compilable qu'avec MSVC

#include "Environment.h" // Permet de connaitre l'os sur lequel on compile
#if defined(OS_WIN)

#ifndef ProviderWMI_H
#define ProviderWMI_H

using namespace std;
#include <iostream>
#include <string>
#include <stdint.h>
#include <vector>


#include <QApplication>
#include <QTimer> //Reste de Qt, il faudra crée mon propre objet TIMER
#include "Util.h"
#include "Info.h"
#include "AbstractProvider.h"
#include "QuerySink.h"

class ProviderWMI: public Info, public AbstractProvider
{
public:
    // Constructeur/destructeur
    ProviderWMI();
    ~ProviderWMI();
    void callbackTest(string reponse);

//============================================================//
//======================    QUERY    =========================//
//============================================================//

public:
    void query(char* sqlString, LPCWSTR property, string outType, QuerySink* responseSink); //Envoi d'une requete au format WQL
    vector<QString>* queryAll(char* sqlString, LPCWSTR property, string dataType);

private:
    string errorMsg(long flag, string fnName);

    //Methodes qui permettent d'initialiser l'interface WMI
    void initializeCOM();
    void comSecurityLevels();
    void initialLocator();
    void connectWMI();
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

    //ATTR STATIC
    static bool notInitializedWMI;

//===============================================================//
//===============================================================//
//===============================================================//

private:
    static bool m_isProviding;
    static bool m_securityInitialized;

protected:
    void provide_once();
    void provide();
};

#endif // ProviderWMI_H
#endif // OS_WIN
