import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';

//Our Ui components
import StatNumberDisplay from './statDisplay/StatNumberDisplay.jsx';
import StatStringDisplay from './statDisplay/StatStringDisplay.jsx';
import StatBoolDisplay from './statDisplay/StatBoolDisplay.jsx';
import ContainerManager from './ContainerManager.jsx';


/*
graph d3 

bideo https://www.pubnub.com/blog/2015-08-27-webrtc-live-video-stream-broadcasting-from-one-to-many/
http://www.magdiblog.fr/divers/raspberry-pi-camera-5-facons-de-faire-du-streaming/

eta deconecter
*/
class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      vaccumConnected: false,
    };
  }

  render() {
        return (
        <div className="container">
          <header className="appHeader">
            <h1>Vaccum cleaner</h1>
            <ContainerManager/>
          </header>
          <div className="statPanel">
              <StatNumberDisplay listenTo="leftWheel" hasGraph={true} />
              <StatNumberDisplay listenTo="rightWheel" hasGraph={true}/>
              <StatNumberDisplay listenTo="kinectPolling" hasGraph={true}/>
              <StatNumberDisplay listenTo="distanceSonar" hasGraph={true}/>
              <StatNumberDisplay listenTo="nbCommands" hasGraph={true}/>
              <StatNumberDisplay listenTo="forcedStop" />
              <StatNumberDisplay listenTo="mapIter" />
              <StatBoolDisplay listenTo="working" />
          </div>
        </div>
      );
  }
}

App.propTypes = {

};
 
export default createContainer(() => {
  return {
  };
}, App);