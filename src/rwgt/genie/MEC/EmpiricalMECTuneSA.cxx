/*
 * EmpiricalMECTuneSA.h:
 *  Second Analysis MEC tune.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneSA.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const Tufts2p2hWgtSA * kTufts2p2hWgtSA = novarwgt::GetWeighter<Tufts2p2hWgtSA>();

	//----------------------------------------------------------------------

	double Tufts2p2hWgtSA::CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		// weights are only valid for CC MEC events.
		// also, as of right now (Nov. 29, 2016) we're recommending people
		// not use the (q0,q3) weights for antineutrinos as no tuning has been done.
		if (!ev.isCC || ev.nupdg < 0)
			return 1.;

		double itlStateWgt = fItlStateWgtr->CalcWeight(ev, params);
		double EdepWgt = fXsecEDepWgtr->CalcWeight(ev, params);
		double wgt = itlStateWgt * EdepWgt;

		// the magic value for neutrinos is to fix the normalization,
		// which otherwise gets inflated for some reason
		double q0q3Wgt = 0.75 * EmpiricalMECq0q3TuneWgt::CalcWeight(ev, params);

		wgt *= q0q3Wgt;

		return wgt;
	}
}