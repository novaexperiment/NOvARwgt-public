/*
 * EmpiricalMECFixups.h:
 *  Some "fixes" for the GENIE Empirical MEC cross section in older versions.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_EMPIRICALMECFIXUPS_H
#define NOVARWGT_EMPIRICALMECFIXUPS_H

#include "NOvARwgt/rwgt/IWeightGenerator.h"

namespace novarwgt
{
	// forward declarations
	struct EventRecord;
	class InputVals;

	/// Weighter which fixes distribution of initial state nucleon pairs of Dytman-MEC events:
	/// generated 80%-20% nn-np; should be 20%-80%.  (See tech note at DocDB 15214 for more info.)
	/// (Note that in antineutrino mode it was done correctly, 80%-20% np-pp.)
	class DytmanMECFixItlStateWgt : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit DytmanMECFixItlStateWgt(const IRegisterable::ClassID<T>& clID)
			  : IWeightGenerator(clID, "DytmanMECFixItlState", {StoredGenSupportCfg(GenCfg::kGENIE_BrokenEmpiricalMECOnly)})
			{}

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;
	};

	extern const DytmanMECFixItlStateWgt * kDytmanMECFixItlStateWgt;

	//----------------------------------------------------------------------------

	/// Weighter which fixes wacky energy dependence of Dytman-MEC cross section (See tech note at DocDB 15214 for more info.)
	class DytmanMECFixXsecEdepWgt : public IWeightGenerator
	{
		public:
			template <typename T>
			explicit DytmanMECFixXsecEdepWgt(const IRegisterable::ClassID<T>& clID)
			  : IWeightGenerator(clID, "DytmanMECFixXsecEdep", {StoredGenSupportCfg(GenCfg::kGENIE_BrokenEmpiricalMECOnly)})
			{}

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;
	};

	extern const DytmanMECFixXsecEdepWgt * kDytmanMECFixXsecEdepWgt;

}

#endif //NOVARWGT_EMPIRICALMECFIXUPS_H
