/*
 * Tunes2018.h:
 *  Tunes for 2018 MC.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_TUNES2018_H
#define NOVARWGT_TUNES2018_H


#include "NOvARwgt/rwgt/Tune.h"

namespace novarwgt
{
	/// GENIE ReWeight knobs used in the 2018 error budget
	extern const std::unordered_set<const novarwgt::ISystKnob*> k2018GENIEKnobs;

	/// Original set of MEC knobs used in the 2018 oscillation analysis
	extern const std::unordered_set<const novarwgt::ISystKnob*> k2018MECKnobs;

	/// Set of MEC knobs tuned using the correct implementation of the RPA knobs
	/// (i.e., not mistakenly applying RPA to Hydrogen in the QE part of the base model)
	extern const std::unordered_set<const novarwgt::ISystKnob*> k2018MECKnobs_RPAFix;

	/// CV tune used for 2018 analysis
	extern const novarwgt::Tune kCVTune2018;

	/// 2018 cross section weight updated after Hydrogen RPA bug fix
	/// (but without update to fixed RPA correction for GENIE 2.12)
	extern const novarwgt::Tune kCVTune2018_RPAfix;

	/// 2018 cross section weight updated after Hydrogen RPA bug fix but without DIS tweak
	/// (and without correction to actual GENIE 2.12 RPA-CCQE weights)
	extern const novarwgt::Tune kCVTune2018_RPAfix_noDIStweak;
}

#endif //NOVARWGT_TUNES2018_H
