import React, { Component, PropTypes } from 'react';
import { createContainer } from 'meteor/react-meteor-data';
import ReactDOM from 'react-dom';



class ModalContainer extends Component {
  constructor(props) {
    super(props);
  }
  render() {
        return (
            <div className="modal-wrapper">
              <div className="overlay-modal"  onClick={this.props.onClose}></div>
              <div className={this.props.addedClassName + " modal-box"}>
                <div className="modal-title">{this.props.title}</div>
                <img className="close icon" src="/icon/close.svg" alt="close" title="Close" onClick={this.props.onClose} />
                
                  {this.props.children}
               
              </div>
            </div>
      );
  }
}
 
ModalContainer.propTypes = {
    onClose: PropTypes.func.isRequired,
};

export default createContainer(() => {
  return {
  };
}, ModalContainer);