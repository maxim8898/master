import React from "react";

function Button(props) {
    return (
        <button className={"toolbarButton"}>{props.name}</button>
    )
}

export default Button;
