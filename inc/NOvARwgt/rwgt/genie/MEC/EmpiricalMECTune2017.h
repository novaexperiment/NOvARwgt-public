/*
 * EmpiricalMECTuneBase.cxx:
 *  MEC tune from 2017 analysis.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_EMPIRICALMECTUNE2017_H
#define NOVARWGT_EMPIRICALMECTUNE2017_H

#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/rwgt/IWeightGenerator.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneBase.h"

namespace novarwgt
{
	// forward declarations
	struct EventRecord;

	//----------------------------------------------------------------------------

	/// Reweight Empirical MEC Enu shape, bracketing theory variations
	class MECEnuShapeWgt : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit MECEnuShapeWgt(const IRegisterable::ClassID<T>& clID)
			  : IWeightGenerator(clID, "MECEnuShape2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)})
			{}

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;
	};

	extern const MECEnuShapeWgt * kMECEnuShapeWgt;

	//----------------------------------------------------------------------------

	///  Reweight Empirical MEC initial state np fraction, bounded by theoretical expectations
	class MECInitStateNPFracWgt : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit MECInitStateNPFracWgt(const IRegisterable::ClassID<T>& clID)
			  : IWeightGenerator(clID, "MECInitStateNPFrac2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)})
			{}

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;
	};

	extern const MECInitStateNPFracWgt * kMECInitStateNPFracWgt;

	//----------------------------------------------------------------------------

	/// Scale Empirical MEC based on fit to NOvA ND data
	class EmpiricalMECWgt2017 : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit EmpiricalMECWgt2017(const IRegisterable::ClassID<T>& clID)
			  : IWeightGenerator(clID, "EmpiricalMEC_CV2017", {StoredGenSupportCfg(GenCfg::kGENIE_Prod2Only)})
			{}

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;
	};

	extern const EmpiricalMECWgt2017 * kEmpiricalMECWgt2017;

	//----------------------------------------------------------------------------
	// alternative tunes for Empirical MEC to bound the q0 response.  For systematics mostly.

	/// Reweight Empirical MEC so that the events have the same q0 distribution as GENIE QE
	class EmpiricalMEC_to_GENIEQE_Wgt : public EmpiricalMECq0q3TuneWgt
	{
		public:
			template <typename T>
			explicit EmpiricalMEC_to_GENIEQE_Wgt(const IRegisterable::ClassID<T>& clID)
				: EmpiricalMECq0q3TuneWgt(clID, "EmpiricalMEC_to_GENIEQE_2017", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                          "$NOVARWGT_DATA/rw_empiricalMEC2017_syst.root", "q0_rwgt_EmpiricalMEC_to_GENIEQE")
			{}
	};

	extern const EmpiricalMEC_to_GENIEQE_Wgt * kEmpiricalMEC_to_GENIEQE_Wgt;

	//----------------------------------------------------------------------------

	/// Reweight Empirical MEC so that the events have the same q0 distribution as GENIE RES
	class EmpiricalMEC_to_GENIERES_Wgt : public EmpiricalMECq0q3TuneWgt
	{
		public:
			template <typename T>
			explicit EmpiricalMEC_to_GENIERES_Wgt(const IRegisterable::ClassID<T>& clID)
				: EmpiricalMECq0q3TuneWgt(clID, "EmpiricalMEC_to_GENIERES_2017", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                          "$NOVARWGT_DATA/rw_empiricalMEC2017_syst.root", "q0_rwgt_EmpiricalMEC_to_GENIERES")
			{}
	};

	extern const EmpiricalMEC_to_GENIERES_Wgt * kEmpiricalMEC_to_GENIERES_Wgt;
}
#endif //NOVARWGT_EMPIRICALMECTUNE2017_H
