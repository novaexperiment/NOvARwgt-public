/*
 * GeneratorSupportConfig.cxx
 *
 *  Created on: Oct 29, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/util/GeneratorSupportConfig.h"

namespace novarwgt
{
	// -------------------------------------------------------------------------
	std::vector<int> DecodeGeneratorVersion(const std::string &versionString)
	{
		std::vector<int> ret;
		std::regex pattern("(\\d+)");
		auto vals_begin = std::sregex_iterator(versionString.begin(), versionString.end(), pattern);
		auto vals_end = std::sregex_iterator();

		for (auto it = vals_begin; it != vals_end; it++)
		ret.push_back(std::stoi(it->str()));

		return ret;
	}

	// -------------------------------------------------------------------------
	std::string EncodeGeneratorVersion(const std::vector<int> &verVec)
	{
		// this assumes we want periods as the separator :grimace:
		std::string ret;
		for (const auto & v : verVec)
			ret += std::to_string(v) + '.';
		if (ret.back() == '.')
			ret.pop_back();
		return ret;
	}

	// -------------------------------------------------------------------------
	std::string DecodeGeneratorID(novarwgt::Generator gen)
	{
		switch (gen)
		{
			case kGENIE:
				return "GENIE";

			default:
				return "Unknown";
		}
	}

	// -------------------------------------------------------------------------
	GeneratorSupportConfig StoredGenSupportCfg(GenCfg cfg)
	{
		switch (cfg)
		{
			// any GENIE
			case GenCfg::kGENIE_AllVersions:
				return GeneratorSupportConfig(novarwgt::kGENIE, {}, "");

			// general GENIE v2 config
			case GenCfg::kGENIE_v2Only:
				return GeneratorSupportConfig(novarwgt::kGENIE, {{{2, 0, 0}, {2, 12, 10}}}, "");

			// versions of GENIE w/ broken MEC model
			case GenCfg::kGENIE_BrokenEmpiricalMECOnly:
				return GeneratorSupportConfig(novarwgt::kGENIE, {{{2, 8, 0}, {2, 10, 6}}}, "");

			// special generator configs corresponding to NOvA production runs
			case GenCfg::kGENIE_Prod2Only:
				return GeneratorSupportConfig(novarwgt::kGENIE, {{{2, 10, 4}, {2, 10, 4}}}, "");

			case GenCfg::kGENIE_Prod3Only:
				return GeneratorSupportConfig(novarwgt::kGENIE, {{{2, 12, 2}, {2, 12, 2}}}, "");

			case GenCfg::kGENIE_Prod5Only:
				return GeneratorSupportConfig(novarwgt::kGENIE, {{ {3,0,6}, {3,0,6} }}, "N18_10j_02_11a");

			default:
				throw std::runtime_error("NOvARwgt::StoredGenSupportCfg(): unknown config " + std::to_string(int(cfg)));
		}
	}
}