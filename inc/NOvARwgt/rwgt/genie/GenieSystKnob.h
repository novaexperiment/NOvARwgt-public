/*
 * GenieSystKnob.h:
 *  Generic knob for use with GENIE systs.
 *
 *  Created on: Nov. 1, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_GENIESYSTKNOB_H
#define NOVARWGT_GENIESYSTKNOB_H

#include "NOvARwgt/rwgt/ISystKnob.h"
#include "NOvARwgt/rwgt/genie/GenieKnobNames.h"

namespace novarwgt
{
	class GenieSystKnob : public novarwgt::ISystKnob
	{
		friend const GenieSystKnob * GetGenieSystKnob(novarwgt::ReweightKnob knob);

		public:
			/// Make a GENIE syst knob.  You can't use this directly; use GetGenieSystKnob() instead.
			template <typename T>
			explicit GenieSystKnob(IRegisterable::ClassID<T> & clID, novarwgt::ReweightKnob knob);

			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			double InterpolateStoredWgts(double sigma, const novarwgt::ReweightVals &wgts) const;

			novarwgt::ReweightKnob fKnobIdx;
			bool fWarnedGenieCalc;             ///< Have we already issued a warning that we're invoking GENIE to calculate weights?
	};

	/// Obtain a knob for a GENIE systematic.
	inline const GenieSystKnob * GetGenieSystKnob(novarwgt::ReweightKnob knob)
	{
		return GetSystKnob<novarwgt::GenieSystKnob>(knob);
	}
} // namespace novarwgt

#endif //NOVARWGT_GENIESYSTKNOB_H
