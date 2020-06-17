/*
 * TunesSA.cxx:
 *  Ingredients to full cross section "tunes" used in Second Analysis.
 *
 *  Created on: Nov. 25, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/tunes/TunesSA.h"

#include "NOvARwgt/rwgt/genie/DIS/Nonres1piWeights.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneSA.h"

namespace novarwgt
{
	const novarwgt::Tune kCVTuneSA(
		{
			{ "Nonres1pi", novarwgt::GetWeighter<Nonres1PiWgt>(true, false) },  // request older approximate version of nonres pi selection
			{ "MEC",       novarwgt::GetWeighter<Tufts2p2hWgtSA>() },
			},
		{}  // we don't have the SA error budget implemented here
	);

}