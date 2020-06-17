/*
 * MAQESysts.h:
 *  CCQE custom syst knobs.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_MAQESYSTS_H
#define NOVARWGT_MAQESYSTS_H


#include "NOvARwgt/rwgt/ISystKnob.h"
#include "NOvARwgt/rwgt/genie/GenieKnobNames.h"
#include "NOvARwgt/rwgt/genie/QE/MAQEWgts.h"

namespace novarwgt
{
	// forward declare
	class GenieSystKnob;

	//----------------------------------------------------------------------
	/// 2018 'reduced' M_A^QE shift.
	//  Same as MAQEGenieReducedSyst2017 exept shift accounts
	//  for M_A^QE CV change from 0.99 to 1.04
	class MAQEGenieReducedSyst2018 : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit MAQEGenieReducedSyst2018(const IRegisterable::ClassID<T> & clID);

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			const novarwgt::GenieSystKnob * fGENIEMAQEKnob;
	};

	extern const MAQEGenieReducedSyst2018 * kMAQEGenieReducedSyst2018;

	//----------------------------------------------------------------------

	/// 2017 'reduced' M_A^QE shift.
	/// Uncertainty in the M_A for CCQE is large because of discrepancies
	/// seen in extracted M_A values from nuclear target materials
	/// (i.e., MiniBooNE, CH2) compared to the free nucleon value.
	/// This discrepancy is now expected to be due to various nuclear
	/// effects like 2p2h and long-range nuclear charge screening (RPA effect),
	/// which we now explicitly handle with their own uncertainties.
	/// Accordingly, we want to return the M_A uncertainty to the free
	/// nucleon one, where it's measured best.  That's ~5%.
	class MAQEGenieReducedSyst2017 : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit MAQEGenieReducedSyst2017(const IRegisterable::ClassID<T> & clID);

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			const novarwgt::GenieSystKnob * fGENIEMAQEKnob;
	};

	extern const MAQEGenieReducedSyst2017 * kMAQEGenieReducedSyst2017;
}

#endif //NOVARWGT_MAQESYSTS_H
