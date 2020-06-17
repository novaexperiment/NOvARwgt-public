/*
 * EmpiricalMECSysts2017.h:
 *  MEC systematics from 2017 analysis.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_EMPIRICALMECSYSTS2017_H
#define NOVARWGT_EMPIRICALMECSYSTS2017_H

#include <string>

#include "NOvARwgt/rwgt/ISystKnob.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECSystsBase.h"
#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECTune2018.h"


namespace novarwgt
{

	//---------------------------------------------------------------------------
	// 2017 analysis uncertainties on MEC treatment have three components (see DocDB 18741):
	// - q0 shape
	// - nn vs. np initial state dinucleon composition
	// - neutrino energy shape dependence
	//---------------------------------------------------------------------------

	/// 2017 analysis uncertainty on the energy transfer dependence of MEC.  See DocDB 18741.
	class MECq0ShapeSyst2017: public novarwgt::ISystKnob
	{
		public:
			explicit MECq0ShapeSyst2017(const IRegisterable::ClassID<MECq0ShapeSyst2017> & clID);

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			const novarwgt::IWeightGenerator * fQELikeWgtr;
			const novarwgt::IWeightGenerator * fRESLikeWgtr;
	};

	extern const MECq0ShapeSyst2017 * kMECq0ShapeSyst2017;

	//---------------------------------------------------------------------------
	/// 2017 analysis uncertainty on the neutrino energy cross-section dependence of MEC.  See DocDB 18741.
	/// 2018 update splits into nu and antinu
	class MECEnuShapeSyst2017 : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit MECEnuShapeSyst2017(const IRegisterable::ClassID<T> & clID, ENuHelicity helicity)
				: ISystKnob(clID,
				            std::string("MECEnuShape") + (helicity == kUnspecifiedHelicity ? "" : (helicity == kNeutrino ? "Nu" : "AntiNu")),
				            {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				            {0, std::numeric_limits<double>::infinity()}),
				  fHelicity(helicity)
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:
			ENuHelicity fHelicity;

			/// this function is the "1 sigma" bound around central value weight 1
			static const TF1 sRwFn;
	};

	extern const MECEnuShapeSyst2017 * kMECEnuShapeSyst2017;
	extern const MECEnuShapeSyst2017 * kMECEnuShapeSyst2017_NuOnly;
	extern const MECEnuShapeSyst2017 * kMECEnuShapeSyst2017_NubarOnly;

	//---------------------------------------------------------------------------
	/// 2017 analysis uncertainty on the fraction of initial-state nucleon pairs
	/// that are neutron-proton for MEC interactions.  See DocDB 18741.
	/// 2018 update splits into nu and antinu versions.
	class MECInitStateNPFracSyst2017 : public novarwgt::ISystKnob
	{
		public:
			template <typename T>
			explicit MECInitStateNPFracSyst2017(const IRegisterable::ClassID<T> & clID, ENuHelicity helicity)
				: ISystKnob(clID,
				            std::string("MECInitStateNPFrac") + (helicity == kUnspecifiedHelicity ? "" : (helicity == kNeutrino ? "Nu" : "AntiNu")),
				            {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)},
				            {0, std::numeric_limits<double>::infinity()}),
				  fHelicity(helicity)
			{}

		protected:
			double CalcWeight(double sigma, const novarwgt::EventRecord &ev, const novarwgt::InputVals &otherParams={}) const override;

		private:

			ENuHelicity fHelicity;

	};

	extern const MECInitStateNPFracSyst2017 * kMECInitStateNPFracSyst2017;
	extern const MECInitStateNPFracSyst2017 * kMECInitStateNPFracSyst2017_NuOnly;
	extern const MECInitStateNPFracSyst2017 * kMECInitStateNPFracSyst2017_NubarOnly;

}

#endif //NOVARWGT_EMPIRICALMECSYSTS2017_H
