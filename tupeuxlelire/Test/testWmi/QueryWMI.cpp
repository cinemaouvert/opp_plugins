#include "QueryWMI.h"
#include <string>
#include <iostream>

using std::string;
using std::hex;
using std::endl;
using std::cout;

//Constructeur/destructeur
QueryWMI::QueryWMI()
{
    m_pResponseSink = new QuerySink;
    m_pResponseSink->setParent(this); //On défini l'objet actuel comme parent de l'objet QuerySink
    m_output = 0;
    pLoc = NULL;
    pSvc = NULL;
    pEnumerator = NULL;

    initializeWMI();
}

QueryWMI::QueryWMI(char* sqlString, LPCWSTR property, string outType){
    m_pResponseSink = new QuerySink;
    m_pResponseSink->setParent(this); //On défini l'objet actuel comme parent de l'objet QuerySink
    m_output = 0;
    pLoc = NULL;
    pSvc = NULL;
    pEnumerator = NULL;

    initializeWMI();

    query(sqlString, property, outType);
}

QueryWMI::~QueryWMI()
{
    delete m_pResponseSink;
    delete pLoc;
    delete pSvc;
    delete pEnumerator;
    delete pclsObj;
}

//Méthodes
void QueryWMI::initializeCom(){
    // Step 1: ----------------------------------------------
    // Initialisation de COM. --------------------------------------
    // Normalement COM est déjà initialisé par QApplication. C'est pour ça qu'on ne l'initialise pas dans certains contextes.
    m_output =  CoInitializeEx(0, COINIT_MULTITHREADED);

    if (FAILED(m_output))
    {
        string error = errorMsg(m_output, "CoInitializeEx");
        if(error != "undefined"){
            cout << "Echec lors de l'initialisation de la bibliotheque COM.\n" << error << endl;
        }else{
            cout << "Erreur non definie. Code erreur: 0x" << hex << m_output << endl;
        }
    }
}

void QueryWMI::comSecurityLevels(){

    // Step 2: --------------------------------------------------
    // initialisation du niveau général de sécurité de COM --------------------------
    // Ne peut être initialisé qu'une seule fois par process
    m_output =  CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
        );

    if (FAILED(m_output))
    {
        string error = errorMsg(m_output, "CoInitializeSecurity");
        if(error != "undefined"){
            cout << "Echec lors de l'initialisation du niveau general de securite de COM.\n" << error << endl;
        }else{
            cout << "Erreur non definie. Code erreur: 0x" << hex << m_output << endl;
        }

        CoUninitialize();
    }
}

void QueryWMI::initialLocator(){
    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------
    m_output = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *) &pLoc
        );

    if (FAILED(m_output))
    {
        cout << "La creation de l'objet IWbemLocator a echoue." << " Err code = 0x" << hex << m_output << endl;
        CoUninitialize();
    }
}

void QueryWMI::connectWmi(){
    // Step 4: -----------------------------------------------------
    // On se connecte à WMI par le biais de la méthode IWbemLocator::ConnectServer

    // On se connecte à l'espace de nom "root\cimv2"
    // avec l'utilisateur courrant et on obtiens le pointeur "pSvc"
    // qui nous permet de faire un appel à IWbemServices.
    m_output = pLoc->ConnectServer(
         _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
         NULL,                    // Nom de l'utilisateur. NULL = utilisateur actuel
         NULL,                    // Mot de passe de l'utilisateur. NULL = actuel.
         0,                       // Locale. NULL indicates current
         NULL,                    // Security flags.
         0,                       // Authority (for example, Kerberos)
         0,                       // Context object
         &pSvc                    // pointer to IWbemServices proxy
         );

    if (FAILED(m_output))
    {
        cout << "Impossible de se connecter au service. Code d'erreur = 0x" << hex << m_output << endl;
        pLoc->Release();
        CoUninitialize();
    }
}

void QueryWMI::proxySecurityLevels(){
    // Etape 5: --------------------------------------------------
    // Définit les niveaux de sécurité sur le proxy -------------------------
    m_output = CoSetProxyBlanket(
       pSvc,                        // Indicates the proxy to set
       RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
       RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
       NULL,                        // Server principal name
       RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
       RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
       NULL,                        // client identity
       EOAC_NONE                    // proxy capabilities
    );

    if (FAILED(m_output))
    {
        cout << "Could not set proxy blanket. Error code = 0x" << hex << m_output << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
    }
}

void QueryWMI::initializeWMI(){
    //On initialise l'interface WMI
    if(notInitializedWMI){
        initializeCom(); // COM est déjà initialisé par QApplication
        comSecurityLevels();
        initialLocator();
        connectWmi();
        proxySecurityLevels();

        notInitializedWMI = false;
    }
}

//Remet tout à zéro
void QueryWMI::cleanUp(){
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    m_output = 1;
}

string QueryWMI::errorMsg(long flag, string fnName){
    string errorMsg("undefined");

    if(flag == WBEM_E_ACCESS_DENIED){
        errorMsg = "Accès refusé. L'utilisateur actuel n'a pas la permission d'accéder au résultat des requêtes.";
    }else if(flag == WBEM_E_FAILED){
        errorMsg = "Erreur non spécifié.";
    }else if(flag == WBEM_E_INVALID_PARAMETER){
        errorMsg = "Un paramêtre spécifié dans la fonction \"" + fnName + "\" n'est pas valide.";
    }else if(flag == WBEM_E_INVALID_QUERY){
        errorMsg = "Erreur de syntaxe dans la requête.";
    }else if(flag == WBEM_E_INVALID_QUERY_TYPE){
        errorMsg = "Le language de requête utilisé n'est pas supporté.";
    }else if(flag == WBEM_E_OUT_OF_MEMORY){
        errorMsg = "Il n'y a pas assez de mémoire disponible pour compléter l'opération.";
    }else if(flag == WBEM_E_SHUTTING_DOWN){
        errorMsg = "Le service Windows Management a était arrété et redémarré. Un nouvel appel à \"ConnectServer\" est requis.";
    }else if(flag == WBEM_E_TRANSPORT_FAILURE){
        errorMsg = "WBEM_E_TRANSPORT_FAILURE";
    }else if(flag == WBEM_E_NOT_FOUND){
        errorMsg = "La requête porte sur une classe qui n'existe pas.";
    }else if(flag == WBEM_S_NO_ERROR){
        errorMsg = "La requête a réussie.";
    }

    return errorMsg;
}

//-----------------REQUETES
void QueryWMI::query(char* sqlString, LPCWSTR property, string dataType){
    // Etape 6: --------------------------------------------------
    //On indique à l'objet de type QuerySink la propriétée que l'on veut
    m_pResponseSink->setProperty(property);

    // On utilise le pointeur IWbemServices pour faire une requète à l'interface WMI ----
    // La méthode IWbemService::ExecQueryAsync appeleras la méthode
    // QuerySink::Indicate quand il recevra la réponse de la requête
    // et la méthode QuerySink::Indicate s'executeras.
    m_output = pSvc->ExecQueryAsync(
        bstr_t("WQL"),
        bstr_t(sqlString),
        WBEM_FLAG_BIDIRECTIONAL,
        NULL,
        m_pResponseSink);

    /*
    m_output = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(sqlString),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);
    */

    if (FAILED(m_output)){
        cout << "La requête: '" << sqlString << "' a échouée." << "' Code d'erreur = 0x" << hex << m_output << endl;
        pSvc->Release();
        pLoc->Release();
        m_pResponseSink->Release();
        CoUninitialize();
    }
    /*
        // Etape 7: -------------------------------------------------
        // On récupère la réponse de la requète faite à l'étape 6 -------------------
        ULONG uReturn = 0;
        QString converti = "";
        QString *reponse = new QString;

        //On récupère le premier résultat qu'on renvoi
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if(0 == uReturn){
            return &QString("Un probleme est survenu");
        }

        VARIANT vtProp;

        // Obtenir la valeur d'une propriétée
        hr = pclsObj->Get(property, 0, &vtProp, 0, 0);

        if(dataType == "string"){
            converti = QString((QChar*)vtProp.bstrVal, ::SysStringLen(vtProp.bstrVal));
        }
        else{
            converti = QString::number(vtProp.uiVal);
        }
        *reponse += converti + "\n";

        VariantClear(&vtProp);

        pclsObj->Release();
    */

    // Step 7: -------------------------------------------------
    // On attend le résultat de la requête de l'étape 6 s'il n'arrive pas
    // au bout de 500 milliseconds, on annule l'appel ----------
    //Sleep(500);
    //pSvc->CancelAsyncCall(m_pResponseSink);
}

vector<QString>* QueryWMI::queryAll(char* sqlString, LPCWSTR property, string dataType){
    ULONG uReturn = 0;
    QString converti = "";
    vector<QString>* reponse = new vector<QString>;

    m_output = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(sqlString),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(m_output)){
        std::cout << "Query: '" << sqlString << "' failed." << " Error code = 0x" << hex << m_output << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
    }

    //On récupère tous les résultats
    while (pEnumerator){
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if(0 == uReturn){
            break;
        }

        VARIANT vtProp;

        // Obtenir la valeur d'une propriétée
        hr = pclsObj->Get(property, 0, &vtProp, 0, 0);

        if(dataType == "string"){
            converti = QString((QChar*)vtProp.bstrVal, ::SysStringLen(vtProp.bstrVal));
        }
        else{
            converti = QString::number(vtProp.uiVal);
        }
        reponse->push_back(converti);

        VariantClear(&vtProp);

        pclsObj->Release();
    }

    return reponse;
}

void QueryWMI::setReponse(string reponse){
    m_reponse = QString::QString(reponse.c_str());
    m_callback(m_reponse);
}

void QueryWMI::setCallback(void (*pt2func)(QString)){
    m_callback = pt2func;
}

bool QueryWMI::notInitializedWMI = true;
