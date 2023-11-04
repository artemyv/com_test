
#include <print>
#include "ccar.h"

int main(void)
{
	std::println("***********************************");
	std::println("The Amazing Car Application via CPP");
	std::println("***********************************");

	// Create a car.
	CCar myCar;
	myCar.CreateACar();

	// Show stats.
	myCar.DisplayCarStats();

	// Speed up until engine block cracks
	while(myCar.GetCurrSpeed() <= myCar.GetMaxSpeed())
		myCar.SpeedUp();

	std::println("{} has blown up!  Lead foot!", myCar.GetPetName());

	return 0;
}




