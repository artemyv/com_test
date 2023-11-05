// CoCarClassFactory.h: interface for the CoCarClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COCARCLASSFACTORY_H__FFA3A107_E326_11D2_B8D0_0020781238D4__INCLUDED_)
#define AFX_COCARCLASSFACTORY_H__FFA3A107_E326_11D2_B8D0_0020781238D4__INCLUDED_

#pragma once

#include <Objbase.h>

class CoCarClassFactory : public IClassFactory  
{
public:
	CoCarClassFactory();
	virtual ~CoCarClassFactory();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** pIFace) override;
	STDMETHODIMP_(DWORD)AddRef() override;
	STDMETHODIMP_(DWORD)Release() override;

	// ICF
	STDMETHODIMP LockServer(BOOL fLock) override;
	STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv) override;

private:
	DWORD	m_refCount;
};

#endif // !defined(AFX_COCARCLASSFACTORY_H__FFA3A107_E326_11D2_B8D0_0020781238D4__INCLUDED_)
