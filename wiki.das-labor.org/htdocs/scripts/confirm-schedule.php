<?

include 'parse-termine.php'

// Parse http://wiki.das-labor.org
if (!isset($termine)) {
	session_register('termine');
	$termine = parse_termine('http://wiki.das-labor.org/');
};

?>

<html>
<head>
   <title>Termine aus dem Wiki bestaetigen und veroeffentlichen</title>
</head>
<body>

<?

?>

<body>
</html>
