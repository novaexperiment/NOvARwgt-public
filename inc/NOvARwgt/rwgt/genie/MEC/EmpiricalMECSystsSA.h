/*
 * EmpiricalMECSystsSA.h:
 *  MEC systematics from Second Analysis.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_EMPIRICALMECSYSTSSA_H
#define NOVARWGT_EMPIRICALMECSYSTSSA_H

#include <string>

#include "NOvARwgt/rwgt/ISystKnob.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSystsBase.h"

namespace novarwgt
{

	//---------------------------------------------------------------------------
	/// Second Analysis uncertainty on empirical MEC model in GENIE: 50% (see DocDB 15113)
	class MECScaleSystSA: public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit MECScaleSystSA(const IRegisterable::ClassID<T> & clID)
				: ISystKnob(clID,
				            "mecScale",
				            {StoredGenSupportCfg(GenCfg::kGENIE_Prod2Only)},
				            {0, 10})
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;
	};

	extern const MECScaleSystSA * kMECScaleSystSA;
}

#endif //NOVARWGT_EMPIRICALMECSYSTSSA_H
