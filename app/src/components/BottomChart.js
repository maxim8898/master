import React from "react";
import * as d3 from 'd3';

class BottomChart extends React.Component {
    constructor(props) {
        super(props);
        this.bottomChartRef = React.createRef()
    }

    componentDidMount() {
        const data = [
            {x:1, y: 23},
            {x:2, y: 54},
            {x:3, y: 75},
            {x:4, y: 23},
            {x:5, y: 45},
            {x:6, y: 85},
            {x:7, y: 3},
            {x:8, y: 67},
            {x:9, y: 9},
            {x:10, y: 12},
        ]

        let x = d3.scaleLinear()
            .domain([0, 10])
            .range([0, 400]);

        let y = d3.scaleLinear()
            .domain([0, 100])
            .range([0, 100]);

        let svg = d3.select(this.bottomChartRef.current)
            .append('svg')
            .attr('width', 400)
            .attr('height', 100);

        svg.append('path')
            .datum(data)
            .attr('fill', 'none')
            .attr('stroke', 'steelblue')
            .attr('d', d3.line()
                .x((d) => x(d.x))
                .y((d) => y(d.y))
            );
    }

    render() {
        return (
            <div ref={this.bottomChartRef}></div>
        )
    }
}

export default BottomChart;