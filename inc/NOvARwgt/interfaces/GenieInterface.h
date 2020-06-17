/*
 * GenieInterface.h:
 *  Interface to GENIE.
 *
 *  Created on: Oct. 19, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_GENIEINTERFACE_H
#define NOVARWGT_GENIEINTERFACE_H

#include <unordered_map>

#include "NOvARwgt/rwgt/EventRecord.h"

namespace genie
{
	// forward declare
	class EventRecord;
}

namespace novarwgt
{
	/// Copy information out of the GENIE record.
	///
	/// \param evt  GENIE record
	/// \return weight for this event
	///
	novarwgt::EventRecord
		ConvertGenieEvent(const genie::EventRecord *evt,
		                  const novarwgt::ReweightList &storedWgts={});


}

#endif //NOVARWGT_GENIEINTERFACE_H
