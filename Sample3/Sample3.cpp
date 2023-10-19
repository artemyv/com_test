#include <iostream>
#include <objbase.h>
#include <atlbase.h>
void trace(const char* msg) { std::cout << msg << std::endl; }
// Предварительные описания GUID
extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;
// Интерфейсы
interface IX : IUnknown
{
	virtual void __stdcall Fx() = 0;
};
interface IY : IUnknown
{
	virtual void __stdcall Fy() = 0;
};
interface IZ : IUnknown
{
	virtual void __stdcall Fz() = 0;
};
//
// Компонент
//
class CA : public IX, public IY
{
	// Реализация IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
	// Реализация интерфейса IX
	virtual void __stdcall Fx() { trace( "Fx"); }
	// Реализация интерфейса IY
	virtual void __stdcall Fy() { trace("Fy"); }
public:
	// Деструктор
	~CA() { trace("CA: destructing"); }
private:
	long m_cRef{ 0 };
};
HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("CA QI: return IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		trace("CA QI: return IX");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		trace("CA QI: return IY");
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		trace("CA QI: unsupported");
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}
ULONG __stdcall CA::AddRef()
{
	trace( "CA: AddRef" );
	return InterlockedIncrement(&m_cRef);
}
ULONG __stdcall CA::Release()
{
	trace( "CA: Release");
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
IUnknown* CreateInstance()
{
	IUnknown* pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}
//
// IID
//
// {32bb8320-b41b-11cf-a6bb-0080c7b2d682}
static const IID IID_IX =
{ 0x32bb8320, 0xb41b, 0x11cf,{0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82} };
// {32bb8321-b41b-11cf-a6bb-0080c7b2d682}
static const IID IID_IY =
{ 0x32bb8321, 0xb41b, 0x11cf,{0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82} };
// {32bb8322-b41b-11cf-a6bb-0080c7b2d682}
static const IID IID_IZ =
{ 0x32bb8322, 0xb41b, 0x11cf,{0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82} };
//
// Клиент
//
int main()
{
	HRESULT hr;
	trace("get IUnknown");
	{
		ATL::CComPtr<IUnknown> pIUnknown;
		pIUnknown.Attach(CreateInstance());
		trace("get IX");
		{
			ATL::CComPtr <IX> pIX;
			hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
			if (SUCCEEDED(hr))
			{
				trace("got IX");
				pIX->Fx(); // Использовать интерфейс IX
			}
		}
		trace("get IY");
		{
			ATL::CComPtr <IY> pIY;
			hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);
			if (SUCCEEDED(hr))
			{
				trace("got IY");
				pIY->Fy(); // Использовать интерфейс IY
			}
		}
		trace("get IZ");
		{
			ATL::CComPtr <IZ> pIZ;
			hr = pIUnknown->QueryInterface(IID_IZ, (void**)&pIZ);
			if (SUCCEEDED(hr))
			{
				trace("got IZ");
				pIZ->Fz();
			}
			else
			{
				trace("failed to get IZ");
			}
		}
		trace("release IUnknown");
	}
	return 0;
}