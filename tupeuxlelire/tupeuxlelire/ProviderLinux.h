
#include <QtGlobal>  // Permet d'avoir les définitions globals, notemment l'os sur lequel on compile
#if defined(Q_OS_UNIX)

#ifndef PROVIDERLINUX_H
#define PROVIDERLINUX_H

/**
  *
  * Les chemins de fichiers de linux dans le répertoire proc
  *
  */

#define PROC "/proc"
#define BUDDYINFO "/proc/buddyinfo"
#define CGROUPS "/proc/cgroups"
#define CMDLINE "/proc/cmdline"
#define CONSOLES "/proc/consoles"
#define CPUINFO "/proc/cpuinfo"
#define CRYPTO "/proc/crypto"
#define DEVICES "/proc/devices"
#define DISKSTATS "/proc/diskstats"
#define DMA "/proc/dma"
#define EXECDOMAINS "/proc/execdomains"
#define FB "/proc/fb"
#define FILESYSTEMS "/proc/filesystems"
#define INTERRUPTS "/proc/interrupts"
#define IOMEM "/proc/iomem"
#define IOPORTS "/proc/ioports"
#define KALLSYMS "/proc/kallsyms"
#define KCORE "/proc/kcore"
#define KEYUSERS "/proc/key-users"
#define KMSG "/proc/kmsg"
#define KPAGECOUNT "/proc/kpagecount"
#define KPAGEFLAGS "/proc/kpageflags"
#define LATENCY_STATS "/proc/latency_stats"
#define LOADAVG "/proc/loadavg"
#define LOCKS "/proc/locks"
#define MDSTAT "/proc/mdstat"
#define MEMINFO "/proc/meminfo"
#define MISC "/proc/misc"
#define MODULES "/proc/modules"
#define MOUNTS "/proc/mounts"
#define MTRR "/proc/mtrr"
#define PAGETYPEINFO "/proc/pagetypeinfo"
#define PARTITIONS "/proc/partitions"
#define SCHED_DEBUG "/proc/sched_debug"
#define SCHEDSTAT "/proc/schedstat"
#define SLABINFO "/proc/slabinfo"
#define SOFTIRQS "/proc/softirqs"
#define STAT "/proc/stat"
#define SWAPS "/proc/swaps"
#define SYSRQ_TRIGGER "/proc/sysrq_trigger"
#define TIMER_LIST "/proc/timer_list"
#define TIMER_STATS "/proc/timer_stats"
#define UPTIME "/proc/uptime"
#define VERSION "/proc/version"
#define VERSION_SIGNATURE "/proc/version_signature"
#define VMALLOCINFO "/proc/vmallocinfo"
#define VMSTAT "/proc/vmstat"
#define ZONEINFO "/proc/zoneinfo"
//-------------------------------

#include "Info.h"
#include "AbstractProvider.h"

#include <vector>
#include <stdio.h> //Permet l'utilisation de "fopen"
#include <string.h>
#include <QObject>
#include <QString>

#include <iostream>

using std::vector;
using std::string;

class ProviderLinux: public Info, public AbstractProvider
{
public:
    //Constructeur/destructeur
    ProviderLinux();
    ~ProviderLinux();

    //Requètes
    QString* query(char *path, int numberLine);
    QString queryAll(char *path);
    vector<QString> command(std::string cmd);

/*
//Dans le cas où le provider hériterais de QObject
signals:
    void data(vector<QString*> osV, vector<QString*> cpuV, vector<QString*> graphV, vector<QString*> soundV, vector<QString*> driverV);
    void numberProc(QString*);
*/

protected:
    void provide_once();
    void provide();
};



#endif // PROVIDERLINUX_H
#endif // Q_OS_UNIX
