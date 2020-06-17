/*
 * NonresPiSysts.cxx:
 *  Nonresonant pion product custom syst knobs.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/DIS/NonresPiSysts.h"

#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const DISnPionSyst * kDIS_CC_0pi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, false, true, true); // 0pi, nu, CC, struck proton
	const DISnPionSyst * kDIS_CC_1pi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, false, true, true); // 1pi, nu, CC, struck proton
	const DISnPionSyst * kDIS_CC_2pi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, false, true, true); // 2pi, nu, CC, struck proton
	const DISnPionSyst * kDIS_CC_3pluspi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, false, true, true); // 3pi, nu, CC, struck proton

	const DISnPionSyst * kDIS_NC_0pi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, false, false, true); // 0pi, nu, NC, struck proton
	const DISnPionSyst * kDIS_NC_1pi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, false, false, true); // 1pi, nu, NC, struck proton
	const DISnPionSyst * kDIS_NC_2pi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, false, false, true); // 2pi, nu, NC, struck proton
	const DISnPionSyst * kDIS_NC_3pluspi_nu_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, false, false, true); // 3pi, nu, NC, struck proton

	const DISnPionSyst * kDIS_CC_0pi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, false, true, false); // 0pi, nu, CC, struck neutron
	const DISnPionSyst * kDIS_CC_1pi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, false, true, false); // 1pi, nu, CC, struck neutron
	const DISnPionSyst * kDIS_CC_2pi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, false, true, false); // 2pi, nu, CC, struck neutron
	const DISnPionSyst * kDIS_CC_3pluspi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, false, true, false); // 3pi, nu, CC, struck neutron

	const DISnPionSyst * kDIS_NC_0pi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, false, false, false); // 0pi, nu, NC, struck neutron
	const DISnPionSyst * kDIS_NC_1pi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, false, false, false); // 1pi, nu, NC, struck neutron
	const DISnPionSyst * kDIS_NC_2pi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, false, false, false); // 2pi, nu, NC, struck neutron
	const DISnPionSyst * kDIS_NC_3pluspi_nu_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, false, false, false); // 3pi, nu, NC, struck neutron

	const DISnPionSyst * kDIS_CC_0pi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, true, true, true); // 0pi, nubar, CC, struck proton
	const DISnPionSyst * kDIS_CC_1pi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, true, true, true); // 1pi, nubar, CC, struck proton
	const DISnPionSyst * kDIS_CC_2pi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, true, true, true); // 2pi, nubar, CC, struck proton
	const DISnPionSyst * kDIS_CC_3pluspi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, true, true, true); // 3pi, nubar, CC, struck proton

	const DISnPionSyst * kDIS_NC_0pi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, true, false, true); // 0pi, nubar, NC, struck proton
	const DISnPionSyst * kDIS_NC_1pi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, true, false, true); // 1pi, nubar, NC, struck proton
	const DISnPionSyst * kDIS_NC_2pi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, true, false, true); // 2pi, nubar, NC, struck proton
	const DISnPionSyst * kDIS_NC_3pluspi_nubar_p_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, true, false, true); // 3pi, nubar, NC, struck proton

	const DISnPionSyst * kDIS_CC_0pi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, true, true, false); // 0pi, nubar, CC, struck neutron
	const DISnPionSyst * kDIS_CC_1pi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, true, true, false); // 1pi, nubar, CC, struck neutron
	const DISnPionSyst * kDIS_CC_2pi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, true, true, false); // 2pi, nubar, CC, struck neutron
	const DISnPionSyst * kDIS_CC_3pluspi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, true, true, false); // 3pi, nubar, CC, struck neutron

	const DISnPionSyst * kDIS_NC_0pi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(0, true, false, false); // 0pi, nubar, NC, struck neutron
	const DISnPionSyst * kDIS_NC_1pi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(1, true, false, false); // 1pi, nubar, NC, struck neutron
	const DISnPionSyst * kDIS_NC_2pi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(2, true, false, false); // 2pi, nubar, NC, struck neutron
	const DISnPionSyst * kDIS_NC_3pluspi_nubar_n_SystKnob = GetSystKnob<novarwgt::DISnPionSyst>(3, true, false, false); // 3pi, nubar, NC, struck neutron
	
	//----------------------------------------------------------------------

	double DISnPionSyst::CalcWeight(double sigma, const novarwgt::EventRecord &ev, const InputVals &) const
	{
		double wgt = 1.0;
		if(ev.reaction != novarwgt::kScDeepInelastic) return wgt;

		// note that the knob variants handle 0, 1, 2, 3+ pions
		// (where the last one handles 3 or more)
		unsigned int npion = ev.npiplus + ev.npizero + ev.npiminus;
		if( fNPion < 3 && npion != fNPion ) return wgt;
		if( fNPion == 3 && npion < fNPion ) return wgt;

		if(ev.nupdg > 0 && fIsAntiNu) return wgt;
		if(ev.nupdg < 0 && !fIsAntiNu) return wgt;
		if(ev.isCC != fIsCC) return wgt;

		if( !( ev.struckNucl == 2212 || ev.struckNucl == 2112 ) ) return wgt; // only proton or neutron
		if( ev.struckNucl == 2212 && !fStruckProton ) return wgt;
		if( ev.struckNucl == 2112 && fStruckProton ) return wgt;

		// 1 sigma is 50% variation
		wgt = 1 + fSystVarLowW * sigma;
		if (ev.W*ev.W > fWcut*fWcut)
			wgt = 1 + fSystVarHighW*sigma; // only 5% variation above W = 3 GeV/c^2

		return wgt;
	}
}