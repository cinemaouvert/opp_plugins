#include <vector>

#include "Environment.h" // Permet de connaitre l'os sur lequel on compile
#include <QTimer>

#include "Info.h"

//Les providers ne peuvent être inclus qu'ici à cause de l'héritage
#include "ProviderLinux.h"
#include "ProviderWMI.h"
#include "ProviderWin32.h"
#include "ProviderMac.h"

using std::vector;
using std::sort;

// Constructeur/destructeur
Info::Info()
{
    Info::m_number_instance += 1;
    m_provider = NULL;
    m_minRefreshTime = 100; //La temps de rafraichissement minimal
    m_refreshTime = m_minRefreshTime;
    m_isRunning = false;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(fetch()));
}

Info::~Info(){}

// LES METHODES DE L'OBJET
void Info::defaultSettings(){ //La fonction permettant de mettre les réglages de l'objet info par defaut

    // Définition des fournisseurs par défaut selon l'os.
    #if defined(OS_LINUX)
        ProviderLinux *provider = new ProviderLinux;
    #elif defined(OS_WIN)
        ProviderWMI *provider = new ProviderWMI;
        //ProviderWin32 *provider = new ProviderWin32;
    #elif defined(OS_MAC)
        ProviderMac *provider = new ProviderMac;
    #endif

    setProvider(provider);
}

void Info::setProvider(AbstractProvider *newProvider){
    //On rêgle le temps de rafraichissement minimal selon celui recommendé par le fournisseur.
    if(newProvider->recommendedRefreshTime() > 0){
        setMinRefreshTime(newProvider->recommendedRefreshTime());
    }

    m_provider = newProvider;
}

void Info::fetch(){
    if(!Info::m_lock){
        Info::m_lock = true;

        // Si le provider n'est pas défini on initialise les réglages par defaut puis on va chercher les informations immuables
        if(m_provider == NULL){
            defaultSettings();
        }

        if(m_isRunning){
            if(m_provider->firstDelivery()){
                m_provider->provide_once();
                m_provider->setFirstDelivery(false);
            }

            //On lance le fournisseur d'informations renouvellables
            m_provider->provide();

            //On fait une copie des variables statiques d'Info dans l'instance actuelle
            copy();
        }else{
            m_isRunning = true;
            Info::m_lock = false;

            fetch();

            m_isRunning = false;

            //Mise à jour des données toutes les demi secondes
            m_timer->start(m_refreshTime);
        }
        Info::m_lock = false;
    }
}

bool Info::isLocked(){
    return Info::m_lock;
}

bool Info::isRunning(){
    return m_isRunning;
}

void Info::stop(){
    m_timer->stop();
    m_isRunning = false;
}

void Info::setRefreshTime(int newTime){
    if(newTime >= m_minRefreshTime){
        m_timer->stop();
        m_refreshTime = newTime;
        m_timer->start(m_refreshTime);
    }else{
        m_timer->stop();
        m_refreshTime = m_minRefreshTime;
        m_timer->start(m_refreshTime);
    }
}

int Info::getRefreshTime() const
{
    return m_refreshTime;
}


void Info::setMinRefreshTime(int newTime){
    m_minRefreshTime = newTime;
}

int Info::getMinRefreshTime() const
{
    return m_minRefreshTime;
}

//LES MUTATEURS
void Info::setDrivers(vector<QString>* newList){
    //On tri la liste par ordre alphabétique
    sort(newList->begin(), newList->end());
    Info::m_static_drivers = *newList;
}

void Info::setCpuTime(string cpuTime){
    Info::m_static_cpuTime = cpuTime;
}

void Info::setCpuName(QString* name){
    Info::m_static_cpuName = *name;
}

void Info::setCpuNbCore(QString* nbCore){
    Info::m_static_cpuNbCore = *nbCore;
}

void Info::setCpuVoltage(QString* voltage){
    Info::m_static_cpuVoltage = *voltage;
}

void Info::setGraphicCardName(QString* name){
    Info::m_static_graphicCardName = *name;
}

void Info::setGraphicCardInfo(QString* info){
    Info::m_static_graphicCardInfo = *info;
}

void Info::setGraphicCardBits(QString* bits){
    Info::m_static_graphicCardBits = *bits;
}

void Info::setSoundCardName(QString* name){
    Info::m_static_soundCardName = *name;
}

void Info::setSoundCardId(QString* id){
    Info::m_static_soundCardId = *id;
}

void Info::setOs(QString* os){
    Info::m_static_os = *os;
}

void Info::setOsDir(QString* osDir){
    Info::m_static_osDir = *osDir;
}

//LES ACCESSEURS
vector<QString> Info::drivers(){
    return m_instance_drivers;
}

string Info::cpuTime(){
    return m_instance_cpuTime;
}

QString Info::cpuName(){
    return m_instance_cpuName;
}

QString Info::cpuNbCore(){
    return m_instance_cpuNbCore;
}

QString Info::cpuVoltage(){
    return m_instance_cpuVoltage;
}

QString Info::graphicCardName(){
    return m_instance_graphicCardName;
}

QString Info::graphicCardInfo(){
    return m_instance_graphicCardInfo;
}

QString Info::graphicCardBits(){
    return m_instance_graphicCardBits;
}

QString Info::soundCardName(){
    return m_instance_soundCardName;
}

QString Info::soundCardId(){
    return m_instance_soundCardId;
}

QString Info::os(){
    return m_instance_os;
}

QString Info::osDir(){
    return m_instance_osDir;
}

//LES METHODES PRIVÉES
//Copie des attributs statiques dans ceux de l'instance actuelle.
void Info::copy(){
    m_instance_drivers = Info::m_static_drivers;
    m_instance_cpuTime = Info::m_static_cpuTime;
    m_instance_cpuName = Info::m_static_cpuName;
    m_instance_cpuNbCore = Info::m_static_cpuNbCore;
    m_instance_cpuVoltage = Info::m_static_cpuVoltage;
    m_instance_graphicCardName = Info::m_static_graphicCardName;
    m_instance_graphicCardInfo = Info::m_static_graphicCardInfo;
    m_instance_graphicCardBits = Info::m_static_graphicCardBits;
    m_instance_soundCardName = Info::m_static_soundCardName;
    m_instance_soundCardId = Info::m_static_soundCardId;
    m_instance_os = Info::m_static_os;
    m_instance_osDir = Info::m_static_osDir;
}

//On initialise les attributs statiques
int Info::m_number_instance = 0;
bool Info::m_lock = false;
vector<QString> Info::m_static_drivers;
string Info::m_static_cpuTime = "undefined";
QString Info::m_static_cpuName = "undefined";
QString Info::m_static_cpuNbCore = "undefined";
QString Info::m_static_cpuVoltage = "undefined";
QString Info::m_static_graphicCardName = "undefined";
QString Info::m_static_graphicCardInfo = "undefined";
QString Info::m_static_graphicCardBits = "undefined";
QString Info::m_static_soundCardName = "undefined";
QString Info::m_static_soundCardId = "undefined";
QString Info::m_static_os = "undefined";
QString Info::m_static_osDir = "undefined";

