/*
 * GenieInterface.cxx:
 *  Interface to GENIE event records.
 *
 *  Created on: Oct. 19, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/interfaces/GenieInterface.h"

#include <memory>
#include <regex>
#include <unordered_map>


// GENIE includes
#if GENIE_MAJOR_VERSION >= 3
#include "Framework/EventGen/EventRecord.h"
#include "Framework/Conventions/Constants.h"
#include "Framework/GHEP/GHepParticle.h"
#else
#include "Conventions/Constants.h"
#include "Conventions/GVersion.h"
#include "EVGCore/EventRecord.h"
#include "GHEP/GHepParticle.h"
#include "ReWeight/GSystUncertainty.h"
#include "ReWeight/GReWeight.h"
#include "ReWeight/GReWeightNuXSecCCQE.h"
#endif

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/GeneratorSupportConfig.h"

// not in the public interface, so not in NOvARwgt/inc
#include "../rwgt/genie/GenieInternalTools.h"

namespace novarwgt
{
	// todo: need to decide how to specify which weights should be calculated on-the-fly if they're not pre-stored
	novarwgt::EventRecord ConvertGenieEvent(const genie::EventRecord *evt,
	                                        const ReweightList &storedWgts)
	{
		novarwgt::EventRecord rec;

		// assume that the version of GENIE used to read the event was also the one used to make it.
		// not foolproof, but best we can do here.
		rec.generator = kGENIE;
		rec.generatorVersion = novarwgt::internal::GetGENIEVersion();
		rec.generatorConfigStr = novarwgt::internal::GetGENIETune();

		// BEWARE: there are two ways to calculate many of the values below:
		//  (1) using the 'selected' kinematics,
		//      i.e., those as seen by the off-shell hit nucleon inside the nucleus;
		//  (2) the kinematics as you'd calculate using the final-state particles.
		// They can be significantly different.
		// The values stored inside NOvASoft
		// (and therefore the ones used to construct many of the weights)
		// correspond to scenario (2).
		//
		// The calculations here are cribbed from nutools/GENIE/GENIEHelper.cxx,
		// which is the 'authoritative' source of the values we have
		// (it's how they enter the event stream in NOvASoft).
		// Please see commentary there on the calculations
		bool hitnucl = evt->Summary()->InitState().Tgt().HitNucIsSet();
		TLorentzVector & k1 = *((evt->Probe())->P4());
		TLorentzVector & k2 = *((evt->FinalStatePrimaryLepton())->P4());
		TLorentzVector q  = k1-k2;      // q=k1-k2, 4-p transfer

		double Q2 = -1 * q.M2();        // momemtum transfer
		double v  = q.Energy();         // v (E transfer to the had system)
		double y  = v/k1.Energy();      // Inelasticity, y = q*P1/k1*P1
		double W2, W;
		W2 = W = -1;
		if ( hitnucl || evt->Summary()->ProcInfo().IsCoherent() )
		{
			const double M  = genie::constants::kNucleonMass;
//			x  = 0.5*Q2/(M*v);
			W2 = M*M + 2*M*v - Q2;
			W  = std::sqrt(W2);
		}

		rec.nupdg      = evt->Probe()->Pdg();
		rec.isCC       = evt->Summary()->ProcInfo().IsWeakCC();
		rec.reaction   = novarwgt::ReactionType(evt->Summary()->ProcInfo().ScatteringTypeId());   // these are the same enum so just cast
		rec.struckNucl = evt->Summary()->InitState().Tgt().HitNucPdg();

		rec.Enu  = evt->Probe()->E();
		rec.y    = y;
		rec.W    = W;
		rec.q    = q;

		rec.A = evt->Summary()->InitState().Tgt().A();

		rec.npiplus = evt->NEntries(211, genie::kIStHadronInTheNucleus);
		rec.npizero = evt->NEntries(111, genie::kIStHadronInTheNucleus);
		rec.npiminus = evt->NEntries(-211, genie::kIStHadronInTheNucleus);

		rec.genieWeights = storedWgts;

		rec.origGenieEvt = evt;

		return rec;
	}

}
