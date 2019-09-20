var configECS = {
  type: "bar",
  data: {
    labels: [
      "ADCS", "C&DH", "Comms", "EPS", "Batt A", "Batt B", "+X", "-X", "+Y",
      "-Y", "+Z", "-Z", "Plant"
    ],
    datasets: [{
      data: [-45, 85, 60, 30, 23, -1, -2, -20, 50, 25, 28, 34, 76],
      "backgroundColor": [
        "#C7042C", "#C7042C", "#C7C72C", "#04C72C", "#04C72C", "#04C72C",
        "#04C72C", "#04C72C", "#04C72C", "#04C72C", "#04C72C", "#04C72C",
        "#C7042C"
      ],
      "borderWidth": 0
    }]
  },
  options: {
    responsive: true,
    legend: {display: false},
    title: {display: false},
    scales: {
      xAxes: [{
        display: true,
        ticks: {fontColor: "#FFFFFF"},
        gridLines: {color: "#FFFFFF22", zeroLineColor: "#FFFFFF"}
      }],
      yAxes: [{
        display: true,
        scaleLabel: {
          display: true,
          labelString: "Temperature - °C",
          fontColor: "#FFFFFF"
        },
        ticks: {min: -50, max: 90, fontColor: "#FFFFFF"},
        gridLines: {color: "#FFFFFF55", zeroLineColor: "#FFFFFF"}
      }]
    }
  }
};

var configEPS = {
  type: "bar",
  data: {
    labels: [
      "Solar Cell", "Battery", "EPS Out", "VBatt", "3.3V", "Comms", "Heating"
    ],
    datasets: [{
      data: [1.3, 4.0, 5.2, 4.3, 0.9, 1.3, 0.2, 0.4],
      "backgroundColor": [
        "#04C72C", "#C7042C", "#C7C72C", "#04C72C", "#04C72C", "#04C72C",
        "#04C72C"
      ],
      "borderWidth": 0
    }]
  },
  options: {
    responsive: true,
    legend: {display: false},
    title: {display: false},
    scales: {
      xAxes: [{
        display: true,
        ticks: {fontColor: "#FFFFFF"},
        gridLines: {color: "#FFFFFF22", zeroLineColor: "#FFFFFF"}
      }],
      yAxes: [{
        display: true,
        scaleLabel:
            {display: true, labelString: "Power - Watts", fontColor: "#FFFFFF"},
        ticks: {beginAtZero: true, fontColor: "#FFFFFF"},
        gridLines: {color: "#FFFFFF55", zeroLineColor: "#FFFFFF"}
      }]
    }
  }
};

var chartECS    = null;
var chartEPS    = null;
var contextADCS = null;
var pointsMax   = 20;

/**
 * Setup the chart
 */
function setup() {
  var contextECS = document.getElementById("chart-ecs").getContext("2d");
  var contextEPS = document.getElementById("chart-eps").getContext("2d");
  contextADCS    = document.getElementById("chart-adcs").getContext("2d");
  chartECS       = new Chart(contextECS, configECS);
  chartEPS       = new Chart(contextEPS, configEPS);

  contextADCS.beginPath();
  contextADCS.fillStyle = "#FFFFFF";
  contextADCS.arc(4 * (180 - 117.17), 4 * (90 - 46.73), 30, 0, 2 * Math.PI);
  contextADCS.fill();
}
window.addEventListener("load", setup);

/**
 * Add a data point(s) to the constellation diagram
 *
 * @param {DOMElement} element
 */
function updateECS(element) {
  configECS.data.datasets[0].data = element.value;
  chartECS.update();
}
