// ApesTestClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <print>

#include <objbase.h>
#include <atlbase.h>
#include <atlcomcli.h>

#include "Apes_h.h"

static bool TestClass(CLSID sid, const char* name);


int main()
{
	std::println("Hello World!");

	// Initialize COM Library
	CoInitialize(NULL);

	TestClass(CLSID_Gorilla, "gorilla");
	TestClass(CLSID_Orangutan, "orangutan");
	TestClass(CLSID_Chimpanzee, "chimpanzee");

	// Uninitialize COM Library
	CoUninitialize();


	return 0;
}

bool TestClass(CLSID sid, const char* name)
{
	std::println("Call CoCreateInstance to create  component and get interface.");

	ATL::CComPtr<IApe> gorilla;
	unsigned long hr = gorilla.CoCreateInstance(sid);
	if (FAILED(hr))
	{
		std::println("Failed to create {} instance {:#x}.", name, hr);
		return false;
	}

	std::println("got IApe interface for {} object.", name);
	hr = gorilla->EatBanana();
	if (FAILED(hr))
	{
		std::println("{} failed to eat banana {:#x}.", name,  hr);
	}
	hr = gorilla->SwingFromTree();
	if (FAILED(hr))
	{
		std::println("{} failed to swing {:#x}.", name, hr);
	}
	long weight;
	hr = gorilla->get_Weight(&weight);
	if (FAILED(hr))
	{
		std::println("{} failed to report weight {:#x}.", name, hr);
	}
	else
	{
		std::println("{} weights {} kg.", name, weight);
	}
	{
		ATL::CComPtr<IWarrior> warrior;
		hr = gorilla.QueryInterface(&warrior);
		if (FAILED(hr))
		{
			std::println("{} failed to report IWarrior interface {:#x}.", name, hr);
		}
		else
		{
			hr = warrior->Hit(5);
			if (FAILED(hr))
			{
				std::println("{} warrior failed to hit {:#x}.", name, hr);
			}
		}
	}
	{
		ATL::CComPtr<IEgghead> egghead;
		hr = gorilla.QueryInterface(&egghead);
		if (FAILED(hr))
		{
			std::println("{} failed to report IEgghead interface {:#x}.", name, hr);
		}
		else
		{
			hr = egghead->Laugh(1);
			if (FAILED(hr))
			{
				std::println("{} egghead failed to laugh {:#x}.", name, hr);
			}
		}
	}
	{
		ATL::CComPtr<IKeeperOfTheFaith> keeper;
		hr = gorilla.QueryInterface(&keeper);
		if (FAILED(hr))
		{
			std::println("{} failed to report IKeeperOfTheFaith interface {:#x}.", name, hr);
		}
		else
		{
			ATL::CComBSTR blessing{ L"O! Great One give us your blessing" };
			hr = keeper->Keep(blessing);
			if (FAILED(hr))
			{
				std::println("{} keeper failed to keep Faith {:#x}.", name, hr);
			}
		}
	}
	return true;
}


