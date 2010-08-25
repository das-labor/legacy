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

params=$#

DATESTRING="$(/bin/date +"%d.%m.%Y, %H:%M:%S"): KEYRONA:"

if [ ! $params -eq 5 ] ; then
    /usr/bin/logger "$DATESTRING Not enough parameter for $0!"
    exit -1
fi

logfile=$5

if [ ! -e $logfile ] ; then
    echo "$DATESTRING New logfile created!" > ${logfile}
    /bin/chown root:keyrona ${logfile}
    /bin/chmod 660 ${logfile}
fi

if [ ! -e $logfile ] ; then
    echo "$DATESTRING Logfile '$logfile' does not exist!"
    /usr/bin/logger "$DATESTRING Logfile '$logfile' does not exist!"
    exit -1
fi

check_result()
{             
    if [ ! $1 -eq 0 ] ; then
	echo "Error occured!" >> ${logfile}
	/sbin/cryptsetup luksClose ${args[1]} 
	exit -1
    fi
}  

args=("$@")
echo "$DATESTRING Opening device ${args[0]} as ${args[1]}" >> ${logfile}

echo ${args[2]} | /sbin/cryptsetup luksOpen ${args[0]} ${args[1]}  2>> ${logfile} 1>> ${logfile}
check_result $?

#echo  "$DATESTRING Mounting device /dev/mapper/${args[1]} to ${args[3]}" >> ${logfile}
#/bin/mount /dev/mapper/${args[1]} ${args[3]} 2>> ${logfile} 1>> ${logfile}
#check_result $?
