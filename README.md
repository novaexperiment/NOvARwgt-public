### NOvARwgt -- NOvA collaboration neutrino event generator reweight tools

----------------------------------------------------------------------

  *Original author*:               J. Wolcott  <jwolcott@fnal.gov>
  
  *Last update to this document*:  May 2020

----------------------------------------------------------------------

This document briefly describes the configuration and usage of the NOvA reweighting tools contained in this package.

**For build and installation instructions, see `INSTALL.md`.**

Contents:
 1. Getting and installing NOvARWGT
 2. How to use
 3. Contributing to NOvARwgt
 4. Support


----------

##### 1. GETTING AND INSTALLING
  Please see INSTALL.md for further instructions.

##### 2. HOW TO USE

   1. PATH SETUP

      No matter how you have obtained the package, you will need to do the following:

      * include `NOvARwgt`'s `inc` directory in your C++ header path (e.g., `-I/path/to/NOvARwgt/inc`)
      * load `NOvARwgt`'s library during linking (e.g., `-L/path/to/NOvARwgt/lib -lNOvARwgt`)
      * ensure `NOvARwgt`'s library is accessible when your compiled code is run
      (e.g., add `/path/to/NOvARwgt/lib` to `$LD_LIBRARY_PATH`).
      * ensure the environment variable `$NOVARWGT_DATA` points to the `data` subdirectory of your installation
        and is available at runtime

      If you are using an installation of `NOvARwgt` in FNAL UPS activated
      via `setup novarwgt`, these paths can be simplified by using

      * `/path/to/NOvARwgt/inc` -->  `${NOVARWGT_INC}`
      * `/path/to/NOvARwgt/lib` -->  `${NOVARWGT_LIB}`

      and `$LD_LIBRARY_PATH`, `$NOVARWGT_DATA` should be set for you automatically.


   2. WRITING CODE USING NOVARWGT

       * **External users**

          Users external to the NOvA collaboration will likely be most interested in the `novarwgt::Tune` interface.
          Named tunes (which correspond to specific public analyses) can be accessed via
          ```c++
          #include "NOvARwgt/rwgt/tunes/Tunes<year of tune>.h"
          ```

          Then, either construct a `novarwgt::EventRecord` (see `inc/NOvARwgt/rwgt/EventRecord.h`) directly,
          or use one of the interfaces (see the headers in `inc/NOvARwgt/interfaces`) to convert an
          event record you already have into a `novarwgt::EventRecord`.

          Obtaining event weights is then simple:

          ```c++
          #include "NOvARwgt/interfaces/GenieInterface.h

          ...

          double GetWeight(const genie::EventRecord * genieEvt)
          {
            return novarwgt::kCVTune2018.EventWeight(novarwgt::ConvertGenieEvent(genieEvt));
          }
         
          // note that you probably want to cache the converted event record
          // rather than repeating it from above, but this is just an example
          double GetSystsWgt(const genie::EventRecord * genieEvt,
                             const std::map<std::string, double> & systPulls)
          {
            double wgt = 1.0;
            auto evt = novarwgt::ConvertGenieEvent(genieEvt);
            for (const auto & pullPair : systPulls)
            {
              if (pullPair.second == 0)
                continue;
              wgt *= novarwgt::kCVTune2018.SystKnobs().at(pullPair.first)->GetWeight(pullPair.second, evt);
            }
            return wgt;
          }
          ```

          If desired, `novarwgt::Tune` can also return a vector of the individual weights that contribute to the total
          for an individual event by calling `Tune::EventWeightComponents()` on the event (see `inc/NOvARwgt/rwgt/Tune.h`).
          This returns a vector of `novarwgt::NamedWeight` objects, each of which contains the weight itself
          as well as a short name to identify the knob.
          Similarly, the `novarwgt::Tune` contains a collections of the names of the systematic knobs it supports, which can be retrieved with `Tune::KnobNames()`.
          The knobs can then be addressed individually by name as in the code example above.

       * **NOvA users**

         NOvA users can of course use the `Tune` interface above.
         This is encouraged for the final tune used for major analyses.
         (Both `CAFAna/Vars/XsecTunes.h` and the _art_ `MCReweight` service expose these tunes,
          so check there to see if you can reuse what's already available.)

         However, there may be situations where the individual weighters or systematic knobs themselves are desired.
         The best way to obtain them is to use the `const` instances of the various types sprinkled throughout the header files.
         Please see `CAFAna/Vars/GenieWeights.h` and `CAFAna/Systs/XSecSysts.h` for  examples of how to do this.
         
         (You can also make use of the `GetWeighter()` function in `inc/NOvARwgt/rwgt/IWeightGenerator.h`,
         or the analogous `GetSystKnob()` in `inc/NOvARwgt/rwgt/ISystKnob.h`,
         which ensures there are not duplicate instances of any individual object,
         if the one you need was not instantiated.
         However, if the one you need is not available, also consider submitting a GitHub issue to request it be included in a future version.)
         
         **Please see HACKING.md for instructions on how to prototype new weights or syst knobs that are not yet ready to be deployed publicly in NOvARwgt.**

##### 3. CONTRIBUTING TO NOVARWGT
  Please see HACKING.md for discussion on how to contribute to NOvARwgt.

##### 4. SUPPORT

  If you are a user of NOvARwgt outside of the NOvA collaboration, and need help or clarification
  regarding the package, please contact the conveners of the NOvA Cross Section Tuning working group.
  As of May 2020, they are:
   - Kirk Bays <kirk.r.bays@gmail.com>
   - Aaron Mislivec <misli002@umn.edu>
  
  If you are unable to reach the conveners, the NOvA co-spokespersons can direct you to the correct
  experts.  The current co-spokespersons and their contact information are listed at
  <http://novaexperiment.fnal.gov/collaboration>.
  
  If you are a member of the NOvA collaboration, please consult with the Cross Section Tuning working group
  via the mailing list or Slack.