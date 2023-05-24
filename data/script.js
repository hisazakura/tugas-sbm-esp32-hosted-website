var mouse_on_humidity = false;
var humidity_randomizer;
var temperature_randomizer;
var resistance_randomizer;

window.addEventListener('load', (event) => {
	updateHumidity();
	updateTemperature();
	updateResistance();
});

function setHumidity(val) {
	document.documentElement.style.setProperty('--humidity', Math.min(Math.max(0, val), 1));
	updateHumidity();
}

function setTemperature(val) {
	document.documentElement.style.setProperty('--temperature', Math.min(Math.max(0, val), 1));
	updateTemperature();
}

function setResistance(val) {
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