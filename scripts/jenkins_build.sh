#!/bin/bash

function echo_and_execute()
{
    cmd="$*"
    echo $cmd
    $cmd
}

reqd_vars=(
    BUILD_QUALIFIER
    BUILDTYPE
    GENIE_QUALIFIERS
    FLAVOR
    OS_STRING
    tag
)
for var in ${reqd_vars[*]}; do
    if [ -z "${!var}" ]; then
        echo "Missing required environment variable $var!  Cannot continue."
        exit 1
    fi
done

# set up the dependencies we need.
echo
echo "==================================================="
echo "               Doing UPS setup:                    "
echo

. /cvmfs/nova.opensciencegrid.org/externals/setup || exit 1

echo "PRODUCTS set to ${PRODUCTS}"

# cetbuildtools and cmake have no qualifiers
echo_and_execute setup cetbuildtools v7_06_04
echo_and_execute setup cmake v3_12_1

# the best way to ensure we get all the dependencies we need
# is to forge a phony UPS product with the table file we have here,
# and set that up.
echo_and_execute rm -rf ups && mkdir ups
echo_and_execute cp NOvARwgtSrc/ups/novarwgt.table ups/novarwgt_setup.table

flavor="`ups flavor -4`"

if [[ ${flavor} != *"Darwin"* ]]; then
    echo_and_execute sed -i 's/PRODUCT=.*/PRODUCT=novarwgt_setup/g' ups/novarwgt_setup.table
    echo_and_execute sed -i 's/VERSION=.*/VERSION=build/g' ups/novarwgt_setup.table
else
    # OS X doesn't play well with the -i option of sed
    sed 's/PRODUCT=.*/PRODUCT=novarwgt_setup/g' ups/novarwgt_setup.table > ups/tmp_setup.table
    sed 's/VERSION=.*/VERSION=build/g' ups/tmp_setup.table > ups/tmp2_setup.table

    mv ups/tmp2_setup.table ups/novarwgt_setup.table
    rm ups/tmp_setup.table

    # for some reason cutting out the minor revisions as above doesn't work on OSX
    flavor=${flavor::14}
fi
    
echo "UPS thinks this OS's flavor is: $flavor"

novarwgt_quals="${BUILD_QUALIFIER}:${BUILDTYPE}:${GENIE_QUALIFIERS}"
echo_and_execute ups declare novarwgt_setup build -f ${flavor} -q "${novarwgt_quals}"  -m ups/novarwgt_setup.table -z $PWD/ups -r $PWD/ups/novarwgt_build
echo_and_execute export PRODUCTS="$PWD/ups:${PRODUCTS}"
echo_and_execute setup novarwgt_setup build -q ${novarwgt_quals}
if [ $? != 0 ]; then
    echo "UPS setup failed!"
    exit 1
fi

echo
ups active

echo
echo "\$CMAKE_PREFIX_PATH = $CMAKE_PREFIX_PATH"

echo
echo "==================================================="
echo "             Preparing build area:                 "
echo

build_dir="build"  # Jenkins is already segregating workspaces by ${BUILDTYPE}
install_dir="novarwgt/${tag}"

# just in case still there from previous Jenkins run
rm -rf ${build_dir}
rm -rf novarwgt

mkdir -p ${build_dir}
mkdir -p ${install_dir}

cd ${build_dir}

test ${BUILDTYPE} = "debug" && debug_opts="-DCMAKE_BUILD_TYPE=Debug"

# UPS will alphabetize qualifiers whether you like it or not,
# so we have to create the directory name alphabetized
full_quals=$(tr ':' '\n' <<< "${novarwgt_quals}" | sort | tr '\n' '.')
full_quals=${full_quals%.}  # strip off the last period

cmake_args="cmake ../NOvARwgtSrc
 ${debug_opts}
 -DNOVARWGT_INSTALL_UPS=ON
 -DNOVARWGT_UPS_FLAVOR=${FLAVOR}
 -DNOVARWGT_UPS_OS=${OS_STRING}
 -DNOVARWGT_UPS_COMPILER=${BUILD_QUALIFIER}
 -DCMAKE_INSTALL_PREFIX=../${install_dir}"

# setup clang as the compiler for building on
# OSX, ie flavor is Darwin and append some
# instructions to the cmake arguments
if [[ ${flavor} == *"Darwin"* ]] ; then
    echo_and_execute setup clang v5_0_1
   cmake_args+=" -DCMAKE_C_COMPILER=${CLANG_FQ_DIR}/bin/clang 
 -DCMAKE_CXX_COMPILER=${CLANG_FQ_DIR}/bin/clang++"

fi

echo
echo "==================================================="
echo "  executing CMake with the following commandline:  "
echo
echo $cmake_args
echo
$cmake_args
echo

echo
echo "==================================================="
echo "                 now building:                     "
echo

test  "$VERBOSE_MAKE" = "true" && verbose_arg="VERBOSE=1"
make -j4 $verbose_arg

echo
echo "==================================================="
echo "                now installing:                    "
echo

make install

unsetup novarwgt_setup
rm -rf ups

echo
echo
echo "Finished."
