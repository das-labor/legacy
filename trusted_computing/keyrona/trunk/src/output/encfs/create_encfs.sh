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
mount_path="/mnt/create_keyrona"

if [ ! $params -eq 2 ] ; then
    /usr/bin/logger "$DATESTRING Not enough parameter for $0!"
    exit -1
fi

check_result()
{             
    if [ ! $1 -eq 0 ] ; then
	echo "Error occured!"
	exit -1
    fi
}  

args=("$@")
/bin/mkdir ${mount_path}
/bin/mkdir -p ${args[0]}
echo "$DATESTRING Opening folder ${args[0]} and mounting the folder to ${mount_path}"
echo -e "\n${args[1]}" | /usr/bin/encfs -S ${args[0]} ${mount_path} >/dev/null
/usr/bin/fusermount -u ${mount_path}
rm -rf ${mount_path}
check_result $?

