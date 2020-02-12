//import * as zmq from 'jszmq';
document.body.innerHTML += "POSTing"

var xhttp = new XMLHttpRequest();
xhttp.open("GET", "localhost:8000", true);
//xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
xhttp.send("hello?"); 

let d = new Date();
document.body.innerHTML += "<p>date: " + d + "</h1>"
