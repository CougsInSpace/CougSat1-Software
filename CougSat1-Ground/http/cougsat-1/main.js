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
    tooltips: {
      callbacks: {
        label: function(tooltipItem, data) {
          return tooltipItem.yLabel + " °C";
        }
      }
    },
    scales: {
      xAxes: [{
        display: true,
        ticks: {fontColor: "#FFFFFF"},
        gridLines: {color: "#FFFFFF22", zeroLineColor: "#FFFFFF"}
      }],
      yAxes: [{
        display: true,
        scaleLabel: {display: false},
        ticks: {
          min: -50,
          max: 90,
          fontColor: "#FFFFFF",
          callback: function(value, index, values) {
            return value + " °C";
          }
        },
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
      data: [1.312312, 4.0, 5.2, 4.3, 0.9, 1.3, 0.2, 0.4],
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
    title: {display: true, text: "Power", fontColor: "#FFFFFF"},
    tooltips: {
      callbacks: {
        label: function(tooltipItem, data) {
          return formatMetricPrefix(tooltipItem.yLabel, 4) + "W";
        }
      }
    },
    scales: {
      xAxes: [{
        display: true,
        ticks: {fontColor: "#FFFFFF"},
        gridLines: {color: "#FFFFFF22", zeroLineColor: "#FFFFFF"}
      }],
      yAxes: [{
        display: true,
        type: "logarithmic",
        ticks: {
          min: 0.01,
          max: 20,
          fontColor: "#FFFFFF",
          callback: function(value, index, values) {
            if ((Math.log10(value) % 1) == 0 || index == 0)
              return formatMetricPrefix(value) + "W";
            else
              return ""
          }
        },
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
