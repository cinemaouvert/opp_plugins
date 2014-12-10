#include "Environment.h" // Permet de connaitre l'os sur lequel on compile
#if defined(OS_WIN)

#ifndef QUERYSINK_H
#define QUERYSINK_H

#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#include <string>
#include "AbstractProvider.h"

using std::string;

#pragma comment(lib, "wbemuuid.lib")

class QuerySink : public IWbemObjectSink
{
    LONG m_lRef;
    bool bDone;
    CRITICAL_SECTION threadLock; // for thread safety

public:
    QuerySink();
    ~QuerySink();

    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
    virtual HRESULT STDMETHODCALLTYPE Indicate(LONG lObjectCount, IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray);
    virtual HRESULT STDMETHODCALLTYPE SetStatus(LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR *pObjParam);
    void setProperty(LPCWSTR newProperty);
    void setParent(AbstractProvider* parent);
    void setSingleResult(bool singleResult);

    bool IsDone();

private:
    LPCWSTR m_property;
    AbstractProvider* m_parent;
    LONG m_status;
    bool m_singleResult;
};

#endif // QUERYSINK_H
#endif // OS_WIN
