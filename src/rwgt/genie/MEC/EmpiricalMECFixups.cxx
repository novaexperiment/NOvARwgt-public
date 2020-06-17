/*
 * EmpiricalMECFixups.cxx:
 *  Some "fixes" for the GENIE Empirical MEC cross section in older versions.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECFixups.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const DytmanMECFixItlStateWgt * kDytmanMECFixItlStateWgt = novarwgt::GetWeighter<DytmanMECFixItlStateWgt>();
	const DytmanMECFixXsecEdepWgt * kDytmanMECFixXsecEdepWgt = novarwgt::GetWeighter<DytmanMECFixXsecEdepWgt>();

	//----------------------------------------------------------------------

	double DytmanMECFixItlStateWgt::CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals&) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		// antineutrino mode has the mix right, so don't reweight.
		if (ev.nupdg < 0)
			return 1.;
		if (!ev.isCC)
			return 1.;

		int pdg = ev.struckNucl;
		// neutron-neutron: weight from 80% to 20% of sample
		if (pdg == 2000000200)
			return 0.25;
			// neutron-proton: weight from 20% to 80% of sample
		else if (pdg == 2000000201)
			return 4;
			// what did you give me?
		else
			throw std::runtime_error( Form("novarwgt::DytmanMECFixItlStateWgt::CalcWeight(): unrecognized nucleon-nucleon pair PDG code: %d", pdg) );
	}

	//----------------------------------------------------------------------------

	double DytmanMECFixXsecEdepWgt::CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals&) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		const double Elo = 1.0;  // units of GeV
		const double turnoffRange = 4.0; // ditto

		// undo the cross section die-out (see referenced DocDB)
		double Enu = ev.Enu;
		if (Enu > Elo && Enu < Elo + turnoffRange)
			return  1. / (1. - (Enu - Elo) / turnoffRange);

		return 1.;
	}

}