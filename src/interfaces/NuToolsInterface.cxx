/*
 * NuToolsInterface.cxx:
 *  Interface to FNAL NuTools simb::Events.
 *
 *  Created on: Oct. 26, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#include <unordered_map>

#include "nusimdata/SimulationBase/GTruth.h"
#include "nusimdata/SimulationBase/MCGeneratorInfo.h"
#include "nusimdata/SimulationBase/MCTruth.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/GeneratorSupportConfig.h"

namespace novarwgt
{

	// anonymous namespace since it shouldn't be in the interface
	namespace
	{
		novarwgt::Generator DecodeGenerator(simb::Generator_t gen)
		{
			switch (gen)
			{
				case simb::Generator_t::kGENIE:
					return novarwgt::kGENIE;

				default:
					return novarwgt::kUnknownGenerator;
			}
		}

	}


	novarwgt::EventRecord ConvertNuToolsEvent(const simb::MCTruth * mctruth, const simb::GTruth * gtruth, const novarwgt::ReweightList & rwList)
	{
		novarwgt::EventRecord rec;

		// this is very rudimentary caching
		rec.generator = DecodeGenerator(mctruth->GeneratorInfo().generator);
		static std::string oldVersionStr = std::string{};
		static std::vector<int> oldVersionVec = std::vector<int>{};
		static std::unordered_map<std::string, std::string> oldGenConfig = std::unordered_map<std::string, std::string>{};
		static std::string oldGenConfigStr = std::string{};
		if (mctruth->GeneratorInfo().generatorVersion != oldVersionStr || mctruth->GeneratorInfo().generatorConfig != oldGenConfig)
		{
			oldVersionStr = mctruth->GeneratorInfo().generatorVersion;
			oldVersionVec = DecodeGeneratorVersion(mctruth->GeneratorInfo().generatorVersion);
			oldGenConfig = mctruth->GeneratorInfo().generatorConfig;
			oldGenConfigStr = "";
			for (const auto & configPair: oldGenConfig)
			{
				if (configPair.first == "tune")
					oldGenConfigStr = configPair.second;
				else
					std::cerr << "Don't know how to store generator config parameter named '" << configPair.first << "'";
			}
		}
		rec.generatorVersion = oldVersionVec;
		if (!oldGenConfigStr.empty())
			rec.generatorConfigStr = oldGenConfigStr;

		const auto & nu = mctruth->GetNeutrino();
		rec.nupdg      = nu.Nu().PdgCode();
		rec.isCC       = nu.CCNC() == simb::kCC;
		rec.reaction   = novarwgt::ReactionType(int(nu.Mode()));        // these were pulled from the GENIE enum I think so just cast
		rec.struckNucl = nu.HitNuc();

		rec.Enu  = nu.Nu().E();
		rec.q    = TLorentzVector(1, 0, 0, nu.Y() * nu.Nu().E());  // need to make up the direction since we didn't store it
		rec.q.SetRho(sqrt(nu.QSqr() + rec.q.E()*rec.q.E()));
		rec.y    = nu.Y();
		rec.W    = nu.W();

		rec.A = nu.Target() % 10000 / 10;   // convert from PDG +10LZZZAAAI format

		rec.npiplus = gtruth->fNumPiPlus;
		rec.npizero = gtruth->fNumPi0;
		rec.npiminus = gtruth->fNumPiMinus;

		rec.genieWeights = rwList;

		return rec;

	}

}
