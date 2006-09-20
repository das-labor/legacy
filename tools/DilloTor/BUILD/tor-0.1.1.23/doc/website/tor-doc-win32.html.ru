<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<html>
<head>
  <title>Tor: ��������� Tor �� MS Windows</title>
  <meta name="Author" content="Roger Dingledine">
  <meta http-equiv="content-type" content="text/html; charset=KOI8-R">
  <link rel="stylesheet" type="text/css" href="../stylesheet.css">
  <link rel="shortcut icon" type="image/x-icon" href="../favicon.ico">
</head>
<body>
<div class="center">
<table class="banner" border="0" cellpadding="0" cellspacing="0" summary="">
    <tr>
        <td class="banner-left"></td>
        <td class="banner-middle">
	<a href="../index.html.ru">�������</a>
<a href="../overview.html.ru">�����</a>
<a href="../download.html.ru">�������</a>
<a href="../documentation.html.ru">������������</a>
<a href="../volunteer.html">�����������</a>
<a href="../people.html.ru">������</a>
<a href="../donate.html">Donate!</a>
        </td>
        <td class="banner-right">
	<img src="../images/green-flagspace.png" alt="" width="24" height="16"> <a href="tor-doc-win32.html.en"><img src="../images/en.png" alt="English" width="24" height="16"></a> <a href="tor-doc-win32.html.es"><img src="../images/es.png" alt="espa&ntilde;ol" width="24" height="16"></a> <a href="tor-doc-win32.html.fr"><img src="../images/fr.png" alt="fran&ccedil;ais" width="24" height="16"></a> <a href="tor-doc-win32.html.it"><img src="../images/it.png" alt="Italiano" width="24" height="16"></a> <img src="../images/green-flagspace.png" alt="" width="24" height="16"> <a href="tor-doc-win32.html.pt"><img src="../images/pt.png" alt="Portugu&ecirc;s" width="24" height="16"></a> <img src="../images/ru.png" alt="&#1056;&#1091;&#1089;&#1089;&#1082;&#1080;&#1081;&nbsp;(Russkij)" width="24" height="16"> <img src="../images/green-flagspace.png" alt="" width="24" height="16">
        </td>
    </tr>
</table>
<div class="center">
<div class="main-column">
<h1>������������� ������� <a href="../index.html.ru">Tor</a> � MS Windows</h1>
<br>
<p>
<b>��������: ��� ���������� �� ��������� ������� Tor � MS Windows (98, 98SE,
NT4, 2000, XP, Server). ���� �� ������ ���������� ��������� ������ ����� ������
����������� ���� Tor (���������� ������� ���), ���������� ���������� ��
<a href="../docs/tor-doc-server.html.ru">��������� �������</a>.</b>
</p>
<hr>
<a id="installing"></a>
<h2><a class="anchor" href="#installing">��� ������: ������� � ���������� Tor</a></h2>
<br>
<p>
����������� ��� MS Windows - <a href="../index.html.ru">Tor</a>,
<a href="http://www.vidalia-project.net/">Vidalia</a>
(GUI (����������� ���������) ��� Tor), � <a
href="http://www.privoxy.org">Privoxy</a> (����������� ��� ������) �
����� ������, ��� ��� ���������� ��������� �������� ������ "�� �������".
<a href="../download.html.ru">�������� <!--���� ���������� ����-->
����������������� ������</a>.
</p>
<p>���� �� �� ������ ������������ �����, �� ������ ������� Tor ��������
�� <a href="../download.html.ru">�������� download</a>, � ����� <a
href="../docs/tor-doc-unix.html#privoxy">���������� � ���������
Privoxy ��������������</a>.
</p>
<img alt="tor installer splash page"
src="../img/screenshot-win32-installer-splash.png" width="503" height="386">
<p>���� �� ����� ��� ���������� Tor, Vidalia ��� Privoxy
�� ������ ������� ������ �� ����������, ������� ������ ��������
� ������� ���������� ����.
</p>
<img alt="select components to install"
src="../img/screenshot-win32-installer-components.png" width="503" height="386">
<p>����� ���������� ���������, ���������� ������� �� �������
����� �������� �������������.
</p>
<!--
<p>Tor comes configured as a client by default. It uses a built-in
default configuration file, and most people won't need to change any of
the settings. Tor is now installed.
</p>
-->
<hr>
<a id="using"></a>
<h2><a class="anchor" href="#using">��� ������: ��������� ���������� ��� ������������� Tor</a></h2>
<br>
<p>����� ���������, �� ������ ��������� ��������� ����������
����� ��� ���o�������� Tor � Privoxy. ��� ������ �������� �������.</p>
<p>���� �� ����������� Firefox (�����������), ���������� <a
href="http://freehaven.net/~squires/torbutton/">������ Torbutton</a>
������� �������� �ӣ �� ���. ���� �� ����������� ������������ Firefox
�� �� ��� ���������� ��� ������� Tor, ���������� ��� <a
href="../docs/tor-switchproxy.html">Tor SwitchProxy howto</a> ����� ���������
������ ������� �������� ����� ������������� ����� Tor'�� � ������ �����������.</p>
<p>� ��������� ������ ��� ���ģ��� ������� ������ ��������� ������ � �����
��������.
� Mozilla � Firefox, ��� Edit - Preferences - Advanced - Proxies.
� Opera ��� Tools - Preferences(Advanced) - Network - Proxy servers.
� IE, ��� Tools - Internet Options - Connections- LAN Settings - Advanced.</p>
<img alt="Proxy settings in Firefox"
src="../img/screenshot-win32-firefox-proxies.jpg" width="525" height="505">
<p>
�� ������ ������� "localhost" � "8118" ����� ��������������
������ ��������� ��������� �� Privoxy, ��� �������� �� �������. (����
�������� ��� Privoxy �� ������������ FTP � Gopher,
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#FtpProxy">��
������ ������� �� � ����� ������</a>.) ����� ���� ��������� ���� socks � ����������
������ ����� �������������� ����� � Tor ("localhost", "9050", � socks5)
��������� ���������, ����� ������ ����ң�. ����� ����� "OK".</p>
<p>������������� Privoxy <strong>�����������</strong> ��� ��� <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#SOCKSAndDNS">��������
���������� ���� DNS ������� ���� ������������ SOCKS ������ ��������</a>, ���
���������� ���������� ���� �����������. Privoxy ����� ������� ��������� ������������
��������� �� http-��������, � ��������� ����������� ��������� �����.</p>
<p>����� ��������������(Torify) ������ ���������� ������� ������������ HTTP ������,
������ ������� �� �� Privoxy (�� ����, localhost ���� 8118). ��� ������������� SOCKS
�������� (������ IM(������� ������������ ������ �����������), Jabber, IRC, ���),
������� ����� � ���� ������� Tor (localhost ���� 9050), �� ������������
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#SOCKSAndDNS">� ����
�������� FAQ</a> ����������� �������������� ������ �������. ��� ����������
������� �� ������������ �� SOCKS �� HTTP, ������գ�� <a
href="http://www.socks.permeo.com/Download/SocksCapDownload/index.asp">SocksCap</a> ���
<a href="http://www.freecap.ru/eng/">FreeCap</a>.
(FreeCap ��� ��������� ��; SocksCap �������������.)</p>
<p>���������� � ��� ��� �������������� ������ ���������� ����� �������� �
<a href="http://wiki.noreply.org/wiki/TheOnionRouter/TorifyHOWTO">Torify
HOWTO</a>.
</p>
<hr>
<a id="verify"></a>
<h2><a class="anchor" href="#verify">��� ������: ��������� ��� �ӣ ��������</a></h2>
<br>
<p>
��������� ��� Privoxy � Vidalia ��������.
������ Privoxy �������� ��� ��̣��� ��� ����� ���� � ������ "P" ������, �
Vidalia ���������� ��̣��� �������� � ������ ���� Tor ��������, ��� ԣ����
� ������� ��������� ���� Tor �� �������. �� ������ ��������� ��� ���������� Tor
������� ������ ������� ���� �� ������ Vidalia � ������ � ����������� ����
Start ��� Stop �������������:
</p>
<img alt="Vidalia Tray Icon" src="../img/screenshot-win32-vidalia.png" width="161" height="215">
<p>
�����, ������� ������������ ������� ����� Tor � ��������� ���
��� IP ����� �� ��������. �������� <a
href="http://serifos.eecs.harvard.edu/cgi-bin/ipaddr.pl?tor=1">Tor
��������</a> � �� ������ ����������� �� �� Tor ��� ���.
(���� ���� �� ��������, ���������� <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#IsMyConnectionPrivate">����
������ FAQ</a> � ������� ������ �������� �������������� Tor.)
</p>
<p>���� ���������� ������������ �������� ������� ���������� ����������� ������
���������� ����������� � ������ ����, ��������� ���������� �� ��������� ����������
�� ��������� ����� 8118 � 9050. ���� ��� ��������
��������� ��������� ����������, �������� �� ������� ���� TCP �����
80 � 443, � ����� ������������ �
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#FirewalledClient">
���� �������� FAQ</a>.
</p>
<p>���� ��� �ӣ-���� �� ���������, ������� ��������� � <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#ItDoesntWork">FAQ</a>.</p>
<hr>
<a id="server"></a>
<h2><a class="anchor" href="#server">��� ���ף����: ��������� ������</a></h2>
<br>
<p>���� Tor ������������ �� ������������ ��������� ����� ���������. ��� ������ �����
��������� �������, ��� ������� �������� ���� Tor. ���� � ��� ���� �� ������� ����
20 ��������/��� � ������ �����������, ���������� �������� ���� Tor �������� ����
������. �� ����������� ����� ���� ����� ������� ������� Tor ̣����� � ��������
� �������������, ������� ����������� ��������, ����������� ������(exit policies),
� ��������� ������������ IP.</p>
<p>���������� �������� � ������ ������� ��������� ��� ������ ��, ���
������������� ������������� Tor. <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#ServerAnonymity">�� ����� ���������
����� ������� ����������� ����</a>,
��� ��� ������ ������� �� ����� ����� ��� ������� ���������� - ����� ��������� �����������
��� ������� ������ ���� Tor.</p>
<p>�������� ��������� �������� <a href="../docs/tor-doc-server.html.ru">��������� �������</a>.</p>
<hr>
<p>���� � ��� ���� ��������� �� ��������� ����� ���������, ���������� <a
href="../contact.html.ru">��������� �� ���</a>. �������!</p>
  </div><!-- #main -->
</div>
</div>
  <div class="bottom" id="bottom">
     <p>
     <i><a href="../contact.html.ru" class="smalllink">Webmaster</a></i> -
      ��������� ���������: Thu Jul 6 14:11:44 2006
      -
      ��������� ����������: Mon Jul 24 03:19:00 2006
     </p>
     <p>
      ��������! ���� ������� �������. ���������� �������� ����� �������
      1.13 ����� ��� ���� ������� ������� �� �������
      1.12.
     </p>
     <p>
       ��� �������� ����� ���������� �� ��������� �����:
       <a href="tor-doc-win32.html.en">English</a>, <a href="tor-doc-win32.html.es">espa&ntilde;ol</a>, <a href="tor-doc-win32.html.fr">fran&ccedil;ais</a>, <a href="tor-doc-win32.html.it">Italiano</a>, <a href="tor-doc-win32.html.pt">Portugu&ecirc;s</a>.<br>
       ��� ���������� <a href="http://www.debian.org/intro/cn#howtoset">���� ���������� �� ���������</a>.
     </p>
  </div>
</body>
</html>
