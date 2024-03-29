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
  document.getElementById("rx-source")
      .addEventListener("input", rxSourceListener);
}
window.addEventListener("DOMContentLoaded", setup);

/**
 * Add a data point(s) to the constellation diagram
 *
 * @param {DOMElement} element
 */
function updateConstellation(element) {
  for (var index = 0; index < 200; index++) {
    var i = element[index * 2];
    var q = element[index * 2 + 1];

    config.data.datasets[0].data[index] = {x: i, y: q};
  }
  chart.update();
}

/**
 * Enables / disables the IQ file button when the source is / isn't IQ File
 *
 * @param {DOMElement} element
 */
function rxSourceListener(element) {
  document.getElementById("iq-file-row").style.display = "none";
  document.getElementById("iq-test-row").style.display = "none";
  if (element.target.value == "IQ File")
    document.getElementById("iq-file-row").style.display = "";
  else if (element.target.value == "Test Telemetry")
    document.getElementById("iq-test-row").style.display = "";
}