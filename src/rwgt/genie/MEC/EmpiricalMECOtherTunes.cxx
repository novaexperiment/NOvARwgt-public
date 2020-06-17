/*
 * EmpiricalMECOtherTunes.cxx:
 *  Weights to construct other variants of MEC besides the NOvA tunes.
 *
 *  Created on: Oct. 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/MEC/EmpiricalMECOtherTunes.h"

#include "NOvARwgt/rwgt/EventRecord.h"
#include "NOvARwgt/rwgt/IWeightGenerator.h"
#include "NOvARwgt/util/InputVals.h"
#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const EmpiricalMEC_to_Valencia_Wgt * kEmpiricalMEC_to_Valencia_Wgt = novarwgt::GetWeighter<EmpiricalMEC_to_Valencia_Wgt>();

	//----------------------------------------------------------------------


	EmpiricalMEC_to_Valencia_Wgt::EmpiricalMEC_to_Valencia_Wgt(const IRegisterable::ClassID<EmpiricalMEC_to_Valencia_Wgt>& clID)
		: IWeightGenerator(clID, "EmpiricalMEC_to_Valencia", {StoredGenSupportCfg(GenCfg::kGENIE_Prod3Only)}),  // these were made with a particular version of GENIE
		  fHist_nu_nn(DefaultNuRwgtFile(), "q0q3_nn"),
		  fHist_nu_np(DefaultNuRwgtFile(), "q0q3_np"),
		  fHist_nubar_np(DefaultNubarRwgtFile(), "q0q3_np"),
		  fHist_nubar_pp(DefaultNubarRwgtFile(), "q0q3_pp")
	{}

	//----------------------------------------------------------------------

	double EmpiricalMEC_to_Valencia_Wgt::CalcWeight(const novarwgt::EventRecord& ev, const InputVals&) const
	{
		// only Dytman-MEC!
		if (ev.reaction != novarwgt::kScMEC /*|| !params.at("EmpiricalMEC")*/)
			return 1.;

		// Valencia doesn't do NC
		if (!ev.isCC)
			return 1.;

		double qmag = ev.q.Vect().Mag();
		double q0 = ev.q.E();
		bool isAntiNu = ev.nupdg < 0;
		int struckNuclPair = ev.struckNucl;   // from GENIE: 2000000200 --> nn, 2000000201 --> np, 2000000201 --> pp

		const HistWrapper<TH2> * hist;
		if (isAntiNu)
		{
			if (struckNuclPair == 2000000200)
				throw std::runtime_error("CC MEC scattering from a nn pair in antineutrinos??");
			else if (struckNuclPair == 2000000201)
				hist = &fHist_nubar_np;
			else if (struckNuclPair == 2000000202)
				hist = &fHist_nubar_pp;
			else
				throw std::runtime_error("unrecognized MEC struck nucleon pair code: " + std::to_string(struckNuclPair));
		}
		else
		{
			if (struckNuclPair == 2000000200)
				hist = &fHist_nu_nn;
			else if (struckNuclPair == 2000000201)
				hist = &fHist_nu_np;
			else if (struckNuclPair == 2000000202)
				throw std::runtime_error("CC MEC scattering from a pp pair in neutrinos??");
			else
				throw std::runtime_error("unrecognized MEC struck nucleon pair code: " + std::to_string(struckNuclPair));
		}

		// x bins 61 and above (|q| > 1.2 GeV/c) are empty because the Valencia model doesn't go there
		return (*hist).GetValueInRange(qmag, q0, {1, 60});
	}


}