<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<html>
<head>
  <title>Tor: Установка Tor на MS Windows</title>
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
	<a href="../index.html.ru">Главная</a>
<a href="../overview.html.ru">Обзор</a>
<a href="../download.html.ru">Скачать</a>
<a href="../documentation.html.ru">Документация</a>
<a href="../volunteer.html">Добровольцы</a>
<a href="../people.html.ru">Авторы</a>
<a href="../donate.html">Donate!</a>
        </td>
        <td class="banner-right">
	<img src="../images/green-flagspace.png" alt="" width="24" height="16"> <a href="tor-doc-win32.html.en"><img src="../images/en.png" alt="English" width="24" height="16"></a> <a href="tor-doc-win32.html.es"><img src="../images/es.png" alt="espa&ntilde;ol" width="24" height="16"></a> <a href="tor-doc-win32.html.fr"><img src="../images/fr.png" alt="fran&ccedil;ais" width="24" height="16"></a> <a href="tor-doc-win32.html.it"><img src="../images/it.png" alt="Italiano" width="24" height="16"></a> <img src="../images/green-flagspace.png" alt="" width="24" height="16"> <a href="tor-doc-win32.html.pt"><img src="../images/pt.png" alt="Portugu&ecirc;s" width="24" height="16"></a> <img src="../images/ru.png" alt="&#1056;&#1091;&#1089;&#1089;&#1082;&#1080;&#1081;&nbsp;(Russkij)" width="24" height="16"> <img src="../images/green-flagspace.png" alt="" width="24" height="16">
        </td>
    </tr>
</table>
<div class="center">
<div class="main-column">
<h1>Использование клиента <a href="../index.html.ru">Tor</a> в MS Windows</h1>
<br>
<p>
<b>Внимание: это инструкции по установке клиента Tor в MS Windows (98, 98SE,
NT4, 2000, XP, Server). Если вы хотите передавать серверный трафик чтобы помочь
увеличиться сети Tor (пожалуйста делайте это), прочитайте инструкции по
<a href="../docs/tor-doc-server.html.ru">настройке сервера</a>.</b>
</p>
<hr>
<a id="installing"></a>
<h2><a class="anchor" href="#installing">Шаг первый: Скачать и установить Tor</a></h2>
<br>
<p>
Инсталлятор для MS Windows - <a href="../index.html.ru">Tor</a>,
<a href="http://www.vidalia-project.net/">Vidalia</a>
(GUI (графический интерфейс) для Tor), и <a
href="http://www.privoxy.org">Privoxy</a> (фильтрующий веб прокси) в
одном пакете, все три приложения настроены работать вместе "из коробки".
<a href="../download.html.ru">Скачайте <!--либо стабильную либо-->
экспериментальную версию</a>.
</p>
<p>Если вы не хотите использовать пакет, вы можете скачать Tor отдельно
со <a href="../download.html.ru">страницы download</a>, и потом <a
href="../docs/tor-doc-unix.html#privoxy">установить и настроить
Privoxy самостоятельно</a>.
</p>
<img alt="tor installer splash page"
src="../img/screenshot-win32-installer-splash.png" width="503" height="386">
<p>Если вы ранее уже установили Tor, Vidalia или Privoxy
вы можете выбрать только те компоненты, которые хотите обновить
в диалоге показанном ниже.
</p>
<img alt="select components to install"
src="../img/screenshot-win32-installer-components.png" width="503" height="386">
<p>После завершения установки, компоненты которые вы выбрали
будут запущены автоматически.
</p>
<!--
<p>Tor comes configured as a client by default. It uses a built-in
default configuration file, and most people won't need to change any of
the settings. Tor is now installed.
</p>
-->
<hr>
<a id="using"></a>
<h2><a class="anchor" href="#using">Шаг второй: Настроить приложения для использования Tor</a></h2>
<br>
<p>После установки, вы должны настроить остальные приложения
чтобы они испoльзовали Tor и Privoxy. Для начала настроим браузер.</p>
<p>Если вы используете Firefox (рекомендуем), попробуйте <a
href="http://freehaven.net/~squires/torbutton/">плагин Torbutton</a>
который настроит всё за вас. Если вы собираетесь использовать Firefox
не на том компьютере где запущен Tor, прочитайте наш <a
href="../docs/tor-switchproxy.html">Tor SwitchProxy howto</a> чтобы настроить
плагин который позволит легко переключаться между Tor'ом и прямым соединением.</p>
<p>В противном случае вам придётся вручную задать настройки прокси в вашем
браузере.
В Mozilla и Firefox, это Edit - Preferences - Advanced - Proxies.
В Opera это Tools - Preferences(Advanced) - Network - Proxy servers.
В IE, это Tools - Internet Options - Connections- LAN Settings - Advanced.</p>
<img alt="Proxy settings in Firefox"
src="../img/screenshot-win32-firefox-proxies.jpg" width="525" height="505">
<p>
Вы должны указать "localhost" и "8118" чтобы перенаправлять
четыре указанных протокола на Privoxy, как показано на рисунке. (Даже
учитывая что Privoxy не поддерживает FTP и Gopher,
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#FtpProxy">вы
должны указать их в любом случае</a>.) Также надо заполнить поле socks в настройках
прокси чтобы перенаправлять прямо в Tor ("localhost", "9050", и socks5)
остальные протоколы, кроме первых четырёх. Потом жмите "OK".</p>
<p>Использование Privoxy <strong>обязательно</strong> так как <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#SOCKSAndDNS">браузеры
пропускают ваши DNS запросы если использовать SOCKS прокси напрямую</a>, что
безусловно раскрывает вашу анонимность. Privoxy также удаляет некоторые небезопасные
заголовки из http-запросов, и блокирует надоедливые рекламные сайты.</p>
<p>Чтобы торифицировать(Torify) другие приложения которые поддерживают HTTP прокси,
просто укажите им на Privoxy (то есть, localhost порт 8118). Для использования SOCKS
напрямую (случай IM(системы мнгновенного обмена сообщениями), Jabber, IRC, итд),
укажите адрес и порт клиента Tor (localhost порт 9050), но ознакомьтесь
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#SOCKSAndDNS">с этим
разделом FAQ</a> описывающим небезопасность такого способа. Для приложений
которые не поддерживают ни SOCKS ни HTTP, попробуёте <a
href="http://www.socks.permeo.com/Download/SocksCapDownload/index.asp">SocksCap</a> или
<a href="http://www.freecap.ru/eng/">FreeCap</a>.
(FreeCap это свободное ПО; SocksCap проприетарное.)</p>
<p>Информацию о том как торифицировать другие приложения можно поискать в
<a href="http://wiki.noreply.org/wiki/TheOnionRouter/TorifyHOWTO">Torify
HOWTO</a>.
</p>
<hr>
<a id="verify"></a>
<h2><a class="anchor" href="#verify">Шаг третий: Убедитесь что всё работает</a></h2>
<br>
<p>
Проверьте что Privoxy и Vidalia запущены.
Иконка Privoxy выглядит как зелёный или синий круг с буквой "P" внутри, а
Vidalia показывает зелёную луковицу в случае если Tor работает, или тёмную
с красным крестиком если Tor не запущен. Вы можете запустить или остановить Tor
кликнув правой кнопкой мыши на иконку Vidalia и выбрав в контекстном меню
Start или Stop соответствено:
</p>
<img alt="Vidalia Tray Icon" src="../img/screenshot-win32-vidalia.png" width="161" height="215">
<p>
Далее, следует использовать браузер через Tor и убедиться что
ваш IP адрес не выдаётся. Посетите <a
href="http://serifos.eecs.harvard.edu/cgi-bin/ipaddr.pl?tor=1">Tor
детектор</a> и он скажет используете ли вы Tor или нет.
(Если сайт не работает, посмотрите <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#IsMyConnectionPrivate">этот
раздел FAQ</a> и узнайте больше способов протестировать Tor.)
</p>
<p>Если установлен персональный фаерволл который огранивает способность вашего
компьютера коннектится к самому себе, разрешите соединения от локальных приложений
на локальные порты 8118 и 9050. Если ваш фаерволл
блокирует исходящие соединения, откройте по крайней мере TCP порты
80 и 443, а потом ознакомьтесь с
<a href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#FirewalledClient">
этим разделом FAQ</a>.
</p>
<p>Если это всё-таки не сработает, поищите подсказки в <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#ItDoesntWork">FAQ</a>.</p>
<hr>
<a id="server"></a>
<h2><a class="anchor" href="#server">Шаг четвёртый: Настройте сервер</a></h2>
<br>
<p>Сеть Tor основывается на добровольцах делящихся своим траффиком. Чем больше людей
запускают серверы, тем быстрее работает сеть Tor. Если у вас есть по крайней мере
20 килобайт/сек в каждом направлении, пожалуйста помогите сети Tor настроив свой
сервер. Мы реализовали много чего чтобы сделать серверы Tor лёгкими и удобными
в использовании, включая ограничение траффика, ограничения выхода(exit policies),
и поддержку динамических IP.</p>
<p>Размещение серверов в разных уголках Интернета это именно то, что
обезопасивает пользователей Tor. <a
href="http://wiki.noreply.org/noreply/TheOnionRouter/TorFAQ#ServerAnonymity">Вы также получаете
более сильную анонимность сами</a>,
так как третьи стороны не могут знать кем открыты соединения - вашим локальным компьютером
или другими узлами сети Tor.</p>
<p>Смотрите подробное описание <a href="../docs/tor-doc-server.html.ru">Настройки сервера</a>.</p>
<hr>
<p>Если у вас есть пожелания по улучшению этого документа, пожалуйста <a
href="../contact.html.ru">высылайте их нам</a>. Спасибо!</p>
  </div><!-- #main -->
</div>
</div>
  <div class="bottom" id="bottom">
     <p>
     <i><a href="../contact.html.ru" class="smalllink">Webmaster</a></i> -
      Последние изменения: Thu Jul 6 14:11:44 2006
      -
      Последняя компиляция: Mon Jul 24 03:19:00 2006
     </p>
     <p>
      Внимание! Этот перевод устарел. Английский оригинал имеет ревизию
      1.13 тогда как этот перевод основан на ревизии
      1.12.
     </p>
     <p>
       Эта страница также переведена на следующие языки:
       <a href="tor-doc-win32.html.en">English</a>, <a href="tor-doc-win32.html.es">espa&ntilde;ol</a>, <a href="tor-doc-win32.html.fr">fran&ccedil;ais</a>, <a href="tor-doc-win32.html.it">Italiano</a>, <a href="tor-doc-win32.html.pt">Portugu&ecirc;s</a>.<br>
       Как установить <a href="http://www.debian.org/intro/cn#howtoset">язык документов по умолчанию</a>.
     </p>
  </div>
</body>
</html>
