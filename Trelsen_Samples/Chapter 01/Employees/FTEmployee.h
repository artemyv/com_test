// FTEmployee.h: interface for the CFTEmployee class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTEMPLOYEE_H__961B0797_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_)
#define AFX_FTEMPLOYEE_H__961B0797_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_

#pragma once

#include "Employee.h"

class CFTEmployee: public CEmployee
{
public:
	CFTEmployee() = default;
	CFTEmployee(int ID, std::string_view name, float startingPay);

	// Inherited pure virtual functions.  FTEmp must
	// specify an implementation of these methods,
	// or else it is also abstract!
	std::string_view  DisplayClassType(EMP_TYPE& eType) override;
	float GetPay() override;

};

#endif // !defined(AFX_FTEMPLOYEE_H__961B0797_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_)
