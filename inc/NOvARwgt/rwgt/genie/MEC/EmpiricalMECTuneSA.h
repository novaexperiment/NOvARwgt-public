/*
 * EmpiricalMECTuneSA.h:
 *  Second Analysis MEC tune.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_EMPIRICALMECTUNESA_H
#define NOVARWGT_EMPIRICALMECTUNESA_H

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECFixups.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTuneBase.h"

namespace novarwgt
{
	/// Weighter which provides the "Tufts" tune of the GENIE MEC process. (See tech note at DocDB 15214 for more info.)
	class Tufts2p2hWgtSA : public EmpiricalMECq0q3TuneWgt
	{
		public:
			template <typename T>
			explicit Tufts2p2hWgtSA(const IRegisterable::ClassID<T> & clID)
				: EmpiricalMECq0q3TuneWgt(clID,
				                          "EmpiricalMEC_CVSA", {StoredGenSupportCfg(GenCfg::kGENIE_Prod2Only)},
				                          "$NOVARWGT_DATA/tufts_q3q0_xs_correction.root", "DytmanMEC_correction_factors"),
				  fItlStateWgtr(GetWeighter<novarwgt::DytmanMECFixItlStateWgt>()),
				  fXsecEDepWgtr(GetWeighter<novarwgt::DytmanMECFixXsecEdepWgt>())
			{}

			virtual ~Tufts2p2hWgtSA() = default;

			double CalcWeight(const novarwgt::EventRecord& ev, const novarwgt::InputVals& params) const override;

		private:
			const novarwgt::DytmanMECFixItlStateWgt * fItlStateWgtr;
			const novarwgt::DytmanMECFixXsecEdepWgt * fXsecEDepWgtr;

	};

	extern const Tufts2p2hWgtSA * kTufts2p2hWgtSA;

}

#endif //NOVARWGT_EMPIRICALMECTUNESA_H
