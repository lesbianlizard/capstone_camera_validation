//var cors = require('cors');
var http = require('http');
const zmq = require("zeromq")
host = "127.0.0.1"
port = "5555"
zmq_addr = "tcp://" + host + ":" + port

async function zmq_send_data(browser_request)
{
  const sock = new zmq.Request

  sock.connect(zmq_addr)
  console.log("ZMQ connected to " + zmq_addr)
  sock.send(browser_request)

  message = sock.receive()
  console.log(message)

  sock.disconnect(zmq_addr)
}

//run()

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
    response.writeHead(200,{"Content-Type":"text\plain"});
    response.end("received GET request.")
    console.log("received GET request.")
  }
  else if(request.method == "POST")
  {
    var post_data = ""
    request.on("data", function(data)
    {
      post_data += data
      //console.log("Recieved partial data: " + data)
    })
    request.on("end", function()
    {
      console.log("received POST request with data: " + post_data);
      zmq_send_data(post_data);
    })
    response.end("received POST request.")
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

