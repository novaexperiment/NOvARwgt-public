/*
 * HighWDISWeight.h:
 *  Reweight for higher-W DIS.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_HIGHWDISWEIGHT_H
#define NOVARWGT_HIGHWDISWEIGHT_H

#include "NOvARwgt/rwgt/IWeightGenerator.h"

namespace novarwgt
{
	// forward declarations
	struct EventRecord;
	class InputVals;

	///  Scale high-W DIS to address numu CC MC deficit at low track length, high-y.
	///  Scaling determined per studies summarized in DocDB 27350
	class HighWDISWgt_2018
		: public IWeightGenerator
	{
		public:
			template <typename T>
			explicit HighWDISWgt_2018(const IRegisterable::ClassID<T> & clID)
			  : IWeightGenerator(clID, "HighWDIS_2018", {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)})
			{}

			double CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &params = {{}}) const override;
	};

	extern const HighWDISWgt_2018 * kHighWDISWgt_2018;
}

#endif //NOVARWGT_HIGHWDISWEIGHT_H
