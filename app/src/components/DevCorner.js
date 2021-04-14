import React from 'react';
import * as d3 from 'd3';

class DevCorner extends React.Component {
    constructor(props) {
        super(props);
        this.myRef = React.createRef()
    }

    componentDidMount() {
        let svg = d3.select(this.myRef.current)
            .append("p")
            .text("HELLO");
    }

    render() {
        return (
            <div ref={this.myRef}></div>
        )
    }
}

export default DevCorner;
