/*
 * NueNumuSysts.cxx:
 *   Systs on possible nue/numu cross section differences.
 *
 *  Created on: Nov. 6, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */
#include "NOvARwgt/rwgt/generic/NueNumuSysts.h"

#include "NOvARwgt/util/Registry.ixx"

namespace novarwgt
{
	const SimpleRadiativeCorrNueXSecSyst * kSimpleRadiativeCorrNueXsecSystKnob
		= novarwgt::GetSystKnob<SimpleRadiativeCorrNueXSecSyst>(false);
	const SimpleRadiativeCorrNueXSecSyst * kSimpleRadiativeCorrNuebarXsecSystKnob
		= novarwgt::GetSystKnob<SimpleRadiativeCorrNueXSecSyst>(true);

	const SimpleSecondClassCurrentsSyst * kSimpleSecondClassCurrentsSystKnob
		= novarwgt::GetSystKnob<SimpleSecondClassCurrentsSyst>();

	//----------------------------------------------------------------------

	double SimpleRadiativeCorrNueXSecSyst::CalcWeight(double sigma, const novarwgt::EventRecord &ev,
	                                                            const novarwgt::InputVals &) const
	{
		if (!ev.isCC)
			return 1.0;
		if ( (!fIsNueBar && ev.nupdg != 12) || (fIsNueBar && ev.nupdg != -12) )
			return 1.0;

		return 1 + .02*sigma;
	}

	//----------------------------------------------------------------------

	double SimpleSecondClassCurrentsSyst::CalcWeight(double sigma, const novarwgt::EventRecord &ev,
	                                                 const InputVals &) const
	{
		if(!ev.isCC || abs(ev.nupdg) != 12)
			return 1.0;

		if(ev.nupdg < 0)
			sigma *= -1;

		return 1 + .02*sigma;
	}

}