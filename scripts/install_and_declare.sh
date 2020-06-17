#!/usr/bin/env bash

function usage()
{
    echo "Usage: $0 <zipfile> /path/to/install"
    echo
    echo "Installs novarwgt from a zipfile (probably from Jenkins)"
    echo "into the designated directory, and declares it to UPS."
}

if [ $# -ne 2 ]; then
    usage
    exit 1
fi

tarball="$1"
path_to_install="$2"

echo "zipfile to use: $tarball"
echo "install path: $path_to_install"

# sanity checks
if ! [[ $PRODUCTS =~ (^|:)${path_to_install}(:|$) ]]; then
    echo "ERROR: Your install area ($path_to_install) was not found in \$PRODUCTS."
    echo "       This installation will not be able to be declared to UPS."
    echo "Abort."
    exit 1
fi

if [ ! -d $path_to_install/novarwgt ]; then
    echo "I didn't find a 'novarwgt' product directory in your install dir: $path_to_install"
    echo "I'm not going to make it for you.  If this is the first installation of novarwgt, go do"
    echo "   mkdir $path_to_install/novarwgt  "
    echo "and come back and run this script again after.  I'll wait."
    echo "Otherwise, you probably passed the install directory incorrectly..."
    exit 1
fi

# work in a tempdir to be reasonably sure nothing else is going to collide with wildcards below
path_to_install=$path_to_install/novarwgt
cd $path_to_install

workdir="$path_to_install/tmp_install"
if [ -d $workdir ]; then
    echo "Working directory ($workdir) already exists."
    echo "Clean up any previous failed attempts before trying to install again..."
    exit 1
fi
mkdir $workdir
cd $workdir

# download if needed
if [[ $tarball =~ https?://.* ]]; then
    wget $tarball -O novarwgt.zip
    if [ $? != 0 ]; then
        rm -f novarwgt.zip
        echo "ERROR: download of resource $tarball failed.  Cannot continue."
        exit 1
    fi
    tarball="$PWD/novarwgt.zip"
else
    tarball="`readlink -f $tarball`"
    echo "using local zipfile: $tarball"
fi

info="$(file $tarball)"
if ! [[ $info == *"Zip archive data"* ]]; then
    echo "Unrecognized zipfile format:"
    echo "  $info"
    echo "Cannot extract."
    echo "Abort."
    exit 1
fi

# ok, let's go
echo "Attempting to install novarwgt tarball to directory: $path_to_install"
if [ $path_to_install != `dirname $tarball` ]; then
    cp $tarball .
fi

echo "Unwinding zipfile: $tarball"
echo "  (working dir: $workdir)"
unzip -q `basename $tarball`

let prod_count=0

# now declare.
# we look for a dirname with the telltale x86_64 string.
# this is possibly not future-proof but we don't have
# any examples of problems yet...
ups_declares=( )
for fullpath in `find . -type d -name '*x86_64*'`; do
    dir=`basename $fullpath`
    path=`dirname $fullpath`

    novarwgt_version="`sed -n 's/.*VERSION=\(.*\)/\1/pg' $path/ups/novarwgt.table`"
    if [ -z "$novarwgt_version" ]; then
        echo "WARNING: Can't parse novarwgt.table to find NOvARwgt version.  Skip"
        continue
    fi

    echo
    echo "deduced version: $novarwgt_version"
    ups_str="$dir"
    flavor=""
    flavor_pattern='(slf|d)([0-9]+).x86_64\.([a-z0-9.]+)'
    if [[ $ups_str =~ $flavor_pattern ]]; then
        os_str="${BASH_REMATCH[1]}"
        if [ "$os_str" = "slf" ]; then
            slf_ver="${BASH_REMATCH[2]}"
            if [ "$slf_ver" = 6 ]; then
                flavor="Linux64bit+2.6-2.12"
            elif [ "$slf_ver" = 7 ]; then
                flavor="Linux64bit+3.10-2.17"
            else
                echo "Unsupported Scientific Linux version: SLF${slf_ver}.  Skip"
                continue
            fi
        elif [ "${os_str}" = "d" ]; then
            osx_ver="${BASH_REMATCH[2]}"
            flavor="Darwin64bit+${osx_ver}"
        else
            echo "Unrecognized operating system string: '$os_str'.  Skip"
        fi
        otherquals="`echo ${BASH_REMATCH[3]} | sed 's/\./:/g'`"
    fi

    if [ -z "$flavor" ]; then
        echo "WARNING: Couldn't parse install dir to determine flavor: $dir.  Skip"
        continue
    elif [ -z "$otherquals" ]; then
        echo "WARNING: Couldn't parse install dir to determine qualifiers: $dir.  Skip"
    fi
    echo "deduced flavor: $flavor"
    echo "deduced qualifiers: $otherquals"

    rsync -aq --include="$dir/" $path $path_to_install/

    ups_declares+=("ups declare novarwgt $novarwgt_version -f $flavor -q $otherquals \
                   -m $path_to_install/$novarwgt_version/ups/novarwgt.table \
                   -r $path_to_install/$novarwgt_version")
done

echo
echo "declaring:"
for path in "${ups_declares[@]}"; do
    cmd="$path"
    echo
    echo $cmd
    $cmd
    [ $? -eq 0 ] && ((prod_count++))
done


echo
echo
if [ $prod_count -eq 0 ]; then
    echo "\nWARNING: No instances of novarwgt declared."
    echo "You may wish to examine the attempted installation directory:"
    echo "    $path_to_install"
    echo "as well as the temporary working area:"
    echo "    $workdir"
else
    rm -rf $workdir
    echo "Successfully declared $prod_count instances of novarwgt."
fi
