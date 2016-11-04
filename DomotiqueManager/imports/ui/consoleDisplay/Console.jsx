import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';

import { ConsoleContainer } from '../../api/consoleContainer.js';
 

class Console extends Component {
  constructor(props) {
    super(props);
    this.handleReset = this.handleReset.bind(this)
  }

  componentWillMount(){
  }

  componentWillUpdate(nextProps) {
    if(nextProps.dataReady && nextProps.consoleContainer){
      if(this.props.dataReady){
        var consolewindow = this.refs.consolewindow;
        this.shouldScrollBottom = consolewindow.scrollTop + consolewindow.offsetHeight === consolewindow.scrollHeight;
      }
      nextProps.consoleContainer.map(function(message) {
        this.props.backLogs[message._id] = message;
      }, this);
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
            <div className="console" ref="consolewindow">
                <ul>
                    {  Object.keys(this.props.backLogs).map(function(key) {
                            return <li key={this.props.backLogs[key]._id} className={this.props.backLogs[key].messageLevel}> 
                                        <span className="timestamp">{this.getTimeString(this.props.backLogs[key].createdAt)}</span> : {this.props.backLogs[key].messageValue}
                                    </li>;
                        }, this)
                    }
                    <li><span className="blink"></span></li>
                </ul>
            </div>
      );
     }else{
      return (
            <div className="loading">
                <div className="circleloader"></div>
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
    consoleContainer:ConsoleContainer.find({}, {sort: {createdAt: 10}, limit:10 }).fetch()
  };
}, Console);