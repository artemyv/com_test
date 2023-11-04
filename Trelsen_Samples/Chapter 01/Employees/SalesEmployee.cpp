// SalesEmployee.cpp: implementation of the CSalesEmployee class.
//
//////////////////////////////////////////////////////////////////////

#include "SalesEmployee.h"
#include <string.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSalesEmployee::CSalesEmployee(int ID, std::string_view name, float startingPay, int sales):
	CEmployee(ID, name, startingPay),
	m_numberOfSales{sales}
{}

std::string_view CSalesEmployee::DisplayClassType(EMP_TYPE& eType)
{
	eType = EMP_TYPE::SP;
	return "I am a Sales Person";
}

float CSalesEmployee::GetPay()
{
	// Our pay is a result of curr pay + number of sales.
	return (m_currPay / 52) + (m_numberOfSales * 5);
}