/*
 * EmpiricalMECTune2018.h:
 *  MEC tune from 2018 analysis.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_EMPIRICALMECTUNE2018_H
#define NOVARWGT_EMPIRICALMECTUNE2018_H

#include "TH2.h"

#include "NOvARwgt/util/HistWrapper.h"
#include "NOvARwgt/rwgt/IWeightGenerator.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneBase.h"


namespace novarwgt
{
	// forward declarations
	struct EventRecord;
	class InputVals;

	/// Weighter base class to build 2018 tune of Empirical MEC.  Se docdb: ??? (final tune doc)
	class EmpiricalMECWgt2018 : public EmpiricalMECq0q3NuNubarTuneWgt
	{
		public:
			template <typename T>
			explicit EmpiricalMECWgt2018(const IRegisterable::ClassID<T>& clID)
			   : EmpiricalMECq0q3NuNubarTuneWgt(clID, "EmpiricalMEC_CV2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
			                                    "$NOVARWGT_DATA/rw_empiricalMEC2018_nu.root", "numu_mec_weights_smoothed",
				                                "$NOVARWGT_DATA/rw_empiricalMEC2018_nubar.root", "numubar_mec_weights_smoothed")
			{}
	};

	extern const EmpiricalMECWgt2018 * kEmpiricalMECWgt2018;

	//----------------------------------------------------------------------------

	/// Similar to EmpiricalMECWgt2018 but after fittig MEC with hydrogen RPA bug fix
	class EmpiricalMECWgt2018RPAFix : public EmpiricalMECq0q3NuNubarTuneWgt
	{
		public:
			template <typename T>
			explicit EmpiricalMECWgt2018RPAFix(const IRegisterable::ClassID<T>& clID)
				: EmpiricalMECq0q3NuNubarTuneWgt(clID, "EmpiricalMECRPAFix_CV2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                                 "$NOVARWGT_DATA/rw_empiricalMEC2018_RPAfix_nu.root", "numu_mec_weights_smoothed",
					                             "$NOVARWGT_DATA/rw_empiricalMEC2018_RPAfix_nubar.root", "numubar_mec_weights_smoothed")
			{}
	};

	extern const EmpiricalMECWgt2018RPAFix * kEmpiricalMECWgt2018RPAFix;

	//----------------------------------------------------------------------------
	// Reweights of Empirical MEC based on MEC fits with MC altered to be 'qe-like' or 'res-like', for systs, ref needed
	//----------------------------------------------------------------------------

	/// MEC fitted with "QE-like" base model
	class MEC2018_QElike_Wgt : public EmpiricalMECq0q3NuNubarTuneWgt
	{
		public:
			template <typename T>
			explicit MEC2018_QElike_Wgt(const IRegisterable::ClassID<T>& clID)
				: EmpiricalMECq0q3NuNubarTuneWgt(clID, "EmpiricalMEC_QElike2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                                 "$NOVARWGT_DATA/rw_empiricalMEC2018_nuqelike_syst.root", "numu_mec_weights_smoothed",
			                                     "$NOVARWGT_DATA/rw_empiricalMEC2018_nubarqelike_syst.root", "numubar_mec_weights_smoothed")
			{}
	};

	extern const MEC2018_QElike_Wgt * kMEC2018_QElike_Wgt;

	//----------------------------------------------------------------------------

	/// MEC fitted with "RES-like" base model
	class MEC2018_RESlike_Wgt : public EmpiricalMECq0q3NuNubarTuneWgt
	{
		public:
			template <typename T>
			explicit MEC2018_RESlike_Wgt(const IRegisterable::ClassID<T>& clID)
				: EmpiricalMECq0q3NuNubarTuneWgt(clID, "EmpiricalMEC_RESlike2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                                 "$NOVARWGT_DATA/rw_empiricalMEC2018_nureslike_syst.root", "numu_mec_weights_smoothed",
					                             "$NOVARWGT_DATA/rw_empiricalMEC2018_nubarreslike_syst.root", "numubar_mec_weights_smoothed")
			{}
	};

	extern const MEC2018_RESlike_Wgt * kMEC2018_RESlike_Wgt;

	//----------------------------------------------------------------------------
	// Versions of above updated for hydrogen RPA bugfix
	//----------------------------------------------------------------------------

	/// MEC fitted with "QE-like" base model (RPA-hydrogen bugfix)
	class MEC2018RPAFix_QElike_Wgt : public EmpiricalMECq0q3NuNubarTuneWgt
	{
		public:
			template <typename T>
			explicit MEC2018RPAFix_QElike_Wgt(const IRegisterable::ClassID<T>& clID)
				: EmpiricalMECq0q3NuNubarTuneWgt(clID, "EmpiricalMEC_RPAFix_QElike2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                                 "$NOVARWGT_DATA/rw_empiricalMEC2018_RPAfix_nu_nonmec_qelike.root", "numu_mec_weights_smoothed",
				                                 "$NOVARWGT_DATA/rw_empiricalMEC2018_RPAfix_nubar_nonmec_qelike.root", "numubar_mec_weights_smoothed")
			{}
	};

	extern const MEC2018RPAFix_QElike_Wgt * kMEC2018RPAFix_QElike_Wgt;

	//----------------------------------------------------------------------------

	/// MEC fitted with "RES-like" base model (RPA-hydrogen bugfix)
	class MEC2018RPAFix_RESlike_Wgt : public EmpiricalMECq0q3NuNubarTuneWgt
	{
		public:
			template <typename T>
			explicit MEC2018RPAFix_RESlike_Wgt(const IRegisterable::ClassID<T>& clID)
				: EmpiricalMECq0q3NuNubarTuneWgt(clID, "EmpiricalMEC_RPAFix_RESlike2018", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                                 "$NOVARWGT_DATA/rw_empiricalMEC2018_RPAfix_nu_nonmec_reslike.root", "numu_mec_weights_smoothed",
				                                 "$NOVARWGT_DATA/rw_empiricalMEC2018_RPAfix_nubar_nonmec_reslike.root", "numubar_mec_weights_smoothed")
			{}
	};

	extern const MEC2018RPAFix_RESlike_Wgt * kMEC2018RPAFix_RESlike_Wgt;
}

#endif //NOVARWGT_EMPIRICALMECTUNE2018_H
