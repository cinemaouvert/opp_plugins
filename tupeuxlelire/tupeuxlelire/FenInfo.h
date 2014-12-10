
#ifndef FENINFO_H
#define FENINFO_H

#include <QMainWindow>
#include <string>
#include <ctime>
#include <fstream>
#include <QFileDialog>
#include <QThread>
#include <QTimer>
#include <vector>

//Inclusion programme
#include "Util.h"
#include "Info.h"
#include <ui_FenInfo.h>

#include <iostream>//tmp

//WINDOWS
#if defined(Q_OS_WIN)
#include <direct.h> //Création de dossier sous windows
#elif defined(Q_OS_LINUX)
#include "sys/stat.h" //Création de dossier sous UNIX
#endif

using std::vector;

class FenInfo : public QMainWindow, private Ui::FenInfo
{
    Q_OBJECT
//NOUVELLE VERSION
public:
    explicit FenInfo(QWidget *parent = 0);
    explicit FenInfo(Info* infoObj, QWidget *parent = 0);
    void setInfo(Info* newInfo);
    Info* getInfo();

private:
    void displayData();

public slots:
    void updateData();
//-----------------------
    void graphicCard(vector<QString*> graph);

public slots:
    void print(); //Permet de faire un print de la machine

    void data(vector <QString*> osV, vector <QString*> cpuV, vector <QString*> graphV, vector <QString*> soundV, vector <QString*> driverV);
    void memory(vector <vector <QString>*> memo);
    void numberProc(QString *nbProc);
    void currentClockSpeed(QString *clockSpeed);

public:
    ~FenInfo();

private:
    void writeFile(std::string filePath);
    QStringList printPath;
    Info* info;
    int m_refreshTime;
};

#endif // FENINFO_H
