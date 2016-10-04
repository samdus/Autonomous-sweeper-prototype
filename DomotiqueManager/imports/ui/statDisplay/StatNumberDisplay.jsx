import React, { Component, PropTypes } from 'react';
import { createContainer } from 'meteor/react-meteor-data';
import ReactDOM from 'react-dom';

//Our date entry points
import { StatContainer } from '../../api/statNumberContainer.js';
import StatNumberChartDisplay from './StatNumberChartDisplay.jsx';



// Task component - represents a single todo item
class StatNumberDisplay extends Component {
  constructor(props) {
        super(props);
        this.state = {showGraph: false};
        this.handleClick = this.handleClick.bind(this)
        this.handleClose = this.handleClose.bind(this)
   }
  handleClick(){
     if(this.props.hasGraph)
        this.setState({showGraph: !this.state.showGraph});
  }
  handleClose(){
    if(this.props.hasGraph)
        this.setState({showGraph: false});
  }
  render() {
     if (this.props.readyStat) {
      var elementTypeClass=this.props.statContainer.statIdentifier;
      return (
        <div className={"numberDisplay statBlockDisplay " + elementTypeClass}>
          <div className="statDisplayTitle">{this.props.statContainer.statNiceName}</div>
          <div className="statDisplayValue">{this.props.statContainer.statValue}<span className="statMesure">{this.props.statContainer.statMesure}</span></div>
            { this.props.hasGraph && 
              this.state.showGraph && 
              <StatNumberChartDisplay listenTo={this.props.listenTo} 
              onClose={this.handleClose} /> 
            }
            <div className="action-wrapper">
            { this.props.hasGraph && 
              <img src="/icon/stats-dots.svg" className="showGraphToggle icon" alt="Chart" title="Show Chart"  onClick={this.handleClick}/>
            }
          </div>
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
  var handleSub=Meteor.subscribe('statNumberContainer', listenTo);
  return {
    readyStat:handleSub.ready(),
    listenTo:listenTo,
    statContainer:StatContainer.findOne({statIdentifier: listenTo}, {fields:{history:0}}),
  };
}, StatNumberDisplay);