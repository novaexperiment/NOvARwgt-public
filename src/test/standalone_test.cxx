/*
 * standalone_test.cxx
 *
 *  Test the standalone interface.
 *
 *  Created on: Oct. 22, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <iostream>

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/test/tests_common.h"

int main()
{
	std::cout << "NOvARwgt self-test: standalone (novarwgt::EventRecord)" << std::endl;
	std::cout << "======================================================" << std::endl;
	std::cout << std::endl;

	novarwgt::InputVals params
	{
		{"EmpiricalMEC", true},
	};

	auto testEvents = novarwgt::test::GetTestEvents();
	bool ok = novarwgt::test::Validate(testEvents, params, true);
	if (ok)
		std::cout << "All " << testEvents.size() << " events produced the expected behavior." << std::endl;

	return ok ? 0 : 1;
}