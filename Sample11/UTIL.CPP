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
#include <fstream>
#include <string_view>
#include <chrono>
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
	static std::string g_filename{};
	bool OpenLog(const char* filename)
	{
		g_filename = filename;
		return true;
	}
	static inline void write_log(std::string_view sz)
	{
		auto t = ::GetCurrentThreadId();

		const auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
		auto log_line = std::format("{:%d-%m-%Y %H:%M:%OS}.{:03} t:{:5} - {}\n", now, ms, t, sz);

		std::ofstream log(g_filename, std::ios_base::app);

		if (log.good())
		{

			log << log_line;
		}
	}

	//
	// Print out the COM/OLE error string for an HRESULT.
	//
	static std::string ErrorMessage(HRESULT hr)
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
		auto line = std::format("{}: \t{}{}", szLabel, szText, ErrorMessage(hr));
		output(line.c_str());
		write_log(line);
	}


}; // End Namespace Util


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
