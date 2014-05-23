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
    ui(new Ui::Plugin_Ocpm)
{
    ui->setupUi(this);
    ui->treeWidget_information->setHeaderLabel(tr("File : "));
}

Plugin_Ocpm::~Plugin_Ocpm()
{
    delete ui;
}

void Plugin_Ocpm::getInfo(QString path)
{
    ui->treeWidget_information->clear();

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
    }

    parceTracks(outList);
    parceAttach(outList);

    parceXML();
    //extract(path,0,QString("2"));
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

                if(tempStr.contains("info.xml")) {
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

    //qDebug() << QString::fromUtf8(process->readAllStandardOutput());
}

QString Plugin_Ocpm::getName()
{
    return "OCPM plugin";
}

void Plugin_Ocpm::launch()
{
    this->show();
    if(_filename != NULL){
        if((*_filename).compare("") != 0)
            this->getInfo(*_filename);
    }
}

void Plugin_Ocpm::setFilename(QString * filename)
{
    this->_filename = filename;
}



void Plugin_Ocpm::on_pushButton_attachment_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Set Output Folder"),QDir::homePath());

    foreach (QTreeWidgetItem *item, _listItems) {
        if (item->checkState(0) == Qt::Checked) {
            extract(*_filename,0,item->text(0).remove(tr("Attachment ")),dir);
        }
    }

}

void Plugin_Ocpm::parceXML() {


    QFile file("info.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("File info.xml was not found."),file.errorString());
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
            name.remove("dc:");
            name = name.toUpper();
            name += " : ";

            QString text = ui->label_xml->text() + "\n" + name + elt;
            ui->label_xml->setText(text);
        }

        file.close();
        file.remove();
    }
}
