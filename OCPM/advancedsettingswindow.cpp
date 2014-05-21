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

#include "advancedsettingswindow.h"
#include "ui_advancedsettingswindow.h"

#include <QProcess>
#include <QListWidgetItem>
#include <QDebug>
#include <QApplication>
#include <QDir>

AdvancedSettingsWindow::AdvancedSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedSettingsWindow)
{
    ui->setupUi(this);
    this->show();

}

AdvancedSettingsWindow::~AdvancedSettingsWindow()
{
    delete ui;
}

void AdvancedSettingsWindow::getInfo(QString path)
{
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
}

void AdvancedSettingsWindow::parceTracks(const QStringList &outList)
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

void AdvancedSettingsWindow::parceAttach(const QStringList &outList)
{

    int nb =1;
    for (int i = 0; i < outList.count(); ++i) {
    if (outList.at(i).contains("| + Joints")) {

        QStringList* list = new QStringList();
        list->append(tr("Attachment ") + QString::number(nb));
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

void AdvancedSettingsWindow::on_buttonBox_OKCancel_accepted()
{

}


void AdvancedSettingsWindow::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}

QFile AdvancedSettingsWindow::extract(QString filepath, int mode, int id){
    QString modeStr = ((mode = 0) ? QString("attachments") : QString("tracks"));
    QProcess* process = new QProcess();
    process->start("mkvextract " + modeStr + " "+filepath+" "+QString::number(id));
}
