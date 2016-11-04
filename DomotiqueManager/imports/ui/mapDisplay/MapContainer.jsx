import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';



class MapContainer extends Component {
  constructor(props) {
    super(props);

  }
  componentDidMount(){

    var robotMapWidth = 200;
    var robotMapHeight = 200;
    var actualWidth = 0;
    var actuelHeight = 0;
    /*Get our wrapper and acual canvas*/
    var mapCanvas=document.getElementById("mapCanvas");
    var wrapper =document.getElementById("mapCanvas").parentNode;
    var ctx = mapCanvas.getContext("2d");

    /*Set the dimensions of the canvas to match its parent*/
    actualWidth = ctx.canvas.width  = wrapper.offsetWidth;
    actuelHeight = ctx.canvas.height = wrapper.offsetHeight;


    /*Draw a simple triangle*/
    ctx.beginPath();
    ctx.moveTo(this.getRealPointX(150, robotMapWidth, actualWidth) ,this.getRealPointY(100, robotMapHeight, actuelHeight));
    ctx.lineTo(this.getRealPointX(200, robotMapWidth, actualWidth) ,this.getRealPointY(150, robotMapHeight, actuelHeight));
    ctx.lineTo(this.getRealPointX(200, robotMapWidth, actualWidth) ,this.getRealPointY(25, robotMapHeight, actuelHeight));
    ctx.fill();
  }
  getRealPointX(pointRobot, robotMapWidth ,mapWidth){
    return pointRobot*(mapWidth/robotMapWidth);
  }
  getRealPointY(pointRobot, robotMapHeight,mapHeight){
    return pointRobot*(mapHeight/robotMapHeight);
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