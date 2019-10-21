var config = {
  type: "scatter",
  data: {
    datasets: [{
      backgroundColor: "#DBB776",
      borderColor: "#DBB776",
      data: new Array(100),
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
        ticks: {min: -1.5, max: 1.5, fontColor: "#FFFFFF", stepSize: 0.5},
        gridLines: {color: "#FFFFFF55", zeroLineColor: "#FFFFFF"}
      }],
      yAxes: [{
        display: true,
        scaleLabel:
            {display: true, labelString: "Quadrature", fontColor: "#FFFFFF"},
        ticks: {min: -1.5, max: 1.5, fontColor: "#FFFFFF", stepSize: 0.5},
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
window.addEventListener("DOMContentLoaded", setup);

/**
 * Add a data point(s) to the constellation diagram
 *
 * @param {DOMElement} element
 */
function updateConstellation(element) {
  for (var i = 0; i < 100; i++) {
    var I = element["dataI-" + i];
    var Q = element["dataQ-" + i];

    config.data.datasets[0].data[i] = {x: I, y: Q};
  }
  console.log(config.data.datasets[0].data);
  chart.update();
}