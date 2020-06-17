/*
 * Registry.cxx:
 *  Stuff needed to create unique registry of various objects.
 *
 *  Created on: Oct. 15, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/util/Registry.h"

namespace novarwgt
{
	// this guy needs to be initialized in only one translation unit.
	// the linker then doesn't get multiple definitions.
	unsigned int IRegisterable::sIdCounter = 0;

	namespace registry
	{
		Registry & __GetRegistry()
		{
			// Registry of all Registerables that have been requested.
			// Centralized here to avoid the "static initialization order problem."
			static std::unordered_map <registry::_ClassID, std::unique_ptr<novarwgt::IRegisterable>, registry::Hasher>
				registerables = {};

			return registerables;
		}
	}

	const novarwgt::IRegisterable * GetRegisterableByName(const std::string & name)
	{
		for (const auto & reg : registry::__GetRegistry())
		{
			if (reg.second->Name() == name)
				return reg.second.get();
		}

		return nullptr;
	}

}