import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';
import FullContainer from './FullContainer.jsx';
import ModalContainer from './ModalContainer.jsx';
import Map from './mapDisplay/Map.jsx';
import Console from './consoleDisplay/Console.jsx';

class ContainerManager extends Component {
  constructor(props) {
    super(props);
    this.state = {
        showMap: props.showMap,
        showConsole: props.showConsole
    };
    this.activateContainer = this.activateContainer.bind(this)
  }
  activateContainer(theProp){
    this.setState({[theProp]: true});
  }
  handleClose(theProp){
    this.setState({[theProp]: false});
  }
  render() {
        return (
            <div className="container-manager">
                {/*THE MAP*/}
                {   /*Toggling object, is dismounted*/
                    this.state.showMap &&
                    <FullContainer onClose={() => this.handleClose('showMap')} title={"Vaccum Map"} addedClassName={"map-wrapper"}>
                        <Map />
                    </FullContainer>      
                }
                <img src="/icon/map-alt.svg" className="containerActivation icon" alt="Chart" title="Show Map"   onClick={() => this.activateContainer('showMap') }  /> 

                 {/*THE CONSOLE*/}
                { /*Persistant object, not removed on close, just hidden*/
                   <div className={"console-view-wrappper "+this.state['showConsole']} >
                      <ModalContainer onClose={() => this.handleClose('showConsole')} title={"Vaccum console"} addedClassName={"console-wrapper"}>
                          <Console />
                      </ModalContainer>   
                    </div>   
                }
                <img src="/icon/console.svg" className="containerActivation icon" alt="Chart" title="Show Console" onClick={() => this.activateContainer('showConsole') }  /> 
            </div>
      );
  }
}

ContainerManager.propTypes = {
    
};

ContainerManager.defaultProps  = {
      showMap: false,
      showConsole: false,
}

export default createContainer(() => {
  return {
  };
}, ContainerManager);