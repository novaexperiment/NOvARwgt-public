/*
 * IWeightGenerator.h:
 *  Base class for weight generators.  Allows them to be stored in containers.
 *
 *  Created on: Oct. 15, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_IWEIGHTGENERATOR_H_
#define NOVARWGT_IWEIGHTGENERATOR_H_

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/Exceptions.h"
#include "NOvARwgt/util/Hash.h"
#include "NOvARwgt/util/ITestGenVersion.h"
#include "NOvARwgt/util/Registry.h"

namespace novarwgt
{
	// forward declarations
	struct EventRecord;
	class InputVals;

	enum Generator : unsigned short;

	/// Base class for weight generators -- allows them to be stored in a single container
	class IWeightGenerator : public novarwgt::IRegisterable, public novarwgt::ITestGenVersion
	{
		public:
			template <typename T>
			IWeightGenerator(const IRegisterable::ClassID<T> & clID,
			                 const std::string& name,
			                 const std::unordered_set<novarwgt::GeneratorSupportConfig> & support)
                : IRegisterable(clID, name), ITestGenVersion(name, support)
			{}

			double GetWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams) const
			{
				// some events don't have enough truth info to be useful
				if (ev.expectNoWeights)
					return 1.0;

				TestIfEvtGenIsSupported(ev, otherParams);

				return CalcWeight(ev, otherParams);
			}

			virtual ~IWeightGenerator() = default;

		protected:
			/// Return the weight.  Passed an event record and optional map of other parameters.
			virtual double CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams) const = 0;
	};

	/// Get me a weighter!  (Just an alias for GetRegisterable(), really, but makes the intent clearer)
	template<typename T, typename ... Args>
	const T * GetWeighter(Args && ... args)
	{
		// not strictly necessary, but let's preserve the intent of the function
		static_assert(std::is_base_of<novarwgt::IWeightGenerator, T>::value,
		              "novarwgt::GetWeighter() should only be used with types derived from novarwgt::IWeightGenerator");
		return dynamic_cast<const T*>(GetRegisterable<T>(std::forward<Args>(args)...));
	}


} /* namespace novarwgt */

#endif /* NOVARWGT_IWEIGHTGENERATOR_H_ */
