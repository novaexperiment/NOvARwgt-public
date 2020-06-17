/*
 * MAQEWgts.cxx:
 *   Reweight for CCQE axial mass
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <stdexcept>

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/rwgt/genie/QE/MAQEWgts.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const MAQEWeight_2018 * kMAQEWeight_2018 = novarwgt::GetWeighter<MAQEWeight_2018>();

	//----------------------------------------------------------------------

	double MAQEWeight_2018::CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &) const
	{
		// warning: this correction only good as long as GENIE's default is 0.99 and the default up error is +25%
		const double correctionInSigma = (1.04 - 0.99) / 0.25;

		if (!ev.isCC)
			return 1.;
		if (ev.reaction != novarwgt::kScQuasiElastic)
			return 1.;

		if ( !ev.genieWeights.IsSet( novarwgt::kKnob_MaCCQE) )
		{
			// externally-entering events may not have full GENIE weights
			if (ev.expectNoWeights)
				return 1.;

			throw std::runtime_error("kRescaleMAQE: Cannot do MA CCQE rescaling without GENIE reweights available.");
		}

		return 1. + correctionInSigma * (ev.genieWeights[novarwgt::kKnob_MaCCQE].plus1sigma - 1.);
	}
}