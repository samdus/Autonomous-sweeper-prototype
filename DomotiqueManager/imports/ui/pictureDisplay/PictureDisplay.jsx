import React, { Component, PropTypes } from 'react';
import { createContainer } from 'meteor/react-meteor-data';
import ReactDOM from 'react-dom';

//Our date entry points
import { FilesContainer } from '../../api/filesContainer.js';




// Task component - represents a single todo item
class PictureDisplay extends Component {
  constructor(props) {
      super()
   }

  render() {
     if (this.props.readyStat) {
      return (
        <div className={"pictureDisplay "}>
            <ul>
             {  Object.keys(this.props.filesContainer).map(function(key) {
                            return <li key={this.props.filesContainer[key]._id}> 
                                        <img className="pictureFromRobot"  src={this.props.filesContainer[key].link} />
                                  </li>;
                        }, this)
                    }
            </ul>
        </div>
      );

     }else{
      return (
        <div className="pictureDisplay  loading">
          <div className="squareloader"></div>
        </div>
      );
     }
  }
}
export default createContainer(() => {
  var handleSub=Meteor.subscribe('FilesContainer');
  return {
    readyStat:handleSub.ready(),
    filesContainer:FilesContainer.find({}).fetch(),
  };
}, PictureDisplay);