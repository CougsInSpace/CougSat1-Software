var batteryPower = 0.0;

var configECS = {
  type: "bar",
  data: {
    labels: [
      "ADCS", "C&DH", "Comms", "EPS", "Batt A", "Batt B", "+X", "-X", "+Y",
      "-Y", "+Z", "-Z", "Plant"
    ],
    datasets:
        [{data: new Array(13), backgroundColor: new Array(13), borderWidth: 0}]
  },
  options: {
    responsive: true,
    legend: {display: false},
    title: {display: false},
    tooltips: {
      callbacks: {
        label: function(tooltipItem, data) {
          return tooltipItem.yLabel.toFixed(1) + " °C";
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
          stepSize: 30,
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
    datasets:
        [{data: new Array(7), backgroundColor: new Array(7), borderWidth: 0}]
  },
  options: {
    responsive: true,
    legend: {display: false},
    title: {display: true, text: "Power", fontColor: "#FFFFFF"},
    tooltips: {
      callbacks: {
        label: function(tooltipItem, data) {
          if (tooltipItem.index == 1 && batteryPower < 0)
            return "-" + formatMetricPrefix(tooltipItem.yLabel, 3) + "W";
          return formatMetricPrefix(tooltipItem.yLabel, 3) + "W";
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

// Only voltage of VBatt is measured, battery voltage and vin voltage can be
// computed from shunts and their currents
var shunts =
    {BATT_IN_A: 0.012, BATT_IN_B: 0.012, BATT_OUT_A: 0.012, BATT_OUT_B: 0.012};

var cellIDs = [
  "PV_CELL_0A", "PV_CELL_0B", "PV_CELL_1A", "PV_CELL_1B", "PV_CELL_2A",
  "PV_CELL_2B", "PV_CELL_3A", "PV_CELL_3B"
];

var chainSuffixes = ["A", "B"];

var critical = {
  STD_TEMP_H: 70,
  STD_TEMP_L: -20,
  BATT_H: 50,
  BATT_L: -5,
  EXTERIOR_H: 80,
  EXTERIOR_L: -40,
  PLANT_H: 30,
  PLANT_L: 10,
  SOLAR_H: 8,
  SOLAR_L: 0,
  BATT_P_H: 20,
  BATT_P_L: 0,
  TOTAL_P_H: 20,
  TOTAL_P_L: 0,
  PR_BATT_H: 15,
  PR_BATT_L: 0,
  PR_3V3_H: 10,
  PR_3V3_L: 0,
  COMMS_H: 4,
  COMMS_L: 0,
  HEATERS_H: 1.5,
  HEATERS_L: 0
};

var warning = {
  STD_TEMP_H: 40,
  STD_TEMP_L: -5,
  BATT_H: 30,
  BATT_L: 0,
  EXTERIOR_H: 60,
  EXTERIOR_L: -20,
  PLANT_H: 26,
  PLANT_L: 20,
  SOLAR_H: 6,
  SOLAR_L: 1,
  BATT_P_H: 10,
  BATT_P_L: 0,
  TOTAL_P_H: 10,
  TOTAL_P_L: 0,
  PR_BATT_H: 8,
  PR_BATT_L: 0,
  PR_3V3_H: 5,
  PR_3V3_L: 0,
  COMMS_H: 2,
  COMMS_L: 0,
  HEATERS_H: 0.5,
  HEATERS_L: 0
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
}
window.addEventListener("DOMContentLoaded", setup);

/**
 * Update the temperature in the ECS chart with value and color
 *
 * @param {Int} index of the element
 * @param {String} value of the element
 * @param {String} thresholds to check
 */
function updateTemp(index, value, thresholds) {
  configECS.data.datasets[0].data[index] = value;
  if (value > critical[thresholds + "_H"] ||
      value < critical[thresholds + "_L"])
    configECS.data.datasets[0].backgroundColor[index] = "red";
  else if (value > warning[thresholds + "_H"] ||
           value < warning[thresholds + "_L"])
    configECS.data.datasets[0].backgroundColor[index] = "yellow";
  else
    configECS.data.datasets[0].backgroundColor[index] = "green";
}

/**
 * Update the powers in the EPS chart with value and color
 *
 * @param {Int} index of the element
 * @param {String} value of the element
 * @param {String} thresholds to check
 */
function updatePower(index, value, thresholds) {
  configEPS.data.datasets[0].data[index] = value;
  if (value > critical[thresholds + "_H"] ||
      value < critical[thresholds + "_L"])
    configEPS.data.datasets[0].backgroundColor[index] = "red";
  else if (value > warning[thresholds + "_H"] ||
           value < warning[thresholds + "_L"])
    configEPS.data.datasets[0].backgroundColor[index] = "yellow";
  else
    configEPS.data.datasets[0].backgroundColor[index] = "green";
}

/**
 * Update the diagram: labels, drawings, etc. from the data attached to the
 * element
 *
 * @param {DOMElement} element
 */
function updateDiagram(element) {
  // ECS
  updateTemp(0, element["TEMP_ADCS"], "STD_TEMP");
  updateTemp(1, element["TEMP_CDH"], "STD_TEMP");
  updateTemp(2, element["TEMP_COMMS"], "STD_TEMP");
  updateTemp(3, element["TEMP_EPS"], "STD_TEMP");
  updateTemp(4, element["TEMP_BATT_A"], "BATT");
  updateTemp(5, element["TEMP_BATT_B"], "BATT");
  updateTemp(6, element["TEMP_PX"], "EXTERIOR");
  updateTemp(7, element["TEMP_NX"], "EXTERIOR");
  updateTemp(8, element["TEMP_PY"], "EXTERIOR");
  updateTemp(9, element["TEMP_NY"], "EXTERIOR");
  updateTemp(10, element["TEMP_PZ"], "EXTERIOR");
  updateTemp(11, element["TEMP_NZ"], "EXTERIOR");
  updateTemp(12, element["TEMP_PLANT"], "PLANT");
  chartECS.update();

  // EPS
  var solarPower   = 0.0;
  var totalPIn     = 0.0;
  var totalPOut    = 0.0;
  var prBattPOut   = 0.0;
  var pr3V3POut    = 0.0;
  var commsPower   = 0.0;
  var heatersPower = 0.0;

  var prBattV = 0.0; // Higher of the two
  var pr3V3V  = 0.0; // Higher of the two

  // Solar cells
  var obj = null;
  for (var i = 0; i < cellIDs.length; i++) {
    var pin = element[cellIDs[i] + "_V"] * element[cellIDs[i] + "_I"];
    solarPower += pin;
    totalPIn += pin;
  }

  // Batteries
  var batteryCharge = 0.0;
  for (var i = 0; i < chainSuffixes.length; i++) {
    var vinI     = element["BATT_IN_" + chainSuffixes[i] + "_I"];
    var vbattI   = element["BATT_OUT_" + chainSuffixes[i] + "_I"];
    var batteryI = vinI - vbattI;

    var batteryV = element["BATT_" + chainSuffixes[i] + "_V"];
    var vbattV   = batteryV - vbattI * shunts["BATT_OUT_" + chainSuffixes[i]];

    prBattV = Math.max(prBattV, vbattV);

    var batteryP = batteryV * batteryI;
    batteryPower += batteryP;
    if (batteryP > 0)
      totalPOut += batteryP;
    else
      totalPIn += batteryP;

    batteryCharge += element["BATT_" + chainSuffixes[i] + "_C"];
  }
  obj     = document.getElementById("battery-charge");
  obj.num = batteryCharge;
  ehbanana.numToMetricPrefix(obj);

  // Vbatt switching
  for (var i = 0; i < 2; i++) {
    var power = prBattV * element["PR_BH_" + i + "_I"];
    heatersPower += power;

    power = prBattV * element["PR_DEPLOY_" + i + "_I"];
    totalPOut += power;
  }
  for (var i = 0; i < 7; i++) {
    var power = prBattV * element["PR_BATT_" + i + "_I"];
    if (i == 0)
      heatersPower += power;
    if (i == 6)
      commsPower += power;
    prBattPOut += power;
    totalPOut += power;
  }

  // Regulators
  for (var i = 0; i < chainSuffixes.length; i++) {
    var outV = element["REG_OUT_" + chainSuffixes[i] + "_V"];

    pr3V3V = Math.max(pr3V3V, outV);
  }

  // 3.3V switching
  for (var i = 0; i < 13; i++) {
    var power = pr3V3V * element["PR_3V3_" + i + "_I"];
    if (i == 6)
      commsPower += power;
    pr3V3POut += power;
    totalPOut += power;
  }

  updatePower(0, solarPower, "SOLAR");
  if (batteryPower > 0)
    updatePower(1, batteryPower, "BATT_P");
  else {
    configEPS.data.datasets[0].data[1]            = -batteryPower;
    configEPS.data.datasets[0].backgroundColor[1] = "cyan";
  }
  updatePower(2, totalPOut, "TOTAL_P");
  updatePower(3, prBattPOut, "PR_BATT");
  updatePower(4, pr3V3POut, "PR_3V3");
  updatePower(5, commsPower, "COMMS");
  updatePower(6, heatersPower, "HEATERS");
  chartEPS.update();

  obj     = document.getElementById("eps-efficiency");
  obj.num = totalPOut / totalPIn * 100.0;
  numToPercent(obj);
}

function updateMap(element) {
  formatDegrees(element);

  contextADCS.clearRect(0, 0, 360, 180);
  contextADCS.beginPath();
  contextADCS.fillStyle = "yellow";
  var lat               = document.getElementById("lat").num;
  var long              = document.getElementById("long").num;
  contextADCS.arc(4 * (180 + lat), 4 * (90 - long), 10, 0, 2 * Math.PI);
  contextADCS.fill();
}