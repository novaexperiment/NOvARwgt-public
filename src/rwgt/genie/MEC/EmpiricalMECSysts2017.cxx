/*
 * EmpiricalMECSysts2017.cxx:
 *  MEC systematics from 2017 analysis.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSysts2017.h"

#include "TF1.h"

#include "NOvARwgt/rwgt/IWeightGenerator.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2017.h"

#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	
	const MECq0ShapeSyst2017 * kMECq0ShapeSyst2017 = novarwgt::GetSystKnob<MECq0ShapeSyst2017>();
	
	const MECEnuShapeSyst2017 * kMECEnuShapeSyst2017 = novarwgt::GetSystKnob<MECEnuShapeSyst2017>(kUnspecifiedHelicity);
	const MECEnuShapeSyst2017 * kMECEnuShapeSyst2017_NuOnly = novarwgt::GetSystKnob<MECEnuShapeSyst2017>(kNeutrino);
	const MECEnuShapeSyst2017 * kMECEnuShapeSyst2017_NubarOnly = novarwgt::GetSystKnob<MECEnuShapeSyst2017>(kAntineutrino);

	const MECInitStateNPFracSyst2017 * kMECInitStateNPFracSyst2017
		= novarwgt::GetSystKnob<MECInitStateNPFracSyst2017>(kUnspecifiedHelicity);
	const MECInitStateNPFracSyst2017 * kMECInitStateNPFracSyst2017_NuOnly
		= novarwgt::GetSystKnob<MECInitStateNPFracSyst2017>(kNeutrino);
	const MECInitStateNPFracSyst2017 * kMECInitStateNPFracSyst2017_NubarOnly
		= novarwgt::GetSystKnob<MECInitStateNPFracSyst2017>(kAntineutrino);

	//---------------------------------------------------------------------------
	
	MECq0ShapeSyst2017::MECq0ShapeSyst2017(const IRegisterable::ClassID<MECq0ShapeSyst2017> & clID)
		: ISystKnob(clID,
	            std::string("MECq0Shape2017"),
	            {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
	            {0, 10}),
		  fQELikeWgtr(GetWeighter<novarwgt::EmpiricalMEC_to_GENIEQE_Wgt>()),
		  fRESLikeWgtr(GetWeighter<novarwgt::EmpiricalMEC_to_GENIERES_Wgt>())
	{}


	//---------------------------------------------------------------------------

	double MECq0ShapeSyst2017::CalcWeight(double sigma, const novarwgt::EventRecord &ev, const InputVals &otherParams) const
	{
		double wgt = 1.0;

		if (ev.reaction != novarwgt::kScMEC)
			return wgt;

		double nomWgt = this->CVWgt(ev, otherParams);

		// not sure why the casting was necessary, but it was
		const IWeightGenerator * wgtr = (sigma < 0) ? static_cast<const IWeightGenerator*>(fQELikeWgtr)
		                                            : static_cast<const IWeightGenerator*>(fRESLikeWgtr);

		if (nomWgt > 0)
			wgt = 1 + std::abs(sigma) * (wgtr->GetWeight(ev, otherParams) / nomWgt - 1);

		return wgt;
	}

	//---------------------------------------------------------------------------

	const TF1 MECEnuShapeSyst2017::sRwFn("f_MECEnuRwFn2018", "1/(2.5*x+1)");

	double MECEnuShapeSyst2017::CalcWeight(double sigma,
	                                       const novarwgt::EventRecord &ev,
	                                       const InputVals &) const
	{
		double wgt = 1;
		if (ev.reaction != novarwgt::kScMEC) return wgt;
		if (    (fHelicity == kNeutrino     && ev.nupdg < 0)
		     || (fHelicity == kAntineutrino && ev.nupdg > 0) )
			return wgt;

		if (ev.Enu < 0) return wgt;

		wgt = 1 + sigma * MECEnuShapeSyst2017::sRwFn.Eval(ev.Enu);

		return wgt;
	}

	//---------------------------------------------------------------------------

	double MECInitStateNPFracSyst2017::CalcWeight(double sigma,
	                                              const novarwgt::EventRecord &ev,
	                                              const InputVals &) const
	{
		double wgt = 1;
		if (ev.reaction != novarwgt::kScMEC) return wgt;
		if (    (fHelicity == kNeutrino     && ev.nupdg < 0)
		        || (fHelicity == kAntineutrino && ev.nupdg > 0) )
			return wgt;

		const double nominalNPfrac = 0.8;  // only true for GENIE 2.12.x
		double newNPfrac = nominalNPfrac + (0.1*sigma);
		if (newNPfrac > 1) newNPfrac = 1;
		else if (newNPfrac < 0) newNPfrac = 0;

		if (ev.struckNucl == 2000000201) // np
			wgt *= newNPfrac / nominalNPfrac;
		else
			wgt *= (1 - newNPfrac) / (1 - nominalNPfrac);

		return wgt;
	}

}