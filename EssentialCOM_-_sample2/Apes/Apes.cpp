#include "pch.h"

#include "Apes_h.h"
#include "resource.h"
#include "DllModule.h"
#include <registrymap.hpp>

#include <atlbase.h>
#include <atlcom.h>
#include <print>
#include <string_view>
#include <locale>



class ATL_NO_VTABLE Gorilla :
	public ATL::CComObjectRootEx<CComMultiThreadModelNoCS>,
	public CComCoClass<Gorilla, &CLSID_Gorilla>,
	public IApe,
	public IWarrior
{
	BEGIN_COM_MAP(Gorilla)
		COM_INTERFACE_ENTRY(IApe)
		COM_INTERFACE_ENTRY(IWarrior)
	END_COM_MAP()

	DECLARE_REGISTRY_RESOURCEID_EX(IDR_LOCAL_SERVER_RGS)

	BEGIN_REGISTRY_MAP()
		REGMAP_ENTRY("HKROOT", "HKCU")
		REGMAP_MODULE("MODULE")
		REGMAP_ENTRY("VERSION", "1.0")
		REGMAP_ENTRY("PROGID", "Ape.Gorilla")
		REGMAP_ENTRY("DESCRIPTION", "ApesLib Gorilla class")
		REGMAP_UUID("CLSID", GetObjectCLSID())
	END_REGISTRY_MAP()


	/* IApe */
	HRESULT STDMETHODCALLTYPE EatBanana(void) override
	{
		std::println("Gorilla eats banana");
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE SwingFromTree(void)override 
	{
		std::println("Gorilla swings from tree");
		return S_OK;
	}


	/* [propget] */ HRESULT STDMETHODCALLTYPE get_Weight(
		/* [retval][out] */ long* plbs) override
	{
			std::println("Gorilla weights 100 kg");
			if (plbs == nullptr)
				return E_POINTER;
			*plbs = 100;
			return S_OK;
	}

	/* IWarrior */
	HRESULT STDMETHODCALLTYPE Hit(long strength) override
	{
		std::println("Gorilla hits the enemy for {} hitpoints", strength);
		return S_OK;
	}

};

class ATL_NO_VTABLE Chimpanzee :
	public ATL::CComObjectRootEx<CComMultiThreadModelNoCS>,
	public CComCoClass<Chimpanzee, &CLSID_Chimpanzee>,
	public IApe,
	public IEgghead
{
	BEGIN_COM_MAP(Chimpanzee)
		COM_INTERFACE_ENTRY(IApe)
		COM_INTERFACE_ENTRY(IEgghead)
	END_COM_MAP()

	DECLARE_REGISTRY_RESOURCEID_EX(IDR_LOCAL_SERVER_RGS)

	BEGIN_REGISTRY_MAP()
		REGMAP_ENTRY("HKROOT", "HKCU")
		REGMAP_MODULE("MODULE")
		REGMAP_ENTRY("VERSION", "1.0")
		REGMAP_ENTRY("PROGID", "Ape.Chimpanzee")
		REGMAP_ENTRY("DESCRIPTION", "ApesLib Chimpanzee class")
		REGMAP_UUID("CLSID", GetObjectCLSID())
	END_REGISTRY_MAP()

	/* IApe */
	HRESULT STDMETHODCALLTYPE EatBanana(void) override
	{
		std::println("Chimpanzee eats banana");
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE SwingFromTree(void)override
	{
		std::println("Chimpanzee swings from tree");
		return S_OK;
	}


	/* [propget] */ HRESULT STDMETHODCALLTYPE get_Weight(
		/* [retval][out] */ long* plbs) override
	{
		std::println("Chimpanzee weights 180 kg");
		if (plbs == nullptr)
			return E_POINTER;
		*plbs = 180;
		return S_OK;
	}

	/* IEgghead */
	HRESULT STDMETHODCALLTYPE Laugh(long loud) override
	{
			std::println("Chimpanzee laughs for {} dB", loud);
			return S_OK;
	}

};

class ATL_NO_VTABLE Orangutan :
	public ATL::CComObjectRootEx<CComMultiThreadModelNoCS>,
	public ATL::CComCoClass<Orangutan, &CLSID_Orangutan>,
	public IApe,
	public IKeeperOfTheFaith
{
	BEGIN_COM_MAP(Orangutan)
		COM_INTERFACE_ENTRY(IApe)
		COM_INTERFACE_ENTRY(IKeeperOfTheFaith)
	END_COM_MAP()

	DECLARE_REGISTRY_RESOURCEID_EX(IDR_LOCAL_SERVER_RGS)

	BEGIN_REGISTRY_MAP()
		REGMAP_ENTRY("HKROOT", "HKCU")
		REGMAP_MODULE("MODULE")
		REGMAP_ENTRY("VERSION", "1.0")
		REGMAP_ENTRY("PROGID", "Ape.Orangutan")
		REGMAP_ENTRY("DESCRIPTION", "ApesLib Orangutan class")
		REGMAP_UUID("CLSID", GetObjectCLSID())
	END_REGISTRY_MAP()

	/* IApe */
	HRESULT STDMETHODCALLTYPE EatBanana(void) override
	{
		std::println("Orangutan eats banana");
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE SwingFromTree(void)override
	{
		std::println("Orangutan swings from tree");
		return S_OK;
	}


	/* [propget] */ HRESULT STDMETHODCALLTYPE get_Weight(
		/* [retval][out] */ long* plbs) override
	{
		std::println("Orangutan weights 280 kg");
		if (plbs == nullptr)
			return E_POINTER;
		*plbs = 280;
		return S_OK;
	}

	// utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
	template<class Facet>
	struct deletable_facet : Facet
	{
		template<class... Args>
		deletable_facet(Args&&... args) : Facet(std::forward<Args>(args)...) {}
		~deletable_facet() {}
	};

	std::string toNarrow(std::wstring_view wide)
	{
		auto required = WideCharToMultiByte(CP_UTF8, 0, wide.data(), (int)wide.size(), nullptr, 0, nullptr, nullptr);
		if (required > 0)
		{
			std::string result;
			result.resize(required, 0);
			WideCharToMultiByte(CP_UTF8, 0, wide.data(), (int)wide.size(), result.data(), (int)result.size(), nullptr, nullptr);
			return result;
		}
		return {};
	}
	/* IKeeperOfTheFaith */
	HRESULT STDMETHODCALLTYPE Keep(BSTR sound) override
	{
		std::wstring_view hello(sound, ::SysStringLen(sound));

		auto hello_utf8 = toNarrow(hello);
		std::println("Orangutan signs {}", hello_utf8);
		return S_OK;
	}

};

OBJECT_ENTRY_AUTO(__uuidof(Gorilla), Gorilla)
OBJECT_ENTRY_AUTO(__uuidof(Chimpanzee), Chimpanzee)
OBJECT_ENTRY_AUTO(__uuidof(Orangutan), Orangutan)
