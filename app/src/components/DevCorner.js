import React from 'react';
import LeftPanel from "./LeftPanel";
import RightPanel from "./RightPanel";
import Toolbar from "./Toolbar";
import BottomChart from "./BottomChart";
import StatusBar from "./StatusBar";

class DevCorner extends React.Component {
    constructor(props) {
        super(props);
        this.myRef = React.createRef();
        this.state = {
            statusBar: { connection: { name: 'Connection', value: 'Connection closed', type: 'error' } },
            connection: { url: '192.168.100.10/sensors' },
            carData: [{
                datetime: 0,
                left: 120,
                left_center: 10,
                right_center: 10,
                right: 35,
                speed: 0,
                temperature: 0,
                angle: 0
            }]
        };
        this.initWebSocket = this.initWebSocket.bind(this);
        this.setConnectionStatus = this.setConnectionStatus.bind(this);
        this.setConnectionUrl = this.setConnectionUrl.bind(this);
        this.setCarData = this.setCarData.bind(this);
    }

    setCarData(data) {
        let carData = this.state.carData;
        carData.push({
            datetime: data.datetime,
            left: data.left,
            left_center: data.left_center,
            right_center: data.right_center,
            right: data.right,
            speed: data.speed,
            temperature: data.temperature,
            angle: data.angle
        })

        this.setState(prevState => {
            return { carData: carData.slice(-10) }
        })
    }

    setConnectionUrl(e) {
        this.setState(prevState => {
            return { connection: { ...prevState.connection, url: e.target.value } }
        })
    }

    setConnectionStatus(status, type) {
        this.setState(prevState => {
            return {
                statusBar: {
                    ...prevState.statusBar,
                    connection: {
                        ...prevState.statusBar.connection,
                        value: status, type: type
                    }
                }
            }
        });
    }

    initWebSocket() {
        this.setConnectionStatus('Trying to open a WebSocket connection...', 'info')

        try {
            var websocket = new WebSocket("ws://" + this.state.connection.url);
        } catch (e) {
            this.setConnectionStatus('Connection URL is empty or invalid', 'error');
        }

        if (websocket) {
            websocket.onopen = () => {
                this.setConnectionStatus('Connection opened', 'success');
            };
            websocket.onclose = () => {
                this.setConnectionStatus('Connection closed', 'error');
                setTimeout(this.initWebSocket, 2000);
            };
            websocket.onmessage = (e) => {
                this.setCarData(e.data.collection);
            };
        } else {
            setTimeout(this.initWebSocket, 2000);
        }
    }

    componentDidMount() {
        this.initWebSocket();
    }

    render() {
        return (
            <div className={'devCorner'}>
                <div className={'statusBar'}>
                    <StatusBar
                        defaultConnectionUrl={this.state.connection.url}
                        setConnectionUrl={this.setConnectionUrl}
                        connection={this.state.statusBar.connection}/>
                </div>
                <div className={'devCornerPanel'}>
                    <LeftPanel carData={this.state.carData}/>
                    <Toolbar carData={this.state.carData}/>
                    <RightPanel carData={this.state.carData}/>
                </div>
                <div className={'bottomChart'}>
                    <BottomChart/>
                </div>
            </div>
        )
    }
}

export default DevCorner;
