var keys = {
	A: ['recta'],
	E: ['recte'],
	I: ['recti'],
	O: ['recto'],
	U: ['rectu']
}
var vowels = {
	A: ['recta'],
	E: ['recte'],
	I: ['recti'],
	O: ['recto'],
	U: ['rectu']
}
var other = {
	P: ['rectp'],
	W: ['rectw'],
	R: ['rectr'],
	F: ['rectf'],
	D: ['rectd'],
	T: ['rectt'],
	H: ['recth'],
	Y: ['recty'],
	S: ['rects'],
	N: ['rectn']
}
var doubles = {
	J: ['rectp', 'rectspace'],
	M: ['rectw', 'rectspace'],
	B: ['rectr', 'rectspace'],
	V: ['rectd', 'rectspace'],
	C: ['rectt', 'rectspace'],
	L: ['recth', 'rectspace'],
	Z: ['recte', 'rectspace'],
	Q: ['recto', 'rectspace'],
	X: ['recty', 'rectspace'],
	G: ['rects', 'rectspace'],
	K: ['rectn', 'rectspace']
	
}
var numbers = {
	0: ['recty', 'rectnum'],
	1: ['rects', 'rectnum'],
	2: ['rectn', 'rectnum'],
	3: ['recti', 'rectnum'],
	4: ['rectt', 'rectnum'],
	5: ['recth', 'rectnum'],
	6: ['recte', 'rectnum'],
	7: ['rectw', 'rectnum'],
	8: ['rectr', 'rectnum'],
	9: ['recta', 'rectnum']

}

var score = 0;
var points = 5;

function keyPress(key) {
	var element = keys[key];
	document.getElementById('press').textContent = key;
	points = 5;

	timer = setTimeout(function() {
		element.forEach(thing => {
			document.querySelector(".svgClass").getSVGDocument().getElementById(thing).style.fill = "blue";
		});
		points = 1;
	},1.5*1000);


	hotkeys('*', async function (event, handler){
		event.preventDefault();

		//if(handler.key == key) {
		if( hotkeys.isPressed(key) == true){
			clearTimeout(timer);
			element.forEach(thing => {
				document.querySelector(".svgClass").getSVGDocument().getElementById(thing).style.fill = 'none';
			});
			//document.getElementById('press').textContent = 'E';
			hotkeys.unbind('*');
			updateScore(points);
			nextKey();
		//} else if (hotkeys.isPressed(key) != true ){
	        } else {
			//clearTimeout(timer);
			//element.forEach(thing => {
			//	document.querySelector(".svgClass").getSVGDocument().getElementById(thing).style.fill = 'none';
			//});
			//hotkeys.unbind('*');
			updateScore(-5);
			speechSynthesis.speak(new SpeechSynthesisUtterance("Fail. "+key.toLowerCase()));
			setTimeout(function() {
				//nextKey();
			}, 1500);
			//console.log(event);
		}
		return false;
	});
}

function clearKeys() {
	var elements = document.querySelector(".svgClass").getSVGDocument().getElementById('g47599').getElementsByTagName('rect');

	for (var i = 0; i < elements.length; i++) {
		elements[i].style.fill = 'none';
		//console.log(elements[i]);
	}
}

function start() {
	clearKeys();
	hotkeys.unbind('*');
	score = 0;
	updateScore(0);
	document.getElementById('press').textContent = "SPACEBAR TO START";
	document.querySelector(".svgClass").getSVGDocument().getElementById('rectspace').style.fill = 'blue';
	hotkeys('space', async function (event, handler){
		document.querySelector(".svgClass").getSVGDocument().getElementById('rectspace').style.fill = 'none';
		hotkeys.unbind('space');
		nextKey();
		
	});
}

function nextKey() {
	key = Object.keys(keys)[ Object.keys(keys).length * Math.random() << 0];
	speechSynthesis.speak(new SpeechSynthesisUtterance(key.toLowerCase()));
	keyPress(key);
}

function updateScore(amount) {
	score += amount;
	if(score>299) {
		changeKeys('vowelsotherdoubles');
		document.getElementById('level').textContent = '4';
	} else if(score> 199) {
		changeKeys('doubles');
		document.getElementById('level').textContent = '3';
	} else if(score>99) {
		changeKeys('other');
		document.getElementById('level').textContent = '2';
	} else {
		changeKeys('vowels');
		document.getElementById('level').textContent = '1';
	}
	document.getElementById('score').textContent = score;
	if(amount > 0) { 
		document.getElementById('score').style.color = "green";
	} else {
		document.getElementById('score').style.color = "red";
	}
}

function changeKeys(type, resetscore=false) {
	if(type == 'vowels') {
		keys = vowels
		if(resetscore) score = 0;
	} else if (type == 'numbers') {
		keys = numbers
		if(resetscore) score = 0;
	} else if (type == 'doubles') {
		keys = doubles
		if(resetscore) score = 200;
	} else if (type == 'other') {
		keys = other
		if(resetscore) score = 100;
	} else if (type == 'vowelsother') {
		keys = Object.assign({}, vowels, other)
	} else if (type == 'vowelsotherdoubles') {
		keys = Object.assign({}, vowels, other, doubles)
		if(resetscore) score = 300;
	} else {
		keys = Object.assign({}, vowels, other)
	}
}
