/*
 * HistWrapper.h
 *
 *  Created on: Jul 24, 2017
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_HISTWRAPPER_H_
#define NOVARWGT_HISTWRAPPER_H_

#include <limits>

#include "TH1.h"
#include "TH2.h"

#include "NOvARwgt/util/LazyROOTObjLoader.h"

namespace novarwgt
{

  /// Template class which will add the type-specific getters.
  /// Explicit specializations used below since it's nice
  /// for them to be the 'same' class?
  template <typename T>
  class HistWrapper
  {};

  template <>
  class HistWrapper<TH1>: public novarwgt::LazyROOTObjLoader<TH1>
  {
    public:
      using LazyROOTObjLoader<TH1>::LazyROOTObjLoader;
      double GetValue(double val) const { return (*this)->GetBinContent((*this)->FindFixBin(val)); };
  };

  template <>
  class HistWrapper<TH2>: public novarwgt::LazyROOTObjLoader<TH2>
  {
    public:
      using novarwgt::LazyROOTObjLoader<TH2>::LazyROOTObjLoader;

      /// \brief Simple getter that pins to the range of the histogram, with optional overrides.
      ///
      /// By default, simply pins to being within the histogram bounds.
      /// If \a xBinRange or \a yBinRange are specified, they further restrict
      /// to those bin numbers.  (The special value -1 can be specified for the upper end
      /// to indicate "last non-overflow bin".)
      /// \a maxRange can also be specified to pin the return value to being within a certain range.
      double GetValueInRange(double xval, double yval,
                             std::pair<int,int> xBinRange = {1,-1},
                             std::pair<int,int> yBinRange = {1,-1},
                             const std::pair<double, double> & maxRange
                               = {-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()} ) const;
  };


} /* namespace novarwgt */

#endif /* NOVARWGT_HISTWRAPPER_H_ */
