
#include "Environment.h" // Permet de connaitre l'os sur lequel on compile
#if defined(OS_WIN)

#include "ProviderWMI.h"
#include "QuerySink.h"

//TEST
#include <iostream>
using std::cout;

// Constructeur/destructeur
ProviderWMI::ProviderWMI():
    AbstractProvider()
{
    //On défini le temps de rafraichissement recommandé.
    m_recommendedRefreshTime = 600;


    //=======  QUERY  =========//

    m_pResponseSink = new QuerySink;
    m_output = 0;
    pLoc = NULL;
    pSvc = NULL;
    pEnumerator = NULL;

    initializeWMI();

    //=========================//
}

ProviderWMI::~ProviderWMI()
{
    delete m_pResponseSink;
    delete pLoc;
    delete pSvc;
    delete pEnumerator;
    delete pclsObj;
}

// Methode provide once
void ProviderWMI::provide_once()
{
    if(!ProviderWMI::m_isProviding){
        ProviderWMI::m_isProviding = true;

        //On initialise l'interface
        //initializeWMI();
        /*
        //------------DEJA IMPLEMENTE---------------
        //DRIVERS
        setDrivers(queryAll("SELECT * FROM Win32_SystemDriver", L"Caption", "string"));

        //OS
        setOs(query("SELECT Name FROM Win32_OperatingSystem", L"Name", "string"));
        setOsDir(query("SELECT * FROM Win32_OperatingSystem", L"WindowsDirectory", "string"));

        //CPU
        setCpuName(query("SELECT * FROM Win32_Environment WHERE Name = 'PROCESSOR_IDENTIFIER'", L"VariableValue", "string"));
        setCpuNbCore(query("SELECT * FROM Win32_Environment WHERE Name = 'NUMBER_OF_PROCESSORS'", L"VariableValue", "string"));
        setCpuVoltage(query("SELECT * FROM Win32_Processor", L"CurrentVoltage", "int"));

        //GRAPHIC CARD
        setGraphicCardName(query("SELECT * FROM CIM_PCVideoController", L"Caption", "string"));
        setGraphicCardInfo(query("SELECT * FROM Win32_VideoController WHERE Availability = '3'", L"VideoModeDescription", "string"));
        setGraphicCardBits(query("SELECT * FROM Win32_VideoController WHERE Availability = '3'", L"CurrentBitsPerPixel", "int"));

        //CARTE SON
        setSoundCardName(query("SELECT * FROM Win32_SoundDevice", L"Caption", "string"));
        setSoundCardId(query("SELECT * FROM Win32_SoundDevice", L"DeviceId", "string"));

        //--------A RE-IMPLEMENTER--------------
        //GRAPHIC CARD: peut être renouvellable
        vector <QString*> graphV;
        graphV.push_back(query("SELECT * FROM Win32_VideoController WHERE Availability = '3'", L"MaxRefreshRate", "int"));
        graphV.push_back(query("SELECT * FROM Win32_VideoController WHERE Availability = '3'", L"MinRefreshRate", "int"));
        */
        //On nettoie et on ferme l'interface
        //cleanUp();

        ProviderWMI::m_isProviding = false;
    }
}
//---TEST
int testIncrement = 0;
void ProviderWMI::callbackTest(string reponse)
{
    setCpuTime(reponse);
}

// Methode provide
void ProviderWMI::provide()
{
    //if(!m_isProviding){
        //m_isProviding = true;

        //CPU
        //-------POURCENTAGE UTILISATION DU CPU A REVOIR-------------
        //Commenter/décommenter: prouve que c'est les requètes wmi qui font freezer
        //QString* cpuTime =
        //QueryWMI* cpuTime = new QueryWMI("SELECT PercentPrivilegedTime FROM Win32_PerfFormattedData_Counters_ProcessorInformation WHERE Name = '_total'", L"PercentPrivilegedTime", "string");

        //setCpuTime(*cpuTime);

        //------------------TEST-------------------------//
        //On défini un pointeur de fonction
        //void (*ptr)(QString);

        //On assigne au pointeur de fonction la fonction de callback
        //pt2fn = callbackTest;
        //ptr = &ProviderWMI::callbackTest;

        //cpuTime1->setCallback(ptr);
        QuerySink* responseSink = new QuerySink;
        query("SELECT PercentPrivilegedTime FROM Win32_PerfFormattedData_Counters_ProcessorInformation WHERE Name = '_total'", L"PercentPrivilegedTime", "string", responseSink);
        //--------------------------------------------------//

        //testIncrement += 1;
        //setCpuTime(QString::number(testIncrement)); //Prouve que c'est les requètes WMI qui font freezer l'affichage.

        //m_isProviding = false;
    //}

    /*
    makeQueries();
    memory();
    numberProc();
    currentClockSpeed();
    cpuTime();

    //Mémoire
    QTimer *timer0 = new QTimer(this);
    //connect(timer0, SIGNAL(timeout()), this, SLOT(memory()));
    timer0->start(600);

    //Nombre de processus en cours
    QTimer *timer1 = new QTimer(this);
    //connect(timer1, SIGNAL(timeout()), this, SLOT(numberProc()));
    timer1->start(600);

    //Vitesse du processeur
    QTimer *timer2 = new QTimer(this);
    //connect(timer2, SIGNAL(timeout()), this, SLOT(currentClockSpeed()));
    timer2->start(600);

    //Pourcentage d'utilisation du CPU
    QTimer *timer3 = new QTimer(this);
    //connect(timer3, SIGNAL(timeout()), this, SLOT(cpuTime()));
    timer3->start(600);
    */
}

//Methodes permettant d'interagir avec l'interface WMI




//----INFO--> envoi des signaux: REQUETES PROPRES AU PROGRAMME

//Les données en constant renouvellement
/*
void ProviderWMI::numberProc(){
    m_nbProc = query("SELECT NumberOfProcesses FROM Win32_OperatingSystem", L"NumberOfProcesses", "int");

    //emit nbProc(m_nbProc);
}

void ProviderWMI::currentClockSpeed(){
    m_clockSpeed = query("SELECT CurrentClockSpeed FROM Win32_Processor", L"CurrentClockSpeed", "int");

    //emit currentClockSpeed(m_clockSpeed);
}

void ProviderWMI::memory(){
    //Lettre de lecteur
    vector <vector<QString>*> memoV;
    memoV.push_back(queryAll("SELECT Caption FROM Win32_LogicalDisk", L"Caption", "string"));

    //Type de lecteur
    vector<QString> *driveType = queryAll("SELECT DriveType FROM Win32_LogicalDisk", L"DriveType", "int");
    for(int i = 0; i<driveType->size(); i++){
        int val = driveType[0][i].toInt();
        switch (val) {
        case 3:
            driveType[0][i] = QString("Hard Drive");
            break;
        case 4:
            driveType[0][i] = QString("Lecteur Reseau");
            break;
        case 5:
            driveType[0][i] = QString("CD-ROM");
            break;
        default:
            break;
        }
    }
    memoV.push_back(driveType);


    //Calcul du pourcentage de mémoire restante
    vector<QString> *memoryFree = queryAll("SELECT FreeSpace FROM Win32_LogicalDisk", L"FreeSpace", "string");
    vector<QString> *size = queryAll("SELECT Size FROM Win32_LogicalDisk", L"Size", "string");
    Util::convertMegaOctet(size);
    Util::convertMegaOctet(memoryFree);

    //*memoryFree = (*memoryFree * 100)/ *size; //Calcul du pourcentage
    vector<QString> *memoryFreeSpace = memoryFree;
    vector<QString> *memorySize = size;

    memoV.push_back(memoryFreeSpace);
    memoV.push_back(memorySize);

    //Vitesse de lecture et d'écriture
    vector<QString> *diskReadBytesPerSec = queryAll("SELECT DiskReadBytesPersec FROM Win32_PerfRawData_PerfDisk_LogicalDisk", L"DiskReadBytesPersec", "string");
    Util::convertMegaOctet(diskReadBytesPerSec);
    memoV.push_back(diskReadBytesPerSec);

    vector<QString> *diskWriteBytesPerSec = queryAll("SELECT DiskWriteBytesPersec FROM Win32_PerfRawData_PerfDisk_LogicalDisk", L"DiskWriteBytesPersec", "string");
    Util::convertMegaOctet(diskWriteBytesPerSec);
    memoV.push_back(diskWriteBytesPerSec);

    //Système de fichier
    vector<QString> *fileSystem = queryAll("SELECT FileSystem FROM Win32_LogicalDisk", L"FileSystem", "string");
    memoV.push_back(fileSystem);

    //emit memory(memoV);
}

//-------POURCENTAGE UTILISATION DU CPU A REVOIR-------------
void ProviderWMI::cpuTime(){
    m_cpuTime = query("SELECT PercentPrivilegedTime FROM Win32_PerfFormattedData_Counters_ProcessorInformation WHERE Name = '_total'", L"PercentPrivilegedTime", "string");

    //emit cpuTime(m_cpuTime);
}
*/
/*
void ProviderWMI::run(){
    initializeWMI();

    provide_once();
    provide();

    exec();
}
*/

//===========================================//
//============|    QUERY    |================//
//===========================================//

//Méthodes
void ProviderWMI::initializeCOM(){
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

void ProviderWMI::comSecurityLevels(){

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

void ProviderWMI::initialLocator(){
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

void ProviderWMI::connectWMI(){
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

void ProviderWMI::proxySecurityLevels(){
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

void ProviderWMI::initializeWMI(){
    //On initialise l'interface WMI
    if(notInitializedWMI){
        //initializeCOM(); // COM est déjà initialisé par QApplication
        comSecurityLevels();
        initialLocator();
        connectWMI();
        proxySecurityLevels();

        notInitializedWMI = false;
    }
}

//Remet tout à zéro
void ProviderWMI::cleanUp(){
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    m_output = 1;
}

string ProviderWMI::errorMsg(long flag, string fnName){
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
void ProviderWMI::query(char* sqlString, LPCWSTR property, string dataType, QuerySink* responseSink){
    // Etape 6: --------------------------------------------------
    //On indique à l'objet de type QuerySink la propriétée que l'on veut
    responseSink->setParent(this);
    responseSink->setProperty(property);

    // On utilise le pointeur IWbemServices pour faire une requète à l'interface WMI ----
    // La méthode IWbemService::ExecQueryAsync appeleras la méthode
    // QuerySink::Indicate quand il recevra la réponse de la requête
    // et la méthode QuerySink::Indicate s'executeras.
    m_output = pSvc->ExecQueryAsync(
        bstr_t("WQL"),
        bstr_t(sqlString),
        WBEM_FLAG_BIDIRECTIONAL,
        NULL,
        responseSink);

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
        responseSink->Release();
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

vector<QString>* ProviderWMI::queryAll(char* sqlString, LPCWSTR property, string dataType){
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


bool ProviderWMI::notInitializedWMI = true;

//===========================================//
//===========================================//
//===========================================//

//Initialisation des attributs statiques
bool ProviderWMI::m_isProviding = false;
bool ProviderWMI::m_securityInitialized = false;

#endif // OS_WIN
