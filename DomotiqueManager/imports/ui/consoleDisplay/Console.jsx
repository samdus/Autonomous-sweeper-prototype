import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';

import { ConsoleContainer } from '../../api/consoleContainer.js';
 


/*
graph d3 

bideo https://www.pubnub.com/blog/2015-08-27-webrtc-live-video-stream-broadcasting-from-one-to-many/
http://www.magdiblog.fr/divers/raspberry-pi-camera-5-facons-de-faire-du-streaming/

eta deconecter
*/
class Console extends Component {
  constructor(props) {
    super(props);
    this.handleReset = this.handleReset.bind(this)
  }
  componentWillMount(){
    //this.props.backLogs = _.uniq(this.props.backLogs);
  }
  componentWillUpdate(nextProps) {
   
    if(nextProps.dataReady && nextProps.consoleContainer){
      //ass arrat
      /*if(this.props.backLogs.length > 0){
        if(this.props.backLogs[this.props.backLogs.length - 1]._id._str != nextProps.consoleContainer._id._str){
           this.props.backLogs.push(nextProps.consoleContainer)
        }
      }else{
         this.props.backLogs.push(nextProps.consoleContainer)
      }*/
      //as object
      nextProps.consoleContainer.map(function(message) {
        console.log(message);
        this.props.backLogs[message._id] = message;
      }, this);
      

    }

    if(this.props.dataReady){
      var consolewindow = this.refs.consolewindow;
      this.shouldScrollBottom = consolewindow.scrollTop + consolewindow.offsetHeight === consolewindow.scrollHeight;
    }
    
  }
  componentDidUpdate() {
    
     if (this.props.dataReady && this.shouldScrollBottom) {
      var consolewindow = this.refs.consolewindow;
      consolewindow.scrollTop = consolewindow.scrollHeight
    }
  }
  getTimeString(timestamp){
    var time= new Date(timestamp)
    return '' + time.getHours() + ':' + time.getMinutes() + ':' + time.getSeconds()
  }
  handleReset(){
     this.props.backLogs.splice(0,this.props.backLogs.length)
     this.forceUpdate()
  }
  render() {
     if (this.props.dataReady) {
      return (
        <div className="modal-wrapper">
          <div className="overlay-modal"  onClick={this.props.onClose}></div>
            <div className={"console-wrapper modal-box"}>
             <div className="modal-title">Vaccum console</div>
              <img className="close icon" src="/icon/close.svg" alt="close" title="Return to main panel" onClick={this.props.onClose} />

              <div className="console" ref="consolewindow">
              <ul>
                { 
                  Object.keys(this.props.backLogs).map(function(key) {
                    return <li key={this.props.backLogs[key]._id} className={this.props.backLogs[key].messageLevel}> <span className="timestamp">{this.getTimeString(this.props.backLogs[key].createdAt)}</span> : {this.props.backLogs[key].messageValue}</li>;
                  }, this)
    
                }
                <li><span className="blink"></span></li>
              </ul>
              </div>
              <div className="action-wrapper console-action-wrapper">
                <img src="/icon/reload.svg" className="resetConsole icon" alt="Chart" title="Reset console"  onClick={this.handleReset}/>
              </div>
          </div>
        </div>
      );
     }else{
      return (
        <div className="modal-wrapper">
          <div className="overlay-modal"  onClick={this.props.onClose}></div>
            <div className={"chartwrapper modal-box"}>
              <div className="modal-title">Vaccum console</div>
              <img className="close icon" src="/icon/close.svg" alt="close" title="Return to main panel" onClick={this.props.onClose} />
              <div className="loading">
                <div className="circleloader"></div>
              </div>
            </div>
        </div>
        
      );
     }
  }
}

Console.propTypes = {

};
 
Console.defaultProps  = {
      backLogs: {},
}

export default createContainer(() => {
  var handleSub=Meteor.subscribe('consoleContainer');
  return {
    dataReady:handleSub.ready(),
    consoleContainer:ConsoleContainer.find({}, {sort: {createdAt: -1, limit: 10}}).fetch()
  };
}, Console);