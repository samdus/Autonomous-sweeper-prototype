var mongojs = require('mongojs')
//For prod
var db = mongojs("mongodb://mainapp:ygMs9GQ@ds049456.mlab.com:49456/domotique_manager");
//For dev
//var db = mongojs("mongodb://127.0.0.1:3001/meteor");


function getInitialNumberData(){
    var now = Date.now();
    return [
        {statIdentifier:"leftWheel", statNiceName:"Left Wheel", statValue:0, statMesure:"%", history:[{value:0, createdAt:now}]},
        {statIdentifier:"rightWheel", statNiceName:"Right Wheel", statValue:0, statMesure:"%", history:[{value:0, createdAt:now}]},
        {statIdentifier:"kinectPolling", statNiceName:"Kinect Polling", statValue:0, statMesure:"poll/s", history:[{value:0, createdAt:now}]},
        {statIdentifier:"forcedStop", statNiceName:"Forced Stop", statValue:0, statMesure:"stops", history:[{value:0, createdAt:now}]},
        {statIdentifier:"distanceSonar", statNiceName:"Sonar distance", statValue:6, statMesure:"cm", history:[{value:0, createdAt:now}]},
        {statIdentifier:"mapIter", statNiceName:"Map update", statValue:0, statMesure:"updates", history:[{value:0, createdAt:now}]},
        {statIdentifier:"nbCommands", statNiceName:"Commands", statValue:0, statMesure:"in queue", history:[{value:0, createdAt:now}]},
    ]
}
/*
* todo check afficher photpo
*/

function getInitialMapData(){
    var now = Date.now();
    return [
        {createdAt:now, lines:[], robotX:0, robotY:0}
    ]
}
function getInitialConsoleData(){
    var now = Date.now();
    return [
        //Levels : warning, sucess, error, ''
        {messageValue:"Starting...", messageLevel:"info", createdAt:now},
    ]
}
function getInitialBoolData(){
    var now = Date.now();
    return [
        {statIdentifier:"working", statNiceName:"Working", statValue:false, history:[{value:true, createdAt:now}]}
    ]
}
function getInitialConsoleData(){
    var now = Date.now();
    return [
        //Levels : warning, sucess, error, '', info
        {messageValue:"Starting...", messageLevel:"...", createdAt:now},
    ]
}
function getInitialCommand(){
    var now = Date.now();
    return [
        //Levels : warning, sucess, error, ''
        {command:"goto",  // Manuel (stopacction goto close startdebug stopdebug scan turn x) Automatique (resume)
        gotoX:24, 
        gotoY:25, 
        createdAt:now},
    ]
}
/********** STRING STATISTICS ***************/
db.createCollection("consoleContainer", {}, function(error, value){
    db.collection('consoleContainer').remove({}, function(e, doc){
        db.collection('consoleContainer').insert(getInitialConsoleData(), function(error, value){
           
        });
      });
});

/********** MAPDATA ***************/
db.createCollection("mapContainer", {}, function(error, value){
    db.collection('mapContainer').remove({}, function(e, doc){
        db.collection('mapContainer').insert(getInitialMapData(), function(error, value){
      
        })
      });
});
/********** MAPDATA ***************/
db.createCollection("commandContainer", {}, function(error, value){
    db.collection('commandContainer').remove({}, function(e, doc){
        db.collection('commandContainer').insert(getInitialCommand(), function(error, value){

        });
      });
});
/********** STRING STATISTICS ****************
db.createCollection("statStringContainer", {}, function(error, value){
    console.log(error)
    console.log(value)
    db.collection('statStringContainer').remove({}, function(e, doc){
        db.collection('statStringContainer').insert(getInitialStringData(), function(error, value){
        });
    });
});*/

/********** BOOLEAN STATISTICS ***************/
db.createCollection("statBoolContainer", {}, function(error, value){
    db.collection('statBoolContainer').remove({}, function(e, doc){
        db.collection('statBoolContainer').insert(getInitialBoolData(), function(error, value){

        })
    });
});


/********** NUMBER STATISTICS ***************/
db.createCollection("statNumberContainer", {}, function(error, value){
    db.collection('statNumberContainer').remove({}, function(e, doc){
        db.collection('statNumberContainer').insert(getInitialNumberData(), function(error, value){

        })
    });
});
/*
setTimeout(function(){ simulateWorking(true)}, 1*1000);
*/
setTimeout(function(){ 
simulateConsole("Sending we new map iteration", "info")}
, 3*1000);

setTimeout(function(){ 
simulatemap1()}
, 4*1000);


setTimeout(function(){ 
simulateConsole("Sending we new map iteration", "info")}
, 5*1000);

setTimeout(function(){ 
simulatemap2()}
, 7*1000);

setTimeout(function(){ 
simulateConsole("Sending we new map iteration", "info")}
, 7*1000);

setTimeout(function(){ 
simulatemap3()}
, 10*1000)

setTimeout(function(){ 
simulateConsole("Awaiting command...", "")}
, 11*1000);


setTimeout(function(){ 
simulateConsole("Shutting down", "warning")}
, 17*1000);

function simulatemap1(){
    var now = Date.now();
    db.collection('mapContainer').insert([
        {createdAt:now, lines:[{
                                x1:0,
                                y1:0,
                                x2:50,
                                y2:0,
                                nbpts:5,
                                }], robotX:150, robotY:150}
    ], function(error, value){
      
    });
}
function simulatemap2(){
    var now = Date.now();
    db.collection('mapContainer').insert([
        {createdAt:now, lines:[{
                                x1:0,
                                y1:0,
                                x2:95,
                                y2:0,
                                nbpts:5,
                                },
                                {
                                x1:100,
                                y1:30,
                                x2:150,
                                y2:20,
                                nbpts:5,
                                }], robotX:150, robotY:150}
    ], function(error, value){
      
    });
}
function simulatemap3(){
    var now = Date.now();
    db.collection('mapContainer').insert([
        {createdAt:now, lines:[{
                                x1:0,
                                y1:0,
                                x2:95,
                                y2:0,
                                nbpts:5,
                                },
                                {
                                x1:100,
                                y1:30,
                                x2:150,
                                y2:20,
                                nbpts:5,
                                }
                                ,
                                {
                                x1:155,
                                y1:0,
                                x2:300,
                                y2:0,
                                nbpts:5,
                                }], robotX:150, robotY:150}
    ], function(error, value){
      
    });
}
/*
setTimeout(function(){ simulateWorking(true)}, 1*1000);
setInterval(simulateKinect, 4* 1000);
setInterval(simulateMapUpdates, 4* 1000);
setTimeout(function(){ simulateWheels(10, 10)}, 1*1000);
setTimeout(function(){ simulateWheels(100, 100)}, 3*1000);
setTimeout(function(){ simulateTravel()}, 3*1000);
setTimeout(function(){ simulateWheels(50, 100)}, 6*1000);
setTimeout(function(){ simulateWheels(75, 100)}, 8*1000);
setTimeout(function(){ simulateWheels(50, 100)}, 10*1000);
setTimeout(function(){ simulateWheels(25, 100)}, 12*1000);
setTimeout(function(){ simulateWheels(50, 100)}, 14*1000);*/
function simulateWheels(left, right){
    var now = Date.now();

    /*Live value update*/
    db.collection('statNumberContainer').update({statIdentifier: "leftWheel"}, {$set:{statValue:left}, $push:{history:{value:left, createdAt:now}}}, function (e, doc) {
    });
    db.collection('statNumberContainer').update({statIdentifier: "rightWheel"},  {$set:{statValue:right}, $push:{history:{value:right, createdAt:now}}}, function () {
    });
}


function simulateKinect(){
   var now = Date.now();
    kinectPollingstatValue = Math.floor((Math.random() * 3) + 1);
    /*Live value*/
    db.collection('statNumberContainer').update({statIdentifier: "kinectPolling"},  {$set:{statValue:kinectPollingstatValue}, $push:{history:{value:kinectPollingstatValue, createdAt:now}}}, function () {
    });
}

function simulateTravel(){
    var now = Date.now();

     /*Live value*/
    db.collection('statNumberContainer').update({statIdentifier: "forcedStop"},  {$inc:{statValue:1}}, function () {
    });

}

function simulateMapUpdates(){
    var now = Date.now();

     /*Live value*/
    db.collection('statNumberContainer').update({statIdentifier: "mapIter"},  {$inc:{statValue:1}}, function () {
    });

}
function simulateTravelRoom(){
    var now = Date.now();
    var rooms= ['Kitchen', 'Living Room', 'Bathroom'];
    currentlyInstatValue= rooms[Math.floor(Math.random()*rooms.length)]
    /*Live value*/
    db.collection('statStringContainer').update({statIdentifier: "currentlyIn"},  {$set:{statValue:currentlyInstatValue}, $push:{history:{value:currentlyInstatValue, createdAt:now}}}, function () {
    });
}
function simulateWorking(value){
    var now = Date.now();
    /*Live value*/
    db.collection('statBoolContainer').update({statIdentifier: "working"},  {$set:{statValue:value}, $push:{history:{value:value, createdAt:now}}}, function () {
    });
}


function simulateConsole(message, level){
    var now = Date.now();
    db.collection('consoleContainer').insert(  {messageValue:message, messageLevel:level, createdAt:now}, function(error, value){
        console.log(error || value)
    })
}

function simulateCommand(){
   var now = Date.now();
    db.collection('commandContainer').insert({command:"goto",  gotoX:24, gotoY:25, createdAt:now}, function(error, value){
        console.log(error || value)
    });

}