/*
 * RPASysts.cxx:
 *  Systematics affecting the 'RPA' weights (GENIE global Fermi gas --> Valencia nuclear model).
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/QE/RPASysts.h"

#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const RPACCQESystSA * kRPACCQESystSA = novarwgt::GetSystKnob<RPACCQESystSA>();

	const RPACCQEshapeSyst2017 * kRPACCQEEnhSyst2017 = novarwgt::GetSystKnob<RPACCQEshapeSyst2017>("enh");
	const RPACCQEshapeSyst2017 * kRPACCQESuppSyst2017 = novarwgt::GetSystKnob<RPACCQEshapeSyst2017>("supp");

	const RPACCQEshapeSyst2018 * kRPACCQEEnhSyst2018 = novarwgt::GetSystKnob<RPACCQEshapeSyst2018>("enh");
	const RPACCQEshapeSyst2018 * kRPACCQESuppSyst2018 = novarwgt::GetSystKnob<RPACCQEshapeSyst2018>("supp");

	const RPACCQEshapeSyst2019 * kRPACCQEEnhSyst2019 = novarwgt::GetSystKnob<RPACCQEshapeSyst2019>("enh");
	const RPACCQEshapeSyst2019 * kRPACCQESuppSyst2019 = novarwgt::GetSystKnob<RPACCQEshapeSyst2019>("supp");

	const RPARESSyst2017 * kRPARESSyst2017 = novarwgt::GetSystKnob<RPARESSyst2017>();

	const RPARESSyst2018 * kRPARESSyst2018 = novarwgt::GetSystKnob<RPARESSyst2018>(false);
	const RPARESSyst2018 * kRPARESSyst2018_ExtrapKludge = novarwgt::GetSystKnob<RPARESSyst2018>(true);

	const RPARESSyst2019 * kRPARESSyst2019 = novarwgt::GetSystKnob<RPARESSyst2019>();
	
	//----------------------------------------------------------------------

	double RPACCQESystSA::CalcWeight(double sigma, const novarwgt::EventRecord &ev, const InputVals &otherParams) const
	{
		double wgt = 1.0;

		// this systematic is kinda funky:
		// it's one-sided (negative variations are not allowed
		// because they don't mean anything.)
		if (sigma < 0)
			return wgt;

		return 1 + sigma * (GetWeighter<novarwgt::RPAWeightCCQESA>()->GetWeight(ev, otherParams) - 1);

	}

	//----------------------------------------------------------------------

	double RPACCQEshapeSyst::CalcWeight(double sigma, const novarwgt::EventRecord &ev, const InputVals &otherParams) const
	{
		double wgt = 1;

		if (!ev.isCC || ev.reaction != novarwgt::kScQuasiElastic)
			return wgt;

		const novarwgt::IWeightGenerator * wgtr = (sigma < 0) ? fWgtrDown : fWgtrUp;
		double nomWgt = this->CVWgt(ev, otherParams);
		if (nomWgt > 0)
			wgt *= 1 + std::abs(sigma) * (wgtr->GetWeight(ev, otherParams)/nomWgt - 1);

		return wgt;
	}

	//----------------------------------------------------------------------

	double RPARESSyst::CalcWeight(double sigma, const novarwgt::EventRecord &ev, const InputVals &otherParams) const
	{
		double wgt = 1.0;

		// todo: should this really be applied to NC?
		if (ev.reaction != novarwgt::kScResonant)
			return wgt;

		// like RPACCQESystSA, this systematic is one-sided
		// (negative variations are not allowed
		// because they don't mean anything.)
		// +1 sigma turns off the RPA effect completely
		if ( sigma < 0 )
			return wgt;

		// add in a bit of increase for 2sigma to give CAFAna one more point to sample
		// (helps extrapolation not go crazy).
		// only needed when clamping at 1sigma (which we do when we're 'undoing' the RPA effect,
		// because >1 sigma 'undoes' more than the CV weight)
		if (sigma > 2 && !fSystIsEnableEffect && fDoExtrapKludge)
			sigma = 1.1;
		else if(sigma > 1)
			sigma = 1;

		double baseWgt = fWgtr->GetWeight(ev, otherParams);
		if (!fSystIsEnableEffect)
			baseWgt = 1./baseWgt;
		wgt *= 1 + sigma * (baseWgt - 1);
		return wgt;
	}
}