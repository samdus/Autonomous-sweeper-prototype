var mongojs = require('mongojs')
//For prod
var db = mongojs("mongodb://mainapp:ygMs9GQ@ds049456.mlab.com:49456/domotique_manager");
//For dev
//var db = mongojs("mongodb://127.0.0.1:3001/meteor");


function getInitialNumberData(){
    var now = Date.now();
    return [
        {statIdentifier:"leftWheel", statNiceName:"Left Wheel", statValue:0, statMesure:"rpm", history:[{value:0, createdAt:now}]},
        {statIdentifier:"rightWheel", statNiceName:"Right Wheel", statValue:0, statMesure:"rpm", history:[{value:0, createdAt:now}]},
        {statIdentifier:"kinectPolling", statNiceName:"Kinect Polling", statValue:0, statMesure:"poll/s", history:[{value:0, createdAt:now}]},
        {statIdentifier:"forcedStop", statNiceName:"Forced Stop", statValue:0, statMesure:"stops", history:[{value:0, createdAt:now}]},
    ]
}
/*
* todo check afficher photpo
*/

function getInitialMapData(){
    var now = Date.now();
    return [
        {createdAt:now, lines:[{
                                x1:150,
                                y1:100,
                                x2:200,
                                y2:25,
                                nbpts:5,
                                },
                                {
                                x1:200,
                                y1:25,
                                x2:200,
                                y2:150,
                                nbpts:5,
                                },
                                {
                                x1:200,
                                y1:150,
                                x2:150,
                                y2:100,
                                nbpts:5,
                                },
                                {
                                x1:50,
                                y1:75,
                                x2:150,
                                y2:175,
                                nbpts:5,
                                },
                                {
                                x1:0,
                                y1:0,
                                x2:150,
                                y2:101500,
                                nbpts:5,
                                },]
    , robotX:0, robotY:0}
    ]
}
function getInitialConsoleData(){
    var now = Date.now();
    return [
        //Levels : warning, sucess, error, ''
        {messageValue:"This is the first message of the simulation", messageLevel:"", createdAt:now},
    ]
}
function getInitialStringData(){
    var now = Date.now();
    return [
        {statIdentifier:"currentlyIn", statNiceName:"Curently In", statValue:"Kitchen", history:[{value:'Kitchen', createdAt:now}]},
    ]
}
function getInitialBoolData(){
    var now = Date.now();
    return [
        {statIdentifier:"working", statNiceName:"Working", statValue:true, history:[{value:true, createdAt:now}]}
    ]
}
function getInitialConsoleData(){
    var now = Date.now();
    return [
        //Levels : warning, sucess, error, '', info
        {messageValue:"This is the first message of the simulation", messageLevel:"", createdAt:now},
    ]
}
function getInitialCommand(){
    var now = Date.now();
    return [
        //Levels : warning, sucess, error, ''
        {command:"goTo",  // Manuel (stopacction goto close startdebug stopdebug scan turn x) Automatique (resume)
        goToX:24, 
        goToY:25, 
        createdAt:now},
    ]
}
/********** STRING STATISTICS ***************/
db.createCollection("consoleContainer", {}, function(error, value){
    db.collection('consoleContainer').remove({}, function(e, doc){
        db.collection('consoleContainer').insert(getInitialConsoleData(), function(error, value){
            setInterval(simulateConsole, 6*1000);
        });
      });
});

/********** MAPDATA ***************/
db.createCollection("mapContainer", {}, function(error, value){
    db.collection('mapContainer').remove({}, function(e, doc){
        /*db.collection('mapContainer').insert(getInitialMapData(), function(error, value){
      
        })*/
      });
});
/********** MAPDATA ***************/
db.createCollection("commandContainer", {}, function(error, value){
    db.collection('commandContainer').remove({}, function(e, doc){
        db.collection('commandContainer').insert(getInitialCommand(), function(error, value){
            setInterval(simulateCommand, 6*1000);
        });
      });
});
/********** STRING STATISTICS ***************
db.createCollection("statStringContainer", {}, function(error, value){
    console.log(error)
    console.log(value)
    db.collection('statStringContainer').remove({}, function(e, doc){
        db.collection('statStringContainer').insert(getInitialStringData(), function(error, value){
            setInterval(simulateTravelRoom, 6*1000);
        })
    });
});
*/
/********** BOOLEAN STATISTICS **************
db.createCollection("statBoolContainer", {}, function(error, value){
    db.collection('statBoolContainer').remove({}, function(e, doc){
        db.collection('statBoolContainer').insert(getInitialBoolData(), function(error, value){
            setInterval(simulateWorking, 7*1000);
        })
    });
});
*/

/********** NUMBER STATISTICS **************
db.createCollection("statNumberContainer", {}, function(error, value){
    db.collection('statNumberContainer').remove({}, function(e, doc){
        console.log('Simulating data from robot...');
        db.collection('statNumberContainer').insert(getInitialNumberData(), function(error, value){
            setInterval(simulateWheels,5*1000);
            //setInterval(simulateWheelsHistory, 5*1000);
            setInterval(simulateKinect, 4*1000);
            setInterval(simulateTravel, 5*1000);
        })
    });
});
*/

function simulateWheels(){
    var now = Date.now();
    leftstatValue = Math.floor((Math.random() * 10) + 1);
    rightstatValue = Math.floor((Math.random() * 10) + 1);
    /*Live value update*/
    db.collection('statNumberContainer').update({statIdentifier: "leftWheel"}, {$set:{statValue:leftstatValue}, $push:{history:{value:leftstatValue, createdAt:now}}}, function (e, doc) {
    });
    db.collection('statNumberContainer').update({statIdentifier: "rightWheel"},  {$set:{statValue:rightstatValue}, $push:{history:{value:rightstatValue, createdAt:now}}}, function () {
    });
}


function simulateKinect(){
   var now = Date.now();
    kinectPollingstatValue = Math.floor((Math.random() * 10) + 1);
    /*Live value*/
    db.collection('statNumberContainer').update({statIdentifier: "kinectPolling"},  {$set:{statValue:kinectPollingstatValue}, $push:{history:{value:kinectPollingstatValue, createdAt:now}}}, function () {
    });
}

function simulateTravel(){
    var now = Date.now();
    forcedStopstatValue = Math.floor((Math.random() * 10) + 1);

     /*Live value*/
    db.collection('statNumberContainer').update({statIdentifier: "forcedStop"},  {$inc:{statValue:1}, $push:{history:{value:forcedStopstatValue, createdAt:now}}}, function () {
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
function simulateWorking(){
    var now = Date.now();
    var values= [true, false];
    workingstatValue= values[Math.floor(Math.random()*values.length)]

    /*Live value*/
    db.collection('statBoolContainer').update({statIdentifier: "forcedStop"},  {$set:{statValue:workingstatValue}, $push:{history:{value:workingstatValue, createdAt:now}}}, function () {
    });
}

function simulateConsole(){
    var now = Date.now();
    var values= [['This is a message', ''],
                ['This is a sucess message', 'success'],
                ['This is an error message', 'error'],
                ['This is an info message', 'info'],
                ['This is a warning message', 'warning']];
   var currentMessage= values[Math.floor(Math.random()*values.length)];
    db.collection('consoleContainer').insert(  {messageValue:currentMessage[0], messageLevel:currentMessage[1], createdAt:now}, function(error, value){
        console.log(error || value)
    })
}

function simulateCommand(){
   var now = Date.now();
    db.collection('commandContainer').insert({command:"goTo",  goToX:24, goToY:25, createdAt:now}, function(error, value){
        console.log(error || value)
    });

}