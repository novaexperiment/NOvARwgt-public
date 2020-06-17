### NOvARwgt -- NOvA collaboration neutrino event generator reweight tools

----------------------------------------------------------------------

  *Original author*:               J. Wolcott  <jwolcott@fnal.gov>
  
  *Last update to this document*:  May 2020

----------------------------------------------------------------------

This document contains instructions on how to build the NOvARwgt package.

*Contents*:
 1. Obtaining the code
 2. Prerequisites
 3. Optional components
 4. Build and installation instructions
 5. Modifications for building the UPS product

------

#### 1. OBTAINING THE CODE
  NOvARwgt's home is on GitHub: https://github.com/novaexperiment/NOvARwgt-public

  Most users will want to obtain the code by downloading a tagged release:
  https://github.com/novaexperiment/NOvARwgt-public

  Should you want to contribute to NOvARwgt, however, you'll need to fork
  or clone the repository instead.  
  Please consult `HACKING.md` for further notes on contributing.

  Some examples given in sec. 4 below.

#### 2. PREREQUISITES


  NOvARwgt is designed to be as standalone as possible, but it does have several requirements.

   1. c++ compiler with C++17 support.  (Reference installation is with g++.)

   2. CMake v3.10 or above.

   3. ROOT v6.  (https://root.cern.ch/downloading-root)

       ROOT should be installed and available to CMake.  
       (This is arranged by default both with system-wide installations
        and with installations that require sourcing `thisroot.(c)sh`.)
       
       Please note that ROOT must be built against the same C++ standard as NOvARwgt,
       so NOvARwgt won't build against ROOT installations with C++14 or lower.

#### 3. OPTIONAL COMPONENTS

   To ease user adoption, NOvARwgt also has optional support for a number of external event formats
   based on tools that are of relatively widespread distribution.
   Configure them as described below, and support will be built automatically for you:

   1. ##### GENIE v2.12+ (https://github.com/GENIE-MC/Generator)
       Assumes the source is available at $GENIE/src and compiled libraries are at $GENIE/lib.

       GENIE has its own set of dependencies that are necessary for its libraries to function:
         - ROOT (as above)
         - PYTHIA6
         - log4cpp
         - libxml2
         
       NOvARwgt can try to build these dependencies into its libraries for you if you pass `-DNOVARWGT_BUILD_GENIE_DEPS`
       when invoking `cmake`, but a much more reliable way to ensure GENIE has what it needs is to use the same
       settings for the `$LD_LIBRARY_PATH` environment variable that are normally in place when GENIE is run.
       (Be sure you use the same version of ROOT to build NOvARwgt as was used to build GENIE.)

   2. ##### FNAL `nutools` and `nusimdata`
   
      Truth events can be built from `simb::MCTruth*` and `simb::GTruth*` objects (both are needed together),
      which are the truth-carrying objects from the FNAL `nusimdata` objects in the _art_ framework.
      Since `nusimdata` has numerous dependencies of its own, the only reliable way to get everything loaded correctly
      is to use the CMake tools from the FNAL `CET` suite, which follow the UPS package dependencies around.
      If the environment variable `$NUSIMDATA_DIR` is defined, the build attempts to use `CET` to check
      whether all the dependencies are available.
      
      If `$NUSIMDATA_DIR` _is_ defined in your build environment, but you'd like to disable it anyway,
      pass `-DNOVARWGT_USE_NUSIMDATA=Off` in your cmake options.
      
      (This is a bit complex, since `CET` makes the assumption that when you invoke it, you want to use it
       to do your own build, and `NOvARwgt` is intended to be as independent as possible.  All the necessary shenanigans
       are contained in `cmake/modules/FindNuSimData.cmake` if you're curious.)
   
   3. ##### NOvA `StandardRecord`
   
      Truth events can also be built from NOvA's `caf::StandardRecord` objects.
      Support for this is in the NOvASoft code repository in the StandardRecord package
      (see `StandardRecord/NOvARwgtInterface.h` therein).  (It's located there, rather than here,
      because NOvASoft is not versioned in UPS, and it's challenging to make sure `NOvARwgt`
      is always paired with the correct version of `NOvASoft`.)


   4. ##### NUISANCE event records

      The NUISANCE comparison software (https://nuisance.hepforge.org) has an internal record format
      for which a NOvARwgt interface is available inside NUISANCE.
      Please consult its build documentation.

#### 4. BUILD AND INSTALLATION INSTRUCTIONS

   * Choose and create installation directory.  For this guide we will assume it is `/path/to/install_dir`.

   * Create a directory for building.  This is separate from the installation directory,
     and can be removed when finished.  We will call it `/path/to/build_dir`.

   * Download the NOvARwgt source into your build directory using the strategy you chose in sec. 1.
     So, for example, if you were downloading fixed release v00.03, you'd get the link from
     https://github.com/novaexperiment/NOvARwgt-public/releases, and then:
     ```bash
     cd /path/to/build_dir
     wget https://github.com/novaexperiment/NOvARwgt-public/archive/v1.0.tar.gz
     tar -xvzf novarwgt_v1.0.tar.gz
     ```

     On the other hand, if you were directly cloning the repository (maybe you want to submit a pull request?):
     ```bash
     cd /path/to/build_dir
     git clone https://github.com/novaexperiment/NOvARwgt-public.git novarwgt
     ```

     Both of these options should create a subdirectory `novarwgt` in your working directory.

   * Prepare a subdirectory for actual building.
     ```bash
     mkdir /path/to/build_dir/build
     cd /path/to/build_dir/build
     ```

   * Decide which options you want to enable.  The full list (and help for them, as well as their default values)
   is available via
     ```bash
     cmake ../novarwgt -LH | grep -B2 NOVARWGT
     ```

   * Execute the build and install
     ```bash
     cmake <any options you want to pass> -DCMAKE_INSTALL_PREFIX=/path/to/install_dir  ../novarwgt
     make
     make install
     ```

   * Profit

     You should now be able to include the NOvARwgt header files from `/path/to/install_dir/inc`
     and link against the libraries in `/path/to/install_dir/lib`.
     (Be sure to set `$LD_LIBRARY_PATH` to whatever it was during installation when you run your eventual executable
     or likely GENIE or another dependency will complain its libraries cannot be found.)

     Happy reweighting!

##### 5. MODIFICATIONS FOR BUILDING THE UPS PRODUCT

The build can be configured to produce an installation ready for deployment via the Fermilab UPS system.
To do so, the following adjustments to the instructions above are needed:

 1. Adjust install directory
    Probably you want your install directory name to contain the version number.
    e.g.: `-DCMAKE_INSTALL_PREFIX=/path/to/novarwgt/v00.00`
    
 2. Extra CMake options:
   When invoking `cmake`, some additional options are required in addition to the usual `-DCMAKE_INSTALL_PREFIX`:
    * `-DNOVARWGT_INSTALL_UPS`: requests that the files in the `ups/` directory be copied to the installation folder.
    * `-DNOVARWGT_UPS_COMPILER`: the qualifier corresponding to the compiler.  E.g.: `e17`.  (See https://cdcvs.fnal.gov/redmine/projects/cet-is-public/wiki/AboutQualifiers.) 
    * `-DNOVARWGT_UPS_FLAVOR`: qualifier specifying the kernel type.  E.g.: `Linux64bit+2.6-2.12`.
    * `-DDNOVARWGT_UPS_OS`: qualifier designating the operating system.  E.g.: `slf6.x86_64`.