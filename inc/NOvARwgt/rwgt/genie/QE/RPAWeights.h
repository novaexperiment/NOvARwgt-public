/*
 * RPAWeights.h:
 *  Reweights related to RPA effects.
 *
 *  Created on: Oct. 17, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_RPAWEIGHTS_H
#define NOVARWGT_RPAWEIGHTS_H

#include <string>

#include "TH1.h"
#include "TH2.h"

#include "NOvARwgt/util/HistWrapper.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/rwgt/IWeightGenerator.h"

namespace novarwgt
{
	// forward declarations
	class InputVals;

	/// Base class for all RPA weighters to share the checks on the reaction type etc.
	class IRPAWeightBase
	{
		public:
			IRPAWeightBase(novarwgt::CurrentType curr = novarwgt::kRxnUnspecified,
			               novarwgt::ReactionType rxn = novarwgt::kScNull,
			               bool applyToHydrogen = false)
			  : fCurrent(curr), fReaction(rxn), fApplyToHydrogen(applyToHydrogen)
			{}

			bool OkReaction(const novarwgt::EventRecord& ev, const InputVals& vals) const;

		private:
			novarwgt::CurrentType fCurrent;    ///< apply only to reactions via this current (if novarwgt::kUnspecified, apply to all)
			novarwgt::ReactionType fReaction;  ///< apply only to this reaction (if novarwgt::kScNull, apply to all)
			bool fApplyToHydrogen;             ///< recreate previous buggy behavior and apply correction to scattering from H?

	};

	//---------------------------------------------------------------------------------
	/// Base class for 2D RPA weights in (q0,q3)
	class IRPAq0q3Weight : public novarwgt::IRPAWeightBase, public novarwgt::IWeightGenerator
	{
		public:
			template <typename T>
			explicit IRPAq0q3Weight(const IRegisterable::ClassID<T> & clID,
			                        const std::string & wgtrName,
			                        const std::unordered_set<novarwgt::GeneratorSupportConfig> & support,
			                        std::string histFile,
			                        std::string nuHistName,
			                        std::string nubarHistName,
			                        novarwgt::CurrentType curr = novarwgt::kRxnUnspecified,
			                        novarwgt::ReactionType rxn = novarwgt::kScNull,
			                        bool applyToHydrogen = false,
			                        bool forceNu = false)
				: novarwgt::IRPAWeightBase(curr, rxn, applyToHydrogen),
				  novarwgt::IWeightGenerator(clID, wgtrName, support),
				  fHist_nu(histFile, nuHistName),
				  fHist_nubar(histFile, nubarHistName),
				  fq0MinBin_nu(-1), fq0MinBin_nubar(-1),
				  fForceNu(forceNu)
			{}

			virtual ~IRPAq0q3Weight() = default;

			/// Draws the weight from the histogram
			double CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &params) const override;

		private:
			const novarwgt::HistWrapper <TH2> fHist_nu;
			const novarwgt::HistWrapper <TH2> fHist_nubar;

			mutable int fq0MinBin_nu;     ///< older (SA) RPA histograms have a minimum q0 below which the histogram is empty.  cache to speed things up
			mutable int fq0MinBin_nubar;  ///< older (SA) RPA histograms have a minimum q0 below which the histogram is empty.  cache to speed things up

			bool fForceNu;                 ///< reproduce old buggy behavior where the neutrino correction was used for antinus?
	};

	//----------------------------------------------------------------------

	/// Reweighter which returns the RPA weight
	/// discussed in arXiv:1307.8105.
	/// Applied only for true CCQE.
	/// This version computed at Enu = 2 GeV.
	class RPAWeightCCQESA : public IRPAq0q3Weight
	{
		public:
			template <typename T>
			explicit RPAWeightCCQESA(const IRegisterable::ClassID<T> & clID)
				: IRPAq0q3Weight(clID,
				                 "RPA_CVSA", {StoredGenSupportCfg(GenCfg::kGENIE_Prod2Only)},
				                 "$NOVARWGT_DATA/ratioNievesRPA-Carbon2GeV-20150911.root",
				                 "hrelratio_nu", "hrelratio_anu",
				                 novarwgt::kRxnCC, novarwgt::kScQuasiElastic)
			{}
	};

	extern const RPAWeightCCQESA * kRPAWeightCCQESA;

	//----------------------------------------------------------------------

	/// \brief Reweighter yielding the 2017 RPA weights for CCQE.
	///
	/// There are five 2017 RPA CCQE weights available, obtained by passing the following strings to the constructor:
	///  * "CV": the central value
	///  * "supp_plus", "supp_minus": +/- 1sigma variations coming from variations in the parameters that result in suppression at low Q^2
	///  * "enh_plus", "enh_minus": +/- 1sigma variations coming from variations in the parameters that result in enhancement at higher Q^2
	/// Note: The 'applyToHydrogen', 'forceNu', and 'forceGENIE210' parameters exist in order to recreate previous buggy behaviors
	///       of the weights.  (This enables us to avoid changing things underneath people's feet.)
	///       You should NOT use them unless you know what you're doing.
	class RPAWeightCCQE_2017 : public novarwgt::IRPAq0q3Weight
	{
		public:
			template <typename T>
			explicit RPAWeightCCQE_2017(const IRegisterable::ClassID<T> & clID,
			                            std::string histName,
			                            novarwgt::ReactionType rxn = novarwgt::kScQuasiElastic,
			                            bool applyToHydrogen = false,
			                            bool forceNu = false,
			                            bool forceGENIE210 = false)
				: IRPAq0q3Weight(clID,
				                 "RPA_CCQE_CV2017", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				                 std::string("$NOVARWGT_DATA/RPA2017.GENIE") + (forceGENIE210 ? "2-10" : "2-12") + ".root",
				                 "RPA_" + histName + "_nu",
				                 "RPA_" + histName + "_nubar",
				                 novarwgt::kRxnCC,
				                 rxn,
				                 applyToHydrogen,
				                 forceNu)
			{}
	};

	/// Buggy 2017 version requests old version with lots of behaviors fixed later
	/// (so that the prediction doesn't suddenly change underneath people):
	///   * process==kScNull ensures it will apply to all events
	///   * forceNu==true ensures the "neutrino" histogram will be used even for antineutrinos
	///   * includeHydrogen==true additionally applies the RPA effect to Hydrogen
	///   * forceGENIE210==true uses the weights calculated for GENIE 2.10
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2017;
	/// "+1sigma" for "higher Q^2 enhancement" error band.  See CV version for explanation of other args.
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_enh_plus;
	/// "-1sigma" for "higher Q^2 enhancement" error band.  See CV version for explanation of other args.
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_enh_minus;
	/// "+1sigma" for "low Q^2 suppression" error band.  See CV version for explanation of other args.
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_supp_plus;
	/// "-1sigma" for "low Q^2 suppression" error band.  See CV version for explanation of other args.
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2017_supp_minus;

	/// @{
	/// "2018" versions of RPA-QE correctly apply only to QE
	///  and the neutrino/antineutrino differences in weights are correctly respected.
	/// Effect still mistakenly applied to hydrogen.
	/// See the class versions for more explanation of the parameters.
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2018;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_enh_plus;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_enh_minus;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_supp_plus;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_supp_minus;

	/// "RPAfix" version doesn't apply RPA to hydrogen.
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2018_RPAFix;
	/// @}

	/// @{
	/// "2019" versions of RPA-QE contain 2018 bugfixes;
	///  additionally the effect is not applied to hydrogen,
	///   and contains the fixes to correctly apply to GENIE 2.12
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2019;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_enh_plus;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_enh_minus;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_supp_plus;
	extern const RPAWeightCCQE_2017 * kRPAWeightCCQE2019_supp_minus;
	/// @}

	//----------------------------------------------------------------------

	/// \brief Reweighter yielding RPA weights as a function of Q^{2}.
	/// Generally the CCQE ones above are better for CCQE, but this verion
	/// can (in an extremely speculative fashion) be extended for use in other reactions...
	//  (note that it doesn't inherit from IRPAq0q3Weight since it's using a 1D histogram in Q2.)
	/// Note: The 'applyToHydrogen' parameter exists in order to recreate previous buggy behavior
	///       of the weight.  (This enables us to avoid changing things underneath people's feet.)
	///       You should NOT use it unless you know what you're doing.
	class RPAWeightQ2_2017
		: public novarwgt::IRPAWeightBase, public novarwgt::IWeightGenerator
	{
		public:
			template <typename T>
			explicit RPAWeightQ2_2017(const IRegisterable::ClassID<T> & clID,
			                          novarwgt::CurrentType curr = novarwgt::kRxnUnspecified,
			                          novarwgt::ReactionType rxn = novarwgt::kScNull,
			                          bool applyToHydrogen = false)
				: IRPAWeightBase(curr, rxn, applyToHydrogen),
				  IWeightGenerator(clID, "RPA_Q2_CV2017", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)}),
				  fHist_nu("$NOVARWGT_DATA/RPA2017.GENIE2-12.root", "RPA_Q2_CV_nu"),
				  fHist_nubar("$NOVARWGT_DATA/RPA2017.GENIE2-12.root", "RPA_Q2_CV_nubar")
		    {}

			virtual ~RPAWeightQ2_2017() = default;

			/// Draws the weight from the histogram
			double CalcWeight(const novarwgt::EventRecord &ev, const novarwgt::InputVals &params) const override;

		private:
			const novarwgt::HistWrapper <TH1> fHist_nu;
			const novarwgt::HistWrapper <TH1> fHist_nubar;
	};

	/// Valencia group RPA (2017 edition) applied to Resonant production rather than CCQE.
	/// For 2017-2018 analyses this was used as an uncertainty,
	/// because the Valencia calculation was done only for CCQE.
	/// Doing what we're doing is sort of reckless,
	/// but it seems to reproduce the suppression effect in pion production
	/// observed elsewhere (e.g., MiniBooNE, MINOS, MINERvA).
	/// Moreover, because it significantly improves data-MC agreement
	/// in our ND distributions, from 2018 we adopt it into the central value scheme.
	/// n.b.: Late in the 2018 analysis it was discovered that the RPA effect
	/// was incorrectly applied to Hydrogen.  includeHydrogen==true reproduces this bug.
	extern const RPAWeightQ2_2017 * kRPAWeightRES2017;

	/// same as 2017 version but the effect is not applied to hydrogen.
	extern const RPAWeightQ2_2017 * kRPAWeightRES2018_RPAFix;

	/// 2019 RPA effect for CCRES.
	/// Same as 2017/2018 version except that the effect is not applied to hydrogen
	extern const RPAWeightQ2_2017 * kRPAWeightRES2019;
}

#endif //NOVARWGT_RPAWEIGHTS_H
