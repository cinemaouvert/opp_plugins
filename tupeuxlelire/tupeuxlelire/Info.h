#include <QObject>
#include <QTimer>

#ifndef INFO_H
#define INFO_H

//On crée les flags
#define INFO_UNDEFINED 1 //On crée la constante qui permettra de tester les attributs
#define INFO_FAILED 2
#define INFO_PROVIDE_ERROR 4
#define INFO_TYPE_ERROR 8

#include "AbstractProvider.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class Info : public QObject //Hérite de QObject pour la gestion du multithread
{
    Q_OBJECT //sert à créer des slots, voir à pouvoir le supprimer
public:
    // Constructeur/destructeur
    Info();
    ~Info();

    //Interface
    void defaultSettings();
    void setProvider(AbstractProvider *newProvider);

    //Temps de rafraichissement des données
    void setRefreshTime(int newTime);
    int getRefreshTime() const;
    void setMinRefreshTime(int newTime);
    int getMinRefreshTime() const;

    bool isRunning();
    bool isLocked();
    void stop();

    // Accesseurs
    vector<QString> drivers();
    string cpuTime();
    QString cpuName();
    QString cpuNbCore();
    QString cpuVoltage();
    QString graphicCardName();
    QString graphicCardInfo();
    QString graphicCardBits();
    QString soundCardName();
    QString soundCardId();
    QString os();
    QString osDir();

protected:
    //Mutateurs
    void setDrivers(vector<QString>* newList);
    void setCpuTime(string cpuTime);
    void setCpuName(QString* name);
    void setCpuNbCore(QString* nbCore);
    void setCpuVoltage(QString* voltage);
    void setOs(QString* os);
    void setOsDir(QString* os);
    void setGraphicCardName(QString* name);
    void setGraphicCardInfo(QString* info);
    void setGraphicCardBits(QString* bits);
    void setSoundCardName(QString* name);
    void setSoundCardId(QString* id);

public slots:
    void fetch();

//private:
    //Méthode privée
    void copy();

private:
    //Le fournisseur
    AbstractProvider *m_provider;
    int m_minRefreshTime;
    int m_refreshTime;
    bool m_isRunning;
    QTimer *m_timer;

    // Attributs Statique changé par les fournisseurs
    static int m_number_instance;
    static vector<QString> m_static_drivers;
    static string m_static_cpuTime;
    static QString m_static_cpuName;
    static QString m_static_cpuNbCore;
    static QString m_static_cpuVoltage;
    static QString m_static_os;
    static QString m_static_osDir;
    static QString m_static_graphicCardName;
    static QString m_static_graphicCardInfo;
    static QString m_static_graphicCardBits;
    static QString m_static_soundCardName;
    static QString m_static_soundCardId;

    /*
        Attribut statique permettant de verrouiller la mise à jour des informations
        au cas où plusieurs intances soient présentes.
    */
    static bool m_lock;

    //Attributs d'instance qui sont une copie des statiques et qui permettent la création
    //de plusieurs objets info et donc de figer les informations à un moment donné.
    //Par la suite on verra à créer un historique.
    vector<QString> m_instance_drivers;
    string m_instance_cpuTime;
    QString m_instance_cpuName;
    QString m_instance_cpuNbCore;
    QString m_instance_cpuVoltage;
    QString m_instance_graphicCardName;
    QString m_instance_graphicCardInfo;
    QString m_instance_graphicCardBits;
    QString m_instance_soundCardName;
    QString m_instance_soundCardId;
    QString m_instance_os;
    QString m_instance_osDir;

};

#endif // INFO_H
