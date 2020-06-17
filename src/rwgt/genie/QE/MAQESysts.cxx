/*
 * MAQESysts.cxx:
 *  QE custom syst knobs.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/QE/MAQESysts.h"

#include "NOvARwgt/rwgt/genie/GenieSystKnob.h"
#include "NOvARwgt/util/Registry.ixx"

#include "../GenieInternalTools.h"

namespace novarwgt
{
	const MAQEGenieReducedSyst2017 * kMAQEGenieReducedSyst2017 = novarwgt::GetSystKnob<MAQEGenieReducedSyst2017>();
	const MAQEGenieReducedSyst2018 * kMAQEGenieReducedSyst2018 = novarwgt::GetSystKnob<MAQEGenieReducedSyst2018>();

	//----------------------------------------------------------------------

	template<>
	MAQEGenieReducedSyst2018::MAQEGenieReducedSyst2018(const IRegisterable::ClassID<MAQEGenieReducedSyst2018> &clID)
	: ISystKnob(clID,
	            novarwgt::internal::GetGenieKnobName(novarwgt::kKnob_MaCCQE) + "_reduced_2018",
	            {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)},
	            {0, std::numeric_limits<double>::infinity()},
	            {GetWeighter<novarwgt::MAQEWeight_2018>()}),
	  fGENIEMAQEKnob(GetGenieSystKnob(novarwgt::kKnob_MaCCQE))
	{}

	//----------------------------------------------------------------------

	template<>
	MAQEGenieReducedSyst2017::MAQEGenieReducedSyst2017(const IRegisterable::ClassID<MAQEGenieReducedSyst2017> &clID)
	: ISystKnob(clID,
	            novarwgt::internal::GetGenieKnobName(novarwgt::kKnob_MaCCQE) + "_reduced_2017",
	            {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)}),
	  fGENIEMAQEKnob(GetGenieSystKnob(novarwgt::kKnob_MaCCQE))
	{}


	//----------------------------------------------------------------------

	double novarwgt::MAQEGenieReducedSyst2018::CalcWeight(double sigma, const novarwgt::EventRecord &ev,
	                                                      const InputVals &otherParams) const
	{
		double wgt = 1;

		// Account for M_A^QE CV shift from 0.99 to 1.04 in 2018 analysis.
		double cv_weight = this->CVWgt(ev, otherParams);
		if ( cv_weight > 0 )
		{

			wgt /= cv_weight; // Shift M_A from 1.04 back to 0.99
			double reduced_error = 0.05;
			double cv_ma = 1.04;
			double genie_ma = 0.99;
			double shifted_ma = cv_ma * ( 1 + sigma * reduced_error );
			double genie_frac_shift = ( shifted_ma - genie_ma ) / genie_ma;
			double genie_error = ( genie_frac_shift > 0 ) ? 0.25 : 0.15;
			double genie_sigma = genie_frac_shift / genie_error;

			wgt *= fGENIEMAQEKnob->GetWeight(genie_sigma, ev, otherParams);
		}

		return wgt;
	}

	//----------------------------------------------------------------------

	double MAQEGenieReducedSyst2017::CalcWeight(double sigma, const novarwgt::EventRecord &ev,
	                                            const InputVals &otherParams) const
	{
		// default GENIE M_A^QE uncertainties are -15%/+25%.
		// we want to make the actual effect +/- 5%,
		// to accord with bubble chamber measurements.
		// (See 2017 analysis Xsec uncertainty tech note.)
		double rescaleFactor = (sigma > 0) ? 0.05 / 0.25 : 0.05 / 0.15;

		return fGENIEMAQEKnob->GetWeight(rescaleFactor * sigma, ev, otherParams);
	}
}