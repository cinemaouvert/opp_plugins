// QuerySink.h
#ifndef QUERYSINK_H
#define QUERYSINK_H

#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#include "AbstractQuery.h"

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
    void setParent(AbstractQuery* parent);

    bool IsDone();

private:
    LPCWSTR m_property;
    AbstractQuery* m_parent;
    LONG m_status;
};

#endif    // end of QuerySink.h
