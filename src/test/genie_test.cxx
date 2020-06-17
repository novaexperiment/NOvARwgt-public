/*
 * genie_test.cxx
 *
 *  Test the GENIE interface.
 *
 *  Created on: Feb. 5, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <iostream>
#include <memory>

#include "TLorentzVector.h"

#include "NOvARwgt/interfaces/GenieInterface.h"
#include "NOvARwgt/rwgt/EventRecord.h"
#include "../rwgt/genie/GenieInternalTools.h"
#include "NOvARwgt/test/tests_common.h"

// from GENIE
#if GENIE_MAJOR_VERSION >= 3
#include "Framework/Algorithm/AlgConfigPool.h"
#include "Framework/Conventions/Constants.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/EventGen/XSecAlgorithmI.h"
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/Utils/RunOpt.h"
#include "RwCalculators/GReWeightNuXSecCCQE.h"
#else
#include "Algorithm/AlgFactory.h"
#include "Algorithm/AlgId.h"
#include "Base/XSecAlgorithmI.h"
#include "Conventions/Constants.h"
#include "EVGCore/EventRecord.h"
#include "GHEP/GHepParticle.h"
#include "Utils/KineUtils.h"
#endif

int main()
{
	std::cout << "\nNOvARwgt self-test: GENIE" << std::endl;
	std::cout << "=========================" << std::endl;

#if GENIE_MAJOR_VERSION >= 3
	// GENIE 3+ needs you to initialize the tune...
	auto runopt = genie::RunOpt::Instance();

	// let's use whatever is set up currently.
	// this will only work by default if user is at FNAL with UPS...
	auto evGenList = getenv("GENIE_XSEC_GENLIST");
	auto tuneName = getenv("GENIE_XSEC_TUNE");
	if (!evGenList || !tuneName)
	{
		std::cerr << "\n\nFor GENIE versions 3+, both EventGeneratorList and Tune name must be specified for this program to work.\n"
		          << "To set them using some sensible defaults, try:\n\n"
		          << "  export GENIE_XSEC_GENLIST=\"Default\"\n"
		          << "  export GENIE_XSEC_TUNE=\"G00_00_00_000\"\n"
		          << std::endl;
		exit(1);
	}

	runopt->SetEventGeneratorList(evGenList);
	runopt->SetTuneName(tuneName);
	runopt->BuildTune();
#endif

	novarwgt::InputVals params
	{
		{"EmpiricalMEC", true},
	};

	auto testEventsIn = novarwgt::test::GetTestEvents();
	decltype(testEventsIn) testEventsOut;
	for (const auto & evtInPair : testEventsIn)
	{
		const auto & evt = evtInPair.second;
		// this may look a little silly
		// (converting novarwgt::EventRecords to the GENIE types and then back)
		// but (assuming the ToGenieEvent() function in the internal tools is written correctly)
		// it tests the ConvertGenieEvent() is working correctly...
		auto genieEvt = novarwgt::internal::ToGenieEvent(evt.Event());

		auto newEvt = novarwgt::ConvertGenieEvent(&genieEvt, evt.Event().genieWeights);
		testEventsOut.emplace( evtInPair.first,
			novarwgt::test::TestEvent<novarwgt::EventRecord>(newEvt, evt.ExpectedWeight(), evt.Tune(), evt.ExpectedSysts()) );
	}
	bool ok = novarwgt::test::Validate(testEventsOut, params, true);
	if (ok)
		std::cout << "\nAll " << testEventsOut.size() << " events produced the expected behavior." << std::endl;

	return ok ? 0 : 1;
}