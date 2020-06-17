/*
 * Nonres1piWeights.cxx:
 *  Reweights for the nonresonant single pion channel in GENIE DIS.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_NONRES1PI_H
#define NOVARWGT_NONRES1PI_H

#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/rwgt/IWeightGenerator.h"

namespace novarwgt
{
	/// Weighter which adjusts non-resonant single pion production rate based on updated fits to deuterium data,
	/// arXiv:1601.01888.
	/// Optional parameter useApproxCut is to replicate older behavior where all nonres 1pi with W<1.7 GeV
	/// was scaled down together.  Don't use it unless you know what you're doing.
	/// Optional parameter useTypeWeight replicates 2018 and prior behavior which had a typo in the weight (0.41 vs correct 0.43)
	class Nonres1PiWgt : public novarwgt::IWeightGenerator
	{
		public:
			template <typename T>
			explicit Nonres1PiWgt(const IRegisterable::ClassID<T> & clID,
			                      bool useApproxCut=false,
			                      bool useTypoWeight=false)
				: IWeightGenerator(clID, "Nonres1pi", {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)}),
			      fUseApproxCut(useApproxCut), fUseTypoWeight(useTypoWeight)
			{}

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params = {}) const override;

		private:
			bool fUseApproxCut;
			bool fUseTypoWeight; ///< in 2018 and prior we accidentally used 0.41 instead of 0.43 <facepalm />
	};

	/// Older version that uses an approximate weighting scheme for when CAFs didn't have all the necessary info
	extern const Nonres1PiWgt * kNonres1PiWgt_Approx;
	/// 2017-8 version that had a typo in the weight value :(
	extern const Nonres1PiWgt * kNonres1PiWgt_Typo;

	extern const Nonres1PiWgt * kNonres1PiWgt;
}


#endif //NOVARWGT_NONRES1PI_H
