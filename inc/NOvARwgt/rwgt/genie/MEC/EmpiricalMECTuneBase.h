/*
 * EmpiricalMECTuneBase.h:
 *  Base class(es) for MEC tunes.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_EMPIRICALMECTUNEBASE_H
#define NOVARWGT_EMPIRICALMECTUNEBASE_H

#include <string>
#include <unordered_set>

#include "TH2.h"

#include "NOvARwgt/util/HistWrapper.h"
#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/rwgt/IWeightGenerator.h"

namespace novarwgt
{
	// forward declarations
	class InputVals;

	/// Weighter base class to build alternative tunes for Empirical MEC.
	class EmpiricalMECq0q3TuneWgt : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit EmpiricalMECq0q3TuneWgt(const IRegisterable::ClassID<T>& clID,
			                                 std::string wgtrName,
			                                 std::unordered_set<novarwgt::GeneratorSupportConfig> support,
			                                 std::string wgtHistFile, std::string wgtHistName)
				: IWeightGenerator(clID, std::move(wgtrName), std::move(support)),
				  fHist(std::move(wgtHistFile), std::move(wgtHistName))
			{}

			/// Draws the weight from the histogram
			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;

		private:
			const HistWrapper<TH2> fHist;
	};

	/// Base class for Empirical MEC weighters that have nu and nubar variants.
	class EmpiricalMECq0q3NuNubarTuneWgt : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit EmpiricalMECq0q3NuNubarTuneWgt(const IRegisterable::ClassID<T>& clID,
			                                        const std::string & wgtrName,
			                                        const std::unordered_set<novarwgt::GeneratorSupportConfig> & support,
			                                        std::string wgtHistFileNu, std::string wgtHistNameNu,
			                                        std::string wgtHistFileNubar, std::string wgtHistNameNubar )
				: IWeightGenerator(clID, wgtrName, support),
				  fWgtrNu(GetWeighter<EmpiricalMECq0q3TuneWgt>(wgtrName + "_nu", support, std::move(wgtHistFileNu), std::move(wgtHistNameNu))),
				  fWgtrNubar(GetWeighter<EmpiricalMECq0q3TuneWgt>(wgtrName + "_nubar", support, std::move(wgtHistFileNubar), std::move(wgtHistNameNubar)))
			{}

			double CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams) const override
			{
				const auto & wgtr = (ev.nupdg > 0) ? fWgtrNu : fWgtrNubar;
				return wgtr->CalcWeight(ev, otherParams);
			};

		private:
			const EmpiricalMECq0q3TuneWgt * fWgtrNu;
			const EmpiricalMECq0q3TuneWgt * fWgtrNubar;

	};


}

#endif //NOVARWGT_EMPIRICALMECTUNEBASE_H
