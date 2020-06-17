/*
 * EmpiricalMECSysts2018.cxx:
 *  MEC systematics from 2018 analysis.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSysts2018.h"

#include "NOvARwgt/rwgt/IWeightGenerator.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNu2018 = novarwgt::GetSystKnob<MECQ0Q3RespSyst2018>(kNeutrino, false);
	const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNubar2018 = novarwgt::GetSystKnob<MECQ0Q3RespSyst2018>(kAntineutrino, false);
	const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNu2018_RPAfix = novarwgt::GetSystKnob<MECQ0Q3RespSyst2018>(kNeutrino, true);
	const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNubar2018_RPAfix = novarwgt::GetSystKnob<MECQ0Q3RespSyst2018>(kAntineutrino, true);

	//---------------------------------------------------------------------------

	double MECQ0Q3RespSyst2018::CalcWeight(double sigma,
	                                       const novarwgt::EventRecord &ev,
	                                       const InputVals &otherParams) const
	{
		double wgt = 1;
		if (ev.reaction != novarwgt::kScMEC) return wgt;
		if (    (fHelicity == kNeutrino     && ev.nupdg < 0)
		     || (fHelicity == kAntineutrino && ev.nupdg > 0) )
			return wgt;

		double nomWgt = this->CVWgt(ev, otherParams);  //knows nu vs anti-nu
		const novarwgt::IWeightGenerator * wgtr = nullptr;
		if (sigma > 0)
			wgtr = fQELikeWgtr;
		else
			wgtr = fRESLikeWgtr;

		if (nomWgt > 0)
			wgt = 1 + std::abs(sigma) * (wgtr->GetWeight(ev, otherParams)/nomWgt - 1);

		return wgt;
	}

}