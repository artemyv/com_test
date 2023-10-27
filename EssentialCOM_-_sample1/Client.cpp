#include "Animals_h.h"
#include <atlbase.h>

void TryToSnoreAndlgnore(/* [in] */ IUnknown* pUnk) {
	if (!pUnk)
		return;

	{
		ATL::CComPtr<IPug> pPug{};
		const auto hr = pUnk->QueryInterface(&pPug);
		if (SUCCEEDED(hr)) { // the object is Pug - compatible
			pPug->Snore();
		}
	}
	{
		ATL::CComPtr<ICat> pCat{};
		const auto hr = pUnk->QueryInterface(&pCat);
		if (SUCCEEDED(hr)) { // the object is Pug - compatible
			pCat->IgnoreMaster();
		}
	}
}