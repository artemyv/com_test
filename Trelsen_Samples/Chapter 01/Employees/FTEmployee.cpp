// FTEmployee.cpp: implementation of the CFTEmployee class.
//
//////////////////////////////////////////////////////////////////////

#include "FTEmployee.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFTEmployee::CFTEmployee(int ID, std::string_view name, float startingPay)
	:CEmployee(ID, name, startingPay)
{

}

// We return a string specifying we are a full time worker.
std::string_view CFTEmployee::DisplayClassType(EMP_TYPE& eType)
{
	eType = EMP_TYPE::FT;
	return "I am a Full Timer worker";
}

float CFTEmployee::GetPay()
{
	// Calculate the pay.
	return m_currPay / 52;
}