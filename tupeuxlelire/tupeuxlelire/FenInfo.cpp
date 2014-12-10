#include "FenInfo.h"
#include "Info.h"

//TEST
#include <QtConcurrent/QtConcurrent>

FenInfo::FenInfo(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    info = new Info;
    info->fetch();
    m_refreshTime = 100;

    //Sous linux l'onglet "driver" devient "module"
    #if defined(Q_OS_LINUX)
        tabWidget->setTabText(5, "Module");
    #endif

    displayData();
}

FenInfo::FenInfo(Info* infoObj, QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    info = infoObj;
    info->fetch();
    m_refreshTime = 100;

    //Sous linux l'onglet "driver" devient "module"
    #if defined(Q_OS_LINUX)
        tabWidget->setTabText(5, "Module");
    #endif

    displayData();
}

Info* FenInfo::getInfo(){
    return info;
}

void FenInfo::setInfo(Info* newInfo){
    info = newInfo;
    displayData();
}

void FenInfo::displayData(){
    //Drivers
    string sautDeLigne = "\n";

    #if defined(Q_OS_LINUX) //On enlêve les saut de ligne sur linux
        sautDeLigne = "";
    #endif

    driverNom->setText("");
    for(int i = 0; i <info->drivers().size(); i++){
        if(i == info->drivers().size() - 1){
            sautDeLigne = "";
        }
        driverNom->setText(driverNom->text() + info->drivers()[i] + sautDeLigne.c_str());
    }

    //OS
    osVersion->setText(info->os());
    osDirectory->setText(info->osDir());

    //CPU
    cpuName->setText(info->cpuName());
    cpuNbCore->setText("Nombre de processeurs : " + info->cpuNbCore());
    cpuVoltage->setText("Voltage actuel : " + info->cpuVoltage());

    //GRAPHIC CARD
    nomCg->setText(info->graphicCardName());
    graphicInfo->setText(info->graphicCardInfo());
    graphicBits->setText("Résolution (bit par pixel) : " + info->graphicCardBits());

    //SOUND CARD
    QTableWidgetItem *csName = new QTableWidgetItem(info->soundCardName());
    tableSound->setItem(0, 0, csName);

    QTableWidgetItem *csId = new QTableWidgetItem(info->soundCardId());
    tableSound->setItem(0, 1, csId);

    //MISE A JOUR DES DONNEES RENOUVELLABLE
    updateData();

    //Mise a jour des informations affichées.
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
    timer->start(m_refreshTime);
}

//Permet la mise à jour des informations changeantes.
void FenInfo::updateData(){  
    //Cpu
    if(cpuUsed->text() != QString::QString(info->cpuTime().c_str())){
        cpuUsed->setText("UC : " + QString::QString(info->cpuTime().c_str()) + "%");
    }
}

// SLOT INFORMATION SYSTEME
void FenInfo::data(vector <QString*> osV, vector <QString*> cpuV, vector <QString*> graphV, vector <QString*> soundV, vector <QString*> driverV){
    /*
    os(osV);
    cpu(cpuV);
    graphicCard(graphV);
    carteSon(soundV);
    drivers(driverV);
    */
}

void FenInfo::numberProc(QString *nbProc){
    //osNbProc->setText("Nombre de processus en cours : " + *nbProc);
}
void FenInfo::currentClockSpeed(QString *clockSpeed){
    //cpuSpeed->setText("Vitesse actuelle (en MHZ) : " + *clockSpeed);
}

// FONCTIONS QUI TRAITENT LES DONNEES
void FenInfo::memory(vector <vector<QString>*> memo){
    /*
        memo[0] correspond aux lettres de lecteur
        memo[1] correspond aux espaces libres
        memo[2] correspond aux tailles total
        memo[3] correspond aux vitesse en lecture
        memo[4] correspond aux vitesse en ecriture
        memo[5] correspond aux systemmes de fichier
    */
    /*
    //On défini le nombre de lignes du tableau
    ui->tableMemoire->setRowCount(memo[0]->size());
    QTableWidgetItem *item;

    int i = 0;
    for(i = 0; i<memo[0]->size(); i++){ //Lettre de lecteur
        item = new QTableWidgetItem(memo[0][0][i]);
        tableMemoire->setItem(i, 0, item);
    }

    i = 0;
    for(i = 0; i<memo[1]->size(); i++){ //Type de lecteur
        item = new QTableWidgetItem(memo[1][0][i]);
        tableMemoire->setItem(i, 1, item);
    }

    i = 0;
    for(i = 0; i<memo[2]->size(); i++){ //Espace libre
        item = new QTableWidgetItem(memo[2][0][i]);
        tableMemoire->setItem(i, 2, item);
    }

    i = 0;
    for(i = 0; i<memo[3]->size(); i++){ //Taille totale
        item = new QTableWidgetItem(memo[3][0][i]);
        ui->tableMemoire->setItem(i, 3, item);
    }

    i = 0;
    for(i = 0; i<memo[4]->size(); i++){ //Vitesse en lecture
        item = new QTableWidgetItem(memo[4][0][i] + "/sec");
        tableMemoire->setItem(i, 4, item);
    }

    i = 0;
    for(i = 0; i<memo[5]->size(); i++){ //Vitesse en ecriture
        item = new QTableWidgetItem(memo[5][0][i] + "/sec");
        tableMemoire->setItem(i, 5, item);
    }

    i = 0;
    for(i = 0; i<memo[6]->size(); i++){ //System de fichier
        item = new QTableWidgetItem(memo[6][0][i]);
        tableMemoire->setItem(i, 6, item);
    }
    */
}

void FenInfo::graphicCard(vector <QString*> graph){
    /*
    graphicRefresh->setText("Rafraichissement Maximum : " + *graph[3] + "Rafraichissement Minimum : " + *graph[4]);
    */
}

//PRINT DU SYSTEME
void FenInfo::print(){
    QString pathString = "";

    //A TERMINER
    if(printPath.isEmpty())
    {
        //WINDOWS ONLY
        QFileDialog dialog(this);
        dialog.setWindowTitle(tr("Définir le dossier de capture"));
        dialog.setDirectory("c:/users");
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setViewMode(QFileDialog::List);
        if (dialog.exec())
            printPath = dialog.selectedFiles();
    }

    if(!printPath.isEmpty())
    {
        //On itere sur la QStringList afin d'écrire dans tous les dossiers choisis
        QStringList::const_iterator constIterator;
        for (constIterator = printPath.constBegin(); constIterator != printPath.constEnd(); ++constIterator)
            pathString = (*constIterator).toLocal8Bit().constData();
            writeFile(pathString.toStdString());
    }
}

void FenInfo::writeFile(std::string filePath){
    /*
    CREATION DU FICHIER
    On crée un fichier dont le nom est l'heure actuelle.
    On le met dans fichier crée don le nom est la date
    */
    time_t temps;
    struct tm *instant;
    instant = 0;

    std::time(&temps);
    #if defined(Q_OS_WIN)
        localtime_s(instant, &temps);
    #elif defined(Q_OS_LINUX)
        localtime(&temps);
    #endif

    std::string day = Util::intToString(instant->tm_mday);
    std::string mon = Util::intToString(instant->tm_mon + 1);
    std::string year = Util::intToString(instant->tm_year + 1900); //year since 1900
    std::string hour = Util::intToString(instant->tm_hour);
    std::string min = Util::intToString(instant->tm_min);
    std::string sec = Util::intToString(instant->tm_sec);

    if(day.size() == 1){
        day = "0" + day;
    }
    if(mon.size() == 1){
        mon = "0" + mon;
    }
    if(hour.size() == 1){
        hour = "0" + hour;
    }
    if(min.size() == 1){
        min = "0" + min;
    }
    if(sec.size() == 1){
        sec = "0" + sec;
    }

    std::string folderName = filePath + "/" + day + "-" + mon + "-" + year;
    std::string fileName = folderName + "/" + hour + "h." + min + "m." + sec + "s.txt";

    #if defined(Q_OS_WIN)
        _mkdir(folderName.c_str());
    #elif defined(Q_OS_LINUX)
        mkdir(folderName.c_str(), 0777);
    #endif

    std::ofstream file(fileName.c_str(), ios::trunc);

    /*
    ECRITURE DANS LE FICHIER
    On ecrit toutes les informations actuelle dans le fichier
    */

    file << "OS : \n";
    file << osVersion->text().toStdString();
    file << osNbProc->text().toStdString();
    file << osDirectory->text().toStdString();
    file << "\n";

    file << "CPU : \n";
    file << cpuName->text().toStdString();
    file << cpuNbCore->text().toStdString();
    file << cpuSpeed->text().toStdString();
    file << cpuVoltage->text().toStdString();
    file << cpuUsed->text().toStdString();
    file << "\n\n";

    file << "MEMOIRE : \n";

    QTableWidgetItem *mem1 = tableMemoire->item(0, 0);
    file << mem1->text().toStdString();

    QTableWidgetItem *mem2 = tableMemoire->item(0, 1);
    file << mem2->text().toStdString();

    QTableWidgetItem *mem3 = tableMemoire->item(0, 2);
    file << mem3->text().toStdString();

    QTableWidgetItem *mem4 = tableMemoire->item(0, 3);
    file << mem4->text().toStdString();

    file << "\n";

    file << "CARTE GRAPHIQUE : \n";
    file << nomCg->text().toStdString();
    file << graphicInfo->text().toStdString();
    file << graphicBits->text().toStdString();
    file << graphicRefresh->text().toStdString();
    file << "\n";

    file << "CARTE SON : \n";
    QTableWidgetItem *son1 = tableSound->item(0, 0);
    file << son1->text().toStdString();

    QTableWidgetItem *son2 = tableSound->item(0, 1);
    file << son2->text().toStdString();
    file << "\n";

    file << "DRIVER : \n";
    file << driverNom->text().toStdString();
    file << "\n";

}

FenInfo::~FenInfo()
{
    delete info;
}
