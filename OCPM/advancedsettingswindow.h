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

#ifndef ADVANCEDSETTINGSWINDOW_H
#define ADVANCEDSETTINGSWINDOW_H

#include <QDialog>
#include <QFile>

namespace Ui {
class AdvancedSettingsWindow;
}
class AdvancedSettingsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AdvancedSettingsWindow(QWidget *parent = 0);
    ~AdvancedSettingsWindow();

    /**
     * @brief getInfo
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void getInfo(QString path);

    /**
     * @brief parceTracks
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void parceTracks(const QStringList &outList);

    /**
     * @brief parceAttach
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void parceAttach(const QStringList &outList);

    /**
      *
      * @brief extract from name
      *
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void extract(QString filepath, int mode, int id,QString outputName = "");

private :
    /**
     * @brief ui The UI
     */
    Ui::AdvancedSettingsWindow *ui;

signals:

public slots:

private slots :

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


};

#endif // ADVANCEDSETTINGSWINDOW_H
