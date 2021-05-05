import React, {useRef, useEffect} from "react";
import {max, select, scaleLinear, line, curveCardinal} from 'd3';

function SmallChart(props) {
    const svgRef = useRef();
    const data = props.data;

    useEffect(() => {
        const svg = select(svgRef.current)
            .attr('width', 100)
            .attr('height', 100);

        const xScale = scaleLinear()
            .range([0,100])
            .domain([0, 10]);
        const yScale = scaleLinear()
            .range([90, 10])
            .domain([0, 1200]);

        const customLine = line()
            .x((d, i) => xScale(i))
            .y(d => yScale(d))
            .curve(curveCardinal);

        svg.selectAll('path')
            .data([data])
            .join('path')
            .attr('d', value => customLine(value))
            .attr('fill', 'none')
            .attr('stroke', '#61dafb')
            .attr('width', 90);
    }, [data, props]);

    return (
        <div className={'smallChart ' + props.position}>
            <svg ref={svgRef}></svg>
        </div>
    )
}

export default SmallChart;
