/*
 * GeneratorSupportConfig.h
 *
 *  Created on: Oct 29, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_GENERATORSUPPORTCONFIG_H
#define NOVARWGT_GENERATORSUPPORTCONFIG_H

#include <functional>
#include <numeric>
#include <regex>
#include <unordered_set>
#include <utility>
#include <vector>

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/util/Hash.h"
#include "NOvARwgt/util/InputVals.h"

// we're going to want to put these std::pair<>s into maps and sets...
namespace std
{
	template<>
	struct hash<std::pair<std::vector<int>, std::vector<int>>>
	{
		size_t operator()(const std::pair<std::vector<int>, std::vector<int>> &x) const
		{

			std::size_t hash = 0;
			for (const auto & v : x.first)
				novarwgt::hash::Hash(v, hash);
			for (const auto & v : x.second)
				novarwgt::hash::Hash(v, hash);

			return hash;
		}
	};

}


namespace novarwgt
{

	std::vector<int> DecodeGeneratorVersion(const std::string &versionString);
	std::string EncodeGeneratorVersion(const std::vector<int>& verVec);

	std::string DecodeGeneratorID(novarwgt::Generator gen);

	class GeneratorSupportConfig
	{
		public:
			GeneratorSupportConfig(novarwgt::Generator supportedGen,
			                       std::unordered_set<std::pair<std::vector<int>, std::vector<int>>> supportRanges,
			                       std::string reqdConfigStr)
			: fSupportedGenerator(supportedGen),
			  fSupportedRanges(std::move(supportRanges)),
			  fRequiredConfigStr(std::move(reqdConfigStr))
			{}

			bool GeneratorIsSupported(novarwgt::Generator gen,
			                          const std::vector<int>& genVersion,
			                          const std::string & genConfigStr) const
			{
				if (gen != fSupportedGenerator)
					return false;

				// empty config string means "all configs ok"
				if (!fRequiredConfigStr.empty() && genConfigStr != fRequiredConfigStr)
					return false;

				// an empty range means "all versions supported"
				if (fSupportedRanges.empty())
					return true;
				for (const auto &supportRange : fSupportedRanges)
				{
					if (genVersion >= supportRange.first && genVersion <= supportRange.second)
						return true;
				}

				return false;

			}

			bool EventIsSupported(const novarwgt::EventRecord &ev, const novarwgt::InputVals &) const
			{
				return GeneratorIsSupported(ev.generator, ev.generatorVersion, ev.generatorConfigStr);
			}

			// accessors
			novarwgt::Generator SupportedGenerator() const			                                          { return fSupportedGenerator; }
			const std::unordered_set<std::pair<std::vector<int>, std::vector<int>>> &SupportedRanges() const  { return fSupportedRanges; }
			const std::string &RequiredConfigStr() const                                                      { return fRequiredConfigStr; }

			// operator overloads
			bool operator==(const GeneratorSupportConfig & rhs) const {
				return     fSupportedGenerator == rhs.fSupportedGenerator
				        && fSupportedRanges == rhs.fSupportedRanges
				        && fRequiredConfigStr == rhs.fRequiredConfigStr;
			}


		private:
			const novarwgt::Generator fSupportedGenerator;
			const std::unordered_set<std::pair<std::vector<int>, std::vector<int>>> fSupportedRanges;  // should be an unordered_set, but I didn't feel like writing the hash function
			const std::string fRequiredConfigStr;

	};
}

// overload the hash to support GeneratorSupportConfig so that we can put them into maps and sets
namespace std
{
	template<>
	struct hash<novarwgt::GeneratorSupportConfig>
	{
		size_t operator()(const novarwgt::GeneratorSupportConfig &x) const
		{

			std::size_t hash = novarwgt::hash::Hash(int(x.SupportedGenerator()));
			for (const auto & r : x.SupportedRanges())
				novarwgt::hash::Hash(std::hash<std::pair<std::vector<int>, std::vector<int>>>{}(r), hash);
			novarwgt::hash::Hash(x.RequiredConfigStr().c_str(), hash);

			return hash;
		}
	};

	template <>
	struct hash<std::unordered_set<novarwgt::GeneratorSupportConfig>>
	{
		size_t operator()(const std::unordered_set<novarwgt::GeneratorSupportConfig> &x) const
		{
			return std::accumulate(x.begin(), x.end(), std::size_t{0},
			                       [](std::size_t hash, const novarwgt::GeneratorSupportConfig & c)
			                       {
				                       return novarwgt::hash::Hash(hash, std::hash<novarwgt::GeneratorSupportConfig>{}(c));
			                       });
		}
	};
} // namespace std

namespace novarwgt
{
	/// Special predefined generator support configs.  Use StoredGeneratorSupportConfig() to retrieve one.
	enum class GenCfg
	{
		kGENIE_AllVersions,
		kGENIE_v2Only,
		kGENIE_BrokenEmpiricalMECOnly,
		kGENIE_Prod2Only,
		kGENIE_Prod3Only,
		kGENIE_Prod5Only,
	};

	/// Get one of the predefined generator support configs.
	/// Can't just be global constants because of the fearsome static initialization order problem.
	GeneratorSupportConfig StoredGenSupportCfg(GenCfg cfg);

} // namespace novarwgt



#endif //NOVARWGT_GENERATORSUPPORTCONFIG_H
