
#include <QtGlobal>  // Permet d'avoir les définitions globals, notemment l'os sur lequel on compile
#if defined(Q_OS_MAC)

#ifndef PROVIDERMAC_H
#define PROVIDERMAC_H

#include "AbstractProvider.h"
#include "Info.h"

class ProviderMac: public Info, public AbstractProvider
{
public:
    ProviderMac();
};

#endif // PROVIDERMAC_H
#endif // Q_OS_MAC
