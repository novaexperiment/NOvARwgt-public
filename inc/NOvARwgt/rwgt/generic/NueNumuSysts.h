/*
 * NueNumuSysts.h:
 *   Systs on possible nue/numu cross section differences.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_NUENUMUSYSTS_H
#define NOVARWGT_NUENUMUSYSTS_H

#include "NOvARwgt/rwgt/ISystKnob.h"

namespace novarwgt
{
	//----------------------------------------------------------------------
	/// \brief T2K's arXiv:1707.01048: radiative corrections
	///
	/// "These include an uncorrelated 2% uncertainty on the nue/numu and
	/// nuebar/numubar cross section ratios to account for radiative
	/// corrections..."
	class SimpleRadiativeCorrNueXSecSyst: public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit SimpleRadiativeCorrNueXSecSyst(const IRegisterable::ClassID<T> & clID, bool isNueBar)
			: ISystKnob(clID, std::string("radcorr") + (isNueBar ? "nue" : "nuebar"), {StoredGenSupportCfg(GenCfg::kGENIE_AllVersions)}),
			  fIsNueBar(isNueBar)
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			bool fIsNueBar;
	};
	extern const SimpleRadiativeCorrNueXSecSyst * kSimpleRadiativeCorrNueXsecSystKnob;
	extern const SimpleRadiativeCorrNueXSecSyst * kSimpleRadiativeCorrNuebarXsecSystKnob;

	//----------------------------------------------------------------------
	/// \brief T2K's arXiv:1707.01048 : 2nd-class currents
	///
	/// "...and an additional 2% uncertainty which is fully anticorrelated
	/// between nue and nuebar to allow for second class currents"
	class SimpleSecondClassCurrentsSyst: public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit SimpleSecondClassCurrentsSyst(const IRegisterable::ClassID<T> & clID)
				: ISystKnob(clID, std::string("2ndclasscurr"), {StoredGenSupportCfg(GenCfg::kGENIE_AllVersions)})
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;
	};
	extern const SimpleSecondClassCurrentsSyst * kSimpleSecondClassCurrentsSystKnob;


}

#endif //NOVARWGT_NUENUMUSYSTS_H
