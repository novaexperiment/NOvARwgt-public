/*
 * Tune.cxx:
 *  User interface for "tunes," collections of weighters that are combined together.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <numeric>

#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/rwgt/Tune.h"

// these have their own #ifdefs in them so they don't do anything if the relevant bit isn't enabled
#include "NOvARwgt/interfaces/GenieInterface.h"


namespace novarwgt
{
	// --------------------------------------
	Tune::Tune(std::unordered_map<std::string, const novarwgt::IWeightGenerator*>  wgts,
	           SystKnobSet knobs)
		: fWeighters(std::move(wgts))
	{
		for (const auto & knob : knobs)
		{
			fSystKnobs.emplace(knob->GetName(), knob);
			fSystKnobNames.emplace_back(knob->GetName());
		}
	}

	// --------------------------------------
	double Tune::EventWeight(const novarwgt::EventRecord & evt, const novarwgt::InputVals & params) const
	{
		auto wgtsByComponent = this->EventWeightComponents(evt, params);
		return std::accumulate(wgtsByComponent.begin(), wgtsByComponent.end(), 1.0,
		                       [](double w1, const Tune::NamedWeight& w2){ return w1 * w2.weight;});
	}

	// --------------------------------------
	std::vector<Tune::NamedWeight>
		Tune::EventWeightComponents(const novarwgt::EventRecord & evt, const novarwgt::InputVals & params) const
	{
		std::vector<Tune::NamedWeight> wgts;
		for (const auto & wgtrPair : fWeighters)
			wgts.emplace_back(wgtrPair.first, wgtrPair.second->GetWeight(evt, params));

		return wgts;
	}

	// --------------------------------------
	double Tune::EventSystKnobWeight(const std::string &knobName,
	                                 double sigma,
	                                 const novarwgt::EventRecord &evt,
	                                 const InputVals &params,
	                                 bool relativeToCV) const
	{
		double wgt = fSystKnobs.at(knobName)->GetWeight(sigma, evt, params);
		if (relativeToCV)
		{
			double cvWgt = this->EventWeight(evt, params);
			if (cvWgt > 0)
				wgt /= cvWgt;
			else
				wgt = 0;
		}

		return wgt;
	}

	// --------------------------------------
	const std::vector<std::string> & Tune::KnobNames() const
	{
		return fSystKnobNames;
	}

	// --------------------------------------
	const std::unordered_map<std::string, const novarwgt::ISystKnob *> & Tune::SystKnobs() const
	{
		return fSystKnobs;
	}


}