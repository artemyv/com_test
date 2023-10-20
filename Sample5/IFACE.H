#include <Unknwn.h>
//
// Iface.h - 
//    Declarations of interfaces, IIDs, and CLSID
//    shared by the client and the component.
//
DECLARE_INTERFACE_IID_(IX,IUnknown, "32bb8320-b41b-11cf-a6bb-0080c7b2d682")
{
	BEGIN_INTERFACE
		STDMETHOD(Fx)() PURE;
	END_INTERFACE
};

DECLARE_INTERFACE_IID_(IY, IUnknown, "32bb8321-b41b-11cf-a6bb-0080c7b2d682")
{
	BEGIN_INTERFACE
		STDMETHOD(Fy)() PURE;
	END_INTERFACE
};

DECLARE_INTERFACE_IID_(IZ, IUnknown, "32bb8322-b41b-11cf-a6bb-0080c7b2d682")
{
	BEGIN_INTERFACE
		STDMETHOD(Fz)() PURE;
	END_INTERFACE
};

//
// Declaration of GUIDs for interfaces and component.
//   These constants are defined in GUIDs.cpp.
//
extern "C" const IID IID_IX ;
extern "C" const IID IID_IY ;
extern "C" const IID IID_IZ ;

extern "C" const CLSID CLSID_Component1 ;
