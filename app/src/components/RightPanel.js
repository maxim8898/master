import React, { useRef, useEffect, useState } from "react";
import { max, select, axisBottom, axisRight, scaleLinear, scaleBand, line } from 'd3';

function RightPanel (props) {
    const bar = {
        width: 100,
        height: 300,
        margin: 5
    }
    const area = {
        width: 200,
        height: 400,
        margin: 10
    }

    const data = [
        {name: 'rightCenterBackground', value: 2000},
        {name: 'rightCenter', value: props.carData.slice(-1)[0].right_center},
        {name: 'rightBackground', value: 1980},
        {name: 'right', value: props.carData.slice(-1)[0].right}
    ]

    const svgRef = useRef();

    useEffect(() => {
        const svg = select(svgRef.current);
        svg
            .attr('width', area.width + area.margin)
            .attr('height', area.height + area.margin);

        const xScale = scaleBand()
            .range([0, area.width], .1)
            .domain(data.map((d) => d.name));

        const yScale = scaleLinear()
            .range([area.height, 0])
            .domain([0, max((data.map((d) => d.value)))]);

        let prepareBars = function (d, i) {
            let barShift = 0;
            let barTransform = 200;
            if (i < 2) {
                barShift = 200;
                barTransform = 0;
            }
            let X = xScale(d.name);
            if (!d.name.includes('Background') && i > 0) {
                X = xScale(data[i - 1].name);
            }

            const preparedData =  [
                { x: X, y: yScale(d.value + barTransform + barShift) },
                { x: X, y: yScale(barTransform + barShift) },
                { x: X + bar.width, y: yScale(barShift) },
                { x: X + bar.width, y: yScale(d.value + barShift) },
                { x: X, y: yScale(d.value + barTransform + barShift) }

            ];

            return lineFunc(preparedData);
        }

        let lineFunc = line()
            .x((d) => d.x)
            .y((d) => d.y);

        svg.selectAll('.bar')
            .data(data)
            .join('path')
            .attr('class', 'bar')
            .attr('d', prepareBars)
            .attr('stroke', 'black')
            .attr('fill', (d) => d.name.includes('Background') ? '#3e444f' : '#61dafb');

    }, [data, props]);


    return (
        <div className={'rightPanel'}>
            <svg ref={svgRef}></svg>
        </div>
    )

}

export default RightPanel;
