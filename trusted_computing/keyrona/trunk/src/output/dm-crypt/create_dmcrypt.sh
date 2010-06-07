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

check_result()
{             
    if [ ! $1 -eq 0 ] ; then
	exit -1
    fi
}  

DATESTRING="$(/bin/date +"%d.%m.%Y, %H:%M:%S"): KEYRONA:"

args=("$@")
/usr/bin/logger "$DATESTRING Creating device ${args[0]}"
echo ${args[2]} | /sbin/cryptsetup create keyrona_create ${args[0]}
check_result $?

/usr/bin/logger "$DATESTRING Formating device /dev/mapper/keyrona_create with ${args[1]} Filesystem"
mkfs.${args[1]} /dev/mapper/keyrona_create
/sbin/cryptsetup remove /dev/mapper/keyrona_create
	
check_result $?
