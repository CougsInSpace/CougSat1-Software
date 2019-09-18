var config = {
  type: "scatter",
  data: {
    datasets: [{
      backgroundColor: "#DBB776",
      borderColor: "#DBB776",
      data: [
        {x: 0.9, y: -0.7}, {x: 0.8, y: 0.9}, {x: -0.85, y: -0.74},
        {x: -0.8, y: 1.13}
      ],
    }]
  },
  options: {
    animation: {duration: 0},
    legend: {display: false},
    responsive: true,
    title:
        {display: true, text: "RX Constellation Diagram", fontColor: "#FFFFFF"},
    scales: {
      xAxes: [{
        display: true,
        scaleLabel:
            {display: true, labelString: "In-Phase", fontColor: "#FFFFFF"},
        ticks: {min: -1.2, max: 1.2, fontColor: "#FFFFFF", stepSize: 0.4},
        gridLines: {color: "#FFFFFF55", zeroLineColor: "#FFFFFF"}
      }],
      yAxes: [{
        display: true,
        scaleLabel:
            {display: true, labelString: "Quadrature", fontColor: "#FFFFFF"},
        ticks: {min: -1.2, max: 1.2, fontColor: "#FFFFFF", stepSize: 0.4},
        gridLines: {color: "#FFFFFF55", zeroLineColor: "#FFFFFF"}
      }]
    }
  }
};

var chart     = null;
var pointsMax = 20;

/**
 * Setup the chart
 */
function setup() {
  var context =
      document.getElementById("constellation-diagram").getContext("2d");
  chart = new Chart(context, config);
}
window.addEventListener("load", setup);

/**
 * Add a data point(s) to the constellation diagram
 *
 * @param {DOMElement} element
 */
function updateConstellation(element) {
  config.data.datasets[0].data = element.value;
  chart.update();
}
