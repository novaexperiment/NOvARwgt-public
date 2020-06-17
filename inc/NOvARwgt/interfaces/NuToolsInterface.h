/*
 * NuToolsInterface.h:
 *  Interface to FNAL NuTools simb::Events.
 *
 *  Created on: Oct. 26, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_NUTOOLSINTERFACE_H
#define NOVARWGT_NUTOOLSINTERFACE_H

#include "NOvARwgt/rwgt/EventRecord.h"

namespace simb
{
	class MCTruth;
	class GTruth;
}

namespace novarwgt
{
	novarwgt::EventRecord ConvertNuToolsEvent(const simb::MCTruth * mctruth, const simb::GTruth * gtruth, const novarwgt::ReweightList & rwList);
}

#endif //NOVARWGT_NUTOOLSINTERFACE_H
