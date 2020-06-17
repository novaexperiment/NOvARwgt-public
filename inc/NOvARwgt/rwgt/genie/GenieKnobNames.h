/*
 * GenieKnobNames.h:
 *  Labels for systematic knobs.
 *    Sadly, duplicates the list in nugen/ NuReweight / ReweightLabels.h,
 *    but we don't want a build dependency...
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_GENIEKNOBNAMES_H
#define NOVARWGT_GENIEKNOBNAMES_H

namespace novarwgt
{
	/// \brief Enum identifying reweight knob indices.
	///
	/// Names are inspired by genie::EGSyst,
	/// but we don't want a build dependency on GENIE
	/// so we keep only the ones we want
	enum ReweightKnob : unsigned short
	{
		kKnob_Null,

		// NCEL tweaking parameters:
		kKnob_MaNCEL,              ///< Ma NCEL, affects dsigma(NCEL)/dQ2 both in shape and normalization
		kKnob_EtaNCEL,             ///< NCEL strange axial form factor eta, affects dsigma(NCEL)/dQ2 both in shape and normalization
		// CCQE tweaking parameters:
		kKnob_NormCCQE,          ///< CCQE normalization (energy independent)
		kKnob_NormCCQEenu,       ///< CCQE normalization (maintains dependence on neutrino energy)
		kKnob_MaCCQEshape,       ///< Ma CCQE, affects dsigma(CCQE)/dQ2 in shape only (normalized to constant integral)
		kKnob_MaCCQE,            ///< Ma CCQE, affects dsigma(CCQE)/dQ2 both in shape and normalization
		kKnob_VecFFCCQEshape,    ///< elastic nucleon form factors (BBA/default -> dipole) - shape only effect of dsigma(CCQE)/dQ2
		// Resonance neutrino-production tweaking parameters:
		kKnob_NormCCRES,         ///< CCRES normalization
		kKnob_MaCCRESshape,      ///< Ma CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
		kKnob_MvCCRESshape,      ///< Mv CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
		kKnob_MaCCRES,           ///< Ma CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
		kKnob_MvCCRES,           ///< Mv CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization

		kKnob_NormNCRES,         ///< NCRES normalization
		kKnob_MaNCRESshape,      ///< Ma NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
		kKnob_MvNCRESshape,      ///< Mv NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
		kKnob_MaNCRES,           ///< Ma NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
		kKnob_MvNCRES,           ///< Mv NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization

		// Coherent pion production tweaking parameters:
		kKnob_MaCOHpi,             ///< Ma for COH pion production
		kKnob_R0COHpi,             ///< R0 for COH pion production
		// Non-resonance background tweaking parameters:
		kKnob_RvpCC1pi,           ///< the 1pi non-RES bkg in the RES region, for v+p CC
		kKnob_RvpCC2pi,           ///< the 2pi non-RES bkg in the RES region, for v+p CC
		kKnob_RvpNC1pi,           ///< the 1pi non-RES bkg in the RES region, for v+p NC
		kKnob_RvpNC2pi,           ///< the 2pi non-RES bkg in the RES region, for v+p NC
		kKnob_RvnCC1pi,           ///< the 1pi non-RES bkg in the RES region, for v+n CC
		kKnob_RvnCC2pi,           ///< the 2pi non-RES bkg in the RES region, for v+n CC
		kKnob_RvnNC1pi,           ///< the 1pi non-RES bkg in the RES region, for v+n NC
		kKnob_RvnNC2pi,           ///< the 2pi non-RES bkg in the RES region, for v+n NC
		kKnob_RvbarpCC1pi,        ///< the 1pi non-RES bkg in the RES region, for vbar+p CC
		kKnob_RvbarpCC2pi,        ///< the 2pi non-RES bkg in the RES region, for vbar+p CC
		kKnob_RvbarpNC1pi,        ///< the 1pi non-RES bkg in the RES region, for vbar+p NC
		kKnob_RvbarpNC2pi,        ///< the 2pi non-RES bkg in the RES region, for vbar+p NC
		kKnob_RvbarnCC1pi,        ///< the 1pi non-RES bkg in the RES region, for vbar+n CC
		kKnob_RvbarnCC2pi,        ///< the 2pi non-RES bkg in the RES region, for vbar+n CC
		kKnob_RvbarnNC1pi,        ///< the 1pi non-RES bkg in the RES region, for vbar+n NC
		kKnob_RvbarnNC2pi,        ///< the 2pi non-RES bkg in the RES region, for vbar+n NC
		// DIS tweaking parameters - applied for DIS events with (Q2>Q2o, W>Wo),
		// typically Q2okReweight =1GeV^2, WokReweight =1.7-2.0GeV
		kKnob_AhtBY,              ///< the Bodek-Yang model parameter A_{ht} - incl. both shape and normalization effect
		kKnob_BhtBY,              ///< the Bodek-Yang model parameter B_{ht} - incl. both shape and normalization effect
		kKnob_CV1uBY,             ///< the Bodek-Yang model parameter CV1u - incl. both shape and normalization effect
		kKnob_CV2uBY,             ///< the Bodek-Yang model parameter CV2u - incl. both shape and normalization effect
		kKnob_AhtBYshape,         ///< the Bodek-Yang model parameter A_{ht} - shape only effect to d2sigma(DIS)/dxdy
		kKnob_BhtBYshape,         ///< the Bodek-Yang model parameter B_{ht} - shape only effect to d2sigma(DIS)/dxdy
		kKnob_CV1uBYshape,        ///< the Bodek-Yang model parameter CV1u - shape only effect to d2sigma(DIS)/dxdy
		kKnob_CV2uBYshape,        ///< the Bodek-Yang model parameter CV2u - shape only effect to d2sigma(DIS)/dxdy
		kKnob_NormDISCC,          ///< the inclusive DIS CC normalization (not currently working in genie)
		kKnob_RnubarnuCC,         ///< the ratio of \sigma(\bar\nu CC) / \sigma(\nu CC) (not currently working in GENIE; retain for future)
		kKnob_DISNuclMod,         ///< DIS nuclear modification (shadowing, anti-shadowing, EMC).  Not currently working in GENIE; retain for future
		//
		kKnob_NC,                ///< NC overall normalization (currently does nothing, but retain for future)

		//
		// Hadronization (free nucleon target)
		//
		kKnob_AGKY_xF1pi,         ///< xF distribution for low multiplicity (N + pi) DIS f/s produced by AGKY
		kKnob_AGKY_pT1pi,         ///< pT distribution for low multiplicity (N + pi) DIS f/s produced by AGKY

		//
		// Medium-effects to hadronization
		//
		kKnob_FormZone,         ///< formation zone

		//
		// Intranuclear rescattering systematics.
		// There are 2 sets of parameters:
		// - parameters that control the total rescattering probability, P(total)
		// - parameters that control the fraction of each process (`fate'), given a total rescat. prob., P(fate|total)
		// These parameters are considered separately for pions and nucleons.
		//

		kKnob_MFP_pi,         ///< mean free path for pions
		kKnob_MFP_N,          ///< mean free path for nucleons
		kKnob_FrCEx_pi,       ///< charge exchange probability for pions, for given total rescattering probability

		/// elastic probability for pions, for given total rescattering probability.
		/// THIS NO LONGER EXISTS FOR GENIE 3 (but can't remove because of enum skew otherwise)
		kKnob_FrElas_pi,

		kKnob_FrInel_pi,      ///< inelastic probability for pions, for given total rescattering probability
		kKnob_FrAbs_pi,       ///< absorption probability for pions, for given total rescattering probability
		kKnob_FrPiProd_pi,    ///< pion production probability for pions, for given total rescattering probability
		kKnob_FrCEx_N,        ///< charge exchange probability for nucleons, for given total rescattering probability

		/// elastic probability for nucleons, for given total rescattering probability
		/// THIS NO LONGER EXISTS FOR GENIE 3 (but can't remove because of enum skew otherwise)
		kKnob_FrElas_N,

		kKnob_FrInel_N,       ///< inelastic  probability for nucleons, for given total rescattering probability
		kKnob_FrAbs_N,        ///< absorption probability for nucleons, for given total rescattering probability
		kKnob_FrPiProd_N,     ///< pion production probability for nucleons, for given total rescattering probability

		//
		// Nuclear model
		//
		kKnob_CCQEPauliSupViaKF,   ///<
		kKnob_CCQEMomDistroFGtoSF, ///<

		//
		// Resonance decays
		//
		kKnob_BR1gamma,         ///< Resonance -> X + gamma branching ratio, eg Delta+(1232) -> p gamma
		kKnob_BR1eta,           ///< Resonance -> X + eta   branching ratio, eg N+(1440) -> p eta
		kKnob_Theta_Delta2Npi,  ///< distort pi angular distribution in Delta -> N + pi

		//
		// Alternative approach to CCQE form factors (z-expansion)
		//

		kKnob_ZNormCCQE,        ///< Z-expansion CCQE normalization (energy independent)
		kKnob_ZExpA1CCQE,       ///< Z-expansion coefficient 1, affects dsigma(CCQE)/dQ2 both in shape and normalization
		kKnob_ZExpA2CCQE,       ///< Z-expansion coefficient 2, affects dsigma(CCQE)/dQ2 both in shape and normalization
		kKnob_ZExpA3CCQE,       ///< Z-expansion coefficient 3, affects dsigma(CCQE)/dQ2 both in shape and normalization
		kKnob_ZExpA4CCQE,       ///< Z-expansion coefficient 4, affects dsigma(CCQE)/dQ2 both in shape and normalization
		kKnob_AxFFCCQEshape,     ///< axial nucleon form factors (dipole -> z-expansion) - shape only effect of dsigma(CCQE)/dQ2
		
		kLastKnob
	};

}

#endif //NOVARWGT_GENIEKNOBNAMES_H
