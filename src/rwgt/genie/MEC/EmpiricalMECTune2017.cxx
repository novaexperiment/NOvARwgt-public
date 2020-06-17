/*
 * EmpiricalMECTuneBase.cxx:
 *  MEC tune from 2017 analysis.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2017.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const MECEnuShapeWgt * kMECEnuShapeWgt = novarwgt::GetWeighter<MECEnuShapeWgt>();
	const MECInitStateNPFracWgt * kMECInitStateNPFracWgt = novarwgt::GetWeighter<MECInitStateNPFracWgt>();
	const EmpiricalMECWgt2017 * kEmpiricalMECWgt2017 = novarwgt::GetWeighter<EmpiricalMECWgt2017>();
	const EmpiricalMEC_to_GENIEQE_Wgt * kEmpiricalMEC_to_GENIEQE_Wgt = novarwgt::GetWeighter<EmpiricalMEC_to_GENIEQE_Wgt>();
	const EmpiricalMEC_to_GENIERES_Wgt * kEmpiricalMEC_to_GENIERES_Wgt = novarwgt::GetWeighter<EmpiricalMEC_to_GENIERES_Wgt>();

	//----------------------------------------------------------------------------

	double MECEnuShapeWgt::CalcWeight(const novarwgt::EventRecord& ev, const InputVals& params) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		double Enu    = ev.Enu;
		double sigma  = params.find( "sigma" ) != params.end() ? params.at("sigma") : 0.;

		if ( Enu < 0 ) return 1.0;

		double weight = 1 + sigma / ( 2.5 * Enu + 1 ); // Same weight function for nu & nubar
		if ( weight < 0 ) weight = 0;
		return weight;
	}

	//----------------------------------------------------------------------------

	double MECInitStateNPFracWgt::CalcWeight(const novarwgt::EventRecord& ev, const InputVals& params) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		int struckNuclPair = ev.struckNucl; // from GENIE: 2000000200 --> nn, 2000000201 --> np, 2000000201 --> pp
		double sigma = params.at( "sigma" );

		// Same nominal NP fraction and uncertainty for nu & nubar
		const double nominalNPfrac = 0.8;
		double newNPfrac = nominalNPfrac + ( 0.1 * sigma ); // 10% is 1-sigma shift (see DocDB 18741)
		if      ( newNPfrac > 1 ) newNPfrac = 1;
		else if ( newNPfrac < 0 ) newNPfrac = 0;

		if ( struckNuclPair == 2000000201 ) // np
			return newNPfrac / nominalNPfrac;
		else
			return ( 1 - newNPfrac ) / ( 1 - nominalNPfrac );
	}

	//----------------------------------------------------------------------------

	double EmpiricalMECWgt2017::CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals&) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		return 1.2;
	}

}