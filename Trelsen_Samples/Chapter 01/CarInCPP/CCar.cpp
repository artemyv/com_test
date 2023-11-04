// CCar.cpp: implementation of the CCar class.
//
//////////////////////////////////////////////////////////////////////

#include "CCar.h"
#include <print>
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



void CCar::DisplayCarStats()
{
	// Show what we got.
	std::println("***********************************");
	std::println("PetName is: {}", m_petName);
	std::println("Max Speed is: {}", m_maxSpeed);
	std::println("***********************************");
	std::println("");

}

void CCar::CreateACar()
{
	std::print("Please enter a pet-name for your car: ");
	std::getline(std::cin, m_petName);

	decltype(m_maxSpeed) spd;
	// Be sure speed isn't beyond reality...
	do {
		std::cin.clear();
		spd = MAX_SPEED + 1;
		std::print("Enter the max speed of this car: ");
		std::cin >> spd;
	} while(spd > MAX_SPEED);

	m_maxSpeed = spd;
}

void CCar::SpeedUp()
{
	if(m_currSpeed <= m_maxSpeed) {
		m_currSpeed = m_currSpeed + 10;
		std::println("Speed is: {}", m_currSpeed);
	}
}
