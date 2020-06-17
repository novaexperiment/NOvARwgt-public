/*
 * EmpiricalMECTuneBase.cxx:
 *  Base class(es) for MEC tunes.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <limits>

#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/rwgt/EventRecord.h"

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneBase.h"

namespace novarwgt
{
	double EmpiricalMECq0q3TuneWgt::CalcWeight(const novarwgt::EventRecord& ev, const InputVals&) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		double qmag = ev.q.Vect().Mag();
		double q0 = ev.q.E();
		return this->fHist.GetValueInRange(qmag, q0,
		                                   {1,-1}, {1,-1},  // these two are defaults -- use the whole bin range
		                                   {0, std::numeric_limits<double>::infinity()}  // don't let weights go below zero.
		);
	}

}