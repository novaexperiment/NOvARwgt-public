/*
 * COHSysts.cxx:
 *   Systs for Coherent scattering.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/COH/COHSysts.h"

#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const COHNormSyst2018 * kCOHNormCCSystKnob = novarwgt::GetSystKnob<COHNormSyst2018>(true);
	const COHNormSyst2018 * kCOHNormNCSystKnob = novarwgt::GetSystKnob<COHNormSyst2018>(false);

	//----------------------------------------------------------------------

	double COHNormSyst2018::CalcWeight(double sigma, const novarwgt::EventRecord &ev, const InputVals &) const
	{
		double wgt = 1.0;
		if (ev.reaction != novarwgt::kScCoherent)
			return wgt;
		if ((fIsCC && !ev.isCC) || (!fIsCC && ev.isCC))
			return wgt;

		return 1 + sigma * 0.2;
	}

}