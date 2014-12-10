#include "AbstractQuery.h"
#include "QuerySink.h"
#include <iostream>
#include <QString>

using std::cout;
using std::hex;
using std::endl;

//Constructeur/destructeur
QuerySink::QuerySink()
{
    m_lRef = 0;
    bDone = false;
    InitializeCriticalSection(&threadLock);
}

QuerySink::~QuerySink()
{
    bDone = true;
    DeleteCriticalSection(&threadLock);
}

//Méthodes
ULONG QuerySink::AddRef()
{
    return InterlockedIncrement(&m_lRef);
}

ULONG QuerySink::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRef);
    if(lRef == 0)
        delete this;
    return lRef;
}

HRESULT QuerySink::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
    {
        *ppv = (IWbemObjectSink *) this;
        AddRef();
        return WBEM_S_NO_ERROR;
    }
    else return E_NOINTERFACE;
}

//On récupère la réponse de la requète et on la traite
HRESULT QuerySink::Indicate(long lObjectCount, IWbemClassObject **apObjArray)
{
    HRESULT hres = S_OK;
    QString converti = "";

    //for (int i = 0; i < lObjectCount; i++)
    //{
        VARIANT varName;
        hres = apObjArray[0]->Get(m_property, 0, &varName, 0, 0);

        if (FAILED(hres))
        {
            cout << "La requête a échouée."
                << " Code erreur = 0x"
                << hex << hres << endl;
            return WBEM_E_FAILED;       // Program has failed.
        }
        converti = QString((QChar*)varName.bstrVal, ::SysStringLen(varName.bstrVal));

        cout << "Reponse:" << converti.toStdString() << endl;
        m_parent->setReponse(converti.toStdString());
    //}

    return WBEM_S_NO_ERROR;
}

HRESULT QuerySink::SetStatus(LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR *pObjParam)
{
    if(lFlags == WBEM_STATUS_COMPLETE){
        cout << "Call complete.\n";

        EnterCriticalSection(&threadLock);
        bDone = true;
        LeaveCriticalSection(&threadLock);
    }else if(lFlags == WBEM_STATUS_PROGRESS){
        cout << "Call in progress.\n";
    }

    m_status = lFlags;
    return WBEM_S_NO_ERROR;
}

void QuerySink::setProperty(LPCWSTR newProperty){
    m_property = newProperty;
}

bool QuerySink::IsDone()
{
    bool done = true;

    EnterCriticalSection(&threadLock);
    done = bDone;
    LeaveCriticalSection(&threadLock);

    return done;
}

void QuerySink::setParent(AbstractQuery* parent){
    m_parent = parent;
}
