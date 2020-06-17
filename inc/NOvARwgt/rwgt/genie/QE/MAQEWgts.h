/*
 * MAQEWgts.h:
 *  Reweight for CCQE axial mass
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_MAQEWGTS_H
#define NOVARWGT_MAQEWGTS_H

#include "NOvARwgt/rwgt/IWeightGenerator.h"

namespace novarwgt
{
	class MAQEWeight_2018 : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit MAQEWeight_2018(const IRegisterable::ClassID<T> & clID)
			  : IWeightGenerator(clID, "MAQE_2018", {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)})  // works for any GENIE v2 (w/ MA = 0.99 and error = -15%/+25% syst knob)
			{}

			double CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &params) const override;

	};

	extern const MAQEWeight_2018 * kMAQEWeight_2018;
}

#endif //NOVARWGT_MAQEWGTS_H
