<html>
	<head>

<?php
$handle = fopen ("position.csv","r") or die("can't open file"); // open for read

while ( ($data = fgetcsv ($handle, 1000, ",")) !== FALSE ) { 	// read data from file into array
		$ts = $data[2];
        $lat = $data[3];
        $lon = $data[5];
		$ns = $data[4];
		$ew = $data[6];
}
function nmea2deg($nmea){
	$deg = (int)($nmea/100) + ($nmea - (int)($nmea / 100) * 100) / 60;
	return $deg;
}
$lat = nmea2deg($lat);	
if ( $ns == "S" )
	$lat = 0-$lat;
$lon = nmea2deg($lon);
if ( $ew == "W" )
	$lon = 0-$lon;
//echo $lat."\n";
//echo $lon."\n";
fclose ($handle);
?>


		<title>My current Position</title>

		<style type="text/css">
			/* styling for OpenLayers Map */
			body {
				margin: auto;
			}

			body p {
				text-align: center;
			}

			body div#map {
				width: 80%;
				height: 80%;
				clear: both;
				margin: 40px auto !important;
				border: 2px solid #fc0;
			}

		</style>

		<!-- bring in the OpenLayers javascript library
		(here we bring it from the remote site, but you could
		easily serve up this javascript yourself) -->
		<script src="http://www.openlayers.org/api/OpenLayers.js"></script>

		<!-- bring in the OpenStreetMap OpenLayers layers.
		Using this hosted file will make sure we are kept up
		to date with any necessary changes -->
		<script src="http://www.openstreetmap.org/openlayers/OpenStreetMap.js"></script>

		<script type="text/javascript">
			// Start position for the map (hardcoded here for simplicity,
			// but maybe you want to get from URL params)
			// (Though to get it from the URL it's simpler to use the Permalink and ArgParser controls)
			var lat=<?php echo $lat; ?>;
			var lon=<?php echo $lon; ?>;
			var zoom=13;

			var map; //complex object of type OpenLayers.Map

			//Initialise the 'map' object
			function init() {

				map = new OpenLayers.Map ("map", {
					controls:[
						new OpenLayers.Control.Navigation(),
						new OpenLayers.Control.PanZoomBar(),
						new OpenLayers.Control.Attribution()],
					maxExtent: new OpenLayers.Bounds(-20037508.34,-20037508.34,20037508.34,20037508.34),
					maxResolution: 156543.0399,
					numZoomLevels: 19,
					units: 'm',
					projection: new OpenLayers.Projection("EPSG:900913"),
					displayProjection: new OpenLayers.Projection("EPSG:4326")
				} );


			// Define the map layer
			// Note that we use a predefined layer that will be
			// kept up to date with URL changes
			// Here we define just one layer, but providing a choice
			// of several layers is also quite simple
			// Other defined layers are OpenLayers.Layer.OSM.Mapnik, OpenLayers.Layer.OSM.Maplint and OpenLayers.Layer.OSM.CycleMap
			layerMapnik = new OpenLayers.Layer.OSM.Mapnik("Mapnik");
			map.addLayer(layerMapnik);
			layerTilesAtHome = new OpenLayers.Layer.OSM.Osmarender("Osmarender");
			map.addLayer(layerTilesAtHome);
			layerCycleMap = new OpenLayers.Layer.OSM.CycleMap("CycleMap");
			map.addLayer(layerCycleMap);
			layerMarkers = new OpenLayers.Layer.Markers("Markers");
			map.addLayer(layerMarkers);

			var lonLat = new OpenLayers.LonLat(lon, lat).transform(new OpenLayers.Projection("EPSG:4326"), map.getProjectionObject());
			map.setCenter (lonLat, zoom);

			var size = new OpenLayers.Size(21,25);
			var offset = new OpenLayers.Pixel(-(size.w/2), -size.h);
			var icon = new OpenLayers.Icon('http://www.openstreetmap.org/openlayers/img/marker.png',size,offset);
			layerMarkers.addMarker(new OpenLayers.Marker(lonLat,icon));

		}
		</script>
	</head>

	<!-- body.onload is called once the page is loaded (call the 'init' function) -->
	<body>
		<div style="text-align: center; font-family: Verdana;">Position: <?php echo $lat.", ".$lon; ?><br/>Timestamp: <?php echo $ts; ?>GMT</div>
		<!-- define a DIV into which the map will appear. Make it take up the whole window -->
		<div id="map"></div>

		<script type="text/javascript">
			init();
		</script>
	</body>
</html>
