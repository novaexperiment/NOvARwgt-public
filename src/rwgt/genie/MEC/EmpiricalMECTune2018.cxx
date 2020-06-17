/*
 * EmpiricalMECTuneBase.cxx:
 *  MEC tune from 2018 analysis.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2018.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const EmpiricalMECWgt2018 * kEmpiricalMECWgt2018 = novarwgt::GetWeighter<EmpiricalMECWgt2018>();
	const EmpiricalMECWgt2018RPAFix * kEmpiricalMECWgt2018RPAFix = novarwgt::GetWeighter<EmpiricalMECWgt2018RPAFix>();
	const MEC2018_QElike_Wgt * kMEC2018_QElike_Wgt = novarwgt::GetWeighter<MEC2018_QElike_Wgt>();
	const MEC2018_RESlike_Wgt * kMEC2018_RESlike_Wgt = novarwgt::GetWeighter<MEC2018_RESlike_Wgt>();
	const MEC2018RPAFix_QElike_Wgt * kMEC2018RPAFix_QElike_Wgt = novarwgt::GetWeighter<MEC2018RPAFix_QElike_Wgt>();
	const MEC2018RPAFix_RESlike_Wgt * kMEC2018RPAFix_RESlike_Wgt = novarwgt::GetWeighter<MEC2018RPAFix_RESlike_Wgt>();
}