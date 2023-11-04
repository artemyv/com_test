// Employee.cpp: implementation of the CEmployee class.
//
//////////////////////////////////////////////////////////////////////

#include "Employee.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CEmployee::CEmployee(int ID, std::string_view name, float startingPay):
	m_currPay{startingPay},
	m_ID{ID},
	m_name{name}
{}

void CEmployee::IncreasePay(float amt)
{
	// Add amount to current pay.
	m_currPay += amt;
}
