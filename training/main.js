const synaptic = require('synaptic'); // this line is not needed in the browser

//Lets require/import the HTTP module
var http = require('http');

const Neuron = synaptic.Neuron;
const Layer = synaptic.Layer;
const Network = synaptic.Network;
const Trainer = synaptic.Trainer;
const Architect = synaptic.Architect;

// create the network
var inputLayer = new Layer(2);
var hiddenLayer = new Layer(3);
var outputLayer = new Layer(1);

inputLayer.project(hiddenLayer);
hiddenLayer.project(outputLayer);

var myNetwork = new Network({
    input: inputLayer,
    hidden: [hiddenLayer],
    output: outputLayer
});

// train the network
var learningRate = .3;
for (var i = 0; i < 20000; i++)
{
    // 0,0 => 0
    myNetwork.activate([0,0]);
    myNetwork.propagate(learningRate, [0]);

    // 0,1 => 1
    myNetwork.activate([0,1]);
    myNetwork.propagate(learningRate, [1]);

    // 1,0 => 1
    myNetwork.activate([1,0]);
    myNetwork.propagate(learningRate, [1]);

    // 1,1 => 0
    myNetwork.activate([1,1]);
    myNetwork.propagate(learningRate, [0]);
}

const EventEmitter = require('events');
class MyEmitter extends EventEmitter {}
const myEmitter = new MyEmitter();
myEmitter.on('test', () => {
  // test the network
  console.log(myNetwork.activate([0,0])); // [0.015020775950893527]
  console.log(myNetwork.activate([0,1])); // [0.9815816381088985]
  console.log(myNetwork.activate([1,0])); // [0.9871822457132193]
  console.log(myNetwork.activate([1,1])); // [0.012950087641929467]
});
myEmitter.on('pre-train', () => {

});
myEmitter.on('activate', () => {
  // Train the network in real-time feed from C++ program
   
});
myEmitter.on('real-time-training', () => {
  // TODO
});
myEmitter.on('save', () => {

});
myEmitter.on('load', () => {

});

//Lets define a port we want to listen to
const PORT=3001; 

//We need a function which handles requests and send response
function handleRequest(request, response){

    if(request.url === '/test')
    {
      console.log('test');
      myEmitter.emit('test');
    }

    response.end('It Works!! Path Hit: ' + request.url);
}

//Create a server
var server = http.createServer(handleRequest);

//Lets start our server
server.listen(PORT, function(){
    //Callback triggered when server is successfully listening. Hurray!
    console.log("Server listening on: http://localhost:%s", PORT);
});
