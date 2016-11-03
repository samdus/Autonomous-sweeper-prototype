import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';



class MapContainer extends Component {
  constructor(props) {
    super(props);
    this.state = {showConsole: false};
    this.handleClick = this.handleClick.bind(this)
    this.handleClose = this.handleClose.bind(this)
  }
  handleClick(){
    this.setState({showConsole: true});
  }
  handleClose(){
    this.setState({showConsole: false});
  }
  render() {
        return (
            <div className="">
            THIS IS THE MAP
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