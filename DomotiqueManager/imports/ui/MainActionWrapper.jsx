import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';

//Our Ui components
import Console from './consoleDisplay/Console.jsx';

class MainActionWrapper extends Component {
  constructor(props) {
    super(props);
    this.state = {showConsole: false};
    this.handleClickConsole = this.handleClickConsole.bind(this)
    this.handleClose = this.handleClose.bind(this)
  }
  handleClickConsole(){
    this.setState({showConsole: true});
  }
  handleClose(){
    this.setState({showConsole: false});
  }
  render() {
        return (
            <div className="action-wrapper main-action-wrapper">
                <div className={"console-view-wrappper "+this.state.showConsole} >
                  <Console  onClose={this.handleClose} /> 
                </div>
                <img src="/icon/console.svg" className="showConsoleToggle icon" alt="Chart" title="Show Console"  onClick={this.handleClickConsole}/>
            </div>
      );
  }
}

MainActionWrapper.propTypes = {

};
 
export default createContainer(() => {
  return {
  };
}, MainActionWrapper);