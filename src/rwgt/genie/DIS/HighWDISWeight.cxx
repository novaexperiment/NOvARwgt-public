/*
 * HighWDISWeight.cxx:
 *  Reweight for higher-W DIS.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/DIS/HighWDISWeight.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const HighWDISWgt_2018 * kHighWDISWgt_2018 = novarwgt::GetWeighter<HighWDISWgt_2018>();

	//----------------------------------------------------------------------

	double HighWDISWgt_2018::CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals&) const
	{
		bool isDIS    = ev.reaction == novarwgt::kScDeepInelastic;
		bool isAntiNu = ev.nupdg < 0;
		double W      = ev.W;

		// a few NOvA records are bonkers
		if ( W < 0 || std::isnan(W) )
			return 1.;

		if ( !isDIS || isAntiNu || W < 1.7 ) return 1.;

		return 1.1;
	}
}