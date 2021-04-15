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
                <StatusField />
                <StatusField name={"Voltage"} value={12.1}/>
                <StatusField name={"Speed"} value={22}/>
            </div>
        )
    }
}

export default Toolbar;
