#!/bin/sh
# $Id: package.sh,v 1.9 2005/08/07 12:43:34 phobos Exp $
# Copyright 2004-2005 Nick Mathewson.
# See LICENSE in Tor distribution for licensing information.

# This script builds a Macintosh OS X metapackage containing 4 packages:
#    - One for Tor.
#    - One for Privoxy.
#    - One for a tor-specific privoxy configuration script.
#    - One for Startup scripts for Tor.
#
# This script expects to be run from the toplevel makefile, with VERSION
# set to the latest Tor version, and Tor already built.
#
# Building Tor for OSX in 3 easy steps:
# 1) Make sure PRIVOXY_PKG_ZIP= is set to the correct path to find privoxyosx_setup_3.0.3.zip
# 2) In the top level, this means ~/tor, not tor/contrib/osx, do a configure with these parameters:
# configure --prefix=/Library/Tor --bindir=/Library/Tor --sysconfdir=/Library
# 3) In same top level dir, make dist-osx.  You'll have a .dmg file when complete.

# Where have we put the zip file containing Privoxy?  Edit this if your
# privoxy lives somewhere else.
PRIVOXY_PKG_ZIP=~/src/privoxy-setup/privoxyosx_setup_3.0.3.zip

###
# Helpful info on OS X packaging:
#   http://developer.apple.com/documentation/DeveloperTools/Conceptual/SoftwareDistribution/index.html
#   man packagemaker

# Make sure VERSION is set, so we don't name the package "Tor  Bundle.dmg"
if [ "XX$VERSION" = 'XX' ]; then
  echo "VERSION not set."
  exit 1
fi

# Where will we put our temporary files?
BUILD_DIR=/tmp/tor-osx-$$
# Path to PackageMaker app.
PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker

umask 022

echo I might ask you for your password now, so you can sudo.

sudo rm -rf $BUILD_DIR
mkdir $BUILD_DIR || exit 1
for subdir in tor_packageroot tor_resources \
              torstartup_packageroot \
              privoxyconf_packageroot \
              torbundle_resources \
              output; do
    mkdir $BUILD_DIR/$subdir
done

### Make Tor package.
make install DESTDIR=$BUILD_DIR/tor_packageroot
#mv $BUILD_DIR/tor_packageroot/Library/Tor/torrc.sample $BUILD_DIR/tor_packageroot/Library/Tor/torrc
cp contrib/osx/ReadMe.rtf $BUILD_DIR/tor_resources
#cp contrib/osx/License.rtf $BUILD_DIR/tor_resources
chmod 755 contrib/osx/TorPostflight
cp contrib/osx/TorPostflight $BUILD_DIR/tor_resources/postflight
cp contrib/osx/addsysuser $BUILD_DIR/tor_resources/addsysuser
cat <<EOF > $BUILD_DIR/tor_resources/Welcome.txt
Tor: an anonymous Internet communication system

Tor is a system for using the internet anonymously, and allowing
others to do so.
EOF

find $BUILD_DIR/tor_packageroot -print0 |sudo xargs -0 chown root:wheel

$PACKAGEMAKER -build              \
    -p $BUILD_DIR/output/Tor.pkg  \
    -f $BUILD_DIR/tor_packageroot \
    -r $BUILD_DIR/tor_resources   \
    -i contrib/osx/TorInfo.plist  \
    -d contrib/osx/TorDesc.plist

### Put privoxy configuration package in place.
mkdir -p $BUILD_DIR/privoxyconf_packageroot/Library/Privoxy
cp contrib/osx/privoxy.config $BUILD_DIR/privoxyconf_packageroot/Library/Privoxy/config

find $BUILD_DIR/privoxyconf_packageroot -print0 |sudo xargs -0 chown root:wheel

$PACKAGEMAKER -build                      \
    -p $BUILD_DIR/output/privoxyconf.pkg  \
    -f $BUILD_DIR/privoxyconf_packageroot \
    -i contrib/osx/PrivoxyConfInfo.plist  \
    -d contrib/osx/PrivoxyConfDesc.plist

### Make Startup Script package

mkdir -p $BUILD_DIR/torstartup_packageroot/Library/StartupItems/Tor
cp contrib/osx/Tor contrib/osx/StartupParameters.plist \
   $BUILD_DIR/torstartup_packageroot/Library/StartupItems/Tor

find $BUILD_DIR/torstartup_packageroot -print0 | sudo xargs -0 chown root:wheel
$PACKAGEMAKER -build                     \
    -p $BUILD_DIR/output/torstartup.pkg  \
    -f $BUILD_DIR/torstartup_packageroot \
    -i contrib/osx/TorStartupInfo.plist  \
    -d contrib/osx/TorStartupDesc.plist

### Assemble the metapackage.  Packagemaker won't buld metapackages from
# the command line, so we need to do it by hand.

MPKG=$BUILD_DIR/output/Tor\ Bundle.mpkg
mkdir -p "$MPKG/Contents/Resources"
echo -n "pmkrpkg1" > "$MPKG/Contents/PkgInfo"
cp contrib/osx/ReadMe.rtf "$MPKG/Contents/Resources"
#cp contrib/osx/License.rtf "$MPKG/Contents/Resources"
cp contrib/osx/TorBundleInfo.plist "$MPKG/Contents/Info.plist"
cp contrib/osx/TorBundleWelcome.rtf "$MPKG/Contents/Resources/Welcome.rtf"
cp contrib/osx/TorBundleDesc.plist "$MPKG/Contents/Resources/Description.plist"

# Move all the subpackages into place.  unzip Privoxy.pkg into place,
# and fix its file permissions so we can rm -rf it later.
mkdir $BUILD_DIR/output/.contained_packages
mv $BUILD_DIR/output/*.pkg $BUILD_DIR/OUTPUT/.contained_packages
( cd $BUILD_DIR/output/.contained_packages && unzip $PRIVOXY_PKG_ZIP && find Privoxy.pkg -type d -print0 | xargs -0 chmod u+w )

### Copy readmes and licenses into toplevel.
PRIVOXY_RESDIR=$BUILD_DIR/output/.contained_packages/Privoxy.pkg/Contents/Resources
cp $PRIVOXY_RESDIR/License.html $BUILD_DIR/output/Privoxy\ License.html
cp $PRIVOXY_RESDIR/ReadMe.txt $BUILD_DIR/output/Privoxy\ ReadMe.txt
cp contrib/osx/ReadMe.rtf $BUILD_DIR/output/Tor\ ReadMe.rtf
cp LICENSE $BUILD_DIR/output/Tor\ License.txt

### Assemble documentation

DOC=$BUILD_DIR/output/Documents
mkdir $DOC
cp doc/tor-doc.html doc/tor-doc.css $DOC
cp AUTHORS $DOC/AUTHORS.txt
groff doc/tor.1 -T ps -m man | ps2pdf - $DOC/tor-reference.pdf
groff doc/tor-resolve.1 -T ps -m man | ps2pdf - $DOC/tor-resolve.pdf

mkdir $DOC/Advanced
cp doc/tor-spec.txt doc/rend-spec.txt doc/control-spec.txt doc/socks-extensions.txt doc/version-spec.txt $DOC/Advanced
cp doc/HACKING $DOC/Advanced/HACKING.txt
cp ChangeLog $DOC/Advanced/ChangeLog.txt

### Package it all into a DMG

find $BUILD_DIR/output -print0 | sudo xargs -0 chown root:wheel

mv $BUILD_DIR/output "$BUILD_DIR/Tor $VERSION Bundle"
rm -f "Tor $VERSION Bundle.dmg"
USER="`whoami`"
sudo hdiutil create -format UDZO -srcfolder "$BUILD_DIR/Tor $VERSION Bundle" "Tor $VERSION Bundle.dmg"
sudo chown "$USER" "Tor $VERSION Bundle.dmg"

sudo rm -rf $BUILD_DIR
