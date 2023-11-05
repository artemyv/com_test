
#include "interfaces.h"
#include "iid.h"
#include <print>

int main()
{
	CoInitialize(NULL);	// This parameter is reserved, and should be NULL

	HRESULT hr;
	IClassFactory* pCF = NULL; 	
	ICreateCar* pICreateCar = NULL;
	IStats* pStats = NULL;
	IEngine* pEngine = NULL;

	std::println( "*********************************" );
	std::println( "The Amazing CoCar Client"          );
	std::println( "*********************************" );
	// Get the class factory pointer of CoCar:
	hr = CoGetClassObject(CLSID_CoCar, CLSCTX_INPROC_SERVER,
				   NULL, IID_IClassFactory, (void**)&pCF);

	// Make a CoCar & get ICreateCar.
	hr = pCF->CreateInstance(NULL, IID_ICreateCar, (void**)&pICreateCar);
	pCF->Release();
	
	if(SUCCEEDED(hr))
	{
		pICreateCar->SetMaxSpeed(30);
		BSTR petName = SysAllocString(L"Shazzam!");
		pICreateCar->SetPetName(petName);
		SysFreeString(petName);

		// Now get IStats
		hr = pICreateCar->QueryInterface(IID_IStats, (void**)&pStats);
		pICreateCar->Release();
	}
	
	if(SUCCEEDED(hr))
	{
		// Show my car!
		pStats->DisplayStats();
		hr = pStats->QueryInterface(IID_IEngine, (void**)&pEngine);
	}

	if(SUCCEEDED(hr))
	{
		int curSp = 0;
		int maxSp = 0;		
		pEngine->GetMaxSpeed(&maxSp);

		do	// Zoom!
		{	
			pEngine->SpeedUp();
			pEngine->GetCurSpeed(&curSp);
			std::println( "Speed is: {}", curSp);

		}while(curSp <= maxSp);

		// Gotta convert to char array.
		char buff[80];
		BSTR bstr;
		pStats->GetPetName(&bstr);
		WideCharToMultiByte(CP_ACP, NULL, bstr, -1, buff, 
							80, NULL, NULL);
		SysFreeString(bstr);

		std::println( "{} has blown up! Lead Foot!", buff);

		if(pEngine) pEngine->Release();
		if(pStats) pStats->Release();
	}

	CoUninitialize();
	return 0;
}
