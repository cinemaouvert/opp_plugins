#include <QtGlobal>  // Permet d'avoir les définitions globals, notemment l'os sur lequel on compile
#if defined(Q_OS_WIN)

#ifndef PROVIDERWIN32_H
#define PROVIDERWIN32_H

#include "AbstractProvider.h"
#include "Info.h"

class ProviderWin32 : public Info, public AbstractProvider
{
public:
    ProviderWin32();
    ~ProviderWin32();

protected:
    void provide_once();
    void provide();
};

#endif // PROVIDERWIN32_H
#endif // Q_OS_WIN
