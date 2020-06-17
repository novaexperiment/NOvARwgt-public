/*
 * RPAWeights.cxx:
 *  Implementation file for weweights related to RPA effects.
 *
 *  Created on: Oct. 17, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/QE/RPAWeights.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	// Second analysis
	const RPAWeightCCQESA * kRPAWeightCCQESA = novarwgt::GetWeighter<RPAWeightCCQESA>();

	// 2017 versions
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2017
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>( "CV", novarwgt::kScNull, true, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_enh_plus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("enh_plus", novarwgt::kScNull, true, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_enh_minus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("enh_minus", novarwgt::kScNull, true, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_supp_plus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("supp_plus", novarwgt::kScNull, true, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_supp_minus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("supp_minus", novarwgt::kScNull, true, true, true);

	// 2018 versions
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2018
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>( "CV", novarwgt::kScQuasiElastic, false, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_enh_plus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("enh_plus", novarwgt::kScQuasiElastic, false, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_enh_minus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("enh_minus", novarwgt::kScQuasiElastic, false, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_supp_plus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("supp_plus", novarwgt::kScQuasiElastic, false, true, true);
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_supp_minus
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>("supp_minus", novarwgt::kScQuasiElastic, false, true, true);

	const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_RPAFix
		= novarwgt::GetWeighter<RPAWeightCCQE_2017>( "CV", novarwgt::kScQuasiElastic, false, false, true);

	// "2019 "versions
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2019 = novarwgt::GetWeighter<RPAWeightCCQE_2017>("CV");
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_enh_plus = novarwgt::GetWeighter<RPAWeightCCQE_2017>("enh_plus");
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_enh_minus = novarwgt::GetWeighter<RPAWeightCCQE_2017>("enh_minus");
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_supp_plus = novarwgt::GetWeighter<RPAWeightCCQE_2017>("supp_plus");
	const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_supp_minus = novarwgt::GetWeighter<RPAWeightCCQE_2017>("supp_minus");

	const RPAWeightQ2_2017 * kRPAWeightRES2017 = novarwgt::GetWeighter<RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant, true);
	const RPAWeightQ2_2017 * kRPAWeightRES2018_RPAFix = novarwgt::GetWeighter<RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant);
	const RPAWeightQ2_2017 * kRPAWeightRES2019 = novarwgt::GetWeighter<RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant);

	//----------------------------------------------------------------------------

	bool IRPAWeightBase::OkReaction(const novarwgt::EventRecord& ev, const InputVals&) const
	{
		// original code from R. Gran excludes tau neutrinos, though I'm not sure why it matters.
		// Won't hurt anything anyway.
		if (abs(ev.nupdg) != 12 && abs(ev.nupdg) != 14)
			return false;

		// don't correct Hydrogen, unless explicitly trying to reproduce old buggy behavior
		if (!fApplyToHydrogen && ev.A == 1)
			return false;

		// if specified, apply only to reactions requested
		if (this->fCurrent != novarwgt::kRxnUnspecified &&
		    ((ev.isCC && this->fCurrent == novarwgt::kRxnNC) || (!ev.isCC && this->fCurrent == novarwgt::kRxnCC)))
			return false;
		if (this->fReaction != novarwgt::kScNull && ev.reaction != this->fReaction)
			return false;

		return true;
	}

	//----------------------------------------------------------------------------

	double IRPAq0q3Weight::CalcWeight(const novarwgt::EventRecord &ev, const InputVals & vals) const
	{
		if (!this->OkReaction(ev, vals))
			return 1.;

		double qmag = ev.q.Vect().Mag();
		double q0 = ev.q.E();
		bool isAntiNu = fForceNu ? false : ev.nupdg < 0;

		auto &hist = (isAntiNu) ? this->fHist_nubar : this->fHist_nu;
		auto &minBin = (isAntiNu) ? this->fq0MinBin_nubar : this->fq0MinBin_nu;
		if (minBin < 0)
			minBin = hist->FindFirstBinAbove(0, 2);
		double val = hist.GetValueInRange(qmag, q0,
		                                  {1, hist->GetNbinsX()},
		                                  {minBin, hist->GetNbinsY()},
		                                  {0.0, 2.0});

		// RPA shouldn't be eliminating events.  This is a failed weight.
		if (val == 0)
			val = 1;
		return val;
	}

	//----------------------------------------------------------------------------

	double RPAWeightQ2_2017::CalcWeight(const novarwgt::EventRecord &ev, const InputVals & vals) const
	{
		if (!this->OkReaction(ev, vals))
			return 1.;

		double q2 = -ev.q.Mag2();
		bool isAntiNu = ev.nupdg < 0;

		auto &hist = (isAntiNu) ? this->fHist_nubar : this->fHist_nu;
		return hist.GetValue(q2);
	}

}