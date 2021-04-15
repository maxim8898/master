import React from "react";

function StatusBar (props) {
    let connectionMessageColor;
    switch (props.connection.type) {
        case 'error':
            connectionMessageColor = '#ec1f1f';
            break;
        case 'info':
            connectionMessageColor = '#ebff00';
            break;
        case 'success':
            connectionMessageColor = '#0d9807';
            break;

        default:
            connectionMessageColor = '#ebff00';
    }

    return (
        <div className={"statusBar"}>
            <label for="connectionUrlInput">Connection URL => ws://</label>
            <input id="connectionUrlInput" type="text" defaultValue={props.defaultConnectionUrl} onBlur={props.setConnectionUrl}/>
            <div>{props.connection.name} => <span style={{color: connectionMessageColor}}>{props.connection.value}</span></div>
        </div>
    )
}

export default StatusBar;
