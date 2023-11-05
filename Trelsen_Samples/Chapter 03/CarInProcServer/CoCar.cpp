// CoCar.cpp: implementation of the CoCar class.
//
//////////////////////////////////////////////////////////////////////

#include "CoCar.h"
#include "iid.h"
#include <format> 
extern DWORD g_objCount;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CoCar::CoCar() : m_refCount(0), m_currSpeed(0), m_maxSpeed(0)
{
	m_petName = SysAllocString(L"Default Pet Name");
	++g_objCount;
}

CoCar::~CoCar()
{
	--g_objCount;
	if(m_petName)
		SysFreeString(m_petName);

}


// IUnknown
STDMETHODIMP CoCar::QueryInterface(REFIID riid, void** pIFace)
{
	// Which aspect of me do they want?
	if(riid == IID_IUnknown)
	{
		*pIFace = (IUnknown*)(IEngine*)this;
	}
	
	else if(riid == IID_IEngine)
	{
		*pIFace = (IEngine*)this;
	}
	
	else if(riid == IID_IStats)
	{
		*pIFace = (IStats*)this;
	}
	
	else if(riid == IID_ICreateCar)
	{
		*pIFace = (ICreateCar*)this;
	}
	else
	{
		*pIFace = NULL;
		return E_NOINTERFACE;
	}

	((IUnknown*)(*pIFace))->AddRef();
	return S_OK;
}

STDMETHODIMP_(DWORD) CoCar::AddRef()
{
	++m_refCount;
	return m_refCount;
}

STDMETHODIMP_(DWORD) CoCar::Release()
{
	if(--m_refCount == 0)
	{
		delete this;
		return 0;
	}
	else
		return m_refCount;

}

// IEngine
STDMETHODIMP CoCar::SpeedUp()
{
	m_currSpeed += 10;
	return S_OK;
}

STDMETHODIMP CoCar::GetMaxSpeed(int* maxSpeed)
{
	*maxSpeed = m_maxSpeed;
	return S_OK;
}

STDMETHODIMP CoCar::GetCurSpeed(int* curSpeed)
{
	*curSpeed = m_currSpeed;
	return S_OK;
}


// IStats
STDMETHODIMP CoCar::DisplayStats()
{
	// Need to transfer a BSTR to a char array.
	MessageBoxW(NULL, m_petName, L"Pet Name",MB_OK | MB_SETFOREGROUND);
	auto buff = std::format(L"{}", m_maxSpeed);
	MessageBoxW(NULL, buff.c_str(), L"Max Speed", MB_OK | MB_SETFOREGROUND);
	return S_OK;
}

STDMETHODIMP CoCar::GetPetName(BSTR* petName)
{
	*petName = SysAllocString(m_petName);
	return S_OK;
}


// ICreateCar
STDMETHODIMP CoCar::SetPetName(BSTR petName)
{
	SysReAllocString(&m_petName, petName);
	return S_OK;
}

STDMETHODIMP CoCar::SetMaxSpeed(int maxSp)
{
	if(maxSp < MAX_SPEED)
		m_maxSpeed = maxSp;
	return S_OK;
}