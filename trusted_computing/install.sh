#!/bin/bash
# Copyright (C) 2008, 2009 by Sirrix AG security technologies
# Copyright (C) 2010 by Philipp Deppenwiese
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

TPMCRYPT_GROUP="tpmcrypt"
TPMCRYPT_SUPERUSER="root"
INSTALLDIR="/etc/tpmcrypt"
BINDIR="/usr/bin"
SBINDIR="/usr/sbin"
KEYDIR="/etc/tpmcrypt/keys"
DATABASEDIR="/etc/tpmcrypt/database"
SCRIPTDIR="/etc/tpmcrypt/scripts"
SOCKETDIR="/var/run/tpmcrypt"
LOGFILE="/var/log/tpmcrypt.log"
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

### BUILD
colored_output "cyan" "Compiling TpmCrypt"
scons
check_result $? "Build"

### CHECK FOR RUNNING KEYPROVIDER
if [[ $(pidof tpmcrypt_keyproviderd) ]] ; then
    colored_output "yellow" "TpmCrypt Keyprovider is running, terminating it..."
    killall tpmcrypt_keyproviderd
fi

### INSTALL
colored_output "cyan" "Installing TpmCrypt to $INSTALLDIR"

### CREATING DIRECTORIES
create_dir $INSTALLDIR
create_dir $KEYDIR
create_dir $SCRIPTDIR
create_dir $DATABASEDIR
create_dir $SOCKETDIR

### CREATING DATABASES
colored_output "cyan" "Installing TpmCrypt databases to $DATABASEDIR"
touch $DATABASEDIR/group.db
touch $DATABASEDIR/subject.db
touch $DATABASEDIR/volume.db
touch $DATABASEDIR/keyprovider.db

### CREATING LOGFILE
touch $LOGFILE

### COPYING CONFIGFILE
colored_output "cyan" "Installing TpmCrypt configuration"
copy_file src/config/tpmcrypt.cfg $INSTALLDIR

### INSTALLING MODULES
colored_output "cyan" "Installing TpmCrypt modules"
for i in $(find src/output/) ;
do
    if [ $(echo $i | grep "\.sh" | grep -v "\.svn" | grep -v $0 | grep -v "\.cxx" | grep -v "\.hxx" | grep -v "\.o") ] ; then
	copy_file $i $SCRIPTDIR
    fi
done

### INSTALLING BINARIES
colored_output "cyan" "Installing TpmCrypt binaries"
copy_file src/core/tpmcrypt_manager/tpmcrypt_manager $BINDIR
copy_file src/core/tpmcrypt_mount/tpmcrypt_mount $BINDIR
copy_file src/core/tpmcrypt_keyproviderd/tpmcrypt_keyproviderd $SBINDIR

### INSTALLING MAN-PAGES
colored_output "cyan" "Installing TpmCrypt man-pages"
copy_file manpage/tpmcrypt_manager.1.bz2 $MANDIR
copy_file manpage/tpmcrypt_mount.1.bz2 $MANDIR
copy_file manpage/tpmcrypt_keyproviderd.1.bz2 $MANDIR

### STRIPPING BINARIES
colored_output "cyan" "Stripping binaries"
strip -s $BINDIR/tpmcrypt_*
strip -s $SBINDIR/tpmcrypt_keyproviderd

### CREATING TPMCRYPT GROUP
colored_output "cyan" "Adding TpmCrypt group '$TPMCRYPT_GROUP'"
groupadd -f tpmcrypt
colored_output "yellow" "Please add the local Linux users to the TpmCrypt group"

### ASSIGNING PERMISSIONS
colored_output "cyan" "Assigning permissions"
chmod -f 770 $SOCKETDIR
chmod -f 750 $INSTALLDIR
chmod -f 700 $SCRIPTDIR
chmod -f 500 $SCRIPTDIR/*
chmod -f 640 $INSTALLDIR/tpmcrypt.cfg
chmod -f 500 $SBINDIR/tpmcrypt_keyproviderd
chmod -f 550 $BINDIR/tpmcrypt_*
chmod -f 770 $DATABASEDIR
chmod -f 660 $DATABASEDIR/*.db
chmod -f 770 $KEYDIR
chmod -f 660 $KEYDIR/* 2>/dev/null
chmod -f 660 $LOGFILE

chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $SOCKETDIR
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $INSTALLDIR
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $INSTALLDIR/* -R
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $SCRIPTDIR/* -R
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $DATABASEDIR
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $DATABASEDIR/* -R
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $BINDIR/tpmcrypt_*
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $SBINDIR/tpmcrypt_keyproviderd
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $KEYDIR
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $KEYDIR/* -R 2>/dev/null
chown -f $TPMCRYPT_SUPERUSER:$TPMCRYPT_GROUP $LOGFILE

### DONE
colored_output "green" "Done... ;)"
colored_output "yellow" "In order to automatically start the TpmCrypt keyprovider upon system boot or within /etc/fstab, you have to add it to the according runlevels. Please execute the following commands according to your distribution:"
colored_output "white" " ==== Gentoo ==== "
colored_output "yellow" "Gentoo: '# cp distributions/gentoo/etc/init.d/tpmcrypt /etc/init.d'"
colored_output "yellow" "Gentoo: '# chmod 550 /etc/init.d/tpmcrypt'"
colored_output "yellow" "Gentoo: '# chown root:tpmcrypt /etc/init.d/tpmcrypt'"
colored_output "yellow" "Gentoo: '# cp distributions/gentoo/sbin/mount.tpmcrypt /sbin'"
colored_output "yellow" "Gentoo: '# chmod 550 /sbin/mount.tpmcrypt'"
colored_output "yellow" "Gentoo: '# chown root:tpmcrypt /sbin/mount.tpmcrypt'"
colored_output "yellow" "Gentoo: '# rc-update add tpmcrypt'"
colored_output "white" " ==== Fedora ==== "
colored_output "yellow" "Fedora: '# cp distributions/fedora/etc/init.d/tpmcrypt /etc/init.d'"
colored_output "yellow" "Fedora: '# chmod 550 /etc/init.d/tpmcrypt'"
colored_output "yellow" "Fedora: '# chown root:tpmcrypt /etc/init.d/tpmcrypt'"
colored_output "yellow" "Fedora: '# cp distributions/fedora/sbin/mount.tpmcrypt /sbin'"
colored_output "yellow" "Fedora: '# chmod 550 /sbin/mount.tpmcrypt'"
colored_output "yellow" "Fedora: '# chown root:tpmcrypt /sbin/mount.tpmcrypt'"
colored_output "yellow" "Fedora: '# chkconfig --add tpmcrypt'"
colored_output "white" " ==== Debian ==== "
colored_output "yellow" "Debian: '# cp distributions/debian/etc/init.d/tpmcrypt /etc/init.d'"
colored_output "yellow" "Debian: '# chmod 550 /etc/init.d/tpmcrypt'"
colored_output "yellow" "Debian: '# chown root:tpmcrypt /etc/init.d/tpmcrypt'"
colored_output "yellow" "Debian: '# cp distributions/debian/sbin/mount.tpmcrypt /sbin'"
colored_output "yellow" "Debian: '# chmod 550 /sbin/mount.tpmcrypt'"
colored_output "yellow" "Debian: '# chown root:tpmcrypt /sbin/mount.tpmcrypt'"
colored_output "yellow" "Debian: '# update-rc.d tpmcrypt defaults'"

if [ -x /etc/init.d/tpmcrypt ] ; then
    /etc/init.d/tpmcrypt restart
fi
