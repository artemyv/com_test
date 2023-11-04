// TextFactory.h: interface for the TextFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTFACTORY_H__466300A4_99B5_11D3_A7DE_0000E885A202__INCLUDED_)
#define AFX_TEXTFACTORY_H__466300A4_99B5_11D3_A7DE_0000E885A202__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <combaseapi.h>
class TextFactory: public IClassFactory
{

public:
	TextFactory();
	~TextFactory();

	// IUnknown methods.
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    ULONG STDMETHODCALLTYPE AddRef(void) override;

    ULONG STDMETHODCALLTYPE Release(void) override;

	// ICF methods.
    /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance(
        /* [annotation][unique][in] */
        _In_opt_  IUnknown* pUnkOuter,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void** ppvObject) override;

    /* [local] */ HRESULT STDMETHODCALLTYPE LockServer(
        /* [in] */ BOOL fLock) override;
private:
	ULONG m_refCount;
};


#endif // !defined(AFX_TEXTFACTORY_H__466300A4_99B5_11D3_A7DE_0000E885A202__INCLUDED_)
