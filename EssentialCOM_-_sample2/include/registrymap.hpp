/** \file comlib/registrymap.hpp
  * Defines a 'registry' map for adding variables to rgs files.
  * Copyright 2001-2003 Michael Geddes.  All rights reserved.
  */

#ifndef COMLIB_REGISTRYMAP_HPP
#define COMLIB_REGISTRYMAP_HPP

#include <atlbase.h>
#define WIN32LEAN_AND_MEAN
#include <windows.h>

#include <string>

HMODULE GetModuleHandleFromAddress(void* address) {
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	size_t result = ::VirtualQuery(address, &mbi, sizeof(mbi));
	return static_cast<HMODULE>(mbi.AllocationBase);
}

HMODULE GetCurrentModuleHandle() {
	return GetModuleHandleFromAddress(GetCurrentModuleHandle);
}


struct _ATL_REGMAP_ENTRYKeeper : public _ATL_REGMAP_ENTRY
{
	_ATL_REGMAP_ENTRYKeeper(){ szKey=NULL; szData=NULL;}
	_ATL_REGMAP_ENTRYKeeper(LPCOLESTR key, LPCOLESTR data) 
	{
		szKey=key;
		LPOLESTR newData;
		szData = LPCOLESTR(newData =  new wchar_t[wcslen(data)+1]);
		wcscpy(newData,data);
	}
	_ATL_REGMAP_ENTRYKeeper(LPCOLESTR key, UINT resid)
	{
		static wchar_t Data[256];
		USES_CONVERSION;

		szKey=key;
		if( 0 == LoadStringW(_pModule->m_hInstResource, resid, Data, 255))
		{
			*Data = '\0';
		}

		auto	length = wcslen(Data);

		szData = new wchar_t[length];
		wcscpy(const_cast<wchar_t *>(szData),Data);
	}

	_ATL_REGMAP_ENTRYKeeper(LPCOLESTR key, REFGUID guid) 
	{
		szKey=key;
		LPOLESTR newData;
		szData = LPCOLESTR(newData =  new wchar_t[40]);
		if(szData!=NULL)
		{
			if(0==StringFromGUID2(guid, newData,40))
			{
				*newData=NULL;
			}
		}
	}

	LPCOLESTR GetModulePath(HMODULE module_handle)
	{
		DWORD pathlen = MAX_PATH - 1;
		DWORD bufsize = MAX_PATH;
		auto newData = new wchar_t[MAX_PATH];


		pathlen = ::GetModuleFileNameW(module_handle,
			newData,
			bufsize);
		if(pathlen >= bufsize)
		{
			delete[] newData;
			bufsize = pathlen + 1;
			newData = new wchar_t[bufsize];

			// On WinXP, if path length >= bufsize, the output is truncated and NOT
			// null-terminated.  On Vista and later, it will null-terminate the
			// truncated string. We call ReleaseBuffer on all OSes to be safe.
			pathlen = ::GetModuleFileNameW(module_handle,
				newData,
				bufsize);
		} 

		return (LPCOLESTR)newData;
	}

	// REGMAP_MODULE(x)
	explicit _ATL_REGMAP_ENTRYKeeper(LPCOLESTR key) {
		szData = GetModulePath(GetCurrentModuleHandle());
		szKey = key;
	}

	~_ATL_REGMAP_ENTRYKeeper()
	{
		delete [] (LPOLESTR)szData;
	}
};

#define BEGIN_REGISTRY_MAP() \
	static struct _ATL_REGMAP_ENTRY *_GetRegistryMap()\
	{\
		static const _ATL_REGMAP_ENTRYKeeper map[] = {
#define REGMAP_ENTRY(x,y) _ATL_REGMAP_ENTRYKeeper(OLESTR(##x),OLESTR(##y)),

#define REGMAP_RESOURCE(x,resid) _ATL_REGMAP_ENTRYKeeper(OLESTR(##x),), 

#define REGMAP_UUID(x,clsid) _ATL_REGMAP_ENTRYKeeper(OLESTR(##x),clsid),

#define REGMAP_MODULE(x) _ATL_REGMAP_ENTRYKeeper(OLESTR(##x)),

#define END_REGISTRY_MAP() _ATL_REGMAP_ENTRYKeeper() }; return (_ATL_REGMAP_ENTRY *)map;}

#define DECLARE_REGISTRY_RESOURCEID_EX(x)\
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister)\
	{\
		return _AtlModule.UpdateRegistryFromResource((UINT)x, bRegister, _GetRegistryMap() );\
	}


#endif /* COMLIB_REGISTRYMAP_HPP */
