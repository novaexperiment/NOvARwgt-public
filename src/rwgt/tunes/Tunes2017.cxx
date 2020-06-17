/*
 * Tunes2017.cxx:
 *  Ingredients to full cross section "tunes" used in 2017 analyses.
 *
 *  Created on: Nov. 25, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/tunes/Tunes2017.h"

// systs
#include "NOvARwgt/rwgt/generic/NueNumuSysts.h"
#include "NOvARwgt/rwgt/genie/GenieSystKnob.h"
#include "NOvARwgt/rwgt/genie/DIS/NonresPiSysts.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSysts2017.h"
#include "NOvARwgt/rwgt/genie/QE/MAQESysts.h"
#include "NOvARwgt/rwgt/genie/QE/RPASysts.h"

// CV wgts
#include "NOvARwgt/rwgt/genie/QE/RPAWeights.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2017.h"
#include "NOvARwgt/rwgt/genie/DIS/Nonres1piWeights.h"

namespace novarwgt
{
	// ------------------------------------------------------------------------------------
	// first: systematics that will be included in the tune.
	// see towards the bottom of this file for the actual tune object.
	const std::unordered_set<const novarwgt::ISystKnob*> k2017GENIEKnobs
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

		// COH
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_MaCOHpi),
		novarwgt::GetGenieSystKnob(novarwgt::kKnob_R0COHpi),

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

	const std::unordered_set<const novarwgt::ISystKnob*> k2017CustomKnobs
	{
		//  (1) "Reduced" M_A uncertainty (as compared to GENIE M_A knob)
		GetSystKnob<novarwgt::MAQEGenieReducedSyst2017>(),

		//  (2) RPA CCQE  (note these MUST be used with the kRPAWeightCCQE2018 central value weight from CAFAnaVars/GenieWeights.h!)
		GetSystKnob<novarwgt::RPACCQEshapeSyst2017>("enh"),
		GetSystKnob<novarwgt::RPACCQEshapeSyst2017>("supp"),

		//  (3) RPA RES
		GetSystKnob<novarwgt::RPARESSyst2017>(),

		//  (4) MEC
		GetSystKnob<novarwgt::MECq0ShapeSyst2017>(),
		GetSystKnob<novarwgt::MECEnuShapeSyst2017>(kUnspecifiedHelicity),
		GetSystKnob<novarwgt::MECInitStateNPFracSyst2017>(kUnspecifiedHelicity),

		//  (5) soft DIS n-pion
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

		//  (6) nue/numu cross section ratio based on T2K's simple model
		GetSystKnob<novarwgt::SimpleRadiativeCorrNueXSecSyst>(false),  // nue
		GetSystKnob<novarwgt::SimpleRadiativeCorrNueXSecSyst>(true),   // nuebar
		GetSystKnob<novarwgt::SimpleSecondClassCurrentsSyst>(),
	};  // k2017GENIEKnobs

	// ------------------------------------------------------------------------------------

	/// CV MC tune used in 2017 analyses
	const novarwgt::Tune kCVTune2017(
	{
		// request old buggy behavior of RPA
		{ "RPA_QE",    novarwgt::GetWeighter<RPAWeightCCQE_2017>("CV", novarwgt::kScNull, true, true, false) },
		{ "Nonres1pi", novarwgt::GetWeighter<Nonres1PiWgt>(true, false) },  // request older approximate version of nonres pi selection
		{ "MEC",       novarwgt::GetWeighter<EmpiricalMECWgt2017>() },
		},
		k2017GENIEKnobs | k2017CustomKnobs
	);

}