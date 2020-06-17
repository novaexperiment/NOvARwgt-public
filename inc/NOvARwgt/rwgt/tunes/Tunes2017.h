/*
 * Tunes2017.h:
 *  Tunes for 2017 MC.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_TUNES2017_H
#define NOVARWGT_TUNES2017_H

#include <unordered_set>
#include "NOvARwgt/rwgt/Tune.h"
#include "NOvARwgt/rwgt/ISystKnob.h"

namespace novarwgt
{
	/// Knobs from GENIE ReWeight included in the 2017 error budget
	extern const std::unordered_set<const novarwgt::ISystKnob*> k2017GENIEKnobs;

	/// NOvA custom knobs included in the 2017 error budget
	extern const std::unordered_set<const novarwgt::ISystKnob*> k2017CustomKnobs;

	/// CV MC tune used in 2017 analyses
	extern const novarwgt::Tune kCVTune2017;
}

#endif //NOVARWGT_TUNES2017_H
