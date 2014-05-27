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

#ifndef Plugin_Ocpm_H
#define Plugin_Ocpm_H


#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QEventLoop>
#include <QMessageBox>
#include <QProcess>

#include <QCryptographicHash>
#include <QDialog>
#include <QFile>
#include <QTreeWidgetItem>
#include "plugins.h"

namespace Ui {
class Plugin_Ocpm;
}
class Plugin_Ocpm : public OCPM
{
    Q_OBJECT
    Q_INTERFACES(OCPM)

public:
    explicit Plugin_Ocpm(QWidget *parent = 0);
    ~Plugin_Ocpm();

    /**
     * @brief getInfo
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void getInfo(QString path,bool withError = true);

    /**
     * @brief parceTracks parce tracks in mkvinfo
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void parceTracks(const QStringList &outList);

    /**
     * @brief parceAttach parce attachments in mkvinfo
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void parceAttach(const QStringList &outList);

    /**
      *
      * @brief extract from name
      * @param : 0 for attachments and anything else for tracks
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void extract(QString filepath, int mode, QString id,QString outputName = "");

    /**
      *
      * @brief Used to return the name of the plugin
      *
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    QString getName();

    /**
     * @brief parceXML parce info.xml
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void parceXML();

    /**
     * @brief check MD5 with a md5 on a website
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    bool checkHash(QString nameFile, bool sha1 = false);

    /**
     * @brief get MD5 for a file
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    QString md5File(QString pathFile);

    /**
     * @brief get SHA1 for a file
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    QString sha1File(QString pathFile);

    /**
     * @brief get all data for a file
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    QByteArray readAllFile(QString pathFile);
    /**
     * @brief FICHIERXML xml file name
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    const QString FICHIERXML;

    const QString NAMEMD5;

    const QString NAMESHA1;

    const QString REFERENCE_IMAGE;

    /**
      *
      * @brief used to extract the referenceImage to the screenshots
      *
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    bool extractReferenceImage();


private :
    /**
     * @brief ui The UI
     */
    Ui::Plugin_Ocpm *ui;

    /**
     * @brief listItems
     */
    QList<QTreeWidgetItem *> _listItems;

    /**
     *@brief link of file md5
     */
    QString _linkMD5;

    /**
     *@brief link of file sha1
     */
    QString _linkSHA1;

    /**
      * @brief id of the reference image
      */
    int _idReferenceImage;

    /**
      *
      * @brief Used to ad an action for the plugin
      *
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void secondaryAction();


signals:

public slots:
    /**
      *
      * @brief Used to launch the plugin
      *
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void launch();


private slots :


    /**
      *
      * @brief Used set the filename
      *
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void setFilename(QString * filename);



    /**
     * @brief Save modified settings and close the window
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void on_buttonBox_OKCancel_accepted();

    /**
     * @brief Close the window
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void on_buttonBox_OKCancel_rejected();


    void on_pushButton_attachment_clicked();
};

#endif // Plugin_Ocpm_H
