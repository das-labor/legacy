<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<html>
<head>
  <title>Tor: Ejecutando el cliente Tor en MS Windows</title>
  <meta name="Author" content="Roger Dingledine">
  <meta http-equiv="content-type" content="text/html; charset=ISO-8859-1">
  <link rel="stylesheet" type="text/css" href="../stylesheet.css">
  <link rel="shortcut icon" type="image/x-icon" href="../favicon.ico">
</head>
<body>
<div class="center">
<table class="banner" border="0" cellpadding="0" cellspacing="0" summary="">
    <tr>
        <td class="banner-left"></td>
        <td class="banner-middle">
	<a href="../index.html.es">Principal</a>
<a href="../overview.html.es">Perspectiva</a>
<a href="../download.html.es">Descarga</a>
<a href="../documentation.html.es">Documentacion</a>
<a href="../volunteer.html">Colabora</a>
<a href="../people.html.es">Gente</a>
<a href="../donate.html.es">Dona!</a>
        </td>
        <td class="banner-right">
	<img src="../images/green-flagspace.png" alt="" width="24" height="16"> <a href="tor-doc-win32.html.en"><img src="../images/en.png" alt="English" width="24" height="16"></a> <img src="../images/es.png" alt="espa&ntilde;ol" width="24" height="16"> <a href="tor-doc-win32.html.fr"><img src="../images/fr.png" alt="fran&ccedil;ais" width="24" height="16"></a> <a href="tor-doc-win32.html.it"><img src="../images/it.png" alt="Italiano" width="24" height="16"></a> <img src="../images/green-flagspace.png" alt="" width="24" height="16"> <a href="tor-doc-win32.html.pt"><img src="../images/pt.png" alt="Portugu&ecirc;s" width="24" height="16"></a> <a href="tor-doc-win32.html.ru"><img src="../images/ru.png" alt="&#1056;&#1091;&#1089;&#1089;&#1082;&#1080;&#1081;&nbsp;(Russkij)" width="24" height="16"></a> <img src="../images/green-flagspace.png" alt="" width="24" height="16">
        </td>
    </tr>
</table>
<div class="center">
<div class="main-column">
<h1>Ejecutando el cliente <a href="../index.html.es">Tor</a> en MS Windows</h1>
<br>
<p><font size="-2">N. del T.(TOR-&gt;The Onion Router, el enrutador "cebolla")</font></p>
<p><b>N&oacute;tese que estas son las instrucciones de instalaci&oacute;n para ejecutar un cliente
en MS Windows (98, 98SE, NT4, 2000, XP, Server).
Si quieres gestionar tr&aacute;fico para otros y ayudar a crecer la red Tor,
 (h&aacute;zlo por favor), lee la gu&iacute;a
<a href="../docs/tor-doc-server.html">Configurar un servidor (English)</a>
</b>
</p>
<hr>
<a id="installing"></a>
<h2><a class="anchor" href="#installing">Paso Uno: Descarga e Instala Tor</a></h2>
<br>
<p>La instalacion del "paquete" para MS Windows <a href="../index.html.es">Tor</a>,
<a href="http://www.freehaven.net/%7Eedmanm/torcp/">TorCP</a>
(un controlador de Tor que monitoriza y contola Tor), y
<a href="http://www.privoxy.org/">Privoxy</a> (un proxy con filtro web) en un solo paquete,
con las tres aplicaciones preconfiguradas para trabajar juntas.
<a href="../download.html.es">Descarga la versi&oacute;n estable o la experimental</a>.
</p>
<p>
<b>Si quieres configurar un servidor Tor, necesitas instalar la versi&oacute;n experimental.</b>
</p>
<p>Si el paquete no funciona para tus requisitos, puedes descargar Tor desde <a href="../download.html.es">la p�gina de descargas</a>, y
<a href="../docs/tor-doc-unix.html#privoxy">instalar y configurar Privoxy por ti mismo</a>.
</p>
<img alt="tor installer splash page" src="../img/screenshot-win32-installer-splash.png" width="503" height="386">
<p>Si ya has instalado Tor, TorCP, o Privoxy previamente, puedes deseleccionar cualquier componente
que no necesites en cuadro de dialogo mostrado abajo.
</p>
<img alt="select components to install" src="../img/screenshot-win32-installer-components.png" width="503" height="386">
<p>Tras completar la instalacion, los componentes seleccionados se iniciar�n autom�ticamente.</p>
<!--
This is explanatory
Esto es aclaratory
-->
<p>Si no se inicia TorCP, hacer click con el boton derecho sobre su icono, y aparece un men&uacute; contextual
con opcion de iniciar, parar, configurar, ... y salir definitivamente.
<!--
<p>Tor comes configured as a client by default. It uses a built-in
default configuration file, and most people won't need to change any of
the settings. Tor is now installed.
</p>
-->
<hr>
<a id="using"></a>
<h2><a class="anchor" href="#using">Paso Dos: Configurar tus aplicaciones para Tor</a></h2>
<br>
<p>Tras instalar Tor y Privoxy, debes configurar tus aplicaciones para usarlos.
El primer paso es configurar tu explorador.</p>
<p>Si usas Firefox (te lo recomendamos), visitar <a href="../docs/tor-switchproxy.html">Tor SwitchProxy howto</a> para
configurar una extensi&oacute;n de Firefox que hace muy facil cambiar entre usar Tor o usar conexi&oacute;n directa.</p>
<p>De otro modo debes configurar tu explorador para
HTTP proxy en localhost, port 8118.
(Este es el puerto en el que Privoxy atiende.)
<ul>
<li>En Mozilla, entrar en Editar|Preferencias|Avanzado|Proxies.</li>
<li>En Opera 7.5x, es en Herramientas|Preferencias|Red|Proxy servers.</li>
<li>En IE, es en Herramientas|Opciones de Internet|Conexiones|Configuraci&oacute;n LAN|Avanzado.</li>
</ul>
Deber&iacute;as marcar la casilla "usar el mismo proxy para todos los protocolos"
pero visita <a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#FtpProxy">esta nota</a> sobre Tor y ftp proxies.
En IE, aparece algo como esto:
<img alt="Proxy settings in IE" src="../img/screenshot-win32-ie-proxies.jpg" width="536" height="559">
<p>Usar Privoxy es <strong>necesario</strong> porque
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#SOCKSAndDNS">
los exploradores fallan al hacer peticiones DNS cuando usan un proxy SOCKS directamente</a>,
lo cual es malo para el anonimato. Privoxy tambien elimina ciertas cabeceras
peligrosas de tus peticiones web, y bloquea inc&oacute;modos sitios como Doubleclick.</p>
<p>Para usar con TOR otras aplicaciones que soportan proxies HTTP, simplemente
dirigirlo a Privoxy (localhost, puerto 8118). Para usar SOCKS directamente (para
mensajer&iacute;a instantanea, Jabber, IRC, etc.) puedes dirigir tu aplicaci&oacute;n directamente
a Tor (localhost, puerto 9050), pero visita <a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#SOCKSAndDNS">esta FAQ</a>
donde se explica por qu&eacute; esto puede ser peligroso. Para aplicaciones que no soportan ni SOCKS ni HTTP,
visitar <a href="http://www.socks.permeo.com/Download/SocksCapDownload/index.asp">SocksCap</a> o
<a href="http://www.freecap.ru/eng/">FreeCap</a>.
(FreeCap es free software; SocksCap es propietario)</p>
<p>Para m&aacute;s informaci&oacute;n sobre como "TOR"ificar otras aplicaciones
visitar <a href="http://wiki.noreply.org/wiki/TheOnionRouter/TorifyHOWTO">Torify
HOWTO</a>.
</p>
<hr>
<a id="verify"></a>
<h2><a class="anchor" href="#verify">Paso Tres: Asegurarse que funciona</a></h2>
<br>
<p>Comprueba que Privoxy y TorCP se est&aacute;n ejecutando y que TorCP ha iniciado TOR con &eacute;xito.
El icono de Privoxy es un circulo azul o verde con una "P" dentro, y TorCP es una
cabeza de cebolla gris con una marca "V" en verde (cuando est&aacute; activado) como aqu&iacute;:</p>
<img alt="TorCP Tray Icon" src="../img/screenshot-win32-torcp.png" width="143" height="76">
<p>Despu&eacute;s, deber&iacute;as intentar usar tu explorador con Tor y asegurarte que tu direccion
IP esta siendo anonimizada.
Visitar <a href="http://serifos.eecs.harvard.edu/cgi-bin/ipaddr.pl?tor=1">Tor detector</a>
y comprobar si cree que usas Tor o no.
(Si este sitio no responde, visitar <a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#IsMyConnectionPrivate">
esta FAQ</a> para mas sugerencias sobre como testear Tor.)
</p>
<p>Si usas un cortafuegos personal que limita la capacidad de conexi&oacute;n de tu computadora,
asegurate que permite conexiones de aplicaciones locales al puerto local 8118 y al puerto 9050. </p>
<p>Si tu cortafuegos bloquea conexiones salientes, configuralo para que puedan hacerse conexiones
al menos por los puertos TCP 80 y 443 y visita
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#FirewalledClient">este apartado de la FAQ</a>.
</p>
<p>Si a&uacute;n no funciona, visita
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#ItDoesntWork">este apartado de la FAQ</a>
para seguir consejos.</p>
<hr>
<a id="server"></a>
<h2><a class="anchor" href="#server">Paso Cuatro: Configurarlo como servidor</a></h2>
<br>
<p>La red Tor se basa en voluntarios que donan su ancho de banda.
Cuantos m&aacute;s servidores, m&aacute;s rapida ser&aacute; la red Tor. Si tienes al menos 20Kbytes/s en ambos sentidos,
por favor ayuda a Tor configur&aacute;ndolo para que sea Servidor tambi&eacute;n. Tenemos varias caracter&iacute;sticas para que
hacer a Tor servidor sea facil y conveniente, incluyendo limitaci&oacute;n del ancho de banda cedido, politicas de
conexiones de salida que pueden limitar tu exposici&oacute;n a casos de abuso, y soporte para IP din&aacute;micas.
<p>Tener servidores en varios y diferentes sitios en la Internet es lo que convierte a los usuarios de Tor seguros.
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#ServerAnonymity">Puedes reforzar tu propio anonimato</a>,
haciendo que sitios remotos no puedan saber si las conexiones se originaron en
tu computadora o fueron iniciadas desde otras.</p>
<p>Lee mas en nuestra gu&iacute;a <a href="../docs/tor-doc-server.html">Configurando un servidor</a></p>
<hr>
<p>Si tienes sugerencias para mejorar este documento, por favor env&iacute;alas a
<a href="http://bugs.noreply.org/tor">nuestro bugtracker</a> en la categor&iacute;a "website". Gracias!</p>
  </div><!-- #main -->
</div>
</div>
  <div class="bottom" id="bottom">
     <p>
     <i><a href="../contact.html.es" class="smalllink">Webmaster</a></i> -
      Ultima modificacion: Tue Mar 28 23:26:37 2006
      -
      Ultima compilacion: Mon Jul 24 03:18:55 2006
     </p>
     <p>
	Aviso: Esta traduccion podria estar obsoleta. La revision original en ingles es la
      1.13 mientras que esta revision esta basada en la
      1.4.
     </p>
     <p>
	Esta pagina tambien esta disponible en los siguientes idiomas:
           <a href="tor-doc-win32.html.en">English</a>, <a href="tor-doc-win32.html.fr">fran&ccedil;ais</a>, <a href="tor-doc-win32.html.it">Italiano</a>, <a href="tor-doc-win32.html.pt">Portugu&ecirc;s</a>, <a href="tor-doc-win32.html.ru">&#1056;&#1091;&#1089;&#1089;&#1082;&#1080;&#1081;&nbsp;(Russkij)</a>.<br>
       Como establecer <a href="http://www.debian.org/intro/cn#howtoset">el idioma por defecto del documento</a>.
     </p>
  </div>
</body>
</html>
