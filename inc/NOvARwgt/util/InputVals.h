/*
 * InputVals.h
 *
 *  Created on: Nov 11, 2016
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_INPUTVALS_H_
#define NOVARWGT_INPUTVALS_H_

#include <map>
#include <string>

namespace novarwgt
{

  // makes it easier if we need to change anything.
  typedef std::map<std::string, double> InputsMapType;

  /// Wrapper around std::map<std::string, double> that yields more useful exceptions when a key isn't found
  class InputVals : public InputsMapType
  {
    public:
      // inherit base class constructors
      using InputsMapType::InputsMapType;

      // shadow the base class methods.  we'll call them in turn anyway
            InputsMapType::mapped_type & at( const InputsMapType::key_type & key );
      const InputsMapType::mapped_type & at( const InputsMapType::key_type & key ) const;

    private:
      void RethrowUnknownKey(const std::string & keyName) const;
  };

} // namespace novarwgt


#endif /* NOVARWGT_INPUTVALS_H_ */
