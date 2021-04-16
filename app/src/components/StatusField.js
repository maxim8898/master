import React from "react";

function StatusField(props) {
    return (
        <div className={"toolbarStatusField"}>
            <span className="statusFieldName">{props.name}</span> <br/>
            <span className="statusFieldValue">{props.value}</span>
        </div>
    )
}

export default StatusField;
