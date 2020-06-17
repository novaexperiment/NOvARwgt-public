/*
 * NonresPiSysts.h:
 *  Nonresonant pion product custom syst knobs.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_NONRESPISYSTS_H
#define NOVARWGT_NONRESPISYSTS_H

#include <string>

#include "NOvARwgt/rwgt/ISystKnob.h"


namespace novarwgt
{
	//----------------------------------------------------------------------
	/// Uncertainty on DIS events, configurable by pion multiplicity, neutrino/antineutrino, CC/NC, and hit nucleon.
	/// These supercede the 1- and 2-pion non-resonant background GENIE tweak parameters.
	class DISnPionSyst : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit DISnPionSyst(const IRegisterable::ClassID<T> & clID,
			                      unsigned int nPion, bool isAntiNu, bool isCC, bool struckProton,
			                      double Wcut = 3.0, double systVarLowW = 0.5, double systVarHighW = 0.1)
				: ISystKnob(clID,
				            std::string("DIS") + (isAntiNu ? "vbar" : "v")
				                + (struckProton ? "p" : "n") + (isCC ? "CC" : "NC") + std::to_string(nPion) + "pi",
				            {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)}),
				  fNPion(nPion), fIsAntiNu(isAntiNu),
				  fIsCC(isCC), fStruckProton(struckProton),
				  fWcut(Wcut), fSystVarLowW(systVarLowW), fSystVarHighW(systVarHighW)

			{
			    if (nPion > 3)
			    	throw std::runtime_error("NOvARwgt::DISnPionSyst only supports 0, 1, 2, 3+ pion final states.  You requested " + std::to_string(nPion));
			}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			unsigned int fNPion;
			bool fIsAntiNu, fIsCC, fStruckProton;
			double fWcut, fSystVarLowW, fSystVarHighW;
	};

	extern const DISnPionSyst * kDIS_CC_0pi_nu_p_SystKnob;
	extern const DISnPionSyst * kDIS_CC_1pi_nu_p_SystKnob;
	extern const DISnPionSyst * kDIS_CC_2pi_nu_p_SystKnob;
	extern const DISnPionSyst * kDIS_CC_3pluspi_nu_p_SystKnob;

	extern const DISnPionSyst * kDIS_NC_0pi_nu_p_SystKnob;
	extern const DISnPionSyst * kDIS_NC_1pi_nu_p_SystKnob;
	extern const DISnPionSyst * kDIS_NC_2pi_nu_p_SystKnob;
	extern const DISnPionSyst * kDIS_NC_3pluspi_nu_p_SystKnob;

	extern const DISnPionSyst * kDIS_CC_0pi_nu_n_SystKnob;
	extern const DISnPionSyst * kDIS_CC_1pi_nu_n_SystKnob;
	extern const DISnPionSyst * kDIS_CC_2pi_nu_n_SystKnob;
	extern const DISnPionSyst * kDIS_CC_3pluspi_nu_n_SystKnob;

	extern const DISnPionSyst * kDIS_NC_0pi_nu_n_SystKnob;
	extern const DISnPionSyst * kDIS_NC_1pi_nu_n_SystKnob;
	extern const DISnPionSyst * kDIS_NC_2pi_nu_n_SystKnob;
	extern const DISnPionSyst * kDIS_NC_3pluspi_nu_n_SystKnob;

	extern const DISnPionSyst * kDIS_CC_0pi_nubar_p_SystKnob;
	extern const DISnPionSyst * kDIS_CC_1pi_nubar_p_SystKnob;
	extern const DISnPionSyst * kDIS_CC_2pi_nubar_p_SystKnob;
	extern const DISnPionSyst * kDIS_CC_3pluspi_nubar_p_SystKnob;

	extern const DISnPionSyst * kDIS_NC_0pi_nubar_p_SystKnob;
	extern const DISnPionSyst * kDIS_NC_1pi_nubar_p_SystKnob;
	extern const DISnPionSyst * kDIS_NC_2pi_nubar_p_SystKnob;
	extern const DISnPionSyst * kDIS_NC_3pluspi_nubar_p_SystKnob;

	extern const DISnPionSyst * kDIS_CC_0pi_nubar_n_SystKnob;
	extern const DISnPionSyst * kDIS_CC_1pi_nubar_n_SystKnob;
	extern const DISnPionSyst * kDIS_CC_2pi_nubar_n_SystKnob;
	extern const DISnPionSyst * kDIS_CC_3pluspi_nubar_n_SystKnob;

	extern const DISnPionSyst * kDIS_NC_0pi_nubar_n_SystKnob;
	extern const DISnPionSyst * kDIS_NC_1pi_nubar_n_SystKnob;
	extern const DISnPionSyst * kDIS_NC_2pi_nubar_n_SystKnob;
	extern const DISnPionSyst * kDIS_NC_3pluspi_nubar_n_SystKnob;
} // namespace novarwgt

#endif //NOVARWGT_NONRESPISYSTS_H
