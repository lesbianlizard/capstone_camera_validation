//import * as zmq from 'jszmq';
document.body.innerHTML += "POSTing"

function listener()
{
  console.log(this.responseText);
}

var xhttp = new XMLHttpRequest();
//xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
xhttp.addEventListener("load", listener);
xhttp.open("GET", "http://localhost:8000", true);
xhttp.send(); 

let d = new Date();
document.body.innerHTML += "<p>date: " + d + "</h1>"
