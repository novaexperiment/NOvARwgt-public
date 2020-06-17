/*
 * EventRecord.h:
 *  NOvARwgt internal event record type.
 *
 *  Created on: Oct 15, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_EVENTRECORD_H
#define NOVARWGT_EVENTRECORD_H

#include <limits>
#include <ostream>
#include <vector>

#include "TLorentzVector.h"

#include "NOvARwgt/rwgt/genie/GenieKnobNames.h"

namespace genie
{
	class EventRecord;
}

namespace novarwgt
{
	enum Generator : unsigned short
	{
		kUnknownGenerator = 0,
		kGENIE = 1
	};

	enum CurrentType : unsigned short
	{
		kRxnUnspecified = 0,
		kRxnCC,
		kRxnNC
	};

	// copied from genie::EScatteringType
	enum ReactionType : unsigned int
	{
		kScNull = 0,
		kScQuasiElastic,
		kScSingleKaon,
		kScDeepInelastic,
		kScResonant,
		kScCoherent,
		kScDiffractive,
		kScNuElectronElastic,
		kScInverseMuDecay,
		kScAMNuGamma,
		kScMEC,
		kScCoherentElas,
		kScInverseBetaDecay,
		kScGlashowResonance,
		kScIMDAnnihilation,
		kScDarkMatterElastic = 101,
		kScDarkMatterDeepInelastic
	};

	//----------------------------------------------------------------------------
	///  Container for typical precomputed weight tables (-2, -1, +1, +2 sigma)
	/// (we use floats so that they can be directly casted from the CAF object,
	///  which uses floats, in order to speed things up)
	/// WARNING: be sure the test in StandardRecord/test still passes if you change this!
	struct ReweightVals
	{
		float minus2sigma;
		float minus1sigma;
		float plus1sigma;
		float plus2sigma;
	};

	/// Type that wraps up the list of reweights in an interface that allows you to query whether they're set
	class ReweightList
	{
		public:
			ReweightList()
			  : fKnobSet(novarwgt::kLastKnob, false)
			{}

			ReweightList(std::vector<ReweightVals> && vec)
			  : fWeights(std::move(vec)), fKnobSet(fWeights.size(), true)
			{}

			ReweightList(const ReweightList & other)
				: fWeights(other.fWeights), fKnobSet(other.fKnobSet)
			{}

			ReweightList(ReweightList && other) noexcept
				: fWeights(std::move(other.fWeights)), fKnobSet(std::move(other.fKnobSet))
			{}

			ReweightList& operator=(const ReweightList & other)
			{
				fWeights = other.fWeights;
				fKnobSet = other.fKnobSet;

				return *this;
			}

			ReweightVals &       operator[]( std::size_t pos );
			const ReweightVals & operator[]( std::size_t pos ) const;

			void clear() { fWeights.clear(); fKnobSet.clear(); }
			void resize(std::size_t size) { fWeights.resize(size); fKnobSet.resize(size); }
			std::size_t size() const { return fWeights.size(); }

			bool IsSet(std::size_t pos) const { if (pos < fKnobSet.size()) return fKnobSet[pos]; return false; }

		private:
			std::vector<ReweightVals>  fWeights;    ///<  The weights
			std::vector<bool>    fKnobSet;          ///<  Has this knob been set?

			const ReweightVals fNaNVals = ReweightVals{std::numeric_limits<float>::signaling_NaN(),
			                                           std::numeric_limits<float>::signaling_NaN(),
			                                           std::numeric_limits<float>::signaling_NaN(),
			                                           std::numeric_limits<float>::signaling_NaN()};   ///< used to return a reference-to-NaN
	};

	/// Internal event record type.
	///
	/// We use a custom type rather than re-using the one from GENIE
	/// or NUISANCE or elsewhere so that NOvARwgt can be built standalone
	/// without dependencies on them.
	///
	/// More fields can be added in future if needed for weights.
	struct EventRecord
	{
		Generator generator;
		std::vector<int> generatorVersion;
		std::string generatorConfigStr;  /// for now, this is either the GENIE 'Comprehensive Model Configuration' (a.k.a. 'tune') or nothing

		int nupdg                       = 0;
		bool isCC                       = false;
		novarwgt::ReactionType reaction = kScNull;
		int struckNucl                  = -1;

		double Enu        = std::numeric_limits<double>::signaling_NaN();  ///< in GeV
		/// Components of four-momentum transfer in GeV
		TLorentzVector q  = {std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN(), };
		double y          = std::numeric_limits<double>::signaling_NaN();  ///< Bjorken y == inelasticity
		double W          = std::numeric_limits<double>::signaling_NaN();  ///< Hadronic system invariant mass

		unsigned int A = std::numeric_limits<int>::signaling_NaN();        ///< Atomic number of struck nucleus

		int npiplus = -1;    ///< number of pi+ BEFORE FSI
		int npizero = -1;    ///< number of pi0 BEFORE FSI
		int npiminus = -1;   ///< number of pi- BEFORE FSI

		// todo: need to write some checks in the StandardRecord interface:
		//       (a) knob enum matching (since we'll be reinterpret_cast<>ing the vector)
		//       (b) verify that reweight table is being correctly converted
		novarwgt::ReweightList genieWeights;

		/// If no GENIE weights are expected for this event, set to true
		/// (useful for weight calculators depending on GENIE weights)
		bool expectNoWeights = false;

		void PrintTo(std::ostream &stream) const;

		void Reset()
		{
			// abuse (?) placement new to just write over everything with the defaults
			this->~EventRecord();
			new (this) EventRecord();
		}

		double Q2() const
		{
			if (std::isnan(this->q2))
				this->q2 = q.Mag2();
			return -this->q2;
		}

		const genie::EventRecord * origGenieEvt = nullptr;   ///< If this event was made from a GENIE event, this is it.

		private:
			mutable double q2 = std::numeric_limits<double>::signaling_NaN();
	};
}

// need to specialize std::hash<> over these guys so they don't wind up causing problems
// when we try to hash them for use in the registry stuff in IWeightGenerator.h
namespace std
{
	template <> struct hash<novarwgt::CurrentType>
	{
		size_t operator()(const novarwgt::CurrentType & x) const
		{
			return hash<int>{}(x);
		}
	};

	template <> struct hash<novarwgt::ReactionType>
	{
		size_t operator()(const novarwgt::ReactionType & x) const
		{
			return hash<int>{}(x);
		}
	};
}

#endif //NOVARWGT_EVENTRECORD_H
