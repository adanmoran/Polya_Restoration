#######################################
# Copyright Adan Moran-MacDonald 2018 #
#######################################
# Script to download Eigen locally
# Running this script should not require super user, as the installation is to a local directory
# The Eigen directory should end up with the header files under "<pwd>/eigen/Eigen"
#    and the FindEigen3.cmake file under "<pwd>/eigen/cmake"

CDIR=${PWD}

echo "Downloading Eigen"

VERSION=3.2.10

TARBALL=$VERSION.tar.gz

wget http://bitbucket.org/eigen/eigen/get/$TARBALL

if [ $? != 0 ]; then
    echo "Failed to download Eigen version $VERSION"
    exit
fi
echo "Unpacking to this directory."

# Check that ./eigen/ exists, and if it doesn't create it.
[ -d $CDIR/eigen ] || mkdir $CDIR/eigen

# --directory chooses where to unpack (it must exists), and --strip-components removes one layer of folders after unpacking.
# We remove 1 layer because it extracts as eigen/eigen-eigen-<MD5>/ 

tar -xzf $TARBALL --directory $CDIR/eigen --strip-components 1

echo "Removing tarball..."

rm $TARBALL

echo "Complete."

