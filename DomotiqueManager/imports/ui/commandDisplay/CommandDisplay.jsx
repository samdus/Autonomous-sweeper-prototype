import React, { Component, PropTypes } from 'react';
import { createContainer } from 'meteor/react-meteor-data';
import ReactDOM from 'react-dom';
import { CommandContainer } from '../../api/commandContainer.js';


class CommandDisplay extends Component {
  constructor(props) {
    super(props);
    this.handleModeSwitch = this.handleModeSwitch.bind(this);
    this.handleDebug = this.handleDebug.bind(this);
    this.handlePowerOff = this.handlePowerOff.bind(this);
    this.handleScan = this.handleScan.bind(this);
    this.closeInput = this.closeInput.bind(this);
    this.openInput = this.openInput.bind(this);
    this.handleTurnSubmit = this.handleTurnSubmit.bind(this);
    this.handleTurn = this.handleTurn.bind(this);
    this.state = { automatic: true, debug: true, turnInput:false };
  }
  handleModeSwitch(){
      this.setState({automatic: !this.state.automatic});
       var now = Date.now();
      if(! this.state.automatic){
        CommandContainer.insert({command:"automatic", createdAt:now})
      }else{
        CommandContainer.insert({command:"manual", createdAt:now})
      }
  }
  handleDebug(){
      this.setState({debug: !this.state.debug});
       var now = Date.now();
      if(! this.state.debug){
        CommandContainer.insert({command:"stopdebug", createdAt:now})
      }else{
        CommandContainer.insert({command:"startdebug", createdAt:now})
      }
  }
  handlePowerOff(){
      var now = Date.now();
       CommandContainer.insert({command:"close", createdAt:now})
  }
  handleScan(){
      var now = Date.now();
       CommandContainer.insert({command:"scan", createdAt:now})
  }
  handleTurn(value){
      var now = Date.now();
       CommandContainer.insert({command:"turn", goToX:value, createdAt:now})
  }
  shouldComponentUpdate(nextProps, nextState){
    if(this.props.dataReady != nextProps.dataReady)  
        return true;
    if(this.state.automatic != nextState.automatic)
        return true; 
    if(this.state.turnInput != nextState.turnInput)
        return true; 
    if(this.state.debug != nextState.debug)
        return false;    
  }
  openInput(theProp){
    this.setState({[theProp]: true});
  }
  closeInput(theProp){
    this.setState({[theProp]: false});
  }
  handleTurnSubmit(e){
    if (e.key === 'Enter') {
      this.handleTurn(Number(e.target.value));
      e.target.value= ""
      this.closeInput("turnInput")
    }
  }
  render() {
      if(this.props.dataReady){
            return (
                <div className={"command-wrapper"}>
                    <label className="switch small toggleDebug" title="Toggle debug">
                        <input type="checkbox" onClick={this.handleDebug}/>
                        <span className="allLabel">Debug</span>
                        <div className="slider round"></div>
                    </label>
                    <div className={"manualCommand-wrapper "+ this.state.automatic}>
                        <label className="switch toggleAutomatic" title="Toggle manual mode" >
                            <input type="checkbox" onClick={this.handleModeSwitch} />
                            <span className="uncheckedLabel">Automatic</span>
                            <span className="checkedLabel">Manual</span>
                            <div className="slider round"></div>
                        </label>
                        <div className="buttonLabel">Turn off</div>
                        <img src="/icon/power.svg" className="icon" alt="Power" title="Turn off"    onClick={this.handlePowerOff} />
                        <div className="buttonLabel">Scan</div>
                        <img src="/icon/scan.svg" className="icon" alt="Chart" title="Scan"  onClick={this.handleScan} />
                        <div className="buttonLabel">Rotate</div>
                        <div className="popupinputWrapper">
                            <img src="/icon/turn.svg" className=" icon" alt="Chart" title="Turn" onClick={() => this.openInput('turnInput')} />
                            <div className={"popupinput " + this.state.turnInput}>
                                <input type="number"  onKeyPress={this.handleTurnSubmit}/>
                                <img className="close icon nohover" src="/icon/closeBlack.svg" alt="close" title="Close" onClick={() => this.closeInput('turnInput')} />
                            </div>
                        </div>
                    </div>
                </div>
        );
      }else{
          return(
              <div className={"command-wrapper"}>
                <div className="loading">
                    <div className="circleloader"></div>
                </div>
            </div>
          );
      }
  }
}
 
CommandDisplay.propTypes = {
};
CommandDisplay.defaultProps  = {
      handleSub: false,
}

export default createContainer(() => {
  var handleSub=Meteor.subscribe('commandContainer');
  return {
    dataReady:handleSub.ready()
  };
}, CommandDisplay);