
#include <QtGlobal>
#if defined(Q_OS_UNIX)

#include <string>
#include "ProviderLinux.h"

//TEST
#include <iostream>
#include <sstream> // Pour convertir un double en string et vice versa

using std::vector;
using std::string;

//Constructeur/destructeur
ProviderLinux::ProviderLinux():
    AbstractProvider()
{
    //On défini le temps de rafraichissement recommandé.
    m_recommendedRefreshTime = 200;
}
ProviderLinux::~ProviderLinux()
{
}

void ProviderLinux::provide_once(){
    setDrivers(command("lsmod"));
}

void ProviderLinux::provide(){

    //TEMPS DE CHARGE CPU
    QString * cpuTime = new QString();
    vector<QString> output;
    string uptime;

    //On doit récupére le nombre de processeurs avant de calculer l'uptime

    //On cherche à calculer l'utilisation actuelle du cpu en récupérant d'abord l'uptime
    output = command("uptime");
    uptime = output[0].toStdString();

    /*
        On récupère dans la chaine la donnée correspondant à la charge moyenne
        depuis une minute, à savoir la première donnée après "load average:"
    */
    unsigned found = uptime.find_last_of(":"); //D'abord on cherche la dernière occurence des deux petits points dans la phrase
    string result = uptime.substr(found + 1);
    result = result.substr(1,4); //Puis on récupère la donnée

    //On change la virgule par un point
    size_t start_pos = result.find(",");
    if(start_pos != std::string::npos){
        result.replace(start_pos, 1, ".");
    }

    //Maintenant on la change en entier et on la multiplie par 100;
    //double value = atof(result.c_str());

    std::stringstream ss(result);
    float value;
    ss >> value;
    float total = value * 100;

    //On converti le total en QString
    *cpuTime = QString::number(total);

    //Enfin, on renvoi la donnée dans "Info"
    setCpuTime(*cpuTime);

    //-------------------------------------------------//


    /*
    //Ancienne implémentation
    vector<QString*> osV;
    osV.push_back(query(VERSION_SIGNATURE, 1)); //Nom du processeur

    //Calcul du nombre de processus
    vector<QString> test = command("ps -ef");
    int nbProc = test.size() - 1; //On enleve la première ligne qui représente la tete du tableau
    QString *out = new QString(QString::number(nbProc));
    //emit numberProc(out);

    vector<QString*> cpuV;
    cpuV.push_back(query(CPUINFO, 5)); //Nom du processeur
    cpuV.push_back(query(CPUINFO, 13)); //Nombre de core
    cpuV.push_back(query(CPUINFO, 9));

    vector<QString*> graphV;
    vector<QString*> soundV;
    vector<QString*> driverV;

    //emit data(osV, cpuV, graphV, soundV, driverV);
    */
}


QString* ProviderLinux::query(char *path, int numberLine){ //Note: this value is in KB!
    FILE* file = fopen(path, "r");
    char line[228];

    for(int i = 0; i != numberLine; i++){
        fgets(line, 1228, file);
    }

    fclose(file);

    string result = line;

    //On enlève l'identifier pour ne garder que le nom
    size_t commentPos;
    while(result.find(':') != string::npos){
        commentPos = result.find(":");
        result.erase(0 , commentPos + 1);
    }

    QString *output = new QString(result.c_str());

    return output;
}

QString ProviderLinux::queryAll(char *path){ //Note: this value is in KB!
    FILE* file = fopen(path, "r");
    char line[228];

    fgets(line, 1228, file);
    fclose(file);
    return QString(line);
}

vector<QString> ProviderLinux::command(std::string cmd){
    vector<QString> output;
    FILE *fichier;
    char chaine[100];

    fichier = popen(cmd.c_str(),"r");


    while(fgets(chaine, 100,fichier) != NULL)
        output.push_back(QString(chaine));

    return output;
}

#endif // Q_OS_UNIX
