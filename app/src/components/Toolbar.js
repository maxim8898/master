import React from 'react';
import Button from "./Button";
import StatusField from "./StatusField";

class Toolbar extends React.Component {
    constructor(props) {
        super(props);
        this.myRef = React.createRef()
    }

    componentDidMount() {

    }

    render() {
        return (
            <div className={'toolbar'}>
                <Button name={"Start"}/>
                <Button name={"Stop"}/>
                <Button name={"Manual"}/>
                <StatusField name={"Angle"} value={this.props.carData.slice(-1)[0].angle}/>
                <StatusField name={"Voltage"} value={12.1}/>
                <StatusField name={"Speed"} value={this.props.carData.slice(-1)[0].speed}/>
            </div>
        )
    }
}

export default Toolbar;
