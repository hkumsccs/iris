const synaptic = require('synaptic'); // this line is not needed in the browser
// train the network
var learningRate = 0.1;
//Create a server
//Lets require/import the HTTP module
var http = require('http');
var server = http.createServer(handleRequest);
var csv = require('csv-parser')
var fs = require('fs')


const Neuron = synaptic.Neuron;
const Layer = synaptic.Layer;
const Network = synaptic.Network;
const Trainer = synaptic.Trainer;
const Architect = synaptic.Architect;

// create the network
var inputLayer = new Layer(30);
var hiddenLayer1 = new Layer(10);
var hiddenLayer2 = new Layer(20);
var outputLayer = new Layer(4);

var myNetwork = new Architect.Perceptron(30, 150, 4);
var trainer = new Trainer(myNetwork);

var emotion = {
  "1": [1, 0, 0, 0],
  "2": [0, 1, 0, 0],
  "3": [0, 0, 1, 0],
  "4": [0, 0, 0, 1] 
};

inputLayer.project(hiddenLayer1);
hiddenLayer1.project(outputLayer);
//hiddenLayer2.project(outputLayer);

/*
var myNetwork = new Network({
    input: inputLayer,
    hidden: [hiddenLayer1],
    output: outputLayer
});
*/

const EventEmitter = require('events');
class MyEmitter extends EventEmitter {}
const myEmitter = new MyEmitter();
myEmitter.on('test', () => {
  // test the network
  //console.log(myNetwork.activate([0,0])); // [0.015020775950893527]
  //console.log(myNetwork.activate([0,1])); // [0.9815816381088985]
  //console.log(myNetwork.activate([1,0])); // [0.9871822457132193]
  //console.log(myNetwork.activate([1,1])); // [0.012950087641929467]
});
myEmitter.on('pre-train', () => {
  // Load csv
  var step = 0;
  var trainingSet = [];
  fs.createReadStream('./training.csv')
  .pipe(csv(['featureVector', 'fileName', 'emotionType']))
  .on('data', function (data) {
    //console.log('fv: %s fn: %s', data.featureVector, data.fileName);
    var inputNodes = data.featureVector.split(";");
    //myNetwork.activate(inputNodes);
    //myNetwork.propagate(learningRate, emotion[data.emotionType]);

    trainingSet.push({
      input: inputNodes,
      output: emotion[data.emotionType]
    });

    console.log(step++, emotion[data.emotionType]);
  })
  .on('end', function(){
    trainer.train(trainingSet, {
      rate: .001,
      iterations: 1,
      error: .8,
      shuffle: true,
      log: 10,
      cost: Trainer.cost.CROSS_ENTROPY
    });
    console.log('Finished')
  });
});
myEmitter.on('activate', (body) => {
  // Train the network in real-time feed from C++ program
  var inputVector = JSON.parse(body).featureVector.split(';');
  console.log('-------Result--------');
  console.log(inputVector);
  console.log(myNetwork.activate(inputVector));
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

    if(request.method === 'POST')
    {
      var body = [];
      if(request.url === '/activate') {
        //response.end(myNetwork.activate([]);
        request.on('data', function(chunk) {
          body.push(chunk);
        }).on('end', function() {
          body = Buffer.concat(body).toString();
          myEmitter.emit('activate', body); 
          response.end('Finished ...');
        });
      }
    }

    if(request.url === '/test'){
      console.log('test');
      myEmitter.emit('test');
      response.end('It Works!! Path Hit: ' + request.url);
    }

    if(request.url === '/load2propagate'){
      myEmitter.emit('pre-train');
      response.end('It Works!! Path Hit: ' + request.url);
    }

    if(request.url === '/save') {
      myEmitter.emit('save');
    }

    if(request.url === '/load') {
      // TODO Load the network 
    }

}

//Lets start our server
server.listen(PORT, function(){
    //Callback triggered when server is successfully listening. Hurray!
    console.log("Server listening on: http://localhost:%s", PORT);
});
