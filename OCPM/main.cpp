#include <QApplication>
#include "advancedsettingswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AdvancedSettingsWindow* adSw = new AdvancedSettingsWindow();
    adSw->show();
    adSw->getInfo(QString("/home/djo/Documents/OCPM/3ours- OCPM.mkv"));

    return a.exec();
}
