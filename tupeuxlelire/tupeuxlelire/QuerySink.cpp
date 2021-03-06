#include "Environment.h" // Permet de connaitre l'os sur lequel on compile
#if defined(OS_WIN)

#include "QuerySink.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::hex;
using std::endl;

//Constructeur/destructeur
QuerySink::QuerySink()
{
    m_lRef = 0;
    bDone = false;
    InitializeCriticalSection(&threadLock);
    m_singleResult = true;
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

    if(m_singleResult){
        //for (int i = 0; i < lObjectCount; i++)
        //{
            VARIANT varName;

            hres = apObjArray[0]->Get(m_property, 0, &varName, 0, 0); //PROBLEME

            if (FAILED(hres))
            {
                cout << "La requête a échouée."
                    << " Code erreur = 0x"
                    << hex << hres << endl;
                return WBEM_E_FAILED;       // Program has failed.
            }

            string converti = _bstr_t(varName.bstrVal);

            cout << "Reponse:" << converti << endl;
            m_parent->callbackTest(converti);
        //}
    }else{
        for (int i = 0; i < lObjectCount; i++)
        {
            VARIANT varName;

            hres = apObjArray[i]->Get(m_property, 0, &varName, 0, 0); //PROBLEME

            if (FAILED(hres))
            {
                cout << "La requête a échouée."
                    << " Code erreur = 0x"
                    << hex << hres << endl;
                return WBEM_E_FAILED;       // Program has failed.
            }

            string converti = _bstr_t(varName.bstrVal);

            cout << "Reponse:" << converti << endl;
            m_parent->callbackTest(converti);
        }
    }
    delete this;
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

void QuerySink::setParent(AbstractProvider* parent){
    m_parent = parent;
}

void QuerySink::setSingleResult(bool singleResult){
    m_singleResult = singleResult;
}

#endif // OS_WIN
