/*
 * COHSysts.h:
 *   Systs for Coherent scattering.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_COHSYSTS_H
#define NOVARWGT_COHSYSTS_H

#include "NOvARwgt/rwgt/ISystKnob.h"

namespace novarwgt
{
	/// \brief 20% uncertainty scale on coherent events
	///
	/// For CC, See page 4 of MINERvA's first COH results:
	/// https://journals.aps.org/prl/pdf/10.1103/PhysRevLett.113.261802
	/// which has 12% (numu) and 17% (numubar) errors in flux-averaged
	/// cross-section (2-20 GeV).
	/// Private communication (Aaron M):
	/// "Although MINERvA's latest coherent paper unfortunately does
	/// not state the flux averaged cross sections, I calculated the
	/// total uncertainty for 2-5 GeV (a range more relevant to NOvA)
	/// to be 14% for numu and 20% for numubar."
	/// We take 20% on both for now for simplicity.
	///
	/// For NC, Duyang's W&C docdb 25119-v12 gives the total stat+syst uncertainty as
	/// 16.7% on p22, and the central value is very close to the GENIE prediction.
	/// Due to this being too precise a value to also use for nubar, choose to
	/// be conservative here and make 20% for both nu and nubar as well.
	class COHNormSyst2018 : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit COHNormSyst2018(const IRegisterable::ClassID<T>& clID, bool isCC)
				: ISystKnob(clID, std::string("COH") + (isCC ? "CC" : "NC") + "NormScale2018", {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)}),
				  fIsCC(isCC)
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			bool fIsCC;
	};
	extern const COHNormSyst2018 * kCOHNormCCSystKnob;
	extern const COHNormSyst2018 * kCOHNormNCSystKnob;
}

#endif //NOVARWGT_COHSYSTS_H
