/*
 * EventRecord.h:
 *  NOvARwgt internal event record type.
 *
 *  Created on: Oct 26, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <cstddef>
#include <iostream>

#include "NOvARwgt/rwgt/EventRecord.h"

namespace novarwgt
{
	// --------------------------------------
	ReweightVals & ReweightList::operator[]( std::size_t pos )
	{
		{
			if ( pos >= fWeights.size() )
			{
				fWeights.resize(pos+1);
				fKnobSet.resize(pos+1, false);

				// this will be overwritten by user unless something goes wrong
				fWeights[pos] = fNaNVals;
			}

			// assume that the use of the non-const version means user is assigning.
			fKnobSet[pos] = true;

			return fWeights[pos];
		};

	}

	// --------------------------------------
	const ReweightVals & ReweightList::operator[]( std::size_t pos ) const
	{
		{
			if ( !IsSet(pos) )
				return fNaNVals;

			return fWeights[pos];
		};

	}

	// --------------------------------------
	void EventRecord::PrintTo(std::ostream &stream) const
	{
		stream << "novarwgt::EventRecord:" << std::endl;
		stream << "  rxn is CC = " << isCC << std::endl;
		stream << "  rxn type (GENIE enum) = " << reaction << std::endl;
		stream << "  struck nucleon pdg = " << struckNucl << std::endl;
		stream << "  nu pdg = " << nupdg << std::endl;
		stream << "  nu energy = " << Enu << std::endl;
		stream << "  4-mom xfer components = (" << q.E() << "," << q.Px() << "," << q.Py() << "," << q.Pz() << ")" << std::endl;
		stream << "  Bjorken y (inelasticity) = " << y << std::endl;
		stream << "  Invariant hadronic mass W = " << W << std::endl;
		stream << "  Struck nucleus A = " << A << std::endl;
		stream << "  Num pi+ in final state BEFORE FSI = " << npiplus << std::endl;
		stream << "  Num pi0 in final state BEFORE FSI = " << npizero << std::endl;
		stream << "  Num pi- in final state BEFORE FSI = " << npiminus << std::endl;
		stream << "  Stored weights for " << genieWeights.size() << " GENIE knobs" << std::endl;
	}
}