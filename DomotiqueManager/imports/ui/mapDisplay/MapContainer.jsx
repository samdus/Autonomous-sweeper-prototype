import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';

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
class MapContainer extends Component {
  constructor(props) {
    super(props);

      this.draw = this.draw.bind(this)
   }


    componentWillMount() {

        
    }
    componentWillUnmount() {
        window.removeEventListener("resize", this.draw);
    }

  draw(){
    var mapCanvas=document.getElementById("mapCanvas");
    var wrapper =document.getElementById("mapCanvas").parentNode;
    var ctx = mapCanvas.getContext("2d");
    //Erase the surface
    ctx.clearRect(0, 0, mapCanvas.width, mapCanvas.height);


    var minX = minY =9999999;
    var maxX= maxY =(-Number.MAX_VALUE);
    robotMap.map( function(item) {
      if(item.x1 < minX)
        minX = item.x1
      if(item.x2 < minX)
        minX = item.x2
      if(item.y1 < minY)
        minY = item.y2
      if(item.y2 < minY)
        minY = item.y2

      if(item.x1 > maxX)
        maxX = item.x1
      if(item.x2 > maxX)
        maxX = item.x2
      if(item.y1 > maxY)
        maxY = item.y2
      if(item.y2 > maxY)
        maxY = item.y2
    })
    var cadrantFixX = 0;
    var cadrantFixY = 0;
    if(minX<0){
      cadrantFixX = Math.abs(minX);
    }
    if(minY<0){
      cadrantFixY = Math.abs(minY);
    }
    //Map width and height from the robot
    var robotMapWidth = maxX + cadrantFixX;
    var robotMapHeight = maxY + cadrantFixY;
    //Canvas width and height
    var actuelHeight = 0;
    var actualWidth = 0;

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
    var scaleX = actualWidth / robotMapWidth;
    var scaleY = actuelHeight / robotMapHeight;
    /*Draw a simple triangle*/
    ctx.strokeStyle = '#ff0000';
    ctx.lineWidth = 2;
    robotMap.map( function(item) {
      ctx.beginPath();
      ctx.moveTo((item.x1+cadrantFixX)*scaleX, (item.y1+cadrantFixY)*scaleY);
      ctx.lineTo((item.x2+cadrantFixX)*scaleX, (item.y2+cadrantFixY)*scaleY);
      ctx.stroke();
    })
    ctx.fillStyle="#FFFFFF";
    //position robot?
    ctx.fillRect(cadrantFixX,cadrantFixY,15,15);

  }

  componentDidMount(){
    window.addEventListener("resize", this.draw);
    this.draw();
  }
  render() {
        return (

            <div className="canvas-wrapper" id="canvawrapper">

              <canvas id="mapCanvas" width="300" height="150" />
            </div>
      );
  }
}

MapContainer.propTypes = {

};
 
export default createContainer(() => {
  return {
  };
}, MapContainer);

/*
    var arrayX = robotMap.map(function(item){return item.x1}).concat(robotMap.map(function(item){return item.x2}));
    var arrayY = robotMap.map(function(item){return item.y1}).concat(robotMap.map(function(item){return item.y2}));
    var minXtest = Math.min.apply( null, arrayX );
    var maxXtest = Math.max.apply( null, arrayX );
    var minYtest = Math.min.apply( null, arrayY );
    var maxYtest = Math.max.apply( null, arrayY );
 */