#include <QtGlobal>
#if defined(Q_OS_MAC)

#include "ProviderMac.h"

ProviderMac::ProviderMac():
    AbstractProvider()
{
    //On défini le temps de rafraichissement recommandé.
    m_recommendedRefreshTime = 100;
}

#endif //Q_OS_MAC
