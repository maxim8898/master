import React, {useRef, useEffect} from "react";
import {select, line, curveCardinal} from 'd3';

function BottomChart(props) {
    const svgRef = useRef();

    useEffect(() => {
        const data = [12, 43, 45, 88, 21, 11, 34, 23, 45, 31];

        const svg = select(svgRef.current)
            .attr('width', 400)
            .attr('height', 100);
        const testLine = line()
            .x((d, i) => i * 50)
            .y(d => d)
            .curve(curveCardinal);

        svg.selectAll('path')
            .data([data])
            .join('path')
            .attr('d', value => testLine(value))
            .attr('fill', 'none')
            .attr('stroke', '#61dafb');


    }, [props]);

    return (
        <div className={'bottomChart'}>
            <svg ref={svgRef}></svg>
        </div>
    )
}

export default BottomChart;