import React from "react";
import * as d3 from 'd3';


class RightPanel extends React.Component {
    constructor(props) {
        super(props);
        this.leftPanel = React.createRef()
    }

    componentDidMount() {
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
            {name: 'rightBackground', value: 300},
            {name: 'right', value: 98},
            {name: 'rightCenterBackground', value: 300},
            {name: 'rightCenter', value: 213}
        ]

        let chart = d3.select(this.leftPanel.current)
            .append('svg')
            .attr('width', area.width + area.margin)
            .attr('height', area.height + area.margin);

        var x = d3.scaleBand()
            .range([0, area.width], .1)
            .domain(data.map(function(d) { return d.name; }));

        var y = d3.scaleLinear()
            .range([area.height, 0])
            .domain([0, d3.max((data.map((d) => d.value)))])
            .nice();

        let prepareBars = function (d, i) {
            let barShift = 0;
            let barTransform = 20;
            if (i < 2) {
                barShift = 20;
                barTransform = 0;
            }
            let X = x(d.name);
            if (!d.name.includes('Background') && i > 0) {
                X = x(data[i - 1].name);
            }

            const preparedData =  [
                { x: X, y: y(barTransform ? barTransform : barShift) },
                { x: X, y: y(d.value) },
                { x: X + bar.width, y: y(d.value - barTransform) },
                { x: X + bar.width, y: y(barTransform ? 0 : barShift) },
                { x: X, y: y(barTransform ? barTransform : barShift) }

            ];

            return lineFunc(preparedData);
        }

        let lineFunc = d3.line()
            .x((d) => d.x)
            .y((d) => d.y);

        chart.selectAll('path')
            .data(data)
            .enter()
            .append('path')
            .attr('d', prepareBars)
            .attr('stroke', 'black')
            .attr('fill', (d) => d.name.includes('Background') ? '#3e444f' : '#61dafb');
    }

    render() {
        return (
            <div ref={this.leftPanel} className={'rightPanel'}></div>
        )
    }
}

export default RightPanel;
