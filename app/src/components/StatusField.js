import React from "react";

function StatusField(props) {
    return (
        <div className={"toolbarStatusField"}>
            <span>{props.name}</span> <br/>
            <span>{props.value}</span>
        </div>
    )
}

export default StatusField;
