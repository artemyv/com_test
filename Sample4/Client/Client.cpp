//
// Client1.cpp
// Комиляция: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//
#include <iostream>
#include <string>
#include <objbase.h>
#include <atlbase.h>
#include <Iface.h>
#include "Create.h"
void trace(const char* msg) { std::cout << "Client 1:\t" << msg << std::endl; }
//
// Клиент1
//
int main()
{
	HRESULT hr;
	// Считать имя компонента

	std::string name;
	std::cout << "Enter the dll name[Component.dll]: ";
	std::getline(std::cin, name);
	if (!name.empty() && name.back() == '\n') name.pop_back();
	
	// Создать компонент вызовом функции CreateInstance из DLL
	trace("get IUnknown");
	{
		ATL::CComPtr<IUnknown> pIUnknown;
		pIUnknown.Attach(CallCreateInstance(name));
		if (!pIUnknown)
		{
			trace("CallCreateInstance failed");
			return 1;
		}
		trace("get IX");
		{
			ATL::CComPtr<IX> pIX;
			hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
			if (SUCCEEDED(hr))
			{
				trace("got IX");
				pIX->Fx(); // Использовать интерфейс IX
			}
			else
			{
				trace("failed to get IX");
			}
		}
		trace("release IUnknown");
	}
	return 0;
}