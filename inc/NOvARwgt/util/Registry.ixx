/*
 * Registry.ixx:
 *  Implementation of the templated Registry internals.
 *
 *  This header needs to be included in places where GetRegisterable() is called.
 *
 *  In general it is not part of the public interface; clients' compilers
 *  can otherwise inadvertently generate incompatible versions of the implementation
 *  when clients call it.
 *
 *  (However, with care, clients may create their own novarwgt::IWeightGenerator
 *   or novarwgt::ISystKnob derived classes and instantiate them; if they do, they need this header.
 *   NOvA collaborators can use this to develop work-in-progress weights, tunes, and knobs.
 *
 *   While it *may* work depending on your compiler and toolchain,
 *   ****** #include'ing THIS FILE OUTSIDE NOVARWGT IS NOT OFFICIALLY SUPPORTED.  USE AT YOUR OWN RISK. *****)
 *
 *  In normal usage, every variant of GetRegisterable() using a NOvARwgt IWeightGenerator or ISystKnob
 *  that a client might want must be either used or explicitly instantiated
 *  inside NOvARwgt somewhere.
 *
 *  Created on: Nov. 23, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_REGISTRY_IXX
#define NOVARWGT_REGISTRY_IXX

#include "NOvARwgt/util/Registry.h"

namespace novarwgt
{
	template<typename T, typename ... Args>
	const novarwgt::IRegisterable * GetRegisterable(Args &&... args)
	{
		static_assert(std::is_base_of<novarwgt::IRegisterable, T>::value,
		              "novarwgt::GetRegisterable() can only be used with classes derived from novarwgt::IRegisterable");

		registry::Registry &registerables = registry::__GetRegistry();
		// registerable ID includes the args since different instances of the weighter result with different args.
		// note that we must COPY the arguments here (!) because if we std::forward them they could be altered,
		// which would make the std::forward<> two lines down get a different set of args.
		// hopefully we don't wind up with copy constructor issues somewhere down the road.
		auto rgstrId = IRegisterable::ClassID<T>(Args(args)...);
		if (registerables.find(rgstrId) == registerables.end())
			registerables.emplace(rgstrId, std::make_unique<T>(rgstrId, std::forward<Args>(args)...));
		return registerables.at(rgstrId).get();
	}

}

#endif // NOVARWGT_REGISTRY_IXX