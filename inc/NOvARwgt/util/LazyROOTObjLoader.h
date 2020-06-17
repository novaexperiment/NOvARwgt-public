/*
 * LazyROOTObjLoader.h
 *
 *  Created on: Apr 14, 2016
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_LAZYROOTOBJLOADER_H_
#define NOVARWGT_LAZYROOTOBJLOADER_H_

#include <algorithm>
#include <exception>
#include <memory>
#include <string>

#include "TFile.h"
#include "TString.h"
#include "TSystem.h"

namespace novarwgt
{

  /// Free function to do the filename lookup so that the CET dependency doesn't go into this header
  std::unique_ptr<TFile> FindAndOpenFile(const std::string& filename);

  /// Container that loads objects from ROOT file lazily (i.e., on access)
  /// Could be used for any type that a ROOT file contains, though primary usage is for histograms.
  template <typename ObjType>
  class LazyROOTObjLoader
  {
    public:
      LazyROOTObjLoader(std::string filename, std::string objname)
        : fObj(nullptr), fFilename(std::move(filename)), fObjname(std::move(objname))
      {
      	if (fFilename.empty())
      		abort();
      }

      // mimic the unique_ptr interface.  we're just adding a loading step in front of it
      typename std::add_lvalue_reference<ObjType>::type operator*() const;
      ObjType * operator->() const;
      ObjType * get() const;

    private:
      void _LoadObj() const;
      mutable std::unique_ptr<ObjType> fObj;

      std::string fFilename;
      std::string fObjname;
  };

  ///  -----
  ///  (templated methods must be implemented in the header)

  template <typename ObjType>
  typename std::add_lvalue_reference<ObjType>::type LazyROOTObjLoader<ObjType>::operator*() const
  {
    if (!fObj)
      _LoadObj();

    return *(fObj);
  }

  template <typename ObjType>
  ObjType * LazyROOTObjLoader<ObjType>::operator->() const
  {
    if (!fObj)
      _LoadObj();

    return fObj.get();
  }

  template <typename ObjType>
  ObjType * LazyROOTObjLoader<ObjType>::get() const
  {
    if (!fObj)
      _LoadObj();

    return fObj.get();
  }

  template <typename ObjType>
  void LazyROOTObjLoader<ObjType>::_LoadObj() const
  {
    std::unique_ptr<TFile> file;

    file = std::move(FindAndOpenFile(fFilename));

    fObj.reset(dynamic_cast<ObjType*>(file->Get(fObjname.c_str())));
    if (!fObj)
      throw std::runtime_error(
          Form(
               "LazyROOTObjLoader (NOvARwgt): Could not load object '%s' from file: %s",
               fObjname.c_str(),
               fFilename.c_str()
               )
      );
    // won't work if fObj doesn't have a SetDirectory method.
    // this could be disabled with SFINAE, but... that's too much for right now.
    fObj->SetDirectory(nullptr);  // don't want histogram to get destroyed when the file closes
  }

} /* namespace novarwgt */

#endif /* NOVARWGT_LAZYROOTOBJLOADER_H_ */
