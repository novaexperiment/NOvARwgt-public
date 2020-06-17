/*
 * nutools_test.cxx
 *
 *  Test the NuTools interface.
 *
 *  Created on: Jan. 17, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <iostream>
#include <memory>

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/interfaces/NuToolsInterface.h"
#include "NOvARwgt/test/tests_common.h"

#include "nusimdata/SimulationBase/MCNeutrino.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "nusimdata/SimulationBase/GTruth.h"
#include "nusimdata/SimulationBase/MCTruth.h"


namespace novarwgt
{
	namespace test
	{
		struct NuSimDataEvt
		{
			std::unique_ptr<simb::MCParticle> nuPart;
			std::unique_ptr<simb::MCParticle> lepPart;

			std::unique_ptr<simb::MCTruth> mctruth;
			std::unique_ptr<simb::GTruth> gtruth;
		};

		/// Utility function to convert an EventRecord back into NuSimData components.
		NuSimDataEvt ToNuSimData(const novarwgt::EventRecord &rec)
		{
			NuSimDataEvt evt;

			evt.mctruth = std::make_unique<simb::MCTruth>();

			std::unordered_map<std::string, std::string> config;
			if (!rec.generatorConfigStr.empty())
				config["tune"] = rec.generatorConfigStr;
			evt.mctruth->SetGeneratorInfo(rec.generator == novarwgt::kGENIE ? simb::Generator_t::kGENIE : simb::Generator_t::kUnknown,
			                              EncodeGeneratorVersion(rec.generatorVersion),
			                              config);

			TLorentzVector fourMom;
			fourMom.SetE(rec.Enu);
			simb::MCParticle p;
			evt.nuPart = std::make_unique<simb::MCParticle>(1, rec.nupdg, "");
			evt.nuPart->AddTrajectoryPoint({0,0,0,0}, fourMom);
			evt.mctruth->Add(*evt.nuPart);

			fourMom.SetE((1-rec.y)*rec.Enu);
			evt.lepPart = std::make_unique<simb::MCParticle>(2, rec.isCC ? (rec.nupdg > 0 ? rec.nupdg-1 : rec.nupdg+1) : rec.nupdg, "", 1);
			evt.lepPart->AddTrajectoryPoint({0,0,0,0}, fourMom);
			evt.mctruth->Add(*evt.lepPart);

			// not exactly right, but we aren't currently storing Z
			int targetPdg = (rec.A == 1) ? 1000010010 : 1000000000 + (rec.A/2)*1000000 + rec.A*1000;
			evt.mctruth->SetNeutrino(
					rec.isCC ? simb::kCC : simb::kNC,
					simb::int_type_(rec.reaction),
					simb::int_type_(rec.reaction+1000),  // not exactly right, but we don't use this
					targetPdg,
					rec.struckNucl,
					0,  // not storing struck quark
					rec.W,
					0,  // not storing x
					rec.y,
					rec.Q2());

			evt.gtruth = std::make_unique<simb::GTruth>();
			evt.gtruth->fNumPi0 = rec.npizero;
			evt.gtruth->fNumPiMinus = rec.npiminus;
			evt.gtruth->fNumPiPlus = rec.npiplus;

			return evt;
		}
	}
}


int main()
{
	std::cout << "NOvARwgt self-test: NuTools (simb::MCTruth and simb::GTruth)" << std::endl;
	std::cout << "============================================================" << std::endl;
	std::cout << std::endl;

	novarwgt::InputVals params
	{
		{"EmpiricalMEC", true},
	};

	auto testEventsIn = novarwgt::test::GetTestEvents();
	std::cout << "Considering " << testEventsIn.size() << " events:" << std::endl;
	decltype(testEventsIn) testEventsOut;
	for (const auto & evtInPair : testEventsIn)
	{
		const auto & evt = evtInPair.second;
		// this may look a little silly
		// (converting novarwgt::EventRecords to the NuSimData types and then back)
		// but (assuming the ToNuSimData() function above is written correctly)
		// it tests the ConvertNuToolsEvent() is working correctly...
		auto nuSimDataEvt = novarwgt::test::ToNuSimData(evt.Event());
		auto newEvt = novarwgt::ConvertNuToolsEvent(nuSimDataEvt.mctruth.get(), nuSimDataEvt.gtruth.get(), evt.Event().genieWeights);

		// since I can't copy the derived class pointer without knowing its type, I have to dynamic_cast it first.  ugh
		std::unique_ptr<std::exception> exc = nullptr;
		if (evt.ExpectedException())
			exc = std::make_unique<novarwgt::UnsupportedGeneratorException>(*dynamic_cast<const novarwgt::UnsupportedGeneratorException*>(evt.ExpectedException()));

		testEventsOut.emplace( evtInPair.first,
		                       novarwgt::test::TestEvent<novarwgt::EventRecord>(newEvt,
		                                                                        evt.ExpectedWeight(),
		                                                                        evt.Tune(),
		                                                                        evt.ExpectedSysts(),
		                                                                        std::move(exc)));
	}
	bool ok = novarwgt::test::Validate(testEventsOut, params, true);
	if (ok)
		std::cout << "All " << testEventsOut.size() << " events produced the expected behavior." << std::endl;

	return ok ? 0 : 1;
}
