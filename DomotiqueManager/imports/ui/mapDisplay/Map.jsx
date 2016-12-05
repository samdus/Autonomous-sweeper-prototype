import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';
import { MapContainer } from '../../api/mapContainer.js';
import { CommandContainer } from '../../api/commandContainer.js';

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
var d;
class Map extends Component {

   constructor(props) {
    super(props);
      this.draw = this.draw.bind(this)
      this.refreshMap = this.refreshMap.bind(this)
      this.calculate = this.calculate.bind(this)
      this.clickCanvas = this.clickCanvas.bind(this)
      this.onMouseEnterHandler = this.onMouseEnterHandler.bind(this)
      this.onMouseLeaveHandler = this.onMouseLeaveHandler.bind(this)
      this.onMouseMoveHandler = this.onMouseMoveHandler.bind(this)
   }

  componentWillUnmount() {
      window.removeEventListener("resize", this.refreshMap);
  }

  componentDidMount(){
    window.addEventListener("resize", this.refreshMap);
    this.refreshMap();
  }

  clickCanvas(e){
     var now = Date.now();
    var mapCanvas=document.getElementById("mapCanvas");
    var rect = mapCanvas.getBoundingClientRect();
    var ctx = mapCanvas.getContext("2d")
    var x = e.clientX - rect.left;
    var y = e.clientY - rect.top;
    robotClickX = (x/scaleX)-cadrantFixX;
    robotClickY = (y/scaleY)-cadrantFixY;
    CommandContainer.insert({command:"goto", goToX: Math.round(robotClickX), goToY: Math.round(robotClickY),createdAt:now})
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
    if(this.props.mapContainer.robotX > maxX){
      maxX = this.props.mapContainer.robotX;
    }else if(this.props.mapContainer.robotX < minX){
      minX = this.props.mapContainer.robotX;
    }

    if(this.props.mapContainer.robotY > maxY){
      maxY = this.props.mapContainer.robotY;
    }else if(this.props.mapContainer.robotY < minY){
      minY = this.props.mapContainer.robotY;
    }


    cadrantFixX = 0;
    cadrantFixY = 0;
    if(minX<0){
      cadrantFixX = Math.abs(minX);
    }
    if(minY<0){
      cadrantFixY = Math.abs(minY);
    }

    //Map width and height from the robot
    robotMapWidth = Math.abs(maxX - minX)  //  maxX + cadrantFixX;
    robotMapHeight = Math.abs(maxY - minY) //maxY + cadrantFixY;



    //Canvas width and height
     actuelHeight = 0;
     actualWidth = 0;

    /*Get our wrapper and acual canvas*/
    var maxCanvasWidth =  wrapper.offsetWidth;
    var maxCanvasHeight =  wrapper.offsetHeight;
    

    if(robotMapWidth > robotMapHeight){
      actualWidth = ctx.canvas.width  = wrapper.offsetWidth;
      actuelHeight = ctx.canvas.height = (robotMapHeight * wrapper.offsetWidth) / robotMapWidth;
    }else{
      actuelHeight = ctx.canvas.height  = wrapper.offsetHeight;
      actualWidth = ctx.canvas.width = (robotMapWidth * wrapper.offsetHeight) / robotMapHeight;
    }

    /*if(actuelHeight > maxCanvasHeight){


      var resizeRatio = maxCanvasHeight  / actuelHeight;

      actuelHeight = ctx.canvas.height = actuelHeight * resizeRatio;
      actualWidth = ctx.canvas.width = actualWidth * resizeRatio;
      actualWidth = ctx.canvas.width  = (Math.abs((maxX - minX)) * actuelHeight /  Math.abs((maxY - minY)))
     
      
    }*/


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
    ctx.fillRect(((this.props.mapContainer.robotX+cadrantFixX)*scaleX) - 5 , ((this.props.mapContainer.robotY+cadrantFixY)*scaleY) -5 ,10,10);
    //Relative 0,0
    ctx.fillStyle="#0000FF";
    ctx.fillRect((0+cadrantFixX)*scaleX-3, (0+cadrantFixY)*scaleY-3,5,5);
    //Position last click
    if(robotClickX != -1){
      ctx.fillStyle="#FFFF00";
      ctx.fillRect((robotClickX+cadrantFixX)*scaleX -5, (robotClickY+cadrantFixY)*scaleY -5,10,10);
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
  onMouseEnterHandler() {
      d = document.getElementById("hoverCoordWrapper");
      d.className = "active";
  }
  onMouseLeaveHandler() {
      d.className = "";
  }
  onMouseMoveHandler(e){
    var mapCanvas=document.getElementById("mapCanvas");
    var rect = mapCanvas.getBoundingClientRect();
    var ctx = mapCanvas.getContext("2d")
    var x = e.clientX - rect.left;
    var y = e.clientY - rect.top;
    robotClickX = (x/scaleX)-cadrantFixX;
    robotClickY = (y/scaleY)-cadrantFixY;
    var d = document.getElementById("hoverCoordWrapper");
    d.innerHTML = Math.round(robotClickX) + ", " + Math.round(robotClickY);
  }
  render() {
    if (this.props.dataReady) {
      return (
            <div className="canvas-wrapper" id="canvawrapper">
              <span id="hoverCoordWrapper"></span>
              <canvas id="mapCanvas" width="300" height="150" onMouseMove={this.onMouseMoveHandler}  onClick={this.clickCanvas} onMouseEnter={this.onMouseEnterHandler} onMouseLeave={this.onMouseLeaveHandler}/>
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
  var handleSub=Meteor.subscribe('commandContainer');
  return {
    dataReady:handleSub.ready(),
    mapContainer:MapContainer.findOne({}, {sort: {createdAt: -1}, limit:1 })
  };
}, Map);