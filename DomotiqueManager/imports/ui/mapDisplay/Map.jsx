import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';
import { MapContainer } from '../../api/mapContainer.js';
 
//vector<segment> 

var robotMap = [{
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
},]
//Map width and height from the robot
var robotMapWidth ;
var robotMapHeight;

var actuelHeight = 0;
var actualWidth = 0;
var cadrantFixX = 0;
var cadrantFixY = 0;
var scaleX = 0;
var scaleY = 0;
var robotClickX=-1;
var robotClickY=-1;
class Map extends Component {

   constructor(props) {
    super(props);
      this.draw = this.draw.bind(this)
      this.refreshMap = this.refreshMap.bind(this)
      this.calculate = this.calculate.bind(this)
      this.clickCanvas = this.clickCanvas.bind(this)
   }

  componentWillUnmount() {
      window.removeEventListener("resize", this.refreshMap);
  }

  componentDidMount(){
    window.addEventListener("resize", this.refreshMap);
    this.refreshMap();
  }

  clickCanvas(e){
    var mapCanvas=document.getElementById("mapCanvas");
    var rect = mapCanvas.getBoundingClientRect();
    var ctx = mapCanvas.getContext("2d")
    var x = e.clientX - rect.left;
    var y = e.clientY - rect.top;
    robotClickX = (x/scaleX)-cadrantFixX;
    robotClickY = (y/scaleY)-cadrantFixY;
    this.draw();
  }

  calculate(){
    var mapCanvas=document.getElementById("mapCanvas");
    var wrapper =document.getElementById("mapCanvas").parentNode;
    var ctx = mapCanvas.getContext("2d");
    var minX = minY = 9999999;
    var maxX= maxY = (-Number.MAX_VALUE);
    this.props.mapContainer.lines.map( function(item) {
      
      if(item.x1 < minX) minX = item.x1;
      if(item.x2 < minX) minX = item.x2;
      if(item.y1 < minY) minY = item.y2;
      if(item.y2 < minY) minY = item.y2;

      if(item.x1 > maxX) maxX = item.x1;
      if(item.x2 > maxX) maxX = item.x2;
      if(item.y1 > maxY) maxY = item.y2;
      if(item.y2 > maxY) maxY = item.y2;

    })
    cadrantFixX = 0;
    cadrantFixY = 0;
    if(minX<0){
      cadrantFixX = Math.abs(minX);
    }
    if(minY<0){
      cadrantFixY = Math.abs(minY);
    }
    //Map width and height from the robot
    robotMapWidth = maxX + cadrantFixX;
    robotMapHeight = maxY + cadrantFixY;

    //Canvas width and height
     actuelHeight = 0;
     actualWidth = 0;

    /*Get our wrapper and acual canvas*/
    var maxCanvasWidth =  wrapper.offsetWidth;
    var maxCanvasHeight =  wrapper.offsetHeight;
    

    actualWidth = ctx.canvas.width  = wrapper.offsetWidth;
    actuelHeight = ctx.canvas.height = (robotMapHeight / robotMapWidth) * actualWidth;
    if(actuelHeight > maxCanvasHeight){
      var resizeRatio = maxCanvasHeight  / actuelHeight;
      actuelHeight = ctx.canvas.height = actuelHeight * resizeRatio;
      actualWidth = ctx.canvas.width  = (robotMapWidth / robotMapHeight) * actuelHeight;
    }
    scaleX = actualWidth / robotMapWidth;
    scaleY = actuelHeight / robotMapHeight;
  }


  draw(){
    var mapCanvas=document.getElementById("mapCanvas");
    var wrapper =document.getElementById("mapCanvas").parentNode;
    var ctx = mapCanvas.getContext("2d");
    //Erase the surface
    ctx.clearRect(0, 0, mapCanvas.width, mapCanvas.height);

    /*Draw all lines*/
    ctx.strokeStyle = '#ff0000';
    ctx.lineWidth = 2;
    this.props.mapContainer.lines.map( function(item) {
      ctx.beginPath();
      ctx.moveTo((item.x1+cadrantFixX)*scaleX, (item.y1+cadrantFixY)*scaleY);
      ctx.lineTo((item.x2+cadrantFixX)*scaleX, (item.y2+cadrantFixY)*scaleY);
      ctx.stroke();
    })
    ctx.fillStyle="#FFFFFF";

    //position robot?
    //ctx.fillRect(cadrantFixX,cadrantFixY,15,15);

    //Position last click
    if(robotClickX != -1){
      ctx.fillStyle="#FFFF00";
      ctx.fillRect((robotClickX+cadrantFixX)*scaleX, (robotClickY+cadrantFixY)*scaleY ,15,15);
    }

  }
 componentWillUpdate(nextProps) {
    if(nextProps.dataReady){
     
    }
  }
  componentDidUpdate(){
    this.refreshMap();
  }
  refreshMap(){
    if(this.props.dataReady && this.props.mapContainer && this.props.mapContainer.lines.length >0){
       this.calculate();
       this.draw();
    }
   
  }
  render() {
    if (this.props.dataReady) {
      return (
            <div className="canvas-wrapper" id="canvawrapper">
              <canvas id="mapCanvas" width="300" height="150"  onClick={this.clickCanvas}/>
            </div>
      );
    }else{
      return(
        <div className="loading">
                <div className="circleloader"></div>
        </div>
      );
    }
      
  }
}

Map.propTypes = {

};
 

export default createContainer(() => {
  var handleSub=Meteor.subscribe('mapContainer');
  return {
    dataReady:handleSub.ready(),
    mapContainer:MapContainer.findOne({}, {sort: {createdAt: 1}, limit:2 })
  };
}, Map);