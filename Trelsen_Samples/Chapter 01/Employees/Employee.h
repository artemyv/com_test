// Employee.h: interface for the CEmployee class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPLOYEE_H__961B0795_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_)
#define AFX_EMPLOYEE_H__961B0795_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_

#pragma once

#include<string>
#include <string_view>
enum class EMP_TYPE { FT, SP };

// Upper limit in program.
constexpr static const int MAX_LENGTH = 20;

// The CEmployee class is an abstract base class.
// It contains a number of virtual & pure virtual functions
// which help define the sub-classes.
//
// As well, it defines a number of protected data items
// which it and all subclasses may use (but not the object user)
class CEmployee
{
public:
	CEmployee() = default;
	CEmployee(int ID, std::string_view name, float startingPay);
	virtual ~CEmployee() = default;

	// The public interface of this abstract base class.
	virtual std::string_view DisplayClassType(EMP_TYPE& eType) = 0;
	virtual float GetPay() = 0;

	void IncreasePay(float amt);

	std::string_view GetName() { return m_name; }
	int GetID() { return m_ID; }


protected:
	// This class and all kids can get to this data,
	// but not the object user.
	float	m_currPay{0.0f};
	int		m_ID{0};
	std::string	m_name{};
};

#endif // !defined(AFX_EMPLOYEE_H__961B0795_CE6C_11D2_AAED_00A0C9312D57__INCLUDED_)
