/*
 * EmpiricalMECOtherTunes.h:
 *  Weights to construct other variants of MEC besides the NOvA tunes.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_EMPIRICALMECOTHERTUNES_H
#define NOVARWGT_EMPIRICALMECOTHERTUNES_H

#include <string>

#include "TH2.h"

#include "NOvARwgt/util/HistWrapper.h"
#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/rwgt/IWeightGenerator.h"

namespace novarwgt
{
	// forward declarations
	class InputVals;

	//----------------------------------------------------------------------------

	/// Reweight Empirical MEC to the Valencia model (q0,q3) distribution
	class EmpiricalMEC_to_Valencia_Wgt : public IWeightGenerator
	{
		public:
			explicit EmpiricalMEC_to_Valencia_Wgt(const IRegisterable::ClassID<EmpiricalMEC_to_Valencia_Wgt>& clID);

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;

		private:
			// functions so that we avoid Static Initialization Order problem
			static std::string DefaultNuRwgtFile() { return "$NOVARWGT_DATA/rw_empiricalMECtoValencia_nu.root"; }
			static std::string DefaultNubarRwgtFile() { return "$NOVARWGT_DATA/rw_empiricalMECtoValencia_nubar.root"; }

			const HistWrapper<TH2> fHist_nu_nn;
			const HistWrapper<TH2> fHist_nu_np;
			const HistWrapper<TH2> fHist_nubar_np;
			const HistWrapper<TH2> fHist_nubar_pp;
	};

	extern const EmpiricalMEC_to_Valencia_Wgt * kEmpiricalMEC_to_Valencia_Wgt;

}

#endif //NOVARWGT_EMPIRICALMECOTHERTUNES_H
