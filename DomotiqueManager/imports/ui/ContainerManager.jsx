import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';
import Fullcontainer from './FullContainer.jsx';
import MapContainer from './mapDisplay/MapContainer.jsx';


class ContainerManager extends Component {
  constructor(props) {
    super(props);
    this.state = {
        showMap: props.showMap
    };
    this.activateContainer = this.activateContainer.bind(this)
  }
  activateContainer(theProp){
    console.log(this.state[theProp])
    this.setState({[theProp]: true});
    console.log(this.state[theProp])
  }
  handleClose(theProp){
    this.props[theProp] = false;
  }
  render() {
        return (
            <div className="container-wrapper">
                {
                    this.state.showMap &&
                    <FullContainer onClose={() => this.handleClose('showMap')}>
                        <MapContainer />
                    </FullContainer>      
                }
                <span className="containerActivation" onClick={() => this.activateContainer('showMap') }>Show map</span>
            </div>
      );
  }
}

ContainerManager.propTypes = {
    
};
ContainerManager.defaultProps  = {
      showMap: false,
}
export default createContainer(() => {
  return {
  };
}, ContainerManager);