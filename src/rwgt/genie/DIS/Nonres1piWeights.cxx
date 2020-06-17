/*
 * Nonres1piWeights.cxx:
 *  Reweights for the nonresonant single pion channel in GENIE DIS.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/DIS/Nonres1piWeights.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const Nonres1PiWgt * kNonres1PiWgt_Approx = novarwgt::GetWeighter<Nonres1PiWgt>(true, false);
	const Nonres1PiWgt * kNonres1PiWgt_Typo = novarwgt::GetWeighter<Nonres1PiWgt>(false, true);
	const Nonres1PiWgt * kNonres1PiWgt = novarwgt::GetWeighter<Nonres1PiWgt>();

	//----------------------------------------------------------------------

	double Nonres1PiWgt::CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &) const
	{
		// todo: if we're going to exclude nubars, we should probably also exclude NC?...

		if (ev.reaction != novarwgt::kScDeepInelastic)
			return 1.;
		// a very few NOvA records are bonkers, hence the NaN checks
		if (ev.W > 1.7 || ev.W < 0 || std::isnan(ev.W))
			return 1.;

		// note that Rodrigues et al. only worked with neutrino scattering--
		// nothing is said about antineutrinos
		if (ev.nupdg < 0)
			return 1.;

		// older versions of this weight used an approximate version that scaled ALL nonres pi prod with W < 1.7
		// since the pion counts before FSI weren't being saved.
		if (fUseApproxCut)
			return 0.65;

		if (ev.npiplus + ev.npizero + ev.npiminus != 1)
			return 1.;

		if (fUseTypoWeight)
			return 0.41;

		return 0.43;
	}
}