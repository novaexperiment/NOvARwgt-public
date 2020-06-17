/*
 * GenieInternalTools.h:
 *  Internal tool for working with GENIE.
 *
 * PLEASE NOTE: These functions are *not* intended to be part of the public interface of NOvARwgt,
 *              which is why this header is not in the inc/ dir.  Internal use only!
 *
 *  Created on: Nov. 4, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_GENIEINTERNALTOOLS_H
#define NOVARWGT_GENIEINTERNALTOOLS_H

#include <memory>
#include <unordered_map>
#include <vector>

#if GENIE_MAJOR_VERSION >= 3
#include "Framework/EventGen/EventRecord.h"
#include "RwCalculators/GReWeightModel.h"
#include "RwFramework/GSystUncertainty.h"
#else
#include "EVGCore/EventRecord.h"
#include "ReWeight/GReWeightI.h"
#include "ReWeight/GSystUncertainty.h"
#endif

#include "NOvARwgt/rwgt/EventRecord.h"


// none of this is of any use if we haven't built against GENIE
#ifdef GENIE_MAJOR_VERSION

namespace novarwgt
{
	// not part of novarwgt public interface
	namespace internal
	{
		/// Convert a GENIE knob enum into a NOvA one.
		novarwgt::ReweightKnob ConvertGenieKnob(genie::rew::GSyst_t);

		/// Convert a NOvA knob enum into a GENIE one.
		genie::rew::GSyst_t    ConvertToGenieKnob(novarwgt::ReweightKnob);

		/// Wrapper that helps with caching of GENIE reweighters
		class ReweightObjWrapper
		{
			public:
				ReweightObjWrapper() = default;

				double GetWeight(const genie::EventRecord *ev, genie::rew::GSyst_t knob, double sigma=1.0) const;
			private:

#if GENIE_MAJOR_VERSION >= 3
				genie::rew::GReWeightModel* GetWeighter(genie::rew::GSyst_t knobType) const;
#else
				genie::rew::GReWeightI* GetWeighter(genie::rew::GSyst_t knobType) const;
#endif

#if GENIE_MAJOR_VERSION >= 3
				mutable std::unordered_map<genie::rew::GSyst_t, std::unique_ptr<genie::rew::GReWeightModel>> fCalcs;
#else
				mutable std::unordered_map<genie::rew::GSyst_t, std::unique_ptr<genie::rew::GReWeightI>> fCalcs;
#endif

		};

		const ReweightObjWrapper kRWCache;

		// ---------------------------------------------

		/// Single-lookup version of compiled GENIE version we were built against
		const std::vector<int> & GetGENIEVersion();

		/// Get the GENIE tune which is currently configured (gracefully falls back to "" for GENIE 2)
		std::string GetGENIETune();

		// ---------------------------------------------

		/// Utility function to convert an EventRecord back into a GENIE event.
		/// Invents some of the internals of the record as needed, so caveat emptor...
		genie::EventRecord ToGenieEvent(const novarwgt::EventRecord &rec);

		// ---------------------------------------------

		/// There's some version skew that happens between our enum
		/// and GENIE's, because of changes in different GENIE versions.
		/// This map ensures we never get out of sync,
		/// even if it's painful to keep updated. :(
		inline const std::unordered_map<genie::rew::GSyst_t, novarwgt::ReweightKnob> & KnobTranslationTable()
		{
			static const std::unordered_map<genie::rew::GSyst_t, novarwgt::ReweightKnob> table {
				{ genie::rew::kXSecTwkDial_MaNCEL, kKnob_MaNCEL },
				{ genie::rew::kXSecTwkDial_EtaNCEL, kKnob_EtaNCEL },
				{ genie::rew::kXSecTwkDial_NormCCQE, kKnob_NormCCQE },
				{ genie::rew::kXSecTwkDial_NormCCQEenu, kKnob_NormCCQEenu },
				{ genie::rew::kXSecTwkDial_MaCCQEshape, kKnob_MaCCQEshape },
				{ genie::rew::kXSecTwkDial_MaCCQE, kKnob_MaCCQE },
				{ genie::rew::kXSecTwkDial_VecFFCCQEshape, kKnob_VecFFCCQEshape },
				{ genie::rew::kXSecTwkDial_NormCCRES, kKnob_NormCCRES },
				{ genie::rew::kXSecTwkDial_MaCCRESshape, kKnob_MaCCRESshape },
				{ genie::rew::kXSecTwkDial_MvCCRESshape, kKnob_MvCCRESshape },
				{ genie::rew::kXSecTwkDial_MaCCRES, kKnob_MaCCRES },
				{ genie::rew::kXSecTwkDial_MvCCRES, kKnob_MvCCRES },
				{ genie::rew::kXSecTwkDial_NormNCRES, kKnob_NormNCRES },
				{ genie::rew::kXSecTwkDial_MaNCRESshape, kKnob_MaNCRESshape },
				{ genie::rew::kXSecTwkDial_MvNCRESshape, kKnob_MvNCRESshape },
				{ genie::rew::kXSecTwkDial_MaNCRES, kKnob_MaNCRES },
				{ genie::rew::kXSecTwkDial_MvNCRES, kKnob_MvNCRES },
				{ genie::rew::kXSecTwkDial_MaCOHpi, kKnob_MaCOHpi },
				{ genie::rew::kXSecTwkDial_R0COHpi, kKnob_R0COHpi },
				{ genie::rew::kXSecTwkDial_RvpCC1pi, kKnob_RvpCC1pi },
				{ genie::rew::kXSecTwkDial_RvpCC2pi, kKnob_RvpCC2pi },
				{ genie::rew::kXSecTwkDial_RvpNC1pi, kKnob_RvpNC1pi },
				{ genie::rew::kXSecTwkDial_RvpNC2pi, kKnob_RvpNC2pi },
				{ genie::rew::kXSecTwkDial_RvnCC1pi, kKnob_RvnCC1pi },
				{ genie::rew::kXSecTwkDial_RvnCC2pi, kKnob_RvnCC2pi },
				{ genie::rew::kXSecTwkDial_RvnNC1pi, kKnob_RvnNC1pi },
				{ genie::rew::kXSecTwkDial_RvnNC2pi, kKnob_RvnNC2pi },
				{ genie::rew::kXSecTwkDial_RvbarpCC1pi, kKnob_RvbarpCC1pi },
				{ genie::rew::kXSecTwkDial_RvbarpCC2pi, kKnob_RvbarpCC2pi },
				{ genie::rew::kXSecTwkDial_RvbarpNC1pi, kKnob_RvbarpNC1pi },
				{ genie::rew::kXSecTwkDial_RvbarpNC2pi, kKnob_RvbarpNC2pi },
				{ genie::rew::kXSecTwkDial_RvbarnCC1pi, kKnob_RvbarnCC1pi },
				{ genie::rew::kXSecTwkDial_RvbarnCC2pi, kKnob_RvbarnCC2pi },
				{ genie::rew::kXSecTwkDial_RvbarnNC1pi, kKnob_RvbarnNC1pi },
				{ genie::rew::kXSecTwkDial_RvbarnNC2pi, kKnob_RvbarnNC2pi },
				{ genie::rew::kXSecTwkDial_AhtBY, kKnob_AhtBY },
				{ genie::rew::kXSecTwkDial_BhtBY, kKnob_BhtBY },
				{ genie::rew::kXSecTwkDial_CV1uBY, kKnob_CV1uBY },
				{ genie::rew::kXSecTwkDial_CV2uBY, kKnob_CV2uBY },
				{ genie::rew::kXSecTwkDial_AhtBYshape, kKnob_AhtBYshape },
				{ genie::rew::kXSecTwkDial_BhtBYshape, kKnob_BhtBYshape },
				{ genie::rew::kXSecTwkDial_CV1uBYshape, kKnob_CV1uBYshape },
				{ genie::rew::kXSecTwkDial_CV2uBYshape, kKnob_CV2uBYshape },
				{ genie::rew::kXSecTwkDial_NormDISCC, kKnob_NormDISCC },
				{ genie::rew::kXSecTwkDial_RnubarnuCC, kKnob_RnubarnuCC },
				{ genie::rew::kXSecTwkDial_DISNuclMod, kKnob_DISNuclMod },
				{ genie::rew::kXSecTwkDial_NC, kKnob_NC },
				{ genie::rew::kHadrAGKYTwkDial_xF1pi, kKnob_AGKY_xF1pi },
				{ genie::rew::kHadrAGKYTwkDial_pT1pi, kKnob_AGKY_pT1pi },
				{ genie::rew::kHadrNuclTwkDial_FormZone, kKnob_FormZone },
				{ genie::rew::kINukeTwkDial_MFP_pi, kKnob_MFP_pi },
				{ genie::rew::kINukeTwkDial_MFP_N, kKnob_MFP_N },
				{ genie::rew::kINukeTwkDial_FrCEx_pi, kKnob_FrCEx_pi },
#if GENIE_MAJOR_VERSION < 3
				{genie::rew::kINukeTwkDial_FrElas_pi,               kKnob_FrElas_pi},
#endif
				{ genie::rew::kINukeTwkDial_FrInel_pi, kKnob_FrInel_pi },
				{ genie::rew::kINukeTwkDial_FrAbs_pi, kKnob_FrAbs_pi },
				{ genie::rew::kINukeTwkDial_FrPiProd_pi, kKnob_FrPiProd_pi },
				{ genie::rew::kINukeTwkDial_FrCEx_N, kKnob_FrCEx_N },
#if GENIE_MAJOR_VERSION < 3
				{genie::rew::kINukeTwkDial_FrElas_N,                kKnob_FrElas_N},
#endif
				{ genie::rew::kINukeTwkDial_FrInel_N, kKnob_FrInel_N },
				{ genie::rew::kINukeTwkDial_FrAbs_N, kKnob_FrAbs_N },
				{ genie::rew::kINukeTwkDial_FrPiProd_N, kKnob_FrPiProd_N },
				{ genie::rew::kSystNucl_CCQEPauliSupViaKF, kKnob_CCQEPauliSupViaKF },
				{ genie::rew::kSystNucl_CCQEMomDistroFGtoSF, kKnob_CCQEMomDistroFGtoSF },
				{ genie::rew::kRDcyTwkDial_BR1gamma, kKnob_BR1gamma },
				{ genie::rew::kRDcyTwkDial_BR1eta, kKnob_BR1eta },
				{ genie::rew::kRDcyTwkDial_Theta_Delta2Npi, kKnob_Theta_Delta2Npi },
				{ genie::rew::kXSecTwkDial_ZNormCCQE, kKnob_ZNormCCQE },
				{ genie::rew::kXSecTwkDial_ZExpA1CCQE, kKnob_ZExpA1CCQE },
				{ genie::rew::kXSecTwkDial_ZExpA2CCQE, kKnob_ZExpA2CCQE },
				{ genie::rew::kXSecTwkDial_ZExpA3CCQE, kKnob_ZExpA3CCQE },
				{ genie::rew::kXSecTwkDial_ZExpA4CCQE, kKnob_ZExpA4CCQE },
				{ genie::rew::kXSecTwkDial_AxFFCCQEshape, kKnob_AxFFCCQEshape, }
			};

			return table;
		}

	} // namespace internal
} // namespace novarwgt

#endif // ifdef GENIE_MAJOR_VERSION

// ---------------------------------------------
// this one has both GENIE-enabled and non-GENIE versions
// (the latter is for when we're using a stored weight table
//  that we got from somewhere and not explicitly building against GENIE)
namespace novarwgt
{
	namespace internal
	{
		std::string GetGenieKnobName(ReweightKnob knob);
	}
}


#endif //  ifdef NOVARWGT_GENIEINTERNALTOOLS_H
