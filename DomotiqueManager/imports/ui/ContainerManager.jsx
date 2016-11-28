import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';
import FullContainer from './FullContainer.jsx';
import ModalContainer from './ModalContainer.jsx';
import Map from './mapDisplay/Map.jsx';
import Console from './consoleDisplay/Console.jsx';
import CommandDisplay from './commandDisplay/CommandDisplay.jsx';
import PictureDisplay from './pictureDisplay/PictureDisplay.jsx';

class ContainerManager extends Component {
  constructor(props) {
    super(props);
    this.state = {
        showMap: props.showMap,
        showConsole: props.showConsole,
        showPicture: props.showPicture
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
                         <img src="/icon/console.svg" className="showConsole icon" alt="Chart" title="Show Console" onClick={() => this.activateContainer('showConsole')}  />
                         <img src="/icon/image.svg" className="showPicture icon" alt="Chart" title="Show picture"   onClick={() => this.activateContainer('showPicture') }  /> 
                        <CommandDisplay showConsole={() => this.activateContainer('showConsole')} />
                    </FullContainer>      
                }
                <img src="/icon/map-alt.svg" className="containerActivation icon" alt="Chart" title="Show Map"   onClick={() => this.activateContainer('showMap') }  /> 

           
                { /*Persistant object, not removed on close, just hidden*/
                   <div className={"picture-view-wrappper "+this.state['showPicture']} >
                      <ModalContainer onClose={() => this.handleClose('showPicture')} title={"Pictures from vaccum"} addedClassName={"picture-wrapper"}>
                            <PictureDisplay />
                      </ModalContainer>   
                    </div>   
                }
              <img src="/icon/image.svg" className="containerActivation icon" alt="Chart" title="Show picture"   onClick={() => this.activateContainer('showPicture') }  /> 

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
      showPicture: false,
}

export default createContainer(() => {
  return {
  };
}, ContainerManager);