#!/bin/bash
# author: Tomas Baca

DOWNLOAD_LINK=$1
VERSION=$2
FILENAME="mvBlueFOX-ARM64_gnu-"$VERSION".tgz"
INSTALL_FILENAME="install_mvBlueFOX_ARM.sh"

versioned_link=$DOWNLOAD_LINK/$VERSION
full_filename_link=$versioned_link"/"$FILENAME
full_install_filename_link=$versioned_link"/"$INSTALL_FILENAME

echo "will try to download version $VERSION from $DOWNLOAD_LINK using link $full_filename_link"

echo "downloading $full_filename_link"
curl -O -J $full_filename_link

#echo "downloading $full_install_filename_link"
#curl -O -J $full_install_filename_link