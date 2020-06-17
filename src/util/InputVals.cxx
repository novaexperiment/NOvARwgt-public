/*
 * InputVals.cxx
 *
 *  Created on: Nov 11, 2016
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include "NOvARwgt/util/InputVals.h"

namespace novarwgt
{

  InputsMapType::mapped_type& InputVals::at (const InputsMapType::key_type& key)
  {
    auto it = InputsMapType::find(key);
    if (it == this->end())
      this->RethrowUnknownKey(key);
    return it->second;
  }

  const InputsMapType::mapped_type& InputVals::at (const InputsMapType::key_type& key) const
  {
    const auto it = InputsMapType::find(key);
    if (it == this->end())
      this->RethrowUnknownKey(key);
    return it->second;
  }

  void InputVals::RethrowUnknownKey(const std::string & keyName) const
  {
    std::string errStr("(NOvARwgt) novarwgt::InputVals::at() : Requested key not in input values map: '");
    errStr += keyName + "'";
    throw std::out_of_range( errStr.c_str() );
  }

} // namespace novarwgt

