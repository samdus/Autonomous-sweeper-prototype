import React, { Component, PropTypes } from 'react';
import { createContainer } from 'meteor/react-meteor-data';
import ReactDOM from 'react-dom';

//Our date entry points
import { StatContainer } from '../../api/statBoolContainer.js';


// Task component - represents a single todo item
class StatBoolDisplay extends Component {
  render() {
     if (this.props.readyStat) {
      var elementTypeClass=this.props.statContainer.statIdentifier;
      var boolVal = this.props.statContainer.statValue;
      return (
        <div className={"boolDisplay statBlockDisplay " + elementTypeClass}>
          <div className="statDisplayTitle">{this.props.statContainer.statNiceName}</div>
          <div className={"statDisplayValue " + boolVal} ></div>
        </div>
      );

     }else{
      return (
        <div className="statBlockDisplay  loading">
          <div className="squareloader"></div>
        </div>
      );
     }
  }
}
export default createContainer(({ listenTo }) => {
  var handleSub=Meteor.subscribe('statBoolContainer', listenTo);
  return {
    readyStat:handleSub.ready(),
    statContainer:StatContainer.findOne({statIdentifier: listenTo}, {history:0}),
  };
}, StatBoolDisplay);