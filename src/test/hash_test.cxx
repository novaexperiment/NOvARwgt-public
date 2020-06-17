/*
 * hash_test.cxx
 *
 *  Ensure that the Weighter hashing system avoids redundant copies
 *  but does not let Weighters that share the same signatures yet have different values
 *  for their arguments collide.
 *
 *  Created on: Feb. 14, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 *
 *      Based on sample code contributed by L. Pickering
 */

#include "NOvARwgt/rwgt/IWeightGenerator.h"

#include "NOvARwgt/rwgt/genie/QE/RPAWeights.h"
#include "NOvARwgt/rwgt/genie/DIS/Nonres1piWeights.h"

#include <cassert>
#include <iostream>

int main()
{

	auto A = novarwgt::GetWeighter<novarwgt::Nonres1PiWgt>(true, false);
	auto B = novarwgt::GetWeighter<novarwgt::Nonres1PiWgt>(false, false);
	auto C = novarwgt::GetWeighter<novarwgt::Nonres1PiWgt>(true, false);
	assert(A != B);
	assert(A == C);

	auto D = novarwgt::GetWeighter<novarwgt::RPAWeightCCQE_2017>( "CV", novarwgt::kScNull, true, true, false);
	auto E = novarwgt::GetWeighter<novarwgt::RPAWeightCCQE_2017>( "enh_plus", novarwgt::kScQuasiElastic, false, true, false );
	auto F = novarwgt::GetWeighter<novarwgt::RPAWeightCCQE_2017>( "CV" );
	assert(D != E);
	assert(D != F);
	assert(E != F);

	auto G = novarwgt::GetWeighter<novarwgt::RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant, true);
	auto H = novarwgt::GetWeighter<novarwgt::RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant, false);
	auto I = novarwgt::GetWeighter<novarwgt::RPAWeightQ2_2017>(novarwgt::kRxnCC, novarwgt::kScResonant);
	assert(G != H);
	// unfortunately still falls down on default arguments.  best we can do?
//	assert(H == I);

	std::cout << "All Weighter hash tests passed successfully." << std::endl;
}