// CoCarApp.cpp : Defines the entry point for the console application.
//

#include "interfaces.h"
#include "cocar.h"
#include "iid.h"
#include <print>
// The global CarFactory function.
HRESULT CarFactory(void** pIFace);


int main()
{
	// The interfaces I want from a CoCar.
	IUnknown* pUnk = NULL;
	IEngine* pIEngine = NULL;
	ICreateCar* pICreateCar = NULL;
	IStats* pStats = NULL;

	HRESULT hr;

	std::println( "*********************************" );
	std::println( "The Amazing Almost COM CoCar"      );
	std::println( "*********************************" );

	// Go Get IUnknown from the CoCar.
	hr = CarFactory((void**) &pUnk);
	
	if(SUCCEEDED(hr))
	{
		// Create a CoCar
		pUnk->QueryInterface(IID_ICreateCar, (void**)&pICreateCar);
		pICreateCar->SetMaxSpeed(30);
		BSTR name = SysAllocString(L"Bertha?!");
		pICreateCar->SetPetName(name);
		SysFreeString(name);

		// Show the new car info.
		pUnk->QueryInterface(IID_IStats, (void**)&pStats);
		pStats->DisplayStats();

		// Rev it!
		int curSp = 0;
		int maxSp = 0;
		pUnk->QueryInterface(IID_IEngine, (void**)&pIEngine);
		pIEngine->GetMaxSpeed(&maxSp);

		do
		{	
			pIEngine->SpeedUp();
			pIEngine->GetCurSpeed(&curSp);
			std::println( "Speed is: {}" , curSp);

		}while(curSp <= maxSp);
		
		// Gotta convert to char array.
		char buff[MAX_NAME_LENGTH];
		BSTR bstr;
		pStats->GetPetName(&bstr);
		size_t len = 0;
		auto ret = wcstombs_s(&len, buff, sizeof(buff) -1, bstr, SysStringLen(bstr));
		if(ret == 0)
			std::println( "{} has blown up! Lead Foot!", buff);
		else {
			std::println(stderr, "wcstombs_s failed {}", ret);
		}
		SysFreeString(bstr);

	}

	// Release any acquired interfaces to destroy the CoCar
	// currently in memory.
	if(pUnk) pUnk->Release();
	if(pIEngine) pIEngine->Release();
	if(pStats) pStats->Release();
	if(pICreateCar) pICreateCar->Release();
    
	return 0;
}


HRESULT CarFactory(void** pIFace)
{
	HRESULT hr;
	LPUNKNOWN pUnk = NULL;

	// Dynamically allocate a CoCar.
	CoCar *pCar = new CoCar();

	// Fetch IUnknown pointer from CoCar.
	hr = pCar->QueryInterface(IID_IUnknown, (void**)&pUnk);

	if(SUCCEEDED(hr))
	{
		*pIFace = pUnk;
		return S_OK;
	}
	else	// We should never get here.  Every COM object supports IUnknown!
	{
		delete pCar;
		return E_FAIL;
	}

}