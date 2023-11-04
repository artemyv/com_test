// CCar.h: interface for the CCar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCAR_H__ECEE7FD6_C488_11D2_B8C3_0020781238D4__INCLUDED_)
#define AFX_CCAR_H__ECEE7FD6_C488_11D2_B8C3_0020781238D4__INCLUDED_

#pragma once

#include <string>

constexpr static const	int MAX_SPEED = 500;

class CCar
{
public:
	// Public interface to the class.
	void DisplayCarStats();
	void SpeedUp();
	auto GetCurrSpeed() { return m_currSpeed; }
	auto GetMaxSpeed() { return m_maxSpeed; }
	auto GetPetName() { return m_petName; }
	void CreateACar();

private:
	// Instance data.
	std::string	m_petName{};
	int		m_maxSpeed{0};
	int		m_currSpeed{0};

};

#endif // !defined(AFX_CCAR_H__ECEE7FD6_C488_11D2_B8C3_0020781238D4__INCLUDED_)
