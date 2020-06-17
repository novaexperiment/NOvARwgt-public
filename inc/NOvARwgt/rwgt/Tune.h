/*
 * Tune.h:
 *  User interface for "tunes," collections of weighters that are combined together.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_TUNE_H
#define NOVARWGT_TUNE_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "NOvARwgt/rwgt/IWeightGenerator.h"
#include "NOvARwgt/rwgt/ISystKnob.h"
#include "NOvARwgt/util/InputVals.h"

namespace novarwgt
{
	// forward declarations
	struct EventRecord;

	class Tune
	{
		public:
			struct NamedWeight
			{
				NamedWeight(std::string  nm, double wgt)
					: name(std::move(nm)), weight(wgt)
				{};

				std::string name;
				double weight;
			};

			typedef std::function<std::vector<NamedWeight>(const novarwgt::EventRecord &,
				                                           const novarwgt::InputVals &)>
				FunctionType;

			explicit Tune(std::unordered_map<std::string, const novarwgt::IWeightGenerator*>  wgts,
			              SystKnobSet knobs = {});

			/// Returns the product of the weights calculated in Tune::EventWeightComponents
			double EventWeight(const novarwgt::EventRecord & evt, const novarwgt::InputVals & params = {}) const;

			/// Workhorse method that uses the provided function to calculate the set of weights.
			std::vector<NamedWeight>
			    EventWeightComponents(const novarwgt::EventRecord & evt, const novarwgt::InputVals & params = {}) const;

			/// Get the weight for a specific knob.
			/// \param knobName       The name of the knob.  (See KnobNames() for a full list.)
			/// \param sigma          Number of sigma away from nominal you want the weight for
			/// \param evt            Event in question
			/// \param params         Any other needed parameters not in the event
			/// \param relativeToCV   Should this weight be given as an absolute event weight (default) or relative to the CV weight for this tune?
			/// \return               The requested weight
			double EventSystKnobWeight(const std::string & knobName,
			                           double sigma,
			                           const novarwgt::EventRecord & evt,
			                           const novarwgt::InputVals & params = {},
			                           bool relativeToCV = false) const;

			/// Get a full list of this Tune's relevant systematic knobs' names
			const std::vector<std::string> & KnobNames() const;

			/// Get the syst knobs associated with this tune
			const std::unordered_map<std::string, const novarwgt::ISystKnob *> & SystKnobs() const;

		private:
			/// This Tune's constituent CV weights.
			std::unordered_map<std::string, const novarwgt::IWeightGenerator*> fWeighters;

			/// This Tune's relevant systematic knobs.
			std::unordered_map<std::string, const novarwgt::ISystKnob*> fSystKnobs;

			/// Internal-use only list of knob names.  Filled from fSystKnobs in the constructor.
			std::vector<std::string> fSystKnobNames;
	};

}
#endif //NOVARWGT_TUNE_H
