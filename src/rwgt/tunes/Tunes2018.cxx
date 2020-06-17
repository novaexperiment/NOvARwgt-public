/*
 * Tunes2018.cxx:
 *  Ingredients to full cross section "tunes" used in 2018 analyses.
 *
 *  Created on: Nov. 25, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/tunes/Tunes2018.h"

#include "NOvARwgt/rwgt/genie/QE/MAQEWgts.h"
#include "NOvARwgt/rwgt/genie/QE/RPAWeights.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2018.h"
#include "NOvARwgt/rwgt/genie/DIS/Nonres1piWeights.h"
#include "NOvARwgt/rwgt/genie/DIS/HighWDISWeight.h"

#include "NOvARwgt/rwgt/generic/NueNumuSysts.h"
#include "NOvARwgt/rwgt/genie/GenieKnobNames.h"
#include "NOvARwgt/rwgt/genie/GenieSystKnob.h"
#include "NOvARwgt/rwgt/genie/COH/COHSysts.h"
#include "NOvARwgt/rwgt/genie/DIS/NonresPiSysts.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSysts2017.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSysts2018.h"
#include "NOvARwgt/rwgt/genie/QE/MAQESysts.h"
#include "NOvARwgt/rwgt/genie/QE/RPASysts.h"


namespace novarwgt
{
	// ------------------------------------------------------------------------------------
	// first: systematics that will be included in the tune.
	// see towards the bottom of this file for the actual tune objects.

	const std::unordered_set<const novarwgt::ISystKnob*> k2018GENIEKnobs
	{
		// BEWARE: this is NOT a list of ALL
		// GENIE knobs; some of them are mutually exclusive
		// (e.g., MA_RES and the combination MARes_Shape, CCRES_Norm),
		// and some of them are superseded by uncertainties that we introduce
		//  with custom knobs below (e.g., MAQE by kMAQEGenieReducedSyst2017,
		//  the Rv{n,p}CC{1,2}pi by the soft DIS n-pion).
		// But it is "complete," in the sense that all of the uncertainties
		// GENIE can treat are represented either here or there.
		// Adjust with care!

		// QE (note that MaCCQE has a custom knob instead)
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MaNCEL),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_EtaNCEL),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_VecFFCCQEshape),

		// RES
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MaCCRES),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MvCCRES),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MaNCRES),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MvNCRES),

		// COH  -- replaced with custom knobs (these are broken)
		//novarwgt::GetGenieSystKnob(novarwgt::kKnob_MaCOHpi),
		//novarwgt::GetGenieSystKnob(novarwgt::kKnob_R0COHpi),

		// Non-resonance background tweaking parameters:

		// DIS: Bodek-Yang (note that R{v,vbar}{n,p}CC{1,2}pi knobs have been replaced by custom knobs)
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_AhtBY),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_BhtBY),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_CV1uBY),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_CV2uBY),

		// DIS: general
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_NormDISCC),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_RnubarnuCC),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_DISNuclMod),

		// NC: general
		/// NC total normalization (currently does nothing, but should retain for future).
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_NC),


		// Hadronization
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_AGKY_xF1pi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_AGKY_pT1pi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FormZone),


		// Intranuclear rescattering
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MFP_pi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MFP_N),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrCEx_pi),
		//novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrElas_pi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrInel_pi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrAbs_pi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrPiProd_pi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrCEx_N),
		//novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrElas_N),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrInel_N),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrAbs_N),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_FrPiProd_N),

		// Nuclear model
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_CCQEPauliSupViaKF),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_CCQEMomDistroFGtoSF),

		// Resonance decays
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_BR1gamma),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_BR1eta),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_Theta_Delta2Npi),

	};

	//----------------------------------------------------------------------

	const std::unordered_set<const novarwgt::ISystKnob*> k2018MECKnobs
	{
		GetSystKnob<novarwgt::MECQ0Q3RespSyst2018>(kNeutrino, false),
		GetSystKnob<novarwgt::MECQ0Q3RespSyst2018>(kAntineutrino, false),
		GetSystKnob<novarwgt::MECEnuShapeSyst2017>(kNeutrino),
		GetSystKnob<novarwgt::MECEnuShapeSyst2017>(kAntineutrino),
		GetSystKnob<novarwgt::MECInitStateNPFracSyst2017>(kNeutrino),
		GetSystKnob<novarwgt::MECInitStateNPFracSyst2017>(kAntineutrino),
	};

	//----------------------------------------------------------------------

	const std::unordered_set<const novarwgt::ISystKnob*> k2018MECKnobs_RPAFix
	{
		GetSystKnob<novarwgt::MECQ0Q3RespSyst2018>(kNeutrino, true),
		GetSystKnob<novarwgt::MECQ0Q3RespSyst2018>(kAntineutrino, true),
		GetSystKnob<novarwgt::MECEnuShapeSyst2017>(kNeutrino),
		GetSystKnob<novarwgt::MECEnuShapeSyst2017>(kAntineutrino),
		GetSystKnob<novarwgt::MECInitStateNPFracSyst2017>(kNeutrino),
		GetSystKnob<novarwgt::MECInitStateNPFracSyst2017>(kAntineutrino),
	};

	//----------------------------------------------------------------------

	/// Other non-MEC custom NOvA knobs
	const std::unordered_set<const novarwgt::ISystKnob*> k2018OtherCustomKnobs
	{
		//  (1) "Reduced" M_A uncertainty (as compared to GENIE M_A knob)
		GetSystKnob<novarwgt::MAQEGenieReducedSyst2018>(),

		//  (2) RPA CCQE  (note these MUST be used with the kRPAWeightCCQE2018 central value weight from CAFAnaVars/GenieWeights.h!)
		GetSystKnob<novarwgt::RPACCQEshapeSyst2018>("enh"),
		GetSystKnob<novarwgt::RPACCQEshapeSyst2018>("supp"),

		//  (3) RPA RES
		GetSystKnob<novarwgt::RPARESSyst2018>(true),   // do the extrapolation kludge (see the documentation)

		//  (4) soft DIS n-pion
		GetSystKnob<novarwgt::DISnPionSyst>(0, false, true, true), // 0pi, nu, CC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(1, false, true, true), // 1pi, nu, CC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(2, false, true, true), // 2pi, nu, CC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(3, false, true, true), // 3pi, nu, CC, struck proton

		GetSystKnob<novarwgt::DISnPionSyst>(0, false, false, true), // 0pi, nu, NC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(1, false, false, true), // 1pi, nu, NC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(2, false, false, true), // 2pi, nu, NC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(3, false, false, true), // 3pi, nu, NC, struck proton

		GetSystKnob<novarwgt::DISnPionSyst>(0, false, true, false), // 0pi, nu, CC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(1, false, true, false), // 1pi, nu, CC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(2, false, true, false), // 2pi, nu, CC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(3, false, true, false), // 3pi, nu, CC, struck neutron

		GetSystKnob<novarwgt::DISnPionSyst>(0, false, false, false), // 0pi, nu, NC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(1, false, false, false), // 1pi, nu, NC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(2, false, false, false), // 2pi, nu, NC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(3, false, false, false), // 3pi, nu, NC, struck neutron

		GetSystKnob<novarwgt::DISnPionSyst>(0, true, true, true), // 0pi, nubar, CC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(1, true, true, true), // 1pi, nubar, CC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(2, true, true, true), // 2pi, nubar, CC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(3, true, true, true), // 3pi, nubar, CC, struck proton

		GetSystKnob<novarwgt::DISnPionSyst>(0, true, false, true), // 0pi, nubar, NC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(1, true, false, true), // 1pi, nubar, NC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(2, true, false, true), // 2pi, nubar, NC, struck proton
		GetSystKnob<novarwgt::DISnPionSyst>(3, true, false, true), // 3pi, nubar, NC, struck proton

		GetSystKnob<novarwgt::DISnPionSyst>(0, true, true, false), // 0pi, nubar, CC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(1, true, true, false), // 1pi, nubar, CC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(2, true, true, false), // 2pi, nubar, CC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(3, true, true, false), // 3pi, nubar, CC, struck neutron

		GetSystKnob<novarwgt::DISnPionSyst>(0, true, false, false), // 0pi, nubar, NC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(1, true, false, false), // 1pi, nubar, NC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(2, true, false, false), // 2pi, nubar, NC, struck neutron
		GetSystKnob<novarwgt::DISnPionSyst>(3, true, false, false), // 3pi, nubar, NC, struck neutron

		//  (5) nue/numu cross section ratio based on T2K's simple model
		GetSystKnob<novarwgt::SimpleRadiativeCorrNueXSecSyst>(false),  // nue
		GetSystKnob<novarwgt::SimpleRadiativeCorrNueXSecSyst>(true),   // nuebar
		GetSystKnob<novarwgt::SimpleSecondClassCurrentsSyst>(),

		//  (6) replacements for broken Coherent scattering knobs
		GetSystKnob<novarwgt::COHNormSyst2018>(true),   // CC
		GetSystKnob<novarwgt::COHNormSyst2018>(false),  // NC
	};

	// ------------------------------------------------------------------------------------

	/// CV tune used for 2018 analysis
	const novarwgt::Tune kCVTune2018(
		{
			{ "MA_QE",     novarwgt::GetWeighter<MAQEWeight_2018>() },
			// request old buggy apply-to-hydrogen behavior for both RPA knobs,
			// as well as weights for 2.10 GENIE (since we didn't know about the difference at the time)
			{ "RPA_QE",    novarwgt::GetWeighter<RPAWeightCCQE_2017>("CV", novarwgt::kScQuasiElastic, true, false, true) },
			{ "RPA_RES",   novarwgt::GetWeighter<RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant, true) },
			{ "Nonres1pi", novarwgt::GetWeighter<Nonres1PiWgt>(false, true) },  // request version with typo in weight (0.41 instead of correct 0.43)
			{ "HighW",     novarwgt::GetWeighter<HighWDISWgt_2018>() },
			{ "MEC",       novarwgt::GetWeighter<EmpiricalMECWgt2018>() },
		},
		k2018GENIEKnobs | k2018MECKnobs | k2018OtherCustomKnobs
	);

	//----------------------------------------------------------------------

	/// 2018 cross section weight updated after Hydrogen RPA bug fix
	/// (but without update to fixed RPA correction for GENIE 2.12)
	const novarwgt::Tune kCVTune2018_RPAfix(
		{
			{ "MA_QE",     novarwgt::GetWeighter<MAQEWeight_2018>() },
			{ "RPA_QE",    novarwgt::GetWeighter<RPAWeightCCQE_2017>("CV", novarwgt::kScQuasiElastic, false, false, true) },
			{ "RPA_RES",   novarwgt::GetWeighter<RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant) },
			{ "Nonres1pi", novarwgt::GetWeighter<Nonres1PiWgt>(false, true) },
			{ "HighW",     novarwgt::GetWeighter<HighWDISWgt_2018>() },
			{ "MEC",       novarwgt::GetWeighter<EmpiricalMECWgt2018RPAFix>() },
		},
		k2018GENIEKnobs | k2018MECKnobs_RPAFix | k2018OtherCustomKnobs
	);

	//----------------------------------------------------------------------

	/// 2018 cross section weight updated after Hydrogen RPA bug fix but without DIS tweak
	/// (and without correction to actual GENIE 2.12 RPA-CCQE weights)
	const novarwgt::Tune kCVTune2018_RPAfix_noDIStweak(
		{
			{ "MA_QE",     novarwgt::GetWeighter<MAQEWeight_2018>() },
			{ "RPA_QE",    novarwgt::GetWeighter<RPAWeightCCQE_2017>("CV", novarwgt::kScQuasiElastic, false, false, true) },
			{ "RPA_RES",   novarwgt::GetWeighter<RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant) },
			{ "Nonres1pi", novarwgt::GetWeighter<Nonres1PiWgt>(false, true) },
			{ "MEC",       novarwgt::GetWeighter<EmpiricalMECWgt2018RPAFix>() },
		},
		k2018GENIEKnobs | k2018MECKnobs_RPAFix | k2018OtherCustomKnobs
	);

}