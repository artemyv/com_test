
#include "employee.h"
#include "FTemployee.h"
#include "Salesemployee.h"
#include <vector>
#include <memory>
#include <print>

int main(void)
{
	// A cookie to see if we have a salesperson.

	// Make an array of Employee pointers.
	std::vector<std::unique_ptr<CEmployee>> theStaff;
	theStaff.push_back(std::make_unique<CFTEmployee>(30, "Fred", 30000.0f));
	theStaff.push_back(std::make_unique<CSalesEmployee>(40, "Mary", 40000.0f, 120));
	theStaff.push_back(std::make_unique<CFTEmployee>(15, "JoJo", 11000.0f));
	theStaff.push_back(std::make_unique<CSalesEmployee>(1, "Jimmy Jones", 100000.0f, 19));


	// Now go through each member in the array and 
	// pull out the stats.
	for(const auto& staff_member : theStaff) {
		// Which class is here?
		EMP_TYPE e;
		std::println("{}", staff_member->DisplayClassType(e));

		// Get name.
		std::println("Name is: {}", staff_member->GetName());

		// Get ID.
		std::println("ID is: {}", staff_member->GetID());

		// Get pay.
		std::println("Pay is: {} a paycheck before taxes.", staff_member->GetPay());

		// Let's see if we have a salesperson.
		if(e == EMP_TYPE::SP) {
			auto sales_person = static_cast<CSalesEmployee*>(staff_member.get());
			std::println("-->My Sales are {}", sales_person->GetNumberOfSales());
		}

		// Just to make it look nice.
		std::println("*************************");
	}
	return 0;
}

