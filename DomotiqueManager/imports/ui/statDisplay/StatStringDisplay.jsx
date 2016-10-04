import React, { Component, PropTypes } from 'react';
import { createContainer } from 'meteor/react-meteor-data';
import ReactDOM from 'react-dom';

//Our date entry points
import { StatContainer } from '../../api/statStringContainer.js';


// Task component - represents a single todo item
class StatStringDisplay extends Component {
  render() {

     if (this.props.readyStat) {
      var elementTypeClass=this.props.statContainer.statIdentifier;
      return (
        <div className={"stringDisplay statBlockDisplay " + elementTypeClass}>
          <div className="statDisplayTitle">{this.props.statContainer.statNiceName}</div>
          <div className="statDisplayValue">{this.props.statContainer.statValue}</div>
        </div>
      );
     }else{
      return (
        <div className="statBlockDisplay loading">
          <div className="squareloader"></div>
        </div>
      );
     }
  }
}
export default createContainer(({ listenTo }) => {
  var handleSub=Meteor.subscribe('statStringContainer', listenTo);
  return {
    readyStat:handleSub.ready(),
    statContainer:StatContainer.findOne({statIdentifier: listenTo}, {history:0}),
  };
}, StatStringDisplay);