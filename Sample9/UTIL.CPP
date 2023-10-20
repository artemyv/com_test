//
//
// util.cpp - Common utilities for printing out messages
//
//
#include <objbase.h>
#include <stdio.h>    //sprintf
#include <stdlib.h>
#include <assert.h>
#include <format>

// #include <tchar.h>

#include "util.h"

#ifdef _OUTPROC_SERVER_ 
// We are building a local or remote server.
	// Listbox window handle
	extern HWND g_hWndListBox ;

	static inline void output(const char* sz)
	{
		::SendMessage(g_hWndListBox, LB_ADDSTRING, 0, (LPARAM)sz) ;
	}

#else
// We are building an in-proc server.
#include <iostream>
	static inline void output(const char* sz)
	{
		std::cout << sz << std::endl ;
	}
#endif //_OUTPROC_SERVER_

//
// Utilities
//
namespace Util
{
//
// Print out the COM/OLE error string for an HRESULT.
//
	std::string ErrorMessage(HRESULT hr)
	{
		if (SUCCEEDED(hr))
		{
			return {};
		}
		void* pMsgBuf;

		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&pMsgBuf,
			0,
			NULL
		);

		auto ret = std::format(" Err: {:#x}: \t{}", hr, (const char*)pMsgBuf);

		// Free the buffer.
		LocalFree(pMsgBuf);
		return ret;
	}


//
// Print out a message with a label.
//
void Trace(const char* szLabel, const char* szText, HRESULT hr)
{
	output(std::format("{}: \t{}{}", szLabel, szText, ErrorMessage(hr)).c_str());
}


} ; // End Namespace Util


//
// Overloaded ostream insertion operator
// Converts from wchar_t to char
//
std::ostream& operator<< (std::ostream& os, const wchar_t* wsz )
{
	// Length of incoming string
	int iLength = wcslen(wsz)+1 ;

	// Allocate buffer for converted string.
	char* psz = new char[iLength] ;

	// Convert from wchar_t to char.
	wcstombs(psz, wsz, iLength) ;

	// Send it out.
	os << psz ;

	// cleanup
	delete [] psz ;
	return os ;
}

///////////////////////////////////////////////////////////
//
// Internal helper functions
//

// Convert a CLSID to a char string.
void CLSIDtochar(const CLSID& clsid,
	char* szCLSID,
	int length)
{
	assert(length >= CLSID_STRING_SIZE);
	// Get CLSID
	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	assert(SUCCEEDED(hr));

	// Covert from wide characters to non-wide.
	wcstombs(szCLSID, wszCLSID, length);

	// Free memory.
	CoTaskMemFree(wszCLSID);
}
