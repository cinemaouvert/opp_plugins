/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2014 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Geoffrey Bergé <geoffrey.berge@live.fr>
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include "plugin_ocpm.h"
#include "ui_plugin_ocpm.h"

#include <QProcess>
#include <QListWidgetItem>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QCheckBox>
#include <QFileDialog>
#include <QtXml>
#include <QMessageBox>


Q_EXPORT_PLUGIN2(Plugin_Ocpm, Plugin_Ocpm)

Plugin_Ocpm::Plugin_Ocpm(QWidget *parent) :
    ui(new Ui::Plugin_Ocpm),
    FICHIERXML("info.xml"),
    NAMEMD5("onlinesum")
{
    ui->setupUi(this);
    ui->treeWidget_information->setHeaderLabel(tr("File : "));
    ui->textBrowser_xml->setReadOnly(1);
}

Plugin_Ocpm::~Plugin_Ocpm()
{
    delete ui;
}

void Plugin_Ocpm::getInfo(QString path)
{
    ui->treeWidget_information->clear();
    ui->textBrowser_xml->clear();

    int i = path.lastIndexOf(QDir::separator());
    QString title = tr("File : ") + path.right(path.length()-i-1);
    ui->treeWidget_information->setHeaderLabel(title);

    QStringList args;
    args<<"--ui-language"<<"fr"<<path;

    QProcess proc;
    proc.start("mkvinfo",args);
    proc.waitForFinished(-1);

    QString output = QString::fromUtf8(proc.readAllStandardOutput());
    QStringList outList = output.split("\n");

    if(outList.at(0).contains("Erreur") || outList.at(0).contains("Error") ) {
        QStringList args;
        args<<"--ui-language"<<"fr_FR"<<path;

        QProcess proc;
        proc.start("mkvinfo",args);
        proc.waitForFinished(-1);


        QString output = QString::fromUtf8(proc.readAllStandardOutput());
        outList = output.split("\n");
        proc.close();
    }

    parceTracks(outList);
    parceAttach(outList);

    parceXML();

    if(_linkMD5 != ""){
        if(checkMd5(_linkMD5)){
            ui->label_md5->setText(tr("MD5 valide"));
        }else{
            ui->label_md5->setText(tr("MD5 invalide"));
        }
    }

    QApplication::restoreOverrideCursor();
}

void Plugin_Ocpm::parceTracks(const QStringList &outList)
{
    int nb =1;
    for (int i = 0; i < outList.count(); ++i) {

        if (outList.at(i).contains("| + Une piste")) {

            QStringList* list = new QStringList();
            list->append(tr("Track ") + QString::number(nb));
            QTreeWidgetItem* track = new QTreeWidgetItem(*list);

            i++;

            QString tempStr;
            while (outList.at(i).contains(QRegExp("\\|\\ \\ \\+\\ .*")) || outList.at(i).contains(QRegExp("\\|\\ \\ \\ \\+\\ .*")) ) {
                tempStr = outList.at(i);

                if(outList.at(i).contains(QRegExp("\\|\\ \\ \\+\\ .*")))
                    tempStr = tempStr.remove("|  + ");
                else if(outList.at(i).contains(QRegExp("\\|\\ \\ \\ \\+\\ .*")))
                    tempStr = tempStr.remove("|   + ");

                QStringList* l = new QStringList();
                l->append(tempStr);
                QTreeWidgetItem* param = new QTreeWidgetItem(*l);
                track->addChild(param);
                i++;
            }
            ui->treeWidget_information->addTopLevelItem(track);
            nb++;
            i--;
        }
    }
}

void Plugin_Ocpm::parceAttach(const QStringList &outList)
{
    int nb =1;
    for (int i = 0; i < outList.count(); ++i) {
        if (outList.at(i).contains("| + Joints")) {

            QStringList* list = new QStringList();
            list->append(tr("Attachment ") + QString::number(nb));
            QTreeWidgetItem* track = new QTreeWidgetItem(*list);
            track->setFlags(track->flags()|Qt::ItemIsUserCheckable);
            track->setCheckState(0,Qt::Unchecked);

            i++;

            QString tempStr;
            while (outList.at(i).contains(QRegExp("\\|\\ \\ \\+\\ .*")) || outList.at(i).contains(QRegExp("\\|\\ \\ \\ \\+\\ .*")) ) {
                tempStr = outList.at(i);

                if(outList.at(i).contains(QRegExp("\\|\\ \\ \\+\\ .*")))
                    tempStr = tempStr.remove("|  + ");
                else if(outList.at(i).contains(QRegExp("\\|\\ \\ \\ \\+\\ .*")))
                    tempStr = tempStr.remove("|   + ");

                QStringList* l = new QStringList();
                l->append(tempStr);
                QTreeWidgetItem* param = new QTreeWidgetItem(*l);

                if(tempStr.contains(FICHIERXML)) {
                    extract(*_filename,0,QString::number(nb));
                }
                track->addChild(param);
                i++;
            }
            ui->treeWidget_information->addTopLevelItem(track);
            _listItems.append(track);
            nb++;
            i--;
        }
    }

}

void Plugin_Ocpm::on_buttonBox_OKCancel_accepted()
{
    this->hide();
}


void Plugin_Ocpm::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}

void Plugin_Ocpm::extract(QString filepath, int mode, QString id,QString outputName)
{
    QString modeStr = ((mode == 0) ? QString("attachments") : QString("tracks"));
    QProcess* process = new QProcess();
    QStringList args;

    args<<modeStr<<filepath<<id;

    process->setWorkingDirectory(outputName);
    process->start("mkvextract",args);
    process->waitForFinished(-1);
    process->close();
}

QString Plugin_Ocpm::getName()
{
    return "OCPM plugin";
}

void Plugin_Ocpm::launch()
{
    this->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    _listItems.clear();
    if(_filename != NULL){
        if((*_filename).compare("") != 0){
            this->getInfo(*_filename);
        }
    }
}

void Plugin_Ocpm::setFilename(QString * filename)
{
    this->_filename = filename;
    this->_linkMD5 = "";
}



void Plugin_Ocpm::on_pushButton_attachment_clicked()
{
    int cpt=0;
    foreach (QTreeWidgetItem *item, _listItems) {
        if (item->checkState(0) == Qt::Checked) {
            cpt++;
        }
    }
    if(_listItems.count() > 0 && cpt >0){
        QString dir = QFileDialog::getExistingDirectory(this, tr("Set Output Folder"),QDir::homePath());
        if(!dir.isEmpty()) {

            foreach (QTreeWidgetItem *item, _listItems) {
                if (item->checkState(0) == Qt::Checked) {
                    extract(*_filename,0,item->text(0).remove(tr("Attachment ")),dir);
                }
            }
            if(cpt > 0)
                QMessageBox::information(this, tr("Extracting"),tr("Attachments were extracted."));
            else
                QMessageBox::information(this, tr("Extracting"),tr("No attachments selected."));
        }
    }else{
        QMessageBox::information(this, tr("Extracting"),tr("There is no attachments to extract."));
    }

}

void Plugin_Ocpm::parceXML() {


    QFile file(FICHIERXML);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("File info.xml was not found."),tr("File info.xml was not found."));
    }else{
        QString xml = QString::fromUtf8(file.readAll());
        QStringList listXml = xml.split("\n");

        for (int i = 2; i < listXml.count()-1; i++) {
            QString tmp = listXml.at(i);

            QString elt = tmp;
            int pos = elt.lastIndexOf('<');
            elt = elt.left(pos);
            pos = elt.indexOf('>');
            elt = elt.right(elt.length()-pos-1);

            QString name = tmp;
            pos = name.indexOf('>');
            name = name.left(pos);
            pos = name.indexOf('<');
            name = name.right(name.length()-pos-1);


            if(name.contains(NAMEMD5)) {
                _linkMD5 = elt;
            }

            if(name.contains("dc:")) {
                name.remove("dc:");
                name = name.toUpper();
                name += " : ";

                if(name.contains("ONLINEINFO")) {
                    ui->textBrowser_xml->setOpenExternalLinks(true);
                    ui->textBrowser_xml->append(name+"<a href=\"" + elt + "\">" + elt + "</a>"+"\n");
                    ui->textBrowser_xml->append("");
                }
                else {
                    ui->textBrowser_xml->append(name + elt + "\n");
                }
            }
        }

        file.close();
        file.remove();
    }
}

bool Plugin_Ocpm::checkMd5(QString filename){
    QNetworkAccessManager manager(this);

    QUrl url_version (filename);
    QNetworkRequest request(url_version);
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    bool result = false;
    QString md5toCheck;
    if (reply->error() == QNetworkReply::NoError){
        md5toCheck = reply->readAll();
        result = true;
        qDebug() << md5toCheck;
    }else{
        QMessageBox::warning(NULL, tr("Check MD5"), tr("Check MD5 can not possible, check your connection"));
    }
    reply->deleteLater();

    if(result){
        if(md5File(*_filename) != md5toCheck){
            result = false;
        }
    }
    return result;
}


QString Plugin_Ocpm::md5File(QString pathFile){
    QFile file(pathFile);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();

    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(data);

    QByteArray hah = md5.result();
    qDebug() << hah.toHex();
    return hah.toHex();
}
