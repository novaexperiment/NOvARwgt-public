/*
 * ITestGenVersion.h:
 *  Interface class for testing generator versions.
 *
 *  Created on: Nov. 1, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_ITESTGENVERSION_H
#define NOVARWGT_ITESTGENVERSION_H

#include "NOvARwgt/util/Exceptions.h"
#include "NOvARwgt/util/GeneratorSupportConfig.h"
#include "NOvARwgt/util/InputVals.h"

namespace novarwgt
{
	class ITestGenVersion
	{
		public:
			explicit ITestGenVersion(std::string name, std::unordered_set<novarwgt::GeneratorSupportConfig> support)
			  : fName(std::move(name)), fGeneratorSupport(std::move(support))
			{}

			void TestIfGenIsSupported(novarwgt::Generator gen,
			                          const std::vector<int>& genVersion,
			                          const std::string & genConfigStr) const
			{
				bool supported = false;
				for (const auto & genSupport : fGeneratorSupport)
					supported = supported || genSupport.GeneratorIsSupported(gen, genVersion, genConfigStr);

				if (!supported)
					ThrowUnsupportedException(gen, genVersion, genConfigStr);
			}

			void TestIfEvtGenIsSupported(const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams) const
			{
				bool supported = false;
				for (const auto & genSupport : fGeneratorSupport)
					supported = supported || genSupport.EventIsSupported(ev, otherParams);

				if (!supported)
					ThrowUnsupportedException(ev.generator, ev.generatorVersion, ev.generatorConfigStr);
			}

			const std::string & GetName() const { return fName; }
			const std::unordered_set<novarwgt::GeneratorSupportConfig> & GetSupport() const { return fGeneratorSupport; }

		private:
			void ThrowUnsupportedException(novarwgt::Generator gen,
			                               const std::vector<int>& genVersion,
			                               const std::string & genConfigStr) const
			{
				throw UnsupportedGeneratorException("NOvARwgt: '" + fName
				                                    + "' does not support generator '" + DecodeGeneratorID(gen)
				                                    + "' with version '" + EncodeGeneratorVersion(genVersion) + "'"
				                                    + " and configuration string '" + genConfigStr + "'");

			}

			const std::string fName;
			const std::unordered_set<novarwgt::GeneratorSupportConfig> fGeneratorSupport;

	};
}

#endif //NOVARWGT_ITESTGENVERSION_H
