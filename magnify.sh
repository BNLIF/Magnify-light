#!/bin/bash

magnify_source="$(dirname $(readlink -f $BASH_SOURCE))"

rootfile=$(readlink -f $1) ; shift
#frame="${1:-decon}"
startdir=$(pwd)

#echo "Loading frame \"$frame\""

cd $magnify_source/scripts
#root -l loadClasses.C Magnify-light.C'("'"$rootfile"'","'"$frame"'")'
root -l loadClasses.C Magnify.C'("'"$rootfile"'")'
