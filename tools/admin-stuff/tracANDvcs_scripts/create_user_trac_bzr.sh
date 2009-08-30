#!/bin/bash

GENERALPREFIX=/srv
BZRBASEDIR=${GENERALPREFIX}/BZRs
TRACBASEDIR=${GENERALPREFIX}/TRACs
TRACADMIN=/usr/bin/trac-admin
BZRADMIN=/usr/bin/bzr
APACHEUSER="www-data"
APACHEGROUP="www-data"
HTPASSWD=/usr/bin/htpasswd
TRACPLUGINBASE=/usr/local/trac-plugins

echo "#################################"
echo "#                               #"
echo "# create user-tracs+svn         #"
echo "#                               #"
echo "#################################"

function help
{
    cat <<EOF
Du weisst nicht was du tust... hier eine kurze Anleitung

Das script wird ueber Umgebungsvariablen gesteuert. D.h. 
der Aufruf ist in etwa so:

DOIT=1 \ # dummyfaenger damit nicht schief gehen kann
USERNAME=admin \ #der trac-admin der erstmal alles darf
PASSWORD=nimda \ #das trac-admin Passwort
TRACNAME="mein super Projekt" \ # der name fuer das Trac
TRACSHORT="msp" \ # der Kurzname zu dem Projekt. die ist auch 
                \ # der Verzeichnisname
./create_user_trac_bzr.sh


was passiert beim Aufrufen:
* es wird ein trac unter /srv/TRACs/TRACSHORT erstellt
* es wird ein svn unter /srv/BZRs/TRACSHORT erstellt
* es wird im trac der user USERNAME angelegt der Adminrechte hat
* es wird im bzr der user USERNAME angelegt der schreibrechte hat
* es wird fuer trac+bzr eine apacheconfig angelegt in 
  /etc/apache2/userSVNandTRAC.available
* es wird ein symlink von userSVNandTRAC.available nach
  userSVNandTRAC.enabled angelegt

was dann noch zu tun ist, ist ein neustart des apache also

/etc/init.d/apache2 restart

danach sollte das bzr+trac fuer den benutzer vorhanden sein
und er/sie alle hat die moeglichkeit benutzer hinzu zufuegen und
zu loeschen.

EOF
exit 2
}


if [[ x${DOIT} == x ]];then
    help
fi

if [[ x${TRACSHORT} == x ]];then
    help
fi


if [[ x${USERNAME} == x ]];then
    help
fi
if [[ x${TRACNAME} == x ]];then
    help
fi

if [[ x${PASSWORD} == x ]];then
    help
fi

if [[ -e ${TRACBASEDIR}/${TRACSHORT} ]];then
    echo "sorry das TRAC ${BZRBASEDIR}/${TRACSHORT} gibt es schon"
    exit 1
fi

if [[ -e ${BZRBASEDIR}/${TRACSHORT} ]];then
    echo "sorry das SVN ${BZRBASEDIR}/${TRACSHORT} gibt es schon"
    exit 1
fi

echo "Sieht alles gut aus, erstelle TRAC und SVN"
mkdir -p ${GENERALPREFIX}
mkdir -p ${BZRBASEDIR}/${TRACSHORT}
mkdir -p ${TRACBASEDIR}

FOO=$(pwd)
cd ${BZRBASEDIR}/${TRACSHORT}
${BZRADMIN} init 
cd ${FOO}
echo ""
echo "svn done..."
echo ""
echo ""
echo "permission gesetzt"
echo ""
${TRACADMIN} "${TRACBASEDIR}/${TRACSHORT}" initenv "${TRACNAME}" sqlite:db/trac.db bzr "${BZRBASEDIR}/${TRACSHORT}"
echo ""
echo "trac erstellt"
echo ""
${TRACADMIN} "${TRACBASEDIR}/${TRACSHORT}" permission add ${USERNAME} TRAC_ADMIN 
echo ""
echo "trac admin erstellt"
echo ""
${HTPASSWD} -c -b ${TRACBASEDIR}/${TRACSHORT}/conf/htpasswd.labor ${USERNAME} ${PASSWORD}
echo ""
echo "passwort und user erstellt"
echo ""

cat <<EOF > /etc/apache2/userSVNandTRAC.available/${TRACSHORT}.conf

Alias /userbzr/${TRACSHORT} "/srv/BZRs/${TRACSHORT}/"
<Location /userbzr/${TRACSHORT}>
        Dav On

        AuthType Basic
	AuthName "${TRACSHORT} Bazaar Repository"
        AuthUserFile ${TRACBASEDIR}/${TRACSHORT}/conf/htpasswd.labor

        <LimitExcept GET OPTIONS>
                Require valid-user
        </LimitExcept>
</Location>

<Location /usertrac/${TRACSHORT}>
        SetHandler mod_python
        PythonInterpreter main_interpreter
        PythonHandler trac.web.modpython_frontend
        PythonOption TracEnv ${TRACBASEDIR}/${TRACSHORT}
        PythonOption TracUriRoot /usertrac/${TRACSHORT}
        SetEnv PYTHON_EGG_CACHE /tmp
</Location>

<Location "/usertrac/${TRACSHORT}/login">
        AuthType Basic
        AuthName "${TRACSHORT} Trac Auth"
        AuthUserFile ${TRACBASEDIR}/${TRACSHORT}/conf/htpasswd.labor
        Require valid-user
</Location>
EOF

ln -s /etc/apache2/userSVNandTRAC.available/${TRACSHORT}.conf /etc/apache2/userSVNandTRAC.enabled/

echo installiere Plugins
cp ${TRACPLUGINBASE}/CodeExample-0.3.1-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracAccountManager-0.2.1dev_r5836-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracDownloads-0.3-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracListOfWikiPagesMacro-0.1-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracSectionEditPlugin-0.1-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracTags-0.6-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracTocMacro-11.0.0.3-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracWikiToPdfPlugin-2.3.1-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/
cp ${TRACPLUGINBASE}/TracBzr-0.2-py2.6.egg ${TRACBASEDIR}/${TRACSHORT}/plugins/


echo erstelle Trac-Konfiguration 

cat <<EOF > ${TRACBASEDIR}/${TRACSHORT}/conf/trac.ini
# -*- coding: utf-8 -*-
[account-manager]
authentication_url = 
force_passwd_change = true
hash_method = HtDigestHashMethod
htdigest_realm = 
password_file = ${TRACBASEDIR}/${TRACSHORT}/conf/htpasswd.labor
password_store = HtPasswdStore


[downloads]
ext = zip,gz,bz2,rar,pdf,ps,tar,rar
path = ${TRACBASEDIR}/${TRACSHORT}/downloads
title = Downloads
unique_filename = true
visible_fields = id,file,description,size,time,count,tags


[attachment]
max_size = 26214400
render_unsafe_content = false

[browser]
downloadable_paths = /trunk, /branches/*, /tags/*
hide_properties = svk:merge
render_unsafe_content = false

[changeset]
max_diff_bytes = 10000000
max_diff_files = 0
wiki_format_messages = true

[components]
tracbzr.* = enabled
acct_mgr.admin.* = enabled
acct_mgr.api.* = enabled
acct_mgr.db.* = enabled
acct_mgr.htfile.* = enabled
acct_mgr.http.* = disabled
acct_mgr.notification.* = enabled
acct_mgr.pwhash.* = enabled
acct_mgr.svnserve.* = disabled
acct_mgr.web_ui.* = enabled
acct_mgr.web_ui.registrationmodule = disabled
acct_mgr.web_ui.emailverificationmodule = disabled
codeexample.* = enabled
navcontrol.filter.* = enabled
pydotorgtheme.theme.* = enabled
themeengine.admin.* = enabled
themeengine.api.* = enabled
themeengine.web_ui.* = enabled
trac.web.auth.loginmodule = disabled
trac.web_ui.changeset = disabled
tracdownloads.admin.downloadswebadmin = enabled # Branch 0.10 and 0.11.
tracdownloads.api.* = enabled
tracdownloads.api.downloadsapi = enabled
tracdownloads.core.* = enabled
tracdownloads.core.downloadscore = enabled
tracdownloads.init.* = enabled
tracdownloads.init.downloadsinit = enabled
tracdownloads.tags.* = enabled
tracdownloads.timeline.* = enabled
tracdownloads.timeline.downloadstimeline = enabled
tracdownloads.webadmin.* = enabled
tracdownloads.wiki.* = enabled
tracdownloads.wiki.downloadswiki = enabled
traclistofwikipages.macro.* = enabled
tracmenus.web_ui.menumanagermodule = enabled
tracsectionedit.web_ui.* = enabled
tractags.* = enabled
tractoc.macro.* = enabled
wikitopdf.formats.* = enabled
wikitopdf.web_ui.* = enabled
wikitopdf.wikitopdf.* = enabled

[header_logo]
alt = 
height = 100
link = https://www.das-labor.org/
src = https://www.das-labor.org/images/logo.png
width = 100

[logging]
log_file = trac.log
log_level = ERROR
log_type = file

[mimeviewer]
enscript_modes = text/x-dylan:dylan:4
enscript_path = enscript
max_preview_size = 262144
mime_map = text/x-dylan:dylan,text/x-idl:ice,text/x-ada:ads:adb
php_path = php
silvercity_modes = 
tab_width = 8

[notification]
always_notify_owner = false
always_notify_reporter = false
always_notify_updater = true
mime_encoding = base64
smtp_always_bcc = 
smtp_always_cc = 
smtp_default_domain = 
smtp_enabled = false
smtp_from = trac@localhost
smtp_password = 
smtp_port = 25
smtp_replyto = trac@localhost
smtp_server = localhost
smtp_subject_prefix = __default__
smtp_user = 
use_public_cc = false
use_short_addr = false
use_tls = false

[project]
descr = ${TRACNAME}
footer = Visit the Labor hackespace project at<br /><a href="http://www.das-labor.org/">http://www.das-labor.org/</a>
icon = 
name = ${TRACNAME}
url = https://www.das-labor.org/

[search]
min_query_length = 3

[ticket]
default_component = 
default_milestone = 
default_priority = major
default_type = defect
default_version = 
restrict_owner = false

[ticket-workflow]
accept = new -> assigned
accept.operations = set_owner_to_self
accept.permissions = TICKET_MODIFY
leave = * -> *
leave.default = 1
leave.operations = leave_status
reassign = new,assigned,reopened -> new
reassign.operations = set_owner
reassign.permissions = TICKET_MODIFY
reopen = closed -> reopened
reopen.operations = del_resolution
reopen.permissions = TICKET_CREATE
resolve = new,assigned,reopened -> closed
resolve.operations = set_resolution
resolve.permissions = TICKET_MODIFY

[timeline]
changeset_long_messages = false
changeset_show_files = 0
default_daysback = 30
ticket_show_details = false

[trac]
authz_file = 
authz_module_name = 
base_url = 
check_auth_ip = true
database = sqlite:db/trac.db
default_charset = utf8
default_handler = WikiModule
htdocs_location = 
ignore_auth_case = false
mainnav = wiki,timeline,roadmap,browser,tickets,newticket,search
metanav = login,logout,settings,help,about
permission_store = DefaultPermissionStore
repository_dir = ${BZRBASEDIR}/${TRACSHORT}
repository_type = bzr
timeout = 20

[wiki]
ignore_missing_pages = false
render_unsafe_content = false
split_page_names = false

EOF

echo "fast fertig"
trac-admin ${TRACBASEDIR}/${TRACSHORT} upgrade
chown -R ${APACHEUSER}:${APACHEGROUP} ${TRACBASEDIR}/${TRACSHORT}
chown -R ${APACHEUSER}:${APACHEGROUP} ${BZRBASEDIR}/${TRACSHORT}

cat << EOF

DONE!

TRAC :      https://www.das-labor.org/usertrac/${TRACSHORT}

SVN  :      https://www.das-labor.org/userbzr/${TRACSHORT}


bitte den Apache einmal neu starten

/etc/init.d/apache2 restart


EOF
