/*
 * HistWrapper.cxx
 *
 *  Created on: Jul 24, 2017
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <iostream>

#include "NOvARwgt/util/HistWrapper.h"

namespace novarwgt
{
  double HistWrapper<TH2>::GetValueInRange(double xval, double yval,
                                           std::pair<int,int> xBinRange,
                                           std::pair<int,int> yBinRange,
                                           const std::pair<double, double> & maxrange) const
  {
    if (xBinRange.second < 0)
      xBinRange.second = (*this)->GetNbinsX();
    if (yBinRange.second < 0)
      yBinRange.second = (*this)->GetNbinsY();

    int binx = (*this)->GetXaxis()->FindFixBin(xval);
    if (binx > xBinRange.second) // overflow
      binx =  xBinRange.second;
    else if (binx < xBinRange.first)
      binx = xBinRange.first;

    int biny = (*this)->GetYaxis()->FindFixBin(yval);
    if (biny > yBinRange.second) // overflow
      biny =  yBinRange.second;
    else if (biny < yBinRange.first)
      biny = yBinRange.first;

    double val = (*this)->GetBinContent(binx, biny);
    if (val < maxrange.first)
      val = maxrange.first;
    else if (val > maxrange.second)
      val = maxrange.second;

    return val;
  }
} /* namespace novarwgt */
