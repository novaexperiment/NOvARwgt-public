/*
 * tests_common.h
 *
 *  Shared objects for testing.
 *
 *  Created on: Oct. 22, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_TESTS_COMMON_H
#define NOVARWGT_TESTS_COMMON_H

#include <cmath>
#include <iostream>

#include "NOvARwgt/rwgt/Tune.h"
#include "NOvARwgt/rwgt/tunes/TunesSA.h"
#include "NOvARwgt/rwgt/tunes/Tunes2018.h"


namespace novarwgt
{
	namespace test
	{
		/// first element of the pair is the sigma, second is the expected weight
		typedef std::unordered_multimap<const novarwgt::ISystKnob*, std::pair<double, double>> ExpRwgtVec;

		/// Convenience class to make checking for correct event weights simpler
		template <typename T>
		class TestEvent
		{
			public:
				// must always provide all elements
				TestEvent(const T & evt,
				          double expWgt,
				          const novarwgt::Tune * tune,
				          ExpRwgtVec expSysts = {},
				          std::unique_ptr<std::exception> expectedException = nullptr)
					: fEvent(evt), fExpectedWeight(expWgt), fExpectedSysts(std::move(expSysts)),
					  fTune(tune), fExpectedException(std::move(expectedException))
				{}

				// accessors...
				const T&               Event() const { return fEvent; }
				const novarwgt::Tune * Tune()  const { return fTune; }

				const std::exception * ExpectedException() const { return fExpectedException.get(); }
				double                 ExpectedWeight()    const { return fExpectedWeight; }
				const ExpRwgtVec &     ExpectedSysts()     const { return fExpectedSysts; }

				double CalcSyst(const novarwgt::ISystKnob * knob, double sigma, const novarwgt::InputVals & params) const
				{
					return knob->GetWeight(sigma, fEvent, params);
				}

				double CalcWeight(const novarwgt::InputVals & params) const
				{
					return fTune->EventWeight(fEvent, params);

				}

				auto WeightComponents(const novarwgt::InputVals & params) const
				{
					return fTune->EventWeightComponents(fEvent, params);
				}

				bool CorrectSysts(const novarwgt::InputVals & params, double tolerance=1e-5) const
				{
					bool ok = true;
					for (const auto & systPair : fExpectedSysts)
					{
						if (std::abs(CalcSyst(systPair.first, systPair.second.first, params)
						             - systPair.second.second) > tolerance)
						{
							ok = false;
							break;
						}
					}
					return ok;
				}

				bool CorrectWeight(const novarwgt::InputVals & params, double tolerance=1e-5) const
				{
					return std::abs(CalcWeight(params) - fExpectedWeight) <= tolerance;
				}

				bool IsExpectedException(const std::exception * exc) const
				{
					if (!exc && !fExpectedException)
						return true;
					else if (!exc || !fExpectedException)
						return false;

					return typeid(*exc) == typeid(*fExpectedException);
				}


			private:
				T fEvent;
				double fExpectedWeight;
				ExpRwgtVec fExpectedSysts;
				const novarwgt::Tune * fTune;
				std::unique_ptr<std::exception> fExpectedException;
		};


		/////////////

		/// check that all events inside the container produce the correct event weights
		template<typename T>
		bool Validate(const std::map<std::string, novarwgt::test::TestEvent<T>> & events,
			const novarwgt::InputVals & params={},
			bool showWgtCompsIfFailed=false,
			bool showWgtCompsIfSucceeded=false)
		{
			bool allOk = true;
			for (const auto & evPair : events)
			{
				const auto & evName = evPair.first;
				const auto & testEv = evPair.second;

				std::cout << "Validating event: '" << evName << "'" << std::endl;

				bool success = false;
				bool okException = false;
				novarwgt::UnsupportedGeneratorException exc("");
				try
				{
					success = testEv.CorrectWeight(params);
					success = success && testEv.CorrectSysts(params);
				}
				catch (novarwgt::UnsupportedGeneratorException & e)
				{
					if (!(okException = testEv.IsExpectedException(&e)))
						exc = e;
				}
				if (!success && !okException)
				{
					allOk = false;
					std::cerr << "Test event for weight '" << evName << "' produced INCORRECT weight or systs!" << std::endl;
					if (strlen(exc.what()))
						std::cerr << "   Got exception: " << exc.what() << std::endl;
					else
					{
						std::cerr << "   Expected weight = " << testEv.ExpectedWeight() << "; got weight = "
						          << testEv.CalcWeight(params) << std::endl;

						for (const auto & systPair : testEv.ExpectedSysts())
						{
							const novarwgt::ISystKnob * knob = systPair.first;
							double sigma = systPair.second.first;
							double expWgt = systPair.second.second;
							std::cerr << "     Expected weight for syst knob '" << knob->Name()
							          << "' at sigma = " << sigma << ": " << expWgt << ".";
							std::cerr << "    Got weight: " << testEv.CalcSyst(knob, sigma, params) << std::endl;

						}
					}

					if (showWgtCompsIfFailed && !strlen(exc.what()))
					{
						for (const auto & wgtComp : testEv.WeightComponents(params))
							std::cerr << "       component '" << wgtComp.name << "': weight = " << wgtComp.weight << std::endl;
					}
					std::cerr << std::endl;
				}
				else if (showWgtCompsIfSucceeded)
				{
					std::cerr << "Test event for weight '" << evName << "' produced CORRECT weight:" << std::endl;
					for (const auto & wgtComp : testEv.WeightComponents(params))
						std::cerr << "       component '" << wgtComp.name << "': weight = " << wgtComp.weight << std::endl;
				}
			}

			return allOk;
		}

		// ----------------------------------------------------------------

		/// Utility method that constructs and returns a set of events with their expected values.
		/// More can be easily produced using the "PrintTestEvents.C" macro in the scripts/ directory.
		std::map<std::string, novarwgt::test::TestEvent<novarwgt::EventRecord>> GetTestEvents()
		{
			novarwgt::EventRecord ev;
			test::ExpRwgtVec expWgts;

			// test each of the various pieces of the tune
			// by creating events of various reaction types.
			// these will need to be updated as new tune(s)
			// for which checking is desired become available.
			// (these events and their weights were sampled
			//  from real GENIE events and the pre-NOvARwgt
			//  implementation of the weights)
			std::map<std::string, novarwgt::test::TestEvent<novarwgt::EventRecord>> testEvents;
			
			const float nan = std::numeric_limits<float>::signaling_NaN();

			// first: CCQE from a numu on a neutron in carbon
			// weights in question: MA reweight, RPA-QE
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = 14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScQuasiElastic;
			ev.struckNucl = 2112;
			ev.A = 12;
			ev.Enu = 1.83406;
			ev.q = {0.319746, 0.449245, 0.316922, 0.216367};
			ev.y = 0.117971;
			ev.W = 0.964462;
			ev.genieWeights[novarwgt::kKnob_NormCCQE] = {0.7, 0.85, 1.2, 1.4};
			ev.genieWeights[novarwgt::kKnob_MaCCQEshape] = {0.994302, 1.00203, 0.989774, 0.972174};
			ev.genieWeights[novarwgt::kKnob_MaCCQE] = {0.677511, 0.839154, 1.23518, 1.41849};
			ev.genieWeights[novarwgt::kKnob_VecFFCCQEshape] = {0.945618, 0.973058, 1.02695, 1.05409};
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {1.13499, 1.07029, 0.931759, 0.868596};
			ev.genieWeights[novarwgt::kKnob_FrCEx_N] = {1.12422, 1.06211, 0.93789, 0.87578};
//			ev.genieWeights[novarwgt::kKnob_FrElas_N] = {0.4, 0.7, 1.3, 1.6};
			ev.genieWeights[novarwgt::kKnob_FrInel_N] = {1.23541, 1.11771, 0.882293, 0.764587};
			ev.genieWeights[novarwgt::kKnob_FrAbs_N] = {1.12759, 1.0638, 0.936204, 0.872409};
			ev.genieWeights[novarwgt::kKnob_CCQEPauliSupViaKF] = {1.00018, 1.00018, 0.998261, 0.975997};

			expWgts = {
				{novarwgt::GetSystKnobByName("MaCCQE_reduced_2018"), {-0.5, 0.976858}},
				{novarwgt::GetSystKnobByName("MaCCQE_reduced_2018"), {0.5, 1.02405}},
				{novarwgt::GetSystKnobByName("RPAShapeenh2018"), {-0.5, 0.98424}},
				{novarwgt::GetSystKnobByName("RPAShapeenh2018"), {0.5, 1.01744}},
				{novarwgt::GetSystKnobByName("RPAShapesupp2018"), {-0.5, 0.993668}},
				{novarwgt::GetSystKnobByName("RPAShapesupp2018"), {0.5, 1.00633}},
				{novarwgt::GetSystKnobByName("VecFFCCQEshape"), {-0.5, 0.986529}},
				{novarwgt::GetSystKnobByName("VecFFCCQEshape"), {0.5, 1.01347}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 1.03515}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 0.96588}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {-0.5, 1.03106}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {0.5, 0.968945}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {-0.5, 0.85}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {0.5, 1.15}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {-0.5, 1.05885}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {0.5, 0.941147}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {-0.5, 1.0319}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {0.5, 0.968102}},
				{novarwgt::GetSystKnobByName("CCQEPauliSupViaKF"), {-0.5, 1.00009}},
				{novarwgt::GetSystKnobByName("CCQEPauliSupViaKF"), {0.5, 0.999131}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-CCQE-on-C12-neutron"),
			                   std::forward_as_tuple(ev, 0.996553, &novarwgt::kCVTune2018, expWgts)
			);



			// next: now an antineutrino CCQE on hydrogen
			// (check that we don't trigger the RPA weight)
			// Please note: the original event was generated with GENIE 2,
			// and the the CCQE knob is generating ever-so-slightly different
			// weights (~0.01%) for these events in GENIE 3.
			// So far I haven't been able to track down why.
			// (no expected syst weights because I can't easily
			//   run the "RPAfix" version in the pre-NOvARwgt releases.)
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = -14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScQuasiElastic;
			ev.struckNucl = 2212;
			ev.A = 1;
			ev.Enu = 2.47766;
			ev.q = {0.423152, 0, 0, 0.092191};
			ev.y = 0.0372089;
			ev.W = 0.940281;
			ev.genieWeights[novarwgt::kKnob_MaCCQE] = {nan, nan, 1.13463, nan};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numubar-CCQE-on-H-proton"),
			                   std::forward_as_tuple(ev, 1.02693, &novarwgt::kCVTune2018_RPAfix));

			// CC MEC on C12 - np pair
			// weights:  MEC
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = 14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScMEC;
			ev.struckNucl = 2000000201;
			ev.A = 12;
			ev.Enu = 0.951661;
			ev.q = {-0.0140463, 0.209568, 0.379804, 0.333357};
			ev.y = 0.35029;
			ev.W = 1.19596;
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {0.828859, 0.940966, 1.02569, 1.03026};
			ev.genieWeights[novarwgt::kKnob_FrCEx_N] = {1.08291, 1.04145, 0.958547, 0.917094};
//			ev.genieWeights[novarwgt::kKnob_FrElas_N] = {1.2366, 1.1183, 0.881701, 0.763402};
			ev.genieWeights[novarwgt::kKnob_FrInel_N] = {1.15736, 1.07868, 0.921318, 0.842635};
			ev.genieWeights[novarwgt::kKnob_FrAbs_N] = {0.6, 0.8, 1.2, 1.4};
			expWgts = {
				{novarwgt::GetSystKnobByName("MECQ0Q3Resp2018Nu"), {-0.5, 0.5}},
				{novarwgt::GetSystKnobByName("MECQ0Q3Resp2018Nu"), {0.5, 1.18661}},
				{novarwgt::GetSystKnobByName("MECEnuShapeNu"), {-0.5, 0.852034}},
				{novarwgt::GetSystKnobByName("MECEnuShapeNu"), {0.5, 1.14797}},
				{novarwgt::GetSystKnobByName("MECInitStateNPFracNu"), {-0.5, 0.9375}},
				{novarwgt::GetSystKnobByName("MECInitStateNPFracNu"), {0.5, 1.0625}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 0.970483}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 1.01284}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {-0.5, 1.02073}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {0.5, 0.979273}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {-0.5, 1.05915}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {0.5, 0.94085}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {-0.5, 1.03934}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {0.5, 0.960659}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {-0.5, 0.9}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {0.5, 1.1}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-CCMEC-on-C12-np"),
			                   std::forward_as_tuple(ev, 0.323675, &novarwgt::kCVTune2018, expWgts));

			// CC MEC on C12 - nn pair
			// weights: MEC
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = 14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScMEC;
			ev.struckNucl = 2000000200;
			ev.A = 12;
			ev.Enu = 1.67678;
			ev.q = {0.235031, -0.246737, 0.26018, 0.215945};
			ev.y = 0.128785;
			ev.W = 1.07233;
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {1.30214, 1.15077, 0.865228, 0.749947};
			ev.genieWeights[novarwgt::kKnob_FrCEx_N] = {1.23023, 1.11228, 0.893379, 0.792419};
//			ev.genieWeights[novarwgt::kKnob_FrElas_N] = {0.492026, 0.780523, 1.15046, 1.2319};
			ev.genieWeights[novarwgt::kKnob_FrInel_N] = {1.47978, 1.22881, 0.79336, 0.608887};
			ev.genieWeights[novarwgt::kKnob_FrAbs_N] = {0.661005, 0.84067, 1.13899, 1.25765};
			expWgts = {
				{novarwgt::GetSystKnobByName("MECQ0Q3Resp2018Nu"), {-0.5, 0.895183}},
				{novarwgt::GetSystKnobByName("MECQ0Q3Resp2018Nu"), {0.5, 1.04498}},
				{novarwgt::GetSystKnobByName("MECEnuShapeNu"), {-0.5, 0.903697}},
				{novarwgt::GetSystKnobByName("MECEnuShapeNu"), {0.5, 1.0963}},
				{novarwgt::GetSystKnobByName("MECInitStateNPFracNu"), {-0.5, 1.25}},
				{novarwgt::GetSystKnobByName("MECInitStateNPFracNu"), {0.5, 0.75}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 1.07538}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 0.932614}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {-0.5, 1.05614}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {0.5, 0.946689}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {-0.5, 0.890261}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {0.5, 1.07523}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {-0.5, 1.1144}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {0.5, 0.89668}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {-0.5, 0.920335}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {0.5, 1.0695}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-CCMEC-on-C12-nn"),
			                   std::forward_as_tuple(ev, 1.54137, &novarwgt::kCVTune2018, expWgts)
			);

			// numubar CC MEC on C12
			// weights: MEC
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = -14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScMEC;
			ev.struckNucl = 2000000201;
			ev.A = 12;
			ev.Enu = 2.55814;
			ev.q = {-0.179904, 0.584783, 0.637605, 0.539736};
			ev.y = 0.210987;
			ev.W = 1.18556;
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {1.38923, 1.17343, 0.859561, 0.744842};
			ev.genieWeights[novarwgt::kKnob_FrCEx_N] = {1.23026, 1.1123, 0.893358, 0.792374};
//			ev.genieWeights[novarwgt::kKnob_FrElas_N] = {0.470707, 0.761869, 1.1851, 1.31717};
			ev.genieWeights[novarwgt::kKnob_FrInel_N] = {1.46928, 1.22418, 0.796735, 0.614388};
			ev.genieWeights[novarwgt::kKnob_FrAbs_N] = {1.19329, 1.09454, 0.909665, 0.823538};
			ev.genieWeights[novarwgt::kKnob_FrPiProd_N] = {0.6, 0.8, 1.2, 1.4};
			expWgts = {
				{novarwgt::GetSystKnobByName("MECQ0Q3Resp2018AntiNu"), {-0.5, 0.615237}},
				{novarwgt::GetSystKnobByName("MECQ0Q3Resp2018AntiNu"), {0.5, 1.54211}},
				{novarwgt::GetSystKnobByName("MECEnuShapeAntiNu"), {-0.5, 0.93239}},
				{novarwgt::GetSystKnobByName("MECEnuShapeAntiNu"), {0.5, 1.06761}},
				{novarwgt::GetSystKnobByName("MECInitStateNPFracAntiNu"), {-0.5, 0.9375}},
				{novarwgt::GetSystKnobByName("MECInitStateNPFracAntiNu"), {0.5, 1.0625}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 1.08672}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 0.929781}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {-0.5, 1.05615}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {0.5, 0.946679}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {-0.5, 0.880934}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {0.5, 1.09255}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {-0.5, 1.11209}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {0.5, 0.898368}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {-0.5, 1.04727}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {0.5, 0.954832}},
				{novarwgt::GetSystKnobByName("FrPiProd_N"), {-0.5, 0.9}},
				{novarwgt::GetSystKnobByName("FrPiProd_N"), {0.5, 1.1}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numubar-CCMEC-on-C12-np"),
			                   std::forward_as_tuple(ev, 0.84649, &novarwgt::kCVTune2018, expWgts)
			);

			// CC RES on C12
			// weights:  "RPA" on RES
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = 14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScResonant;
			ev.struckNucl = 2112;
			ev.A = 12;
			ev.Enu = 1.94074;
			ev.q = {-0.161041, -0.139551, 1.25147, 1.21158};
			ev.y = 0.62429;
			ev.W = 1.73582;
			ev.genieWeights[novarwgt::kKnob_NormCCRES] = {0.6, 0.8, 1.2, 1.4};
			ev.genieWeights[novarwgt::kKnob_MaCCRES] = {0.663081, 0.862691, 1.09207, 1.15492};
			ev.genieWeights[novarwgt::kKnob_MvCCRES] = {0.900264, 0.953315, 1.04044, 1.07521};
			ev.genieWeights[novarwgt::kKnob_MFP_pi] = {1.01217, 1.00855, 0.98623, 0.968074};
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {0.549518, 0.798902, 1.16146, 1.29252};
			ev.genieWeights[novarwgt::kKnob_FrCEx_pi] = {1.10247, 1.05123, 0.948767, 0.897533};
//			ev.genieWeights[novarwgt::kKnob_FrElas_pi] = {1.1617, 1.08085, 0.919151, 0.838302};
			ev.genieWeights[novarwgt::kKnob_FrInel_pi] = {0.2, 0.6, 1.4, 1.8};
			ev.genieWeights[novarwgt::kKnob_FrAbs_pi] = {1.30295, 1.15148, 0.848524, 0.697049};
			ev.genieWeights[novarwgt::kKnob_BR1eta] = {1.001, 1.0005, 0.999499, 0.998999};
			expWgts = {
//				{novarwgt::GetSystKnobByName("RPAShapeRES2018"), {0.5, 1.17911}},
				{novarwgt::GetSystKnobByName("RPAShapeRES2018_ExtrapKludge"), {0.5, 1.17911}},
				{novarwgt::GetSystKnobByName("MaCCRES"), {-0.5, 0.931345}},
				{novarwgt::GetSystKnobByName("MaCCRES"), {0.5, 1.04604}},
				{novarwgt::GetSystKnobByName("MvCCRES"), {-0.5, 0.976657}},
				{novarwgt::GetSystKnobByName("MvCCRES"), {0.5, 1.02022}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {-0.5, 1.00427}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {0.5, 0.993115}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 0.899451}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 1.08073}},
				{novarwgt::GetSystKnobByName("FrCEx_pi"), {-0.5, 1.02562}},
				{novarwgt::GetSystKnobByName("FrCEx_pi"), {0.5, 0.974383}},
//				{novarwgt::GetSystKnobByName("FrElas_pi"), {-0.5, 1.04042}},
//				{novarwgt::GetSystKnobByName("FrElas_pi"), {0.5, 0.959576}},
				{novarwgt::GetSystKnobByName("FrInel_pi"), {-0.5, 0.8}},
				{novarwgt::GetSystKnobByName("FrInel_pi"), {0.5, 1.2}},
				{novarwgt::GetSystKnobByName("FrAbs_pi"), {-0.5, 1.07574}},
				{novarwgt::GetSystKnobByName("FrAbs_pi"), {0.5, 0.924262}},
				{novarwgt::GetSystKnobByName("RDecBR1eta"), {-0.5, 1.00025}},
				{novarwgt::GetSystKnobByName("RDecBR1eta"), {0.5, 0.99975}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-CCRES-on-C12"),
			                   std::forward_as_tuple(ev, 0.736261, &novarwgt::kCVTune2018_RPAfix, expWgts)
			);

			// CC DIS on C12, W < 1.7
			// weights:  nonres 1pi (buggy typo version)
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = 14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScDeepInelastic;
			ev.struckNucl = 2112;
			ev.A = 12;
			ev.Enu = 2.42064;
			ev.q = {-0.659572, -0.199357, 1.47283, 1.24408};
			ev.y = 0.513948;
			ev.W = 1.45653;
			ev.npiplus = 0;
			ev.npizero = 1;
			ev.npiminus = 0;
			ev.genieWeights[novarwgt::kKnob_RvnCC1pi] = {0, 0.5, 1.5, 2};
			ev.genieWeights[novarwgt::kKnob_FormZone] = {3.01763, 2.41545, 0.195616, 0.0198126};
			ev.genieWeights[novarwgt::kKnob_MFP_pi] = {0.905247, 0.963358, 1.0252, 1.04359};
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {1.61144, 1.23425, 0.840435, 0.724764};
			ev.genieWeights[novarwgt::kKnob_FrCEx_N] = {1.12415, 1.06207, 0.937927, 0.875855};
//			ev.genieWeights[novarwgt::kKnob_FrElas_N] = {0.4, 0.7, 1.3, 1.6};
			ev.genieWeights[novarwgt::kKnob_FrInel_N] = {1.22724, 1.11362, 0.886382, 0.772765};
			ev.genieWeights[novarwgt::kKnob_FrAbs_N] = {1.15273, 1.07636, 0.923637, 0.847274};
			ev.genieWeights[novarwgt::kKnob_FrPiProd_N] = {1.00734, 1.00367, 0.996328, 0.992656};
			expWgts = {
				{novarwgt::GetSystKnobByName("DISvnCC1pi"), {-0.5, 0.75}},
				{novarwgt::GetSystKnobByName("DISvnCC1pi"), {0.5, 1.25}},
				{novarwgt::GetSystKnobByName("FormZone"), {-0.5, 1.70773}},
				{novarwgt::GetSystKnobByName("FormZone"), {0.5, 0.597808}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {-0.5, 0.981679}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {0.5, 1.0126}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 1.11712}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 0.920218}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {-0.5, 1.03104}},
				{novarwgt::GetSystKnobByName("FrCEx_N"), {0.5, 0.968964}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {-0.5, 0.85}},
//				{novarwgt::GetSystKnobByName("FrElas_N"), {0.5, 1.15}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {-0.5, 1.05681}},
				{novarwgt::GetSystKnobByName("FrInel_N"), {0.5, 0.943191}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {-0.5, 1.03818}},
				{novarwgt::GetSystKnobByName("FrAbs_N"), {0.5, 0.961819}},
				{novarwgt::GetSystKnobByName("FrPiProd_N"), {-0.5, 1.00184}},
				{novarwgt::GetSystKnobByName("FrPiProd_N"), {0.5, 0.998164}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-CCDIS-on-C12-lowW-typo"),
			                   std::forward_as_tuple(ev, 0.41, &novarwgt::kCVTune2018_RPAfix, expWgts)
			);

			// NC DIS on C12, W < 1.7
			// weights:  nonres 1pi (buggy typo version)
			ev.nupdg = 14;
			ev.isCC = false;
			ev.reaction = novarwgt::kScDeepInelastic;
			ev.struckNucl = 2112;
			ev.A = 12;
			ev.Enu = 3.84486;
			ev.q = {0.118875, 1.12273, 1.30425, 1.06469};
			ev.y = 0.276913;
			ev.W = 1.01917;
			ev.npiplus = 0;
			ev.npizero = 1;
			ev.npiminus = 0;
			ev.genieWeights[novarwgt::kKnob_RvnNC1pi] = {0, 0.5, 1.5, 2};
			ev.genieWeights[novarwgt::kKnob_FormZone] = {0.284017, 0.646455, 1.18021, 1.25048};
			ev.genieWeights[novarwgt::kKnob_MFP_pi] = {0.879739, 0.953087, 1.03255, 1.05645};
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {0.962079, 0.985608, 1.00971, 1.01671};
			expWgts = {
				{novarwgt::GetSystKnobByName("DISvnNC1pi"), {-0.5, 0.75}},
				{novarwgt::GetSystKnobByName("DISvnNC1pi"), {0.5, 1.25}},
				{novarwgt::GetSystKnobByName("FormZone"), {-0.5, 0.823227}},
				{novarwgt::GetSystKnobByName("FormZone"), {0.5, 1.09011}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {-0.5, 0.976544}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {0.5, 1.01628}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 0.992804}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 1.00486}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-NCDIS-on-C12-lowW-typo"),
			                   std::forward_as_tuple(ev, 0.41, &novarwgt::kCVTune2018_RPAfix, expWgts)
			);


			// CC DIS on C12, W > 1.7
			// weights:  DIS norm
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = 14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScDeepInelastic;
			ev.struckNucl = 2112;
			ev.A = 12;
			ev.Enu = 2.5645;
			ev.q = {0.0744993, 0.171256, 2.24654, 2.18091};
			ev.y = 0.850422;
			ev.W = 2.15673;
			ev.npiplus = 1;
			ev.npizero = 0;
			ev.npiminus = 1;
			ev.genieWeights[novarwgt::kKnob_FormZone] = {0.448824, 0.782906, 1.02948, 0.982786};
			ev.genieWeights[novarwgt::kKnob_MFP_pi] = {1.06619, 1.03955, 0.954746, 0.908189};
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {0.940204, 0.977143, 1.01553, 1.02678};
			ev.genieWeights[novarwgt::kKnob_FrCEx_pi] = {0, 0.5, 1.5, 2};
//			ev.genieWeights[novarwgt::kKnob_FrElas_pi] = {1.19437, 1.09719, 0.902814, 0.805628};
			ev.genieWeights[novarwgt::kKnob_FrInel_pi] = {1.65045, 1.32523, 0.674773, 0.349545};
			ev.genieWeights[novarwgt::kKnob_FrAbs_pi] = {1.49221, 1.2461, 0.753896, 0.507792};
			expWgts = {
				{novarwgt::GetSystKnobByName("DISvnCC2pi"), {-0.5, 0.75}},
				{novarwgt::GetSystKnobByName("DISvnCC2pi"), {0.5,  1.25}},
				{novarwgt::GetSystKnobByName("FormZone"),   {-0.5, 0.891453}},
				{novarwgt::GetSystKnobByName("FormZone"),   {0.5,  1.01474}},
				{novarwgt::GetSystKnobByName("MFP_pi"),     {-0.5, 1.01977}},
				{novarwgt::GetSystKnobByName("MFP_pi"),     {0.5,  0.977373}},
				{novarwgt::GetSystKnobByName("MFP_N"),      {-0.5, 0.988572}},
				{novarwgt::GetSystKnobByName("MFP_N"),      {0.5,  1.00777}},
				{novarwgt::GetSystKnobByName("FrCEx_pi"),   {-0.5, 0.75}},
				{novarwgt::GetSystKnobByName("FrCEx_pi"),   {0.5,  1.25}},
//				{novarwgt::GetSystKnobByName("FrElas_pi"),  {-0.5, 1.04859}},
//				{novarwgt::GetSystKnobByName("FrElas_pi"),  {0.5,  0.951407}},
				{novarwgt::GetSystKnobByName("FrInel_pi"),  {-0.5, 1.16261}},
				{novarwgt::GetSystKnobByName("FrInel_pi"),  {0.5,  0.837386}},
				{novarwgt::GetSystKnobByName("FrAbs_pi"),   {-0.5, 1.12305}},
				{novarwgt::GetSystKnobByName("FrAbs_pi"),   {0.5,  0.876948}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-CCDIS-on-C12-highW"),
			                   std::forward_as_tuple(ev, 1.1, &novarwgt::kCVTune2018, expWgts)
			);

			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 12, 2};
			ev.nupdg = 14;
			ev.isCC = false;
			ev.reaction = novarwgt::kScDeepInelastic;
			ev.struckNucl = 2212;
			ev.A = 12;
			ev.Enu = 5.62839;
			ev.q = {0.157888, 0.446962, 5.45012, 5.12195};
			ev.y = 0.91002;
			ev.W = 2.60875;
			ev.npiplus = 1;
			ev.npizero = 0;
			ev.npiminus = 1;
			ev.genieWeights[novarwgt::kKnob_AhtBY] = {0.949798, 0.975005, 1.02478, 1.04934};
			ev.genieWeights[novarwgt::kKnob_BhtBY] = {1.07452, 1.0368, 0.964127, 0.92909};
			ev.genieWeights[novarwgt::kKnob_CV1uBY] = {1.04582, 1.0224, 0.978563, 0.958027};
			ev.genieWeights[novarwgt::kKnob_CV2uBY] = {0.961063, 0.980531, 1.01947, 1.03894};
			ev.genieWeights[novarwgt::kKnob_FormZone] = {0.153553, 0.538312, 1.2679, 1.32963};
			ev.genieWeights[novarwgt::kKnob_MFP_pi] = {0.913287, 0.966558, 1.02294, 1.03964};
			ev.genieWeights[novarwgt::kKnob_MFP_N] = {0.90171, 0.961945, 1.0262, 1.04534};
			expWgts = {
				{novarwgt::GetSystKnobByName("DISvpNC2pi"), {-0.5, 0.75}},
				{novarwgt::GetSystKnobByName("DISvpNC2pi"), {0.5, 1.25}},
				{novarwgt::GetSystKnobByName("AhtBY"), {-0.5, 0.987502}},
				{novarwgt::GetSystKnobByName("AhtBY"), {0.5, 1.01239}},
				{novarwgt::GetSystKnobByName("BhtBY"), {-0.5, 1.0184}},
				{novarwgt::GetSystKnobByName("BhtBY"), {0.5, 0.982063}},
				{novarwgt::GetSystKnobByName("CV1uBY"), {-0.5, 1.0112}},
				{novarwgt::GetSystKnobByName("CV1uBY"), {0.5, 0.989282}},
				{novarwgt::GetSystKnobByName("CV2uBY"), {-0.5, 0.990266}},
				{novarwgt::GetSystKnobByName("CV2uBY"), {0.5, 1.00973}},
				{novarwgt::GetSystKnobByName("FormZone"), {-0.5, 0.769156}},
				{novarwgt::GetSystKnobByName("FormZone"), {0.5, 1.13395}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {-0.5, 0.983279}},
				{novarwgt::GetSystKnobByName("MFP_pi"), {0.5, 1.01147}},
				{novarwgt::GetSystKnobByName("MFP_N"), {-0.5, 0.980972}},
				{novarwgt::GetSystKnobByName("MFP_N"), {0.5, 1.0131}},
			};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numu-NCDIS-on-C12-highW"),
			                   std::forward_as_tuple(ev, 1.1, &novarwgt::kCVTune2018, expWgts)
			);

			// now a few events to verify that the version checking is working
			// (don't need syst knobs for these)
			ev = {};
			ev.generator = novarwgt::kGENIE;
			ev.generatorVersion = {2, 10, 4};
			ev.nupdg = 14;
			ev.isCC = true;
			ev.reaction = novarwgt::kScMEC;
			ev.struckNucl = 2000000200;
			ev.A = 12;
			ev.Enu = 1.64964;
			ev.q = {0.302988, 0.0856046, 0.172804, 0.135924};
			ev.y = 0.0823959;
			ev.W = 1.01306;
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numubar-CCMEC-on-C12-np-GENIE2104"),
			                   std::forward_as_tuple(ev, 0.190331, &novarwgt::kCVTuneSA)
			);

			ev.generatorVersion = {2, 12, 2};
			testEvents.emplace(std::piecewise_construct,
			                   std::forward_as_tuple("numubar-CCMEC-on-C12-np-wrongGENIE"),
			                   std::forward_as_tuple(ev, 2.36936, &novarwgt::kCVTuneSA,
			                                         novarwgt::test::ExpRwgtVec{},
			                                         std::make_unique<novarwgt::UnsupportedGeneratorException>(""))

			);


			return testEvents;
		}
	}
}

#endif //NOVARWGT_TESTS_COMMON_H
