// CoCar.h: interface for the CoCar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COCAR_H__86A1A88D_D36B_11D2_B8CF_0020781238D4__INCLUDED_)
#define AFX_COCAR_H__86A1A88D_D36B_11D2_B8CF_0020781238D4__INCLUDED_

#pragma once

constexpr static const int MAX_SPEED = 500;
constexpr static const int MAX_NAME_LENGTH = 20;

#include "interfaces.h"

class CoCar : 
	public IEngine, 
	public ICreateCar, 
	public IStats  
{
public:
	CoCar();
	virtual ~CoCar();
 
	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** pIFace) override;
	STDMETHODIMP_(DWORD)AddRef() override;
	STDMETHODIMP_(DWORD)Release() override;

	// IEngine
	STDMETHODIMP SpeedUp() override;
	STDMETHODIMP GetMaxSpeed(int* maxSpeed) override;
	STDMETHODIMP GetCurSpeed(int* curSpeed) override;
	
	// IStats
	STDMETHODIMP DisplayStats() override;
	STDMETHODIMP GetPetName(BSTR* petName) override;

	// ICreateCar
	STDMETHODIMP SetPetName(BSTR petName) override;
	STDMETHODIMP SetMaxSpeed(int maxSp) override;

private:
	DWORD	m_refCount;
	BSTR	m_petName;
	int		m_maxSpeed;
	int		m_currSpeed;
};

#endif // !defined(AFX_COCAR_H__86A1A88D_D36B_11D2_B8CF_0020781238D4__INCLUDED_)
