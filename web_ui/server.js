//var cors = require('cors');
var http = require('http');
var server = http.createServer (
function(request,response)
{
  //response.writeHead(200,{"Content-Type":"text\plain"});
  response.setHeader('Access-Control-Allow-Origin', '*');

  if (request.method == "OPTIONS")
  {
    response.writeHead(200);
    response.end();
    console.log("recieved OPTIONS request");
    return;
  }
  else if(request.method == "GET")
  {
    response.end("received GET request.")
    console.log("received GET request.")
  }
  else if(request.method == "POST")
  {
    response.end("received POST request.");
    console.log("received POST request.")
  }
  else
  {
    response.end("Undefined request .");
  }
});

//server.options('*', cors());
//server.use(cors())

server.listen(8000);
console.log("Server running on port 8000");

