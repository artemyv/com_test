#ifndef _INTERFACES
#define _INTERFACES

#include <Objbase.h>

DECLARE_INTERFACE_(IStats, IUnknown)
{
	STDMETHOD(DisplayStats)() PURE;
	STDMETHOD(GetPetName)(BSTR* petName) PURE;
};

DECLARE_INTERFACE_(IEngine, IUnknown)
{
	STDMETHOD(SpeedUp)() PURE;
	STDMETHOD(GetMaxSpeed)(int* maxSpeed) PURE;
	STDMETHOD(GetCurSpeed)(int* curSpeed) PURE;
};

DECLARE_INTERFACE_(ICreateCar, IUnknown)
{
	STDMETHOD(SetPetName)(BSTR petName) PURE;
	STDMETHOD(SetMaxSpeed)(int maxSp) PURE;
};

#endif // _INTERFACES
