//
// Cmpnt1.cpp
// Компиляция: cl /LD Cmpnt1.cpp GUIDs.cpp UUID.lib Cmpnt1.def
//
#include <iostream>
#include <objbase.h>
#include "Iface.h"
void trace(const char* msg) { std::cout << "Component 1:\t" << msg << std::endl; }
//
// Компонент
//
class CA : public IX
{
	// Реализация IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
	// Реализация интерфейса IX
	virtual void __stdcall Fx() { trace( "Fx"); }
public:
	// Деструктор
	~CA() { trace("Destructing"); }
private:
	long m_cRef{ 0 };
};
HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("return IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		trace("return IX");
		*ppv = static_cast<IX*>(this);
	}
	else
	{
		trace("not supported");
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}
ULONG __stdcall CA::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}
ULONG __stdcall CA::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}
//
// Функция создания
//
extern "C" IUnknown * CreateInstance()
{
	IUnknown* pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}