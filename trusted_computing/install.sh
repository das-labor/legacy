#!/bin/bash
# Copyright (C) 2008, 2009 by Sirrix AG security technologies
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

KEYRONA_GROUP="keyrona"
KEYRONA_SUPERUSER="root"
INSTALLDIR="/etc/keyrona"
BINDIR="/usr/bin"
SBINDIR="/usr/sbin"
KEYDIR="/etc/keyrona/keys"
DATABASEDIR="/etc/keyrona/database"
SCRIPTDIR="/etc/keyrona/scripts"
SOCKETDIR="/var/run/keyrona"
LOGFILE="/var/log/keyrona.log"
LIBDIR="/usr/lib"
INCLUDEDIR="/usr/include"
MANDIR="/usr/share/man/man1"

colored_output()
{               
    #Style           Foreground      Background
    #1st Digit       2nd Digit       3rd Digit 
    #0 - Reset       30 - Black      40 - Black
    #1 - FG Bright   31 - Red        41 - Red  
    #2 - Unknown     32 - Green      42 - Green
    #3 - Unknown     33 - Yellow     43 - Yellow
    #4 - Underline   34 - Blue       44 - Blue  
    #5 - BG Bright   35 - Magenta    45 - Magenta
    #6 - Unknown     36 - Cyan       46 - Cyan   
    #7 - Reverse     37 - White      47 - White  

    STYLE=1 # style bright
    BG=40   # background black (default)

    if [[ "$1" == "red" ]] ; then
        FG=31
    fi

    if [[ "$1" == "green" ]] ; then
        FG=32
    fi

    if [[ "$1" == "yellow" ]] ; then
        FG=33
    fi

    if [[ "$1" == "blue" ]] ; then
        FG=34
    fi

    if [[ "$1" == "magenta" ]] ; then
        FG=35
    fi

    if [[ "$1" == "cyan" ]] ; then
        FG=36
    fi

    if [[ "$1" == "white" ]] ; then
        FG=37
    fi

    CTRL="\033[${STYLE};${FG};${BG}m"
    echo -en "${CTRL}"
    echo -n "=> $2"
    echo -en "\033[0m"
    echo -e "\033[0m"
} 

error()
{
    colored_output "red" "Error: $1"
    colored_output "red" "Giving up..."
    exit -1
}

check_result()
{
    if [[ ! $1 -eq 0 ]] ; then
	error $2
    fi
}

check_binary()
{
    colored_output "yellow" "Verifying existence of: $1"
    which $1 2>/dev/null 1>/dev/null
    check_result $? "$1 not found, please install it first!"
    echo -e "\t$(which $1)"
}

check_root()
{
    if test "$UID" != 0 ; then
	colored_output "red" "You are not root!"
	exit -1
    fi  
}

create_dir()
{
    if [[ $1 == "" ]] ; then
	error "create_dir"
    fi

    if [[ ! -d $1 ]] ; then
        colored_output "white" "Creating $1"
        mkdir -p $1
    fi
    
    if [[ ! -d $1 ]] ; then
	error "create_dir"
    fi
}

copy_file()
{
    if [[ $1 == "" ]] ; then
	error "copy file, param 1"
    fi
    if [[ $2 == "" ]] ; then
	error "copy file, param 2"
    fi
    
    if [[ ! -e $1 ]] ; then
	error "$1 does not exist!"
    fi

    if [[ ! -e $2 ]] ; then
	error "$2 does not exist!"
    fi
    
    colored_output "white" "Installing $1 to $2"
    cp $1 $2
}

### VERIFY_ROOT
check_root
check_binary "scons"
check_binary "make"
check_binary "unzip"
check_binary "patch"
check_binary "ldconfig"
check_binary "dos2unix"

### VERIFYING EXISTENCE OF KEYRONA CRYPTLIB PORT
colored_output "cyan" "Verifying existence of Keyronas cryptlib port"
if [ ! -e "$INCLUDEDIR/keyrona_cryptlib.h" ] || [ ! -e $LIBDIR/libkeyronacl.so ] ; then
    colored_output "yellow" "Keyrona include not available, installing it..."
    cd src/libs/crypto/cryptlib
    check_result $? "CD cryptlib"

    colored_output "white" "Creating cryptlib temp directory"
    mkdir -p temp
    check_result $? "Creating cryptlib temp directory"

    colored_output "white" "Changing into cryptlib temp directory"
    cd temp
    check_result $? "Changing into  cryptlib temp directory"

    colored_output "white" "Unzipping cryptlib"
    unzip -o ../cl332.zip
    check_result $? "Unzipping cryptlib"

    colored_output "white" "Patching cryptlib [1/4]"
    patch -p1 < ../cl_sha1_patch.diff
    check_result $? "Patching cryptlib [1/4]"

    colored_output "white" "Patching cryptlib [2/4]"
    patch -p1 < ../cl_bindings_patch.diff
    check_result $? "Patching cryptlib [2/4]"

    colored_output "white" "Patching cryptlib [3/4]"
    patch -p1 < ../cl_tools_patch.diff
    check_result $? "Patching cryptlib [3/4]"

    colored_output "white" "Patching cryptlib [4/4]"
    patch -p1 < ../cl_makefile_patch.diff
    check_result $? "Patching cryptlib [4/4]"

    colored_output "white" "dos2unix cryptlib/tools"
    dos2unix tools/*
    check_result $? "dos2unix cryptlib/tools"

    colored_output "white" "Building cryptlib"
    make shared
    check_result $? "Building cryptlib"
    
    colored_output "white" "Installing cryptlib header to $INCLUDEDIR"
    cp cryptlib.h $INCLUDEDIR/keyrona_cryptlib.h
    check_result $? "Installing cryptlib header"
    if [ ! -e libkeyronacl.so.3.3.2 ] ; then
	colored_output "red" "Building of cryptlib failed!"
	exit -1
    fi

    colored_output "white" "Installing cryptlib library to $LIBDIR"
    cp libkeyronacl.so.3.3.2 $LIBDIR/libkeyronacl.so
    check_result $? "Installing cryptlib library"

    colored_output "white" "Removing cryptlib temp directory"
    cd ..
    rm -rf temp
    check_result $? "Removing cryptlib temp directory"

    
    cd ../../../../
    colored_output "green" "Successfully installed cryptlib library"
fi

### BUILD
colored_output "cyan" "Compiling Keyrona"
scons
check_result $? "Build"

### CHECK FOR RUNNING KEYPROVIDER
if [[ $(pidof keyrona_keyproviderd) ]] ; then
    colored_output "yellow" "Keyrona Keyprovider is running, terminating it..."
    killall keyrona_keyproviderd
fi

### INSTALL
colored_output "cyan" "Installing Keyrona to $INSTALLDIR"

### CREATING DIRECTORIES
create_dir $INSTALLDIR
create_dir $KEYDIR
create_dir $SCRIPTDIR
create_dir $DATABASEDIR
create_dir $SOCKETDIR

### CREATING DATABASES
colored_output "cyan" "Installing Keyrona databases to $DATABASEDIR"
touch $DATABASEDIR/group.db
touch $DATABASEDIR/subject.db
touch $DATABASEDIR/volume.db
touch $DATABASEDIR/keyprovider.db

### CREATING LOGFILE
touch $LOGFILE

### COPYING CONFIGFILE
colored_output "cyan" "Installing Keyrona configuration"
copy_file src/config/keyrona.cfg $INSTALLDIR

### INSTALLING MODULES
colored_output "cyan" "Installing Keyrona modules"
for i in $(find src/output/) ;
do
    if [ $(echo $i | grep "\.sh" | grep -v "\.svn" | grep -v $0 | grep -v "\.cxx" | grep -v "\.hxx" | grep -v "\.o") ] ; then
	copy_file $i $SCRIPTDIR
    fi
done

### INSTALLING BINARIES
colored_output "cyan" "Installing Keyrona binaries"
copy_file src/core/keyrona_manager/keyrona_manager $BINDIR
copy_file src/core/keyrona_mount/keyrona_mount $BINDIR
copy_file src/core/keyrona_keyproviderd/keyrona_keyproviderd $SBINDIR

### INSTALLING MAN-PAGES
colored_output "cyan" "Installing Keyrona man-pages"
copy_file manpage/keyrona_manager.1.bz2 $MANDIR
copy_file manpage/keyrona_mount.1.bz2 $MANDIR
copy_file manpage/keyrona_keyproviderd.1.bz2 $MANDIR

### STRIPPING BINARIES
colored_output "cyan" "Stripping binaries"
strip -s $BINDIR/keyrona_*
strip -s $SBINDIR/keyrona_keyproviderd

### CREATING KEYRONA GROUP
colored_output "cyan" "Adding Keyrona group '$KEYRONA_GROUP'"
groupadd -f keyrona
colored_output "yellow" "Please add the local Linux users to the Keyrona group"

### ASSIGNING PERMISSIONS
colored_output "cyan" "Assigning permissions"
chmod -f 770 $SOCKETDIR
chmod -f 750 $INSTALLDIR
chmod -f 700 $SCRIPTDIR
chmod -f 500 $SCRIPTDIR/*
chmod -f 640 $INSTALLDIR/keyrona.cfg
chmod -f 500 $SBINDIR/keyrona_keyproviderd
chmod -f 550 $BINDIR/keyrona_*
chmod -f 770 $DATABASEDIR
chmod -f 660 $DATABASEDIR/*.db
chmod -f 770 $KEYDIR
chmod -f 660 $KEYDIR/* 2>/dev/null
chmod -f 660 $LOGFILE

chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $SOCKETDIR
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $INSTALLDIR
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $INSTALLDIR/* -R
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $SCRIPTDIR/* -R
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $DATABASEDIR
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $DATABASEDIR/* -R
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $BINDIR/keyrona_*
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $SBINDIR/keyrona_keyproviderd
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $KEYDIR
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $KEYDIR/* -R 2>/dev/null
chown -f $KEYRONA_SUPERUSER:$KEYRONA_GROUP $LOGFILE

### APPLYING SE-LINUX RULES
which "sestatus" 2>/dev/null 1>/dev/null
if [ $? -eq 0 ] ; then
    colored_output "cyan" "Applying SE-Linux rules"
    check_binary "semanage"
    check_binary "restorecon"
    check_binary "getenforce"
    colored_output "white" "Current SE-Linux status: $(getenforce)"
    semanage fcontext -a -t textrel_shlib_t /usr/lib/libkeyronacl.so
    restorecon -R /usr/lib/libkeyronacl.so 
fi

### DONE
colored_output "green" "Done... ;)"
colored_output "yellow" "In order to automatically start the Keyrona keyprovider upon system boot or within /etc/fstab, you have to add it to the according runlevels. Please execute the following commands according to your distribution:"
colored_output "white" " ==== Gentoo ==== "
colored_output "yellow" "Gentoo: '# cp distributions/gentoo/etc/init.d/keyrona /etc/init.d'"
colored_output "yellow" "Gentoo: '# chmod 550 /etc/init.d/keyrona'"
colored_output "yellow" "Gentoo: '# chown root:keyrona /etc/init.d/keyrona'"
colored_output "yellow" "Gentoo: '# cp distributions/gentoo/sbin/mount.keyrona /sbin'"
colored_output "yellow" "Gentoo: '# chmod 550 /sbin/mount.keyrona'"
colored_output "yellow" "Gentoo: '# chown root:keyrona /sbin/mount.keyrona'"
colored_output "yellow" "Gentoo: '# rc-update add keyrona'"
colored_output "white" " ==== Fedora ==== "
colored_output "yellow" "Fedora: '# cp distributions/fedora/etc/init.d/keyrona /etc/init.d'"
colored_output "yellow" "Fedora: '# chmod 550 /etc/init.d/keyrona'"
colored_output "yellow" "Fedora: '# chown root:keyrona /etc/init.d/keyrona'"
colored_output "yellow" "Fedora: '# cp distributions/fedora/sbin/mount.keyrona /sbin'"
colored_output "yellow" "Fedora: '# chmod 550 /sbin/mount.keyrona'"
colored_output "yellow" "Fedora: '# chown root:keyrona /sbin/mount.keyrona'"
colored_output "yellow" "Fedora: '# chkconfig --add keyrona'"
colored_output "white" " ==== Debian ==== "
colored_output "yellow" "Debian: '# cp distributions/debian/etc/init.d/keyrona /etc/init.d'"
colored_output "yellow" "Debian: '# chmod 550 /etc/init.d/keyrona'"
colored_output "yellow" "Debian: '# chown root:keyrona /etc/init.d/keyrona'"
colored_output "yellow" "Debian: '# cp distributions/debian/sbin/mount.keyrona /sbin'"
colored_output "yellow" "Debian: '# chmod 550 /sbin/mount.keyrona'"
colored_output "yellow" "Debian: '# chown root:keyrona /sbin/mount.keyrona'"
colored_output "yellow" "Debian: '# update-rc.d keyrona defaults'"

if [ -x /etc/init.d/keyrona ] ; then
    /etc/init.d/keyrona restart
fi
