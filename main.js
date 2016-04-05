var GoogleMapsAPI = require("googlemaps");
var sleep = require("sleep");

var publicConfig = {
	key: "giturown",
	stagger_time: 1000,
	encode_polylines: false,
	secure: true
};

var gmAPI = new GoogleMapsAPI(publicConfig);

//From http://stackoverflow.com/questions/12740659/downloading-images-with-node-js

var fs = require("fs");
var request = require("request");

var download = function(uri, filename, callback){
	request.head(uri, function(err, res, body){
		console.log("content-type:", res.headers["content-type"]);
		console.log("content-length:", res.headers["content-length"]);
		request(uri).pipe(fs.createWriteStream(filename)).on("close", callback);
	})
};

var jsonfilename = "overpass_stopsign_set5"

fs.readFile("GeographicCoordinates/" + jsonfilename + ".geojson", "utf8", function(err, data) {
	if (err) throw err;
	console.log("Loaded GeoJSON text file: " + jsonfilename);
	var json = JSON.parse(data);
	console.log("Parsed JSON");
	var elementIndex = 0;
	json.features.forEach(function(feature) {
		if(elementIndex % 10 != 0) {
			elementIndex += 1;
			return;
		}
		console.log("At element index #" + String(elementIndex));
		var p = feature.geometry.coordinates
		var params = {
			location: String(p[1]) + ", " + String(p[0]),
			size: "1024x1024",
			//heading: 0,
			//pitch: 0,
			fov: 90
		};
		var name = "Scraped/" + jsonfilename + "/" + String(elementIndex) + "_location" + params.location + "_fov" + String(params.fov) + "_size" + params.size + ".png";
		console.log("Downloading " + name);
		var url = gmAPI.streetView(params);
		download(url, name, function(){console.log("Downloaded image");});

		elementIndex += 1
	});
});

console.log("Hello World");
