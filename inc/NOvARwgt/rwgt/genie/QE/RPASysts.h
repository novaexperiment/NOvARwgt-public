/*
 * RPASysts.h:
 *  Systematics affecting the 'RPA' weights (GENIE global Fermi gas --> Valencia nuclear model).
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_RPASYSTS_H
#define NOVARWGT_RPASYSTS_H

#include "NOvARwgt/rwgt/ISystKnob.h"
#include "RPAWeights.h"

namespace novarwgt
{
	//----------------------------------------------------------------------
	/// Uncertainty derived from variations in QE scattering XS due to RPA suppression (see DocDB 15113).
	/// This is now deprecated as a systematic since we include the RPA correction in the central value.
	/// For better uncertainties see RPACCQESyst2017 below.
	class RPACCQESystSA : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit RPACCQESystSA(const IRegisterable::ClassID<T>& clID)
				: ISystKnob(clID, "RPA", {StoredGenSupportCfg(GenCfg::kGENIE_Prod2Only)})
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;
	};

	extern const RPACCQESystSA * kRPACCQESystSA;

	//----------------------------------------------------------------------
	/// From the Valencia group via Rik Gran (see documentation on kRPAWeightCCQE2017).
	/// You pick which one you want by passing the name of it to the constructor:
	///  (1) 'supp': deeper RPA suppression at lower Q^2
	///  (2) 'enh': stronger RPA enhancement at higher Q^2
	/// n.b.: Be sure to pass whatever central value weight these are relative to as the nomWgtr argument! (nullptr if none)
	class RPACCQEshapeSyst : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit RPACCQEshapeSyst(const IRegisterable::ClassID<T>& clID,
			                          const std::string & whichUnc,
			                          const novarwgt::IWeightGenerator * nomWgtr,
			                          const novarwgt::IWeightGenerator * wgtUp,
			                          const novarwgt::IWeightGenerator * wgtDown)
				: ISystKnob(clID,
				            "RPAShape" + whichUnc,
				            {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},  // all of the derived versions only work with the version of GENIE we built the histograms from
				            {0, std::numeric_limits<double>::infinity()},
				            {nomWgtr}),
				  fWgtrUp(wgtUp),
				  fWgtrDown(wgtDown)
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			const novarwgt::IWeightGenerator * fWgtrUp;
			const novarwgt::IWeightGenerator * fWgtrDown;
	};

	//----------------------------------------------------------------------
	/// (For explanation of the additional arguments to RPAWeightVar_q0q3()
	///  please see comments on kRPAWeightCCQE2017_NT in CAFAna/Vars/GenieWeights.h)
	/// Uncertainties in the RPA correction kRPAWeightCCQE2017 used in the central value
	class RPACCQEshapeSyst2017 : public RPACCQEshapeSyst
	{
		public:
			template <typename T>
			explicit RPACCQEshapeSyst2017(const IRegisterable::ClassID<T>& clID, const std::string & whichUnc)
				: RPACCQEshapeSyst(clID, whichUnc,
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>("CV", novarwgt::kScNull, true, true, true),
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>( whichUnc + "_plus", novarwgt::kScNull, true, true, true ),
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>( whichUnc + "_minus", novarwgt::kScNull, true, true, true ))
			{}
	};

	extern const RPACCQEshapeSyst2017 * kRPACCQEEnhSyst2017;
	extern const RPACCQEshapeSyst2017 * kRPACCQESuppSyst2017;
		
	//----------------------------------------------------------------------
	/// 2018 version to go with the 2018 CV weight
	class RPACCQEshapeSyst2018 : public RPACCQEshapeSyst
	{
		public:
			template <typename T>
			explicit RPACCQEshapeSyst2018(const IRegisterable::ClassID<T>& clID, const std::string & whichUnc)
				: RPACCQEshapeSyst(clID,
				                   whichUnc + "2018",
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>("CV", novarwgt::kScQuasiElastic, false, true, true),
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>( whichUnc + "_plus", novarwgt::kScQuasiElastic, false, true, true ),
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>( whichUnc + "_minus", novarwgt::kScQuasiElastic, false, true, true ))
			{}
	};

	extern const RPACCQEshapeSyst2018 * kRPACCQEEnhSyst2018;
	extern const RPACCQEshapeSyst2018 * kRPACCQESuppSyst2018;

	//----------------------------------------------------------------------
	/// 2019 version to go with the 2019 CV weight
	class RPACCQEshapeSyst2019 : public RPACCQEshapeSyst
	{
		public:
			template <typename T>
			explicit RPACCQEshapeSyst2019(const IRegisterable::ClassID<T>& clID, const std::string & whichUnc)
				: RPACCQEshapeSyst(clID,
				                   whichUnc + "2019",
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>("CV"),
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>( whichUnc + "_plus", novarwgt::kScQuasiElastic),
				                   GetWeighter<novarwgt::RPAWeightCCQE_2017>( whichUnc + "_minus", novarwgt::kScQuasiElastic))
			{}
	};

	extern const RPACCQEshapeSyst2019 * kRPACCQEEnhSyst2019;
	extern const RPACCQEshapeSyst2019 * kRPACCQESuppSyst2019;

	//----------------------------------------------------------------------

	/// Base class for "RPA-RES" systs.
	/// @param[in] systIsEnableEffect  The systematic corresponds to enabling the RPA-RES effect (true) or undoing it (false)
	/// @param[in] fDoExtrapKludge     CAFAna's extrapolation makes a mess of the sharp turn-off point at 1sigma; this adds a small 2sigma increase to help it along.
	class RPARESSyst : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit RPARESSyst(const IRegisterable::ClassID<T>& clID,
			                    const std::string & name,
			                    const IWeightGenerator * wgtr,
			                    bool systIsEnableEffect = false,
			                    bool extrapKludge=true)
				: ISystKnob(clID, name, {StoredGenSupportCfg(GenCfg::kGENIE_v2Only)}),
				  fWgtr(wgtr),
				  fSystIsEnableEffect(systIsEnableEffect),
				  fDoExtrapKludge(extrapKludge)
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			const novarwgt::IWeightGenerator * fWgtr;
			bool fSystIsEnableEffect;
			bool fDoExtrapKludge;
	};

	//----------------------------------------------------------------------
	/// Apply "RPA" (+LFG) correction to GENIE RES.
	class RPARESSyst2017 : public RPARESSyst
	{
		public:
			template <typename T>
			explicit RPARESSyst2017(const IRegisterable::ClassID<T>& clID)
				: RPARESSyst(clID,
				             "RPAShapeRES",
				             GetWeighter<novarwgt::RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant, true),
				             true)
			{}
	};

	extern const RPARESSyst2017 * kRPARESSyst2017;

	//----------------------------------------------------------------------
	/// Different from 2017 weight because now we turn it off instead of on as the syst
	class RPARESSyst2018 : public RPARESSyst
	{
		public:
			template <typename T>
			explicit RPARESSyst2018(const IRegisterable::ClassID<T>& clID, bool doExtrapKludge)
				: RPARESSyst(clID,
				             std::string("RPAShapeRES2018") + (doExtrapKludge ? "_ExtrapKludge" : ""),
				             GetWeighter<novarwgt::RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant, true),
				             false,
				             doExtrapKludge)
			{}
	};

	extern const RPARESSyst2018 * kRPARESSyst2018;
	extern const RPARESSyst2018 * kRPARESSyst2018_ExtrapKludge;

	//----------------------------------------------------------------------
	/// 2019 version has the "apply to hydrogen" bug fixed in the CV and takes the 'extrap kludge' for granted
	class RPARESSyst2019 : public RPARESSyst
	{
		public:
			template <typename T>
			explicit RPARESSyst2019(const IRegisterable::ClassID<T>& clID)
				: RPARESSyst(clID,
				             "RPAShapeRES2019",
				             GetWeighter<novarwgt::RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant))
			{}
	};

	extern const RPARESSyst2019 * kRPARESSyst2019;
}

#endif //NOVARWGT_RPASYSTS_H
