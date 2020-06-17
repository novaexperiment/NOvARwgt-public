### NOvARwgt -- NOvA collaboration neutrino event generator reweight tools

----------------------------------------------------------------------

  *Original author*:               J. Wolcott  <jwolcott@fnal.gov>

  *Last update to this document*:  May 2020

----------------------------------------------------------------------

This document contains notes about working on NOvARwgt.

##### 1. CODING CONVENTIONS
  In general fights about brace placement and tabbing aren't worth the bytes they take up.

  However, as much of the code has been written in one style, where it is reasonable,
  users are requested to observe the following conventions:
   * NOvARwgt general style places braces on their own line.
   * NOvARwgt general style uses tabs for indentation, and spaces for subsequent alignment.  Thus:
     ```c++
     namespace novarwgt
     {
     <TAB>class NewClass
     <TAB>{
     <TAB><TAB>void MyVeryLongFunctionSignature(double item1,
     <TAB><TAB><SPACES -  -   -   -   -  -  --->double item2) const;

     }
     ```
     etc.

     Your IDE can help you with this. :)

##### 2. GIT WORKFLOW
  The NOvARwgt workflow is roughly modeled on GitFlow
  (https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow):
  * Commits *should not* be made directly to the `master` branch (apart from where noted below).
  * New code, patches that fix bugs, etc. should in general be made on a branch forked from `develop`:
    * New code should be in a branch with name beginning `feature_`
    * Bugfix patches that require more than a single commit should be in a branch with name beginning `bugfix_`
  * Single, simple commits (to, e.g., fix a typo) can be made directly to `develop`.
    (But this practice should not be abused.)
  * If a release requires significant cleanup from `develop`,
    a release branch (`release_<version>`) can be forked from `develop`
    to allow further work on `develop` to continue.  (Changes from this
    release branch should be merged back into `develop` eventually, however.)
  * Tagged releases in the private repository will be made from `master`.
    Every commit to `master` should be a merge from another branch
    (either a release branch or `develop`) and should be associated
    with a tag.
  * Tagged releases intended for the public repository should be branches with the appropriate name:
    `release/<version>`.
    (More instructions on constructing them in "PUBLISHING RELEASES TO THE PUBLIC REPOSITORY" below.)
  
  The main NOvARwgt repository is maintained as a private GitHub repository
  because it contains work-in-progress physics results. 
  Anyone outside the NOvA collaboration with a contribution to NOvARwgt
  may submit a pull request against the public-facing repository
  (https://github.com/novaexperiment/NOvARwgt-public)
  when their work is ready to be merged into NOvARwgt,
  and it will be considered, and integrated as appropriate
  into the next public release.

  NOvA Collaborators who wish to be core developers can join the NOvA "organization"
   on GitHub, https://github.com/novaexperiment, by virtue of which they automatically
   are granted write access to the private repository.)


##### 3. MODIFYING THE CODE

  * ###### General notes
    * The package is built using CMake.  Following CMake recommended practice, all C++ code files
      that are compiled into the output library are mentioned explicitly in the relevant `CMakeLists.txt`.

      Thus, *if you add any source or header files, be sure they get included in the relevant lists
      in `src/CMakeLists.txt`*.

    * Be sure to update the version number in the following files before creating a new tagged release:
      - `CMakeLists.txt` in top directory
      - `ups/novarwgt.table`

  * ###### Working on a local copy of the NOvARwgt UPS product
    If you are editing NOvARwgt within novasoft, you will likely find it convenient to have a local copy
    of the NOvARwgt package that you can build other NOvA software against.  To do this, use the following
    recipe:
      1. Prepare a location for your local UPS database.

         Mine is in `/nova/app/users/jwolcott/ups` on the NOvA gpvm machines.

      2. Follow the build & installation instructions in `INSTALL.md`.

         You'll probably want to choose an obviously non-official "version" so that it's obvious
         if your custom installation didn't work.  Perhaps: `testing`

         Make sure you set up your normal novasoft environment beforehand (i.e., `setup_nova`).
         Also be sure to set your installation directory as a subdirectory of the install area.
         My typical invocation of `cmake` on a NOvA gpvm looks something like:
         ```shell script
           cmake /nova/app/users/jwolcott/NOvARwgt -DNOVARWGT_INSTALL_UPS=ON -DNOVARWGT_UPS_COMPILER=e17 -DNOVARWGT_UPS_FLAVOR=Linux64bit+2.6-2.12 -DNOVARWGT_UPS_OS=slf6.x86_64 -DCMAKE_INSTALL_PREFIX=/nova/app/users/jwolcott/ups/novarwgt/testing
         ```
      3. Set up your environment to use the new UPS area.

         Point `$PRODUCTS` at your local UPS database from above:
         ```shell script
            export PRODUCTS="/nova/app/users/jwolcott/ups:$PRODUCTS"
         ```

      4. Register your UPS product in your local database.

         Note that if you need to update the UPS information, you'll want to delete the `version` file
         produced in the previous run (in my case: `/nova/app/users/jwolcott/ups/novarwgt/testing.version`).

         Sample invocation:

         ```shell script
            ups declare novarwgt testing -f Linux64bit+2.6-2.12 -q "e17:debug:genie3" \
                -m /nova/app/users/jwolcott/ups/novarwgt/testing/ups/novarwgt.table \
                -z /nova/app/users/jwolcott/ups \
                -r /nova/app/users/jwolcott/ups/novarwgt/testing
         ```

         The `-f` is the flavor and the `-q` are the qualifiers; ask for release manager help if you
         don't know how to set these.  (They need to correspond to a recognized combination in the
         UPS product table file, `ups/novarwgt.table`.)

         The `-m` argument indicates where the UPS table file to use is located.  This should have been installed
         in the correct location by `make install`.

         The `-z` argument specifies the UPS database directory to use.

         The `-r` argument tells UPS what the top directory of the product for this version should be.

      5. Once your UPS product is registered, you can set it up:

         ```shell script
            unsetup novarwgt   # remove the version that comes with novasoft
            setup novarwgt testing -q debug:e17:genie3   # obviously specify your version and qualifiers here
         ```

      6. Verify that everything worked as it should have.

         The simplest way to do this is by inspecting the environment.  Check the output of `printenv | grep NOVARWGT`.

         (If you didn't get any errors about `testing` then it's also likely you succeeded.)

      At this point you should now have NOvARwgt set up to use your local copy.
      In the future, you will only need to repeat steps 3 & 5 to set it up again.
      To do development work, you should only need to do step 2, unless you need to
      add a new version or qualifiers, or you need to change the UPS table file.

      Happy hacking!
      
  * ###### Prototyping weights or systematic knobs before public deployment
  
    NOvARwgt is, by design, accessible to people other than NOvA collaborators.
    It is often desirable to maintain work-in-progress versions of `IWeightGenerator`s
    or `ISystKnob`s in a non-public location while they are being developed.
    
    It is possible to obtain such an arrangement by creating classes
    derived from `IWeightGenerator` or  `ISystKnob` in another codebase that is not public
    (e.g., NOvA's CAFAna framework).  Then, one instantiates an instance of the `GetRegisterable()`
    template for the new class.
    Typically this is done by creating an `extern` concrete instance of the class
    and loading it via `GetWeighter()` or `GetSystKnob()`.
    (A full example follows below.)
    
    **However, one essential precaution must be observed**: 
    The compiler and toolchain used to compile the new class (outside NOvARwgt) **must** be identical
    to the one used to compile the instance of NOvARwgt that is being used.
    If this precaution is not taken, it is possible for the compiler used to compile
    the new class to generate a version of the the `GetRegisterable()` template
    that is incompatible with the others inside the NOvARwgt library.
    The resulting run-time errors are almost impossible to debug.
    
    **Within CAFAna, this means that classes used with `GetRegisterable()`
    must be in .h/.cxx files compiled with the (test) release, *not* in .C macros invoked with `cafe`.**
    This is because the version of NOvARwgt loaded by the (test) release is tied to
    the same compiler & toolchain used in the release (gcc) by the UPS system.
    However, `cafe` uses ROOT's ACLiC system, which (as of ROOT 6)
    employs the Clang compiler under the hood.
    
    Here's an example of what this might look like for a GENIE 2.X weight in CAFAna:
    
    File: `CAFAna/Vars/SampleXsecWeight.h`
    ```c++
    #pragma once
    
    #include "NOvARwgt/rwgt/IWeightGenerator.h"
    namespace ana
    {
      // first, the class definition
      class SampleXsecWeight: public novarwgt::IWeightGenerator
      {
        public:
          // this constructor signature is required for the Registry
          explicit SampleXsecWeight(const novarwgt::IRegisterable::ClassID<SampleXsecWeight>& clID)
            : novarwgt::IWeightGenerator(clID,
                                         "SampleXsecWeight",
                                         {novarwgt::StoredGenSupportCfg(novarwgt::GenCfg::kGENIE_v2Only)})
          {}
      };
    
      // now, an instance of the class.
      // declared `extern` so that we can define it once, in the .cxx,
      // using GetRegisterable(),
      // rather than have it be initialized here and be subject
      // to the "static initialization order problem".
      extern const SampleXsecWeight * kSampleXsecWgtr;
    }
    ```
    File: `CAFAna/Vars/SampleXsecWeight.cxx`
    ```c++
    #include "CAFAna/Vars/SampleXsecWeight.h
    
    // needed to instantiate GetRegisterable()
    #include "NOvARwgt/util/Registry.ixx"

    namespace ana
    {
      const SampleXsecWeight * kSampleXsecWgtr = novarwgt::GetWeighter<SampleXsecWeight>();
    }
    ```

--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
**please note**: the remaining sections pertain only to work done by NOvA software maintainers;
                 general users of NOvARwgt do not need to bother with them.    
--------------------------------------------------------------------------------------
##### 4. PUBLISHING RELEASES TO THE PUBLIC REPOSITORY (NOVARWGT MAINTAINERS)
  Public release preparation has two steps: preparing a release branch and pushing them to the public repository.
  Because we wish to squash the history between public releases into a single commit,
  and we have to copy changes between two otherwise unrelated repositories,
  the procedure is a bit tedious. 
  
  1. Prepare a release branch for the release.
     (This follows the rough outlines laid out in the "Git Workflow" section above.)
     - Work in a clone of the *private* repository.  (https://github.com/novaexperiment/NOvARwgt.git)
     - Fork a new branch from the `master` branch at the tag in the private repository where the release is to be made.
       ``` 
       git checkout -b release/<external version number> <internal version number>
       ```
     - If any patches need to be backported onto this branch, cherry-pick the relevant commits now.
     - Edit the version number in `ups/novarwgt.table` and `CMakeLists.txt`.
     - Update `CHANGELOG.md`:
       - Copy the version from the last _public_ release branch (which contains only public releases):
         ```shell script
            git checkout release/<last public version>/CHANGELOG.md
         ```
       - Add the relevant entry for the release you are about to tag,
         taking account of all the changes that have happened since the preceding _public_ version.
     - Commit these three files, referencing the new version number in the commit message.
     - Push the new branch to the private repository.
         ```shell script
           git push release/<external version number>
         ```
     
  2. Replicate the release to the public repository.   
     - Ensure the public repository is set up as a non-default remote of your working copy,
       with its `master` branch tracked (only need to do this once):
       ```shell script
          git remote add public_repo https://github.com/novaexperiment/NOvARwgt-public.git
          git fetch public_repo master
          git checkout -b public/master --track public_repo/master
       ```
       This means you now have a local branch called `public/master`
       that tracks the `master` branch of the `public_repo` remote.
     - Ensure the `master` branch of the public repository is up-to-date in your copy:
       ```shell script
          git pull public/master
       ``` 
       and that you have the tag of the last public release from the public repository available:
       ```shell script
          git pull public/<previous public version tag name>
       ```
     - If you didn't just finish preparing the release branch in the private repository using the step above,
       ensure it's also up to date:
       ```shell script
         git fetch origin/release/<version>
       ``` 
     
     - Merge the private repo's branch into the public repository's master branch
       ```shell script
         git checkout public/master
         # pull in every change from the private repo, taking its word for everything
         git merge --allow-unrelated-histories --strategy recursive --strategy-option theirs origin/release/<version>
       ``` 
       
     - Now squash all of the history since the last release in the _public_ repository into a single commit:
       ```shell script
         git reset --soft public/<previous public version tag name>  # rolls this branch's pointer back to the last release, so anything different looks new
         git commit -m "<new version tag name>"
         git tag <new version tag name>
         git push  # pushes this branch
         git push public_repo <new version tag name>  # pushes the tag.  DO NOT use `git push --tags`; that pushes ALL tags you have, which could make a huge mess
       ```
       
     - You probably want to check out a different branch so you don't accidentally commit anything further to the public repository. :)

##### 5. UPDATING THE UPS PRODUCT IN NOVASOFT (NOVA RELEASE MANAGERS)

  NOvARwgt packages are designed to be created and installed with a minimum of hassle.
  Once a new version is tagged in the private GitHub (see "Git Workflow", above):

  1. Go to the `novarwgt_build` Jenkins page
     (requires FNAL VPN and being registered on Jenkins, but these are normal release manager requirements):
     https://buildmaster.fnal.gov/buildmaster/job/external/job/novarwgt_build/
     
  2. Click "Build With Parameters" on the left side
  
  3. Enter the tag string (e.g.: `v00.17`) into the box and click "Build"
  
  4. Wait until the build finishes (watch the progress bar on the left, or come back later).
  
  5. Navigate to the `novarwgt_collect` Jenkins page:
     https://buildmaster.fnal.gov/buildmaster/job/external/job/novarwgt_collect/
     
     This Jenkins project is auto-triggered by a successful completion of `novarwgt_build`.
     It serves to collect the various builds together into a single tarball for easy upload to UPS.
     
     Make sure the last item in the "build history" on the left is a successful build
     around the time that the build finished.
     
   6. Click the "Workspace" link in the middle.
      Copy the link to "(all files in zip)" in the middle of the page;
      this is a link to a zipfile with all the build files organized inside
      a subdirectory named after the release. 
      We'll use this in the next step.
   
   7.  Log into the oasis CVMFS machine used for publishing to CVMFS:
      ```ssh cvmfsnova@oasiscfs.fnal.gov```
   
   8. Ensure there are no other transactions in progress:
      ```nova_cvmfs_check```
   
   9. Set up the environment, open a transaction:
      ```shell script
      $ source /cvmfs/nova.opensciencegrid.org/externals/setup
      $ setup upd
      $ nova_cvmfs_transaction
      ```
   
   10. Use the convenience installer script from a _previous_ version of the UPS product
       with the link you saved above and the CVMFS area as a target.  For example:
       
       ```
       cvmfs_dir=/cvmfs/nova.opensciencegrid.org/externals
       bash $cvmfs_dir/novarwgt/v00.16/scripts/install_and_declare.sh 'https://buildmaster.fnal.gov/buildmaster/job/external/job/novarwgt_collect/ws/*zip*/novarwgt_collect.zip' $cvmfs_dir
       ```
       As of this writing, you should get the following success message when it finishes:
       ```
       Successfully declared 4 instances of novarwgt.
       ```
       or however many instances you were expecting. If you don't get this message, read back up and look for errors in the
       preceding output.
       
   11. Publish the transaction:
      ```nova_cvmfs_publish```
      
   12. Notify the relevant people, as well as whoever asked for the upgrade. The best way to do this is by posting in the #novarwgt channel on Slack. 
      
  Once the new version is available, you will likely want to update the externals for development to pull it in by default.
  Edit `setup/nova-offline-ups-externals-development` and `setup/nova-offline-ups-externals-development-prof`
  to change the version and/or qualifiers as necessary.
  
##### 6. MODIFYING THE JENKINS CONFIGURATION (NOVA RELEASE MANAGERS)

  The Jenkins configuration (used to build official NOvA versions of NOvARwgt
  for publication to UPS, as discussed in the preceding section)
  generally only needs to be modified when the operating system, compiler/ABI stack,
  or external dependency changes (and requires new qualifiers).
  
  The configuration for the project can be viewed by navigating to the Jenkins page
  for the `novarwgt_build` project (URL in the previous section) and clicking the "Configure"
  link to the left.
  
  **Warning: be careful not to _accidentally_ modify the configuration.
    If you can view it, you can also change it.**

  Most of the configuration has been assembled through trial and error and by inspecting
  the configuration for other Jenkins projects on the server.
  (Unfortunately, good documentation for the Jenkins plugins is sometimes hard to find.)
  
  If you need to update, add, or remove build configurations, you will likely only need
  to edit the **Configuration matrix** section.  This section enumerates the various values
  for each parameter passed into the build system when the parameter has multiple values
  that are not specified by the user when the "Build with Parameters" link is chosen.
  (Values chosen directly by the user are specified closer to the top of the page
  in the **General** section.)
  
  The basic strategy is to have one field for each parameter which lists all of the available
  options, and then to filter them so that only valid combinations actually produce a build.
  This latter behavior is achieved with the "Combination filter" at the bottom of the section.
  (Tip: though the syntax is relatively self-explanatory, because it's so verbose,
  I find it helpful to copy-paste the full filter expression into another editor
  where I can reorganize the spacing to make editing easier, and only copy-paste it back
  into the Jenkins page when I'm done.) 
       
  So for instance, to add support for a new compiler qualifier `e17` to the SLF6 build
  with GENIE3-only support without modifying the other choices:
  1. Add `e17` to the `BUILD_QUALIFIER` axis options (separated by a space from the other(s))
  2. Edit the combination filter to now read:
  ```
       ((OS=="SLF6") && (OS_STRING=="slf6.x86_64") && (FLAVOR=="Linux64bit+2.6-2.12") && (BUILD_QUALIFIER == "e15"))
    || ((OS=="SLF6") && (OS_STRING=="slf6.x86_64") && (FLAVOR=="Linux64bit+2.6-2.12") && (BUILD_QUALIFIER == "e17") && (GENIE_QUALIFIERS == "genie3"))
    || ((OS=="SLF7") && (OS_STRING=="slf7.x86_64") && (FLAVOR=="Linux64bit+3.10-2.17") && (BUILD_QUALIFIER == "e15") && (GENIE_QUALIFIERS == "genie3"))
    || ((OS=="OSX-10.13") && (OS_STRING=="d17.x86_64") && (FLAVOR=="Darwin64bit+17") && (BUILD_QUALIFIER == "c2"))
  ```
  3. Click the *Save* button.  (Jenkins does some sanity checking of the filter while saving,
     so if you make a mistake, you'll get an error message and you can go back and fix it.)
     
  **Please note that this configuration change also requires alterations to the `ups/novarwgt.table`
  file which are not discussed here**
     
  After modifying the available options, you should see them reflected
  in the configuration matrix display.  Due to the combination filter,
  not all of the options shown will be activated when a build is initiated.
  Be sure to verify when building that the options you expected were actually run,
  and if not, go back and edit the filter string.
  
