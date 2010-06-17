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

mount_path="/mnt/create_keyrona"
last_truecrypt_slot="12"
DATESTRING="$(/bin/date +"%d.%m.%Y, %H:%M:%S"): KEYRONA:"

check_result()
{             
    if [ ! $1 -eq 0 ] ; then
	exit -1
    fi
}  

args=("$@")
/usr/bin/logger "$DATESTRING Creating device ${args[0]}"

/usr/bin/truecrypt -t -c ${args[0]} --encryption=AES --filesystem=None --hash=RIPEMD-160 --password=${args[2]} --random-source=/dev/urandom --volume-type=normal -k "" --quick
check_result $?
mkdir -p ${mount_path}
check_result $?
echo ${args[2]} | /usr/bin/truecrypt --text --filesystem=None --slot=$last_truecrypt_slot --protect-hidden=no -k "" ${args[0]} /mnt/create_keyrona

path=$(/usr/bin/truecrypt -t -l 2>&1 | grep "/dev/" | grep "$last_truecrypt_slot" | cut -d' ' -f3)
check_result $?

echo Now formating $path :
mkfs.${args[1]} $path
check_result $?
/usr/bin/truecrypt -t -d ${args[0]}
check_result $?
/bin/rmdir ${mount_path}
check_result $?

