/*
 * EmpiricalMECSysts2018.h:
 *  MEC systematics from 2018 analysis.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_EMPIRICALMECSYSTS2018_H
#define NOVARWGT_EMPIRICALMECSYSTS2018_H

#include <string>

#include "NOvARwgt/rwgt/ISystKnob.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSystsBase.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2018.h"


namespace novarwgt
{
	//---------------------------------------------------------------------------
	// 2018 analysis uncertainties on MEC treatment have three components
	// each are currently considered uncorrelated nu vs anti-nu and thus have separate systematics
	// - MEC (q0, |q|) response
	// - nn vs. np initial state dinucleon composition (same idea as 2017, but 15% instead of 10%)
	// - neutrino energy shape dependence (same as 2017)
	// See Doc 27755.
	//---------------------------------------------------------------------------

	/// 2018 MEC (q0, |q|) response uncertainty (see Doc 27755).
	/// Parameters:
	/// @param helicity  apply to neutrinos or antineutrinos
	/// @param RPAfix    use the tune derived with correction for RPA effect mistakenly applied to hydrogen in the QE prediction (true, default)
	///                  or the original 2018 version with the bug (false)
	class MECQ0Q3RespSyst2018 : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit MECQ0Q3RespSyst2018(const IRegisterable::ClassID<T> & clID, ENuHelicity helicity, bool RPAfix = true)
				: ISystKnob(clID,
				            std::string("MECQ0Q3Resp2018") + ((RPAfix) ? "RPAfix" : "") + (helicity == kNeutrino ? "Nu" : "AntiNu"),
				            {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				            {0, 10},
				            { RPAfix ? static_cast<const IWeightGenerator*>(novarwgt::GetWeighter<novarwgt::EmpiricalMECWgt2018RPAFix>())
				                     : static_cast<const IWeightGenerator*>(novarwgt::GetWeighter<novarwgt::EmpiricalMECWgt2018>())}),
				  fQELikeWgtr(RPAfix ? static_cast<const IWeightGenerator*>(novarwgt::GetWeighter<novarwgt::MEC2018RPAFix_QElike_Wgt>())
				                     : static_cast<const IWeightGenerator*>(novarwgt::GetWeighter<novarwgt::MEC2018_QElike_Wgt>())),
				  fRESLikeWgtr(RPAfix ? static_cast<const IWeightGenerator*>(novarwgt::GetWeighter<novarwgt::MEC2018RPAFix_RESlike_Wgt>())
				                      : static_cast<const IWeightGenerator*>(novarwgt::GetWeighter<novarwgt::MEC2018_RESlike_Wgt>())),
		          fHelicity(helicity)
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			const novarwgt::IWeightGenerator * fQELikeWgtr;
			const novarwgt::IWeightGenerator * fRESLikeWgtr;

			ENuHelicity fHelicity;
	};

	extern const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNu2018;
	extern const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNubar2018;
	extern const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNu2018_RPAfix;
	extern const MECQ0Q3RespSyst2018 * kMECQ0Q3RespSystNubar2018_RPAfix;

	//---------------------------------------------------------------------------
	// note that the Enu dependence and NP fraction uncertainties are unchanged from 2017
	// apart from being configured to treat nu and antinu separately
	// (see EmpiricalMECSysts2017.h).
}

#endif //NOVARWGT_EMPIRICALMECSYSTS2018_H
