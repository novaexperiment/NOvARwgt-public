/*
 * GenieSystKnob.cxx:
 *  Generic knob for use with GENIE systs.
 *
 *  Created on: Nov. 1, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/rwgt/genie/GenieSystKnob.h"
#include "GenieInternalTools.h"

namespace novarwgt
{

	//----------------------------------------------------------------------------
	// please note that we're explicitly specializing the constructor, but we don't need any other variants anyway
	template<>
	GenieSystKnob::GenieSystKnob(IRegisterable::ClassID<GenieSystKnob> &clID, novarwgt::ReweightKnob knob)
	: ISystKnob(clID,
	            novarwgt::internal::GetGenieKnobName(knob),
	            {StoredGenSupportCfg(GenCfg::kGENIE_AllVersions)},
	            {0, 10}),
	  fKnobIdx(knob), fWarnedGenieCalc(false)
	{}

	//----------------------------------------------------------------------------

	double GenieSystKnob::CalcWeight(double sigma, const novarwgt::EventRecord &ev, const InputVals &) const
	{
		// Leave weight unaltered
		if(sigma == 0)
			return 1.;

		if (ev.expectNoWeights)
			return 1.;

		if (ev.genieWeights.IsSet(fKnobIdx))
			return InterpolateStoredWgts(sigma, ev.genieWeights[fKnobIdx]);
		else
		{
#ifdef GENIE_MAJOR_VERSION
			double wgt;
			// use the stored GENIE record if one exists.  that's much faster and more accurate
			if (ev.origGenieEvt)
				wgt = novarwgt::internal::kRWCache.GetWeight(ev.origGenieEvt,
				                                             novarwgt::internal::ConvertToGenieKnob(fKnobIdx),
				                                             sigma);
			else
			{
				// if we have to make a new one, shenanigans may ensue.  but, no other choice.
				if (!fWarnedGenieCalc)
				{
					std::cerr << "Warning: no stored weights found for knob "
					          << "'" << novarwgt::internal::GetGenieKnobName(fKnobIdx) << "'"
					          << " (GENIE enum value " << std::size_t(fKnobIdx) << ").  ";
					std::cerr.flush();

					std::cerr << "Invoking GENIE to calculate requested weight!" << std::endl;
					TestIfGenIsSupported(novarwgt::kGENIE,
					                     novarwgt::internal::GetGENIEVersion(),
					                     novarwgt::internal::GetGENIETune());
					std::cerr << "(No further warnings will be issued for this knob.)" << std::endl;
				}
				auto rec = novarwgt::internal::ToGenieEvent(ev);
				wgt = novarwgt::internal::kRWCache.GetWeight(&rec,
				                                             novarwgt::internal::ConvertToGenieKnob(fKnobIdx),
				                                             sigma);
			}
			return wgt;
#else
			throw std::runtime_error("NOvARwgt::GenieSystKnob: GENIE weight for knob '" + std::to_string(fKnobIdx) + "'"
									 + "requested but stored weights not available and GENIE support was not built into NOvARwgt");
#endif
		}

	} // GenieSystKnob::CalcWeight()

	//----------------------------------------------------------------------------

	double GenieSystKnob::InterpolateStoredWgts(double sigma, const novarwgt::ReweightVals &wgts) const
	{
		double weight = 1;
		if (sigma == 0)
			return weight;

		// Linear interpolation
		double w0, w1, y0, y1;
		if(sigma >= 1              )      {w0 = sigma-1; w1 =  2-sigma; y0 = wgts.plus2sigma;  y1 = wgts.plus1sigma; }
		else if(sigma >= 0  && sigma < 1) {w0 = sigma-0; w1 =  1-sigma; y0 = wgts.plus1sigma;  y1 = 1;               }
		else if(sigma >= -1 && sigma < 0) {w0 = sigma+1; w1 =  0-sigma; y0 = 1;                y1 = wgts.minus1sigma;}
		else                /*sigma < -1*/{w0 = sigma+2; w1 = -1-sigma; y0 = wgts.minus1sigma; y1 = wgts.minus2sigma;}


		// These tests for NaN don't actually work. We'd have to disable the
		// FE_INVALID exceptions we currently have turned on.
		if(std::isnan(y0) || std::isnan(y1)){
			std::cerr << "Warning: NaN GENIE weight found for '" << this->GetName() << "', ignoring." << std::endl;
			return 1.0;
		}

		if(std::isinf(y0) || std::isinf(y1)){
			std::cerr << "Warning: Inf GENIE weight found for '" << this->GetName() << "', ignoring." << std::endl;
			return 1.0;
		}

		weight *= (w0*y0 + w1*y1);

		return weight;
	}

}
