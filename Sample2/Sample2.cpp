
#include <iostream>
#include <objbase.h>

void trace(const char* msg) { std::cout << msg << std::endl; }
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
// Предварительные объявления GUID
extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;

//
// Компонент
//
class CA : public IX, public IY
{
	// Реализация IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef() { return 0; }
	virtual ULONG __stdcall Release() { return 0; }
	// Реализация интерфейса IX
	virtual void __stdcall Fx() { trace( "Fx"); }
	// Реализация интерфейса IY
	virtual void __stdcall Fy() { trace( "Fy"); }
};
HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{
	if (iid == IID_IUnknown)
	{
		trace("QueryInterface: return IUnknown");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		trace("QueryInterface: return IX");
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY)
	{
		trace("QueryInterface: return IY");
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		trace("QueryInterface: unsupported");
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef(); // См. гл. 4
	return S_OK;
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
	IUnknown* pIUnknown = CreateInstance();
	trace("get IX");
	IX* pIX = NULL;
	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
	if (SUCCEEDED(hr))
	{
		trace("got IX");
		pIX->Fx(); // Использовать интерфейс IX
	}
	trace("get IY");
	IY* pIY = NULL;
	hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);
	if (SUCCEEDED(hr))
	{
		trace("got IY");
		pIY->Fy(); // Использовать интерфейс IY
	}
	trace("get IZ");
	IZ* pIZ = NULL;
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
	trace("get IY through IX");
	IY* pIYfromIX = NULL;
	hr = pIX->QueryInterface(IID_IY, (void**)&pIYfromIX);
	if (SUCCEEDED(hr))
	{
		trace("got IY");
		pIYfromIX->Fy();
	}
	trace("get IUnknown through IY");
	IUnknown* pIUnknownFromIY = NULL;
	hr = pIY->QueryInterface(IID_IUnknown, (void**)&pIUnknownFromIY);
	if (SUCCEEDED(hr))
	{
		trace( "Do  IUnknown pointers match? ");
		if (pIUnknownFromIY == pIUnknown)
		{
			trace( "\tYes");
		}
		else
		{
			trace("\tNo");
		}
	}
	// Удалить компонент
	delete pIUnknown;
	return 0;
}