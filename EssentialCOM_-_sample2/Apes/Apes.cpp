#include "pch.h"

#include "Apes_h.h"
#include <atlbase.h>
#include <atlcom.h>
#include <print>
#include <string_view>
#include <locale>
class ATL_NO_VTABLE Gorilla :
	public ATL::CComObjectRootEx<CComMultiThreadModelNoCS>,
	public IApe,
	public IWarrior
{
	BEGIN_COM_MAP(Gorilla)
		COM_INTERFACE_ENTRY(IApe)
		COM_INTERFACE_ENTRY(IWarrior)
	END_COM_MAP()

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
			std::println("Gorilla wheights 100 kg");
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
	public IApe,
	public IEgghead
{
	BEGIN_COM_MAP(Gorilla)
		COM_INTERFACE_ENTRY(IApe)
		COM_INTERFACE_ENTRY(IWarrior)
	END_COM_MAP()

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
		std::println("Chimpanzee wheights 180 kg");
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
	public IApe,
	public IKeeperOfTheFaith
{
	BEGIN_COM_MAP(Gorilla)
		COM_INTERFACE_ENTRY(IApe)
		COM_INTERFACE_ENTRY(IWarrior)
	END_COM_MAP()

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
		std::println("Orangutan wheights 280 kg");
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