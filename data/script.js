var mouse_on_humidity = false;
var humidity_randomizer;
var temperature_randomizer;
var resistance_randomizer;

window.addEventListener('load', (event) => {
	updateHumidity();
	updateTemperature();
	updateResistance();
	setInterval(() => {
		getReadings();
	}, 1000);
});

if (!!window.EventSource) {
	var source = new EventSource('/events');

	source.addEventListener('open', function (e) {
		console.log("Events Connected");
	}, false);

	source.addEventListener('error', function (e) {
		if (e.target.readyState != EventSource.OPEN) {
			console.log("Events Disconnected");
		}
	}, false);

	source.addEventListener('message', function (e) {
		console.log("message", e.data);
	}, false);

	source.addEventListener('new_readings', function (e) {
		var reading = JSON.parse(e.data);
		var temperature = parseFloat(reading.temperature) / 100;
		var humidity = parseFloat(reading.humidity) / 100;
		var resistance = parseFloat(reading.resistance) / 4095;
		setHumidity(humidity);
		setResistance(resistance);
		setTemperature(temperature);
	}, false);
}

function getReadings() {
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function () {
		if (this.readyState == 4 && this.status == 200) {
			var reading = JSON.parse(this.responseText);
			var temperature = parseFloat(reading.temperature) / 100;
			var humidity = parseFloat(reading.humidity) / 100;
			var resistance = parseFloat(reading.resistance) / 4095;
			setHumidity(humidity);
			setResistance(resistance);
			setTemperature(temperature);
		}
	};
	xhr.open("GET", "/readings", true);
	xhr.send();
}

function setHumidity(val) {
	if (isNaN(val)) return;
	document.documentElement.style.setProperty('--humidity', Math.min(Math.max(0, val), 1));
	updateHumidity();
}

function setTemperature(val) {
	if (isNaN(val)) return;
	document.documentElement.style.setProperty('--temperature', Math.min(Math.max(0, val), 1));
	updateTemperature();
}

function setResistance(val) {
	if (isNaN(val)) return;
	document.documentElement.style.setProperty('--resistance', Math.min(Math.max(0, val), 1));
	updateResistance();
}

function randomizeAll(enabled) {
	randomizeHumidity(enabled);
	randomizeTemperature(enabled);
	randomizeResistance(enabled);
}

function randomizeHumidity(enabled) {
	if (enabled) humidity_randomizer = setInterval(() => {
		setHumidity(Math.random());
	}, 2000);
	else clearInterval(humidity_randomizer);
}

function randomizeTemperature(enabled) {
	if (enabled) temperature_randomizer = setInterval(() => {
		setTemperature(Math.random());
	}, 2000);
	else clearInterval(temperature_randomizer);
}

function randomizeResistance(enabled) {
	if (enabled) resistance_randomizer = setInterval(() => {
		setResistance(Math.random());
	}, 2000);
	else clearInterval(resistance_randomizer);
}

function updateHumidity() {
	var humidity = getComputedStyle(document.documentElement).getPropertyValue('--humidity');
	var value = document.getElementById("humidity-value");
	value.innerHTML = (humidity * 100).toFixed(0) + '%';
}

function updateTemperature() {
	var temperature = getComputedStyle(document.documentElement).getPropertyValue('--temperature');
	var value = document.getElementById("temperature-value");
	value.innerHTML = (temperature * 100).toFixed(0) + '°';
}

function updateResistance() {
	var resistance = getComputedStyle(document.documentElement).getPropertyValue('--resistance');
	var value = document.getElementById("resistance-value");
	value.innerHTML = (resistance * 10000).toFixed(0) + 'Ω';
}