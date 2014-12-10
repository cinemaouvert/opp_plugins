#include <QApplication>
#include <QTimer>

//TEST
#include <iostream>

//On inclus les classes du programme
#include "FenInfo.h"
#include "Info.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
        NOTE:
            - tests du projet en faisant hériter les fournisseurs de QObject à faire ainsi que les signaux et slots
            - Améliorer les drivers version linux.
            - Réfléchir aux problématiques de l'historique.
            - test du multithread
    */

    //Création de l'objet information
    //PROBLEME QUAND IL Y A DEUX INSTANCE, A CAUSE DE WMI
    Info *information = new Info;

    information->fetch();
    information->setRefreshTime(600);

    FenInfo *w = new FenInfo(information);
    w->show();

    return a.exec();
}

