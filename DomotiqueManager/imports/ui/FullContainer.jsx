import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';



class FullContainer extends Component {
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
            <div className="fullContainer">
                <img className="back icon" src="/icon/arrow-left.svg" onClick={this.props.onClose} title="Back" alt="Back" />
                <div className="title">{ this.props.title }</div>
                <div className="content map-wrapper">{ this.props.children }</div>
            </div>
      );
  }
}

FullContainer.propTypes = {
    onClose: PropTypes.func.isRequired,
};
 
export default createContainer(() => {
  return {
  };
}, FullContainer);