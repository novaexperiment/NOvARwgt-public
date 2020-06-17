/*
 * LazyROOTObjLoader.cxx
 *  Wrapper around ROOT object loading that only loads objects when needed.
 *
 *  Created on: Apr 14, 2016
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#include <exception>
#include <memory>
#include <string>
#include <wordexp.h>

#include "TFile.h"

#ifdef USE_CETLIB
#include "cetlib/search_path.h"
#endif

#include "NOvARwgt/util/LazyROOTObjLoader.h"

namespace novarwgt
{

  std::unique_ptr<TFile> FindAndOpenFile(const std::string& filename)
  {
    std::string fn;

    bool badFilename = false;
    std::unique_ptr<TFile> f;

    // expand any environment variables, etc.
	wordexp_t p;
	  // the 0 is the set of flags passed. see: https://www.gnu.org/software/libc/manual/html_node/Calling-Wordexp.html
    if ( wordexp( filename.c_str(), &p, 0 ) != 0 || p.we_wordc != 1)
	    throw std::runtime_error( Form("novarwgt::FindAndOpenFile(): bad filename '%s'", filename.c_str()) );
	fn = *(p.we_wordv);
	for (size_t i=0; i<p.we_wordc; i++)
		wordfree(&p);

	  // only check the search path if the path is not absolute
    if (!fn.empty() && fn.compare(0, 1, "/")==0)
    {
      f = std::make_unique<TFile>(fn.c_str(), "read");
      if (f->IsZombie())
        badFilename = true;
    }
    else
    {
#ifdef USE_CETLIB
      cet::search_path sp("FW_SEARCH_PATH");
      if( !sp.find_file(filename, fn) )
        badFilename = true;
#endif
	  f = std::make_unique<TFile>(fn.c_str(), "read");
      if (f->IsZombie())
      	badFilename = true;
    }

    if (badFilename)
      throw std::runtime_error( Form("novarwgt::LookupFile(): purported file '%s' can't be located", filename.c_str()) );

    return f;
  } // FindAndOpenFile()
}  // namespace novarwgt
