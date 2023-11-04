// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "iids.h"
#include "interfaces.h"
#include <print>

int main()
{
	auto hr = CoInitialize(NULL);

	if(FAILED(hr))
		std::println("CoInitialize failed {}", hr);

	IText* pText = nullptr;
	hr = CoCreateInstance(CLSID_CoText, NULL, CLSCTX_SERVER, IID_IText, (void**)&pText);
	if(FAILED(hr))
		std::println("CoCreateInstance failed {}", hr);

	// First, create a BSTR and send it into the object.
	//

	BSTR bstr = SysAllocString(L"Is anybody out there?!");
	pText->Speak(bstr);		// Server pops up a message box.
	SysFreeString(bstr);		// I made it, I free it.

	// Now, we are asking the object for a new BSTR, which is allocated for us.
	BSTR bstr2;
	pText->GiveMeABSTR(&bstr2);

	// As the BSTR is Unicode, we can convert to ANSI using wcstombs().
	char buff[80];
	size_t retval = 0;
	errno_t res = wcstombs_s(&retval, buff, sizeof(buff) - 1,
		bstr2, SysStringLen(bstr2));
	if(res != 0) {
		std::println("wcstombs_s failed {}", res);
	}
	std::println( "\'{}\' is what the server said!", buff );

	SysFreeString(bstr2); 	// It was made for me, I free it.
	pText->Release();		// Done with interface.

	CoUninitialize();

	return 0;
}
