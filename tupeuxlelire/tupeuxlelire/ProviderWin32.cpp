#include <QtGlobal>
#if defined(Q_OS_WIN)

#include "ProviderWin32.h"
#include <Windows.h>

//TEST
#include <iostream>

ProviderWin32::ProviderWin32():
    AbstractProvider()
{
    //On défini le temps de rafraichissement recommandé.
    m_recommendedRefreshTime = 100;
}

ProviderWin32::~ProviderWin32(){};

// Methode provide once
void ProviderWin32::provide_once()
{
    //DRIVERS
    ENUM_SERVICE_STATUS struct_ServiceStatus;
    ENUM_SERVICE_STATUS *lpServiceStatus;
    BOOL b_RetVal = FALSE;
    DWORD dw_BytesNeeded;
    DWORD dw_ServiceCount;
    DWORD dw_ResumeHandle = 0;
    DWORD dw_ServiceType;
    DWORD dw_ServiceState;
    //We are interested only in drivers
    dw_ServiceType = SERVICE_DRIVER;
    // interested to know about drivers in all states
    dw_ServiceState = SERVICE_STATE_ALL;
    //Call EnumServicesStatus using the handle returned by OpenSCManager

    /* //NE FONCTIONNE PAS A CAUSE DE h_SCM
    b_RetVal = ::EnumServicesStatus(h_SCM,
                                    dw_ServiceType,
                                    dw_ServiceState,
                                    &struct_ServiceStatus,
                                    sizeof(struct_ServiceStatus),
                                    &dw_BytesNeeded,
                                    &dw_ServiceCount,
                                    &dw_ResumeHandle);
    */
    DWORD dw_Error = GetLastError();
    // Verify if EnumServicesStatus needs more memory space
    if ((b_RetVal == FALSE) || dw_Error == ERROR_MORE_DATA)
    {
        DWORD dw_Bytes = dw_BytesNeeded + sizeof(ENUM_SERVICE_STATUS);
        lpServiceStatus = new ENUM_SERVICE_STATUS [dw_Bytes];

        /* //NE FONCTIONNE PAS A CAUSE DE h_SCM
        EnumServicesStatus(h_SCM,
                        dw_ServiceType,
                        dw_ServiceState,
                        lpServiceStatus,
                        dw_Bytes,
                        &dw_BytesNeeded,
                        &dw_ServiceCount,
                        &dw_ResumeHandle);
        */
    }
}

// Methode provide
void ProviderWin32::provide()
{
    //CPU
    //-------POURCENTAGE UTILISATION DU CPU A REVOIR-------------

    setCpuTime(0);
}

#endif //Q_OS_WIN
