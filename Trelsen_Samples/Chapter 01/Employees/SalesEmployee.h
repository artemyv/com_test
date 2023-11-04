// SalesEmployee.h: interface for the CSalesEmployee class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SALESEMPLOYEE_H__961B0796_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_)
#define AFX_SALESEMPLOYEE_H__961B0796_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_

#pragma once

#include "Employee.h"

class CSalesEmployee: public CEmployee
{
public:
	CSalesEmployee() = default;
	CSalesEmployee(int ID, std::string_view name, float startingPay, int sales);

	// Inherited virtual functions
	std::string_view DisplayClassType(EMP_TYPE& eType) override;
	float GetPay() override;

	// Sales specific functions.
	int GetNumberOfSales() { return m_numberOfSales; }
	void SetNumberOfSales(int numb) { m_numberOfSales = numb; }

private:
	int m_numberOfSales{0};

};

#endif // !defined(AFX_SALESEMPLOYEE_H__961B0796_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_)
