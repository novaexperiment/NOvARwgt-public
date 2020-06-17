/*
 * ISystKnob.h:
 *  Base class for systematics knobs.  Allows them to be stored in containers.
 *
 *  Created on: Oct. 15, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_ISYSTKNOB_H
#define NOVARWGT_ISYSTKNOB_H

#include <string>
#include <numeric>

#include "NOvARwgt/rwgt/IWeightGenerator.h"

#include "NOvARwgt/util/ITestGenVersion.h"
#include "NOvARwgt/util/Registry.h"

namespace novarwgt
{
	class ISystKnob : public novarwgt::IRegisterable, public novarwgt::ITestGenVersion
	{
		public:
			/// Request the weight for this knob for given event at given sigma.
			double GetWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const
			{
				// some events don't have enough truth info to be useful
				if (ev.expectNoWeights)
					return 1.0;

				TestIfEvtGenIsSupported(ev, otherParams);

				double wgt = CalcWeight(sigma, ev, otherParams);

				return std::min(std::max(wgt, fClampRange.first), fClampRange.second);
			}

		protected:
			/// Build a syst knob.  Note that this constructor is not callable directly; use GetSystKnob() instead.
			/// \param name        Short name
			/// \param support     Which generator configurations does this knob work with?
			/// \param clampRange  Clamp the weights to a specified range
			/// \param cvWgts      Weighters whose values are used as central value weights that these weights are relative to
			template <typename T>
			explicit ISystKnob(const IRegisterable::ClassID<T> & clID,
			                   const std::string& name, std::unordered_set<novarwgt::GeneratorSupportConfig> support,
			                   std::pair<double, double> clampRange = {-std::numeric_limits<double>::infinity(),
			                                                           std::numeric_limits<double>::infinity()},
			                   std::vector<const novarwgt::IWeightGenerator *> cvWgts = {})
				: IRegisterable(clID, name),
				  ITestGenVersion(name, std::move(support)),
				  fClampRange(std::move(clampRange)),
				  fCVWgts(std::move(cvWgts))
			{}

			/// Actually compute the weight in derived classes.
			virtual double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const = 0;

			double CVWgt(const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const
			{
				return std::accumulate(fCVWgts.begin(), fCVWgts.end(), 1.0,
				                       [&](double wgt, const novarwgt::IWeightGenerator* wgtr)
				                       {
					                       return wgt * wgtr->GetWeight(ev, otherParams);
				                       });
			}

		private:
			std::pair<double, double> fClampRange;
			std::vector<const novarwgt::IWeightGenerator*> fCVWgts;
	};

	// ---------------------------------------------------------------------

	/// Get me a syst knob!  (Just an alias for GetRegisterable(), really, but makes the intent clearer)
	template<typename T, typename ... Args>
	const T * GetSystKnob(Args && ... args)
	{
		// not strictly necessary, but let's preserve the intent of the function
		static_assert(std::is_base_of<novarwgt::ISystKnob, T>::value,
		              "novarwgt::GetWeighter() should only be used with types derived from novarwgt::ISystKnob");
		return dynamic_cast<const T*>(GetRegisterable<T>(std::forward<Args>(args)...));
	}

	/// Get a syst knob by name.
	/// (Like with GetRegisterableByName(), the GetSystKnob() variant is more efficient
	///  and won't suffer from potential duplicates so use that where possible.)
	/// If 'okNotFound' is 'false' (default), will throw an exception if the syst knob is not found.
	inline const ISystKnob * GetSystKnobByName(const std::string & name, bool okNotFound = false)
	{
		auto knob = GetRegisterableByName(name);
		if (!knob && !okNotFound)
			throw std::runtime_error("NOvARwgt: Syst knob '" + name + "' was never registered");
		return dynamic_cast<const ISystKnob*>(knob);
	}

	// ---------------------------------------------------------------------
	/// Save some typing
	typedef std::unordered_set<const ISystKnob*> SystKnobSet;

	/// Enable combining SystKnobSets with 's1 | s2' syntax.
	inline SystKnobSet operator|(const SystKnobSet& r1, const SystKnobSet& r2)
	{
		SystKnobSet ret;
		std::set_union(r1.begin(), r1.end(),
		               r2.begin(), r2.end(),
		               std::inserter(ret, ret.end())
		);

		return ret;
	}

}

#endif //NOVARWGT_ISYSTKNOB_H
