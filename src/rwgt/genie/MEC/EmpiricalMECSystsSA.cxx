/*
 * EmpiricalMECSystsSA.cxx:
 *  MEC systematics from Second Analysis.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/IWeightGenerator.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSystsSA.h"
#include "NOvARwgt/util/Registry.ixx"


namespace novarwgt
{
	const MECScaleSystSA * kMECScaleSystSA = novarwgt::GetSystKnob<MECScaleSystSA>();

	//----------------------------------------------------------------------

	double novarwgt::MECScaleSystSA::CalcWeight(double sigma, const novarwgt::EventRecord &ev,
	                                            const InputVals &) const
	{
		double wgt = 1.0;
		if(!ev.isCC || ev.reaction != novarwgt::kScMEC)
			return wgt;

		// i.e., 1 sigma is 50% variation, but don't allow negative weights
		double scale = 1 + 0.5*sigma;
		if (scale < 0)
			scale = 0;
		return scale;
	}
}