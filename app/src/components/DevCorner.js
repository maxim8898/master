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
            carData: [{left: 240, left_center: 430, right_center: 700, right: 360, speed: 0, temperature: 0, angle: 0},
                {left: 310, left_center: 240, right_center: 310, right: 420, speed: 0, temperature: 0, angle: 0},
                {left: 720, left_center: 250, right_center: 110, right: 520, speed: 0, temperature: 0, angle: 0},
                {left: 102, left_center: 430, right_center: 245, right: 236, speed: 0, temperature: 0, angle: 0},
                {left: 409, left_center: 435, right_center: 377, right: 567, speed: 0, temperature: 0, angle: 0},
                {left: 307, left_center: 156, right_center: 235, right: 455, speed: 0, temperature: 0, angle: 0},
                {left: 680, left_center: 964, right_center: 366, right: 456, speed: 0, temperature: 0, angle: 0},
                {left: 100, left_center: 346, right_center: 345, right: 234, speed: 0, temperature: 0, angle: 0},
                {left: 720, left_center: 753, right_center: 134, right: 345, speed: 0, temperature: 0, angle: 0},
                {left: 110, left_center: 330, right_center: 270, right: 620, speed: 0, temperature: 0, angle: 0}]
        };
        this.initWebSocket = this.initWebSocket.bind(this);
        this.setConnectionStatus = this.setConnectionStatus.bind(this);
        this.setConnectionUrl = this.setConnectionUrl.bind(this);
        this.setCarData = this.setCarData.bind(this);
    }

    setCarData(data) {
        data = JSON.parse(data);
        let carData = this.state.carData;
        carData.push({
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
                this.setCarData(e.data);
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
