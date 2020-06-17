/*
 * GenieInternalTools.cxx:
 *  NOvARwgt internal tools for working with GENIE.
 *
 *  Created on: Nov. 4, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "GenieInternalTools.h"

// if we weren't built against GENIE, nothing to see here
#ifdef GENIE_MAJOR_VERSION

#if GENIE_MAJOR_VERSION >= 3
#include "Framework/Algorithm/AlgConfigPool.h"
#include "Framework/Conventions/GVersion.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/EventGen/XSecAlgorithmI.h"
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/Utils/XSecSplineList.h"
#include "RwCalculators/GReWeightNuXSecCCQE.h"
#include "RwFramework/GSystUncertainty.h"
#else
#include "Algorithm/AlgFactory.h"
#include "Algorithm/AlgId.h"
#include "Base/XSecAlgorithmI.h"
#include "Conventions/GVersion.h"
#include "EVGCore/EventRecord.h"
#include "GHEP/GHepParticle.h"
#include "ReWeight/GSystUncertainty.h"
#include "ReWeight/GReWeight.h"
#include "ReWeight/GReWeightNuXSecCCQE.h"
#endif

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/GeneratorSupportConfig.h"


namespace novarwgt
{
	namespace internal
	{

#if GENIE_MAJOR_VERSION >= 3
		genie::rew::GReWeightModel* ReweightObjWrapper::GetWeighter(genie::rew::GSyst_t knobType) const
#else
		genie::rew::GReWeightI* ReweightObjWrapper::GetWeighter(genie::rew::GSyst_t knobType) const
#endif
		{
			auto knobPair = fCalcs.find(knobType);
			if (knobPair != fCalcs.end())
				return knobPair->second.get();

			// add more as needed
			if (knobType == genie::rew::kXSecTwkDial_MaCCQE)
			{
				// for now, force this to go back to the old 2.12.X error budget.
				// todo: what if the CV isn't what we're expecting?
				genie::rew::GSystUncertainty::Instance()->SetUncertainty(genie::rew::kXSecTwkDial_MaCCQE, 0.25, 0.15);
				fCalcs[knobType] = std::make_unique<genie::rew::GReWeightNuXSecCCQE>();
				auto knob = dynamic_cast<genie::rew::GReWeightNuXSecCCQE*>(fCalcs.at(knobType).get());
				knob->SetMode(genie::rew::GReWeightNuXSecCCQE::kModeMa);
				knob->SetSystematic(genie::rew::kXSecTwkDial_MaCCQE, +1);  // this is the only one we need right now...
				knob->Reconfigure();
			}
			else
				throw std::runtime_error("GenieInterface.cxx: Requesting GENIE reweight for unhandled knob:" + GetGenieKnobName(ConvertGenieKnob(knobType)));

			return fCalcs.at(knobType).get();
		}


		//----------------------------------------------------------------------------

		novarwgt::ReweightKnob ConvertGenieKnob(genie::rew::GSyst_t genieKnob)
		{
			return novarwgt::internal::KnobTranslationTable().at(genieKnob);
		}

		//----------------------------------------------------------------------------

		genie::rew::GSyst_t ConvertToGenieKnob(novarwgt::ReweightKnob novaKnob)
		{
			for (const auto & knobPair : novarwgt::internal::KnobTranslationTable())
			{
				if (knobPair.second == novaKnob)
					return knobPair.first;
			}
			throw std::runtime_error("A NOvA knob was given that has no GENIE knob equivalent: " + std::to_string(novaKnob));
		}

		//----------------------------------------------------------------------------

		double ReweightObjWrapper::GetWeight(const genie::EventRecord *ev, genie::rew::GSyst_t knob, double sigma) const
		{
			auto wgtr = GetWeighter(knob);
			wgtr->SetSystematic(knob, sigma);
			return wgtr->CalcWeight(*ev);
		}

		//----------------------------------------------------------------------------

		const std::vector<int> & GetGENIEVersion()
		{
			static const std::vector<int> genieVersion = DecodeGeneratorVersion(__GENIE_RELEASE__);
			return genieVersion;
		}

		//----------------------------------------------------------------------------
		std::string GetGENIETune()
		{
#if GENIE_MAJOR_VERSION >= 3
			std::string cmc = genie::XSecSplineList::Instance()->CurrentTune();

			if (cmc.empty())
				throw std::runtime_error("NOvARwgt::GenieInterface: For GENIE v3, you MUST set a tune in genie::XSecSplineList to use NOvARwgt with the GENIE interface!");

			return cmc;
#else
			return "";
#endif
		}

		//----------------------------------------------------------------------------

		genie::EventRecord ToGenieEvent(const novarwgt::EventRecord &rec)
		{
#if GENIE_MAJOR_VERSION >= 3

			genie::AlgConfigPool * conf_pool = genie::AlgConfigPool::Instance();
			genie::Registry * gpl = conf_pool->GlobalParameterList();
			RgAlg xsec_alg = gpl->GetAlg("XSecModel@genie::EventGenerator/QEL-CC");
			genie::AlgId id(xsec_alg);
#else
			genie::AlgId id("genie::LwlynSmithQELCCPXSec", "Default");
#endif
			genie::AlgFactory * algf = genie::AlgFactory::Instance();


			genie::EventRecord evt;

			// currently not saving Z <grimace>
			genie::InitialState itlState(rec.A > 1 ? rec.A/2 : 1, rec.A, rec.nupdg);
			itlState.SetProbeE(rec.Enu);
			itlState.TgtPtr()->SetHitNucPdg(rec.struckNucl);
			genie::ProcessInfo procInfo(genie::EScatteringType(rec.reaction), rec.isCC ? genie::kIntWeakCC : genie::kIntWeakNC);
			auto interaction = new genie::Interaction(itlState, procInfo);
			genie::Kinematics kinematics;
			kinematics.SetQ2(rec.Q2());
			kinematics.SetW(rec.W);
			kinematics.Sety(rec.y);
//			kinematics.Setx(genie::utils::kinematics::Q2YtoX(rec.Enu, genie::constants::kNucleonMass, rec.Q2(), rec.y), true);

			interaction->SetKine(kinematics);
			evt.AttachSummary(interaction);


			// first the neutrino
			evt.AddParticle(rec.nupdg, genie::kIStInitialState,
			                -1, -1, 1, 1,              // first mother, last mother, first daughter, last daughter
			                0, 0, rec.Enu, rec.Enu,  // four-momentum (all in z-dir)
			                0, 0, 0, 0);             // four-vector of vertex

			// final-state lepton
			auto lepE = (1-rec.y)*rec.Enu;
			auto lepPDG = rec.isCC ? (rec.nupdg > 0 ? rec.nupdg-1 : rec.nupdg+1) : rec.nupdg;
			evt.AddParticle( lepPDG, genie::kIStStableFinalState,
			                 0, 0, -1, -1,        // first mother, last mother, first daughter, last daughter
			                 -rec.q.Px(), -rec.q.Py(), rec.Enu-rec.q.Pz(), lepE,
			                 0, 0, 0, 0);

			// any pions needed
			for (const auto & pdg : {211, -211, 111})
			{
				int n;
				if (pdg == 211)
					n = rec.npiplus;
				else if (pdg == -211)
					n = rec.npiminus;
				else if (pdg == 111)
					n = rec.npizero;

				for (int i = 0; i < n; i++)
					evt.AddParticle(pdg, genie::kIStHadronInTheNucleus,
					                0, 0, 0, 0,  // first mother, last mother, first daughter, last daughter
					                0, 0, 0, 0,  // all four-mom zero.  probably doesnt matter here
					                0, 0, 0, 0); // four-vector of vertex
			}

			bool is_qe = interaction->ProcInfo().IsQuasiElastic();
			bool is_cc = interaction->ProcInfo().IsWeakCC();
			bool charm = interaction->ExclTag().IsCharmEvent();
			if(is_cc && is_qe && !charm)
			{
				interaction->SetBit(genie::kISkipKinematicChk, true);
				interaction->KinePtr()->UseSelectedKinematics();
				interaction->SetBit(genie::kIAssumeFreeNucleon);
				auto model = dynamic_cast<genie::XSecAlgorithmI *>(algf->AdoptAlgorithm(id));
				double difflXsec = model->XSec(interaction, genie::kPSQ2fE);
				evt.SetDiffXSec(difflXsec, genie::kPSQ2fE);
			}

			return evt;
		}

	} // namespace internal
} // namespace novarwgt

#endif // ifdef GENIE_MAJOR_VERSION

namespace novarwgt
{
	namespace internal
	{
		std::string GetGenieKnobName(ReweightKnob knob)
		{
#ifdef GENIE_MAJOR_VERSION
			return genie::rew::GSyst::AsString(ConvertToGenieKnob(knob));
#else
			return "GENIE knob #" + std::to_string(knob);
#endif
		}
	}
}
