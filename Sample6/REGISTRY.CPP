//
// Registry.cpp
//

#include <objbase.h>
#include <assert.h>

#include "Registry.h"

////////////////////////////////////////////////////////
//
// Internal helper functions prototypes
//

// Set the given key and its value.
HRESULT setKeyAndValue(const char* pszPath,
                    const char* szSubkey,
                    const char* szValue) ;

// Convert a CLSID into a char string.
HRESULT CLSIDtochar(const CLSID& clsid, 
                 char* szCLSID,
                 int length) ;

// Delete szKeyChild and all of its descendents.
LONG recursiveDeleteKey(HKEY hKeyParent, const char* szKeyChild) ;

////////////////////////////////////////////////////////
//
// Constants
//

// Size of a CLSID as a string
const int CLSID_STRING_SIZE = 39 ;

/////////////////////////////////////////////////////////
//
// Public function implementation
//

//
// Register the component in the registry.
//
HRESULT RegisterServer(HMODULE hModule,            // DLL module handle
                       const CLSID& clsid,         // Class ID
                       const char* szFriendlyName, // Friendly Name
                       const char* szVerIndProgID, // Programmatic
                       const char* szProgID)       //   IDs
{
	// Get server location.
	char szModule[512] ;
	DWORD dwResult =
		::GetModuleFileNameA(hModule, 
		                    szModule,
		                    sizeof(szModule)/sizeof(char)) ;
	assert(dwResult != 0) ;

	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE] ;
	auto hr = CLSIDtochar(clsid, szCLSID, sizeof(szCLSID)) ;
	if (FAILED(hr))
		return hr;

	// Build the key CLSID\\{...}
	char szKey[64] ;
	strcpy(szKey, "CLSID\\") ;
	strcat(szKey, szCLSID) ;
  
	// Add the CLSID to the registry.
	hr = setKeyAndValue(szKey, NULL, szFriendlyName) ;
	if (FAILED(hr))
		return hr;

	// Add the server filename subkey under the CLSID key.
	hr = setKeyAndValue(szKey, "InprocServer32", szModule) ;
	if (FAILED(hr))
		return hr;

	// Add the ProgID subkey under the CLSID key.
	hr = setKeyAndValue(szKey, "ProgID", szProgID) ;
	if (FAILED(hr))
		return hr;

	// Add the version-independent ProgID subkey under CLSID key.
	hr = setKeyAndValue(szKey, "VersionIndependentProgID",
	               szVerIndProgID) ;
	if (FAILED(hr))
		return hr;

	// Add the version-independent ProgID subkey under HKEY_CLASSES_ROOT.
	hr = setKeyAndValue(szVerIndProgID, NULL, szFriendlyName) ;
	if (FAILED(hr))
		return hr;
	hr = setKeyAndValue(szVerIndProgID, "CLSID", szCLSID) ;
	if (FAILED(hr))
		return hr;
	hr = setKeyAndValue(szVerIndProgID, "CurVer", szProgID) ;
	if (FAILED(hr))
		return hr;

	// Add the versioned ProgID subkey under HKEY_CLASSES_ROOT.
	hr = setKeyAndValue(szProgID, NULL, szFriendlyName) ;
	if (FAILED(hr))
		return hr;
	hr = setKeyAndValue(szProgID, "CLSID", szCLSID) ;
	if (FAILED(hr))
		return hr;

	return S_OK ;
}

//
// Remove the component from the registry.
//
LONG UnregisterServer(const CLSID& clsid,         // Class ID
                      const char* szVerIndProgID, // Programmatic
                      const char* szProgID)       //   IDs
{
	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE] ;
	auto hr = CLSIDtochar(clsid, szCLSID, sizeof(szCLSID)) ;
	if (FAILED(hr))
		return hr;

	// Build the key CLSID\\{...}
	char szKey[64] ;
	strcpy(szKey, "CLSID\\") ;
	strcat(szKey, szCLSID) ;

	// Delete the CLSID Key - CLSID\{...}
	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey) ;
	if (!(lResult == ERROR_SUCCESS) ||
		(lResult == ERROR_FILE_NOT_FOUND))  // Subkey may not exist.

	{
		return HRESULT_FROM_WIN32(lResult);
	}
	// Delete the version-independent ProgID Key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID) ;
	if (!(lResult == ERROR_SUCCESS) ||
		(lResult == ERROR_FILE_NOT_FOUND))  // Subkey may not exist.

	{
		return HRESULT_FROM_WIN32(lResult);
	}

	// Delete the ProgID key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID) ;
	if (!(lResult == ERROR_SUCCESS) ||
		(lResult == ERROR_FILE_NOT_FOUND))  // Subkey may not exist.

	{
		return HRESULT_FROM_WIN32(lResult);
	}

	return S_OK ;
}

///////////////////////////////////////////////////////////
//
// Internal helper functions
//

// Convert a CLSID to a char string.
HRESULT CLSIDtochar(const CLSID& clsid,
                 char* szCLSID,
                 int length)
{
	assert(length >= CLSID_STRING_SIZE) ;
	// Get CLSID
	LPOLESTR wszCLSID = NULL ;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID) ;
	if (SUCCEEDED(hr))
	{
		// Covert from wide characters to non-wide.
		wcstombs(szCLSID, wszCLSID, length);
	}
	// Free memory.
	CoTaskMemFree(wszCLSID) ;
	return hr;
}

//
// Delete a key and all of its descendents.
//
LONG recursiveDeleteKey(HKEY hKeyParent,           // Parent of key to delete
                        const char* lpszKeyChild)  // Key to delete
{
	// Open the child.
	HKEY hKeyChild ;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0,
	                         KEY_ALL_ACCESS, &hKeyChild) ;
	if (lRes != ERROR_SUCCESS)
	{
		return lRes ;
	}

	// Enumerate all of the decendents of this child.
	FILETIME time ;
	char szBuffer[256] ;
	DWORD dwSize = 256 ;
	while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL,
	                    NULL, NULL, &time) == S_OK)
	{
		// Delete the decendents of this child.
		lRes = recursiveDeleteKey(hKeyChild, szBuffer) ;
		if (lRes != ERROR_SUCCESS)
		{
			// Cleanup before exiting.
			RegCloseKey(hKeyChild) ;
			return lRes;
		}
		dwSize = 256 ;
	}

	// Close the child.
	RegCloseKey(hKeyChild) ;

	// Delete this child.
	return RegDeleteKey(hKeyParent, lpszKeyChild) ;
}

//
// Create a key and set its value.
//   - This helper function was borrowed and modifed from
//     Kraig Brockschmidt's book Inside OLE.
//
HRESULT setKeyAndValue(const char* szKey,
                    const char* szSubkey,
                    const char* szValue)
{
	HKEY hKey;
	char szKeyBuf[1024] ;

	// Copy keyname into buffer.
	strcpy(szKeyBuf, szKey) ;

	// Add subkey name to buffer.
	if (szSubkey != NULL)
	{
		strcat(szKeyBuf, "\\") ;
		strcat(szKeyBuf, szSubkey ) ;
	}

	// Create and open key and subkey.
	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT ,
	                              szKeyBuf, 
	                              0, NULL, REG_OPTION_NON_VOLATILE,
	                              KEY_ALL_ACCESS, NULL, 
	                              &hKey, NULL) ;
	if (lResult != ERROR_SUCCESS)
	{
		return HRESULT_FROM_WIN32(lResult);
	}

	// Set the Value.
	if (szValue != NULL)
	{
		lResult = RegSetValueEx(hKey, NULL, 0, REG_SZ,
		              (BYTE *)szValue, 
		              strlen(szValue)+1) ;
	}

	RegCloseKey(hKey) ;
	return HRESULT_FROM_WIN32(lResult);
}
