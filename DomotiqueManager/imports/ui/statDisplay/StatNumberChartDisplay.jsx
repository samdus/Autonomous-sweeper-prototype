import React, { Component, PropTypes } from 'react';
import { createContainer } from 'meteor/react-meteor-data';
import ReactDOM from 'react-dom';
import { StatContainer } from '../../api/statNumberContainer.js';
var LineChart = require("react-chartjs").Line;
var currentNumbStatShown = 0;
var currentChartId = 0
// Task component - represents a single todo item
class StatNumberChartDisplay extends Component {
  constructor(props) {
    super(props);
    this.updateChart = this.updateChart.bind(this)
    this.handleReset = this.handleReset.bind(this)
    currentNumbStatShown=1;
  }
  handleReset(){
    currentNumbStatShown=1;
    /*Erase the data*/
    this.props.mylabels.splice(0,this.props.mylabels.length);
    this.props.mydataset[0].data.splice(0,this.props.mydataset[0].data.length);
    /*Force remounting of the graph by changing its id/key*/
    currentChartId = currentChartId+1;
    this.props.readyStat = false;
    this.forceUpdate();
  }
  componentDidMount() {		
    this.updateChart(this.props, this);

  }
  componentWillUpdate(nextProps) {
    if(this.props.statContainer && nextProps.statContainer &&
       this.props.statContainer.history.length != nextProps.statContainer.history.length){
          this.updateChart(nextProps, this);
      }
  }
 updateChart(props, node) {
   /*Reset our data arrays*/
   props.readyStat = true;
   props.mylabels.splice(0,props.mylabels.length)
   props.mydataset[0].data.splice(0,props.mydataset[0].data.length)
   /*Iterator setter*/
   if(currentNumbStatShown < props.maxShownValue && currentNumbStatShown < props.statContainer.history.length){
      currentNumbStatShown = currentNumbStatShown +1;
   }
   for(var i=props.statContainer.history.length-currentNumbStatShown; i < props.statContainer.history.length; i++){
     let theTime = new Date(props.statContainer.history[i].createdAt);
     props.mylabels.push(theTime.getMinutes()+":"+theTime.getSeconds())
     props.mydataset[0].data.push(props.statContainer.history[i].value);
   }
 }

  render() {
     if (this.props.readyStat) {
      return (
        <div>
              <LineChart data={{datasets: this.props.mydataset, labels: this.props.mylabels}} 
                         width="800" 
                         height="400"
                         options={this.props.chartOptions}  id="testId"  key={"chart"+currentChartId} />
              <div className="action-wrapper">
                <img src="/icon/reload.svg" className="resetToLive icon" alt="Chart" title="Reset to live values"  onClick={this.handleReset}/>
              </div>
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

StatNumberChartDisplay.defaultProps  = {
      mylabels: [],
      mydataset:  [{
                    label: "Historical data",
                    fill:false,
                    fillColor: "rgba(220,220,220,0.2)",
                    strokeColor: "rgba(220,220,220,1)",
                    pointColor: "rgba(220,220,220,1)",
                    pointStrokeColor: "#fff",
                    pointHighlightFill: "#fff",
                    pointHighlightStroke: "rgba(220,220,220,1)",
                    data: []
                }],
      chartOptions:{
        responsive: true,
        animation: false,
        maintainAspectRatio: true,
        animationEasing:"easeInSine",
        showLines:false,
        pointHitDetectionRadius:1
      },
      maxShownValue:20,
      currentShownValue:1,

}


export default createContainer(({ listenTo }) => {
  var handleSub=Meteor.subscribe('statNumberContainer', listenTo);
  return {
    readyStat:handleSub.ready(),
    statContainer:StatContainer.findOne({statIdentifier: listenTo}, {history:{$slice:10}}),
  };
}, StatNumberChartDisplay);

