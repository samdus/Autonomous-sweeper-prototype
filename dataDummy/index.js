var mongojs = require('mongojs')
//For prod
//var db = mongojs("mongodb://mainapp:ygMs9GQ@ds049456.mlab.com:49456/domotique_manager");
//For dev
var db = mongojs("mongodb://127.0.0.1:3001/meteor");


function getInitialNumberData(){
    var now = Date.now();
    return [
        {statIdentifier:"leftWheel", statNiceName:"Left Wheel", statValue:0, statMesure:"rpm", history:[{value:0, createdAt:now}]},
        {statIdentifier:"rightWheel", statNiceName:"Right Wheel", statValue:0, statMesure:"rpm", history:[{value:0, createdAt:now}]},
        {statIdentifier:"kinectPolling", statNiceName:"Kinect Polling", statValue:0, statMesure:"poll/s", history:[{value:0, createdAt:now}]},
        {statIdentifier:"forcedStop", statNiceName:"Forced Stop", statValue:0, statMesure:"stops", history:[{value:0, createdAt:now}]},
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
    

/********** STRING STATISTICS ***************/
db.createCollection("statStringContainer", {}, function(error, value){
    console.log(error)
    console.log(value)
    db.collection('statStringContainer').remove({}, function(e, doc){
        db.collection('statStringContainer').insert(getInitialStringData(), function(error, value){
            setInterval(simulateTravelRoom, 6*1000);
        })
    });
});

/********** BOOLEAN STATISTICS **************/
db.createCollection("statBoolContainer", {}, function(error, value){
    db.collection('statBoolContainer').remove({}, function(e, doc){
        db.collection('statBoolContainer').insert(getInitialBoolData(), function(error, value){
            setInterval(simulateWorking, 7*1000);
        })
    });
});


/********** NUMBER STATISTICS **************/
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