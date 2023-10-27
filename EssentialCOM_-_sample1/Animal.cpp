#include "Animals_h.h"
#include <atlbase.h>
#include <atlcom.h>

class ATL_NO_VTABLE PugCat : 
	public ATL::CComObjectRootEx<CComMultiThreadModelNoCS>,
	public IPug, public ICat {
	LONG m_cRef{ 0 };
protected:
	virtual ~PugCat() = default;
public:
	PugCat();
	//IUnknown methods
	BEGIN_COM_MAP(PugCat)
		COM_INTERFACE_ENTRY(IPug)
		COM_INTERFACE_ENTRY(IDog)
		COM_INTERFACE_ENTRY2(IAnimal,IDog)
		COM_INTERFACE_ENTRY(ICat)
	END_COM_MAP()

	// IAnimal methods
	STDMETHODIMP Eat() override;
	// IDog methods
	STDMETHODIMP Bark() override;
	// IPug methods
	STDMETHODIMP Snore() override;
	// ICat methods
	STDMETHODIMP IgnoreMaster(void) override;
};

class GorillaClass : 
	public ATL::CComObjectRootEx<CComMultiThreadModelNoCS>,
	public IApe {
public:
	// class objects are singletons, so don't delete
	BEGIN_COM_MAP(GorillaClass)
		COM_INTERFACE_ENTRY(IApe)
	END_COM_MAP()
	// IApe methods
	STDMETHODIMP EatBanana(void);
	STDMETHODIMP SwingFromTree(void);
	STDMETHODIMP get_Weight(long* plbs);
};