// Only voltage of VBatt is measured, battery voltage and vin voltage can be
// computed from shunts and their currents
var shunts =
    {BATT_IN_A: 0.012, BATT_IN_B: 0.012, BATT_OUT_A: 0.012, BATT_OUT_B: 0.012};

var cellIDs = [
  "PV_CELL_0A", "PV_CELL_0B", "PV_CELL_1A", "PV_CELL_1B", "PV_CELL_2A",
  "PV_CELL_2B", "PV_CELL_3A", "PV_CELL_3B"
];

var mpptIDs = [
  "PV_IN_0A", "PV_IN_0B", "PV_IN_1A", "PV_IN_1B", "PV_IN_2A", "PV_IN_2B",
  "PV_IN_3A", "PV_IN_3B"
];

var chainSuffixes = ["A", "B"];

/**
 * Update the diagram: labels, drawings, etc. from the data attached to the
 * element
 *
 * @param {DOMElement} element
 */
function updateDiagram(element) {
  var totalPIn  = 0.0; // Sum of PV in and battery in if discharging
  var totalPOut = 0.0; // Sum of PR out and battery out if charging

  // Busses
  var vinPIn     = 0.0
  var vinPOut    = 0.0;
  var prBattPIn  = 0.0;
  var prBattPOut = 0.0;
  var pr3V3PIn   = 0.0;
  var pr3V3POut  = 0.0;

  var prBattV = 0.0; // Higher of the two
  var pr3V3V  = 0.0; // Higher of the two

  // Solar cells
  var obj = null;
  for (var i = 0; i < cellIDs.length; i++) {
    var pin  = element[cellIDs[i] + "_V"] * element[cellIDs[i] + "_I"];
    var pout = element[mpptIDs[i] + "_V"] * element[mpptIDs[i] + "_I"];
    totalPIn += pin;
    vinPIn += pout;

    obj     = document.getElementById(cellIDs[i]);
    obj.num = pin;
    ehbanana.numToMetricPrefix(obj);

    obj     = document.getElementById(mpptIDs[i]);
    obj.num = pout / pin * 100.0;
    numToPercent(obj);
  }

  // Batteries
  for (var i = 0; i < chainSuffixes.length; i++) {
    var vinI     = element["BATT_IN_" + chainSuffixes[i] + "_I"];
    var vbattI   = element["BATT_OUT_" + chainSuffixes[i] + "_I"];
    var batteryI = vinI - vbattI;
    element["BATT_" + chainSuffixes[i] + "_I"] = batteryI;

    var batteryV = element["BATT_" + chainSuffixes[i] + "_V"];
    var vinV     = batteryV + vinI * shunts["BATT_IN_" + chainSuffixes[i]];
    element["BATT_IN_" + chainSuffixes[i] + "_V"] = vinV;
    var vbattV = batteryV - vbattI * shunts["BATT_OUT_" + chainSuffixes[i]];
    element["BATT_OUT_" + chainSuffixes[i] + "_V"] = vbattV;

    prBattV = Math.max(prBattV, vbattV);

    var vinP = vinV * vinI;
    vinPOut += vinP;

    var batteryP = batteryV * batteryI;
    if (batteryP > 0)
      totalPOut += batteryP;
    else
      totalPIn += batteryP;

    var vbattP = vbattV * vbattI;
    prBattPIn += vbattP;

    obj     = document.getElementById("BATT_V_" + chainSuffixes[i]);
    obj.num = vbattV;
    ehbanana.numToMetricPrefix(obj);

    obj     = document.getElementById("BATT_C_" + chainSuffixes[i]);
    obj.num = element["BATT_" + chainSuffixes[i] + "_C"];
    ehbanana.numToMetricPrefix(obj);

    obj     = document.getElementById("BATT_P_" + chainSuffixes[i]);
    obj.num = -batteryP;
    ehbanana.numToMetricPrefix(obj);
  }

  // Vbatt switching
  for (var i = 0; i < 2; i++) {
    obj     = document.getElementById("PR_BH_" + i);
    obj.num = prBattV * element["PR_BH_" + i + "_I"];
    prBattPOut += obj.num;
    ehbanana.numToMetricPrefix(obj);

    obj     = document.getElementById("PR_DEPLOY_" + i);
    obj.num = prBattV * element["PR_DEPLOY_" + i + "_I"];
    prBattPOut += obj.num;
    totalPOut += obj.num;
    ehbanana.numToMetricPrefix(obj);
  }
  for (var i = 0; i < 7; i++) {
    obj     = document.getElementById("PR_BATT_" + i);
    obj.num = prBattV * element["PR_BATT_" + i + "_I"];
    prBattPOut += obj.num;
    totalPOut += obj.num;
    ehbanana.numToMetricPrefix(obj);
  }

  // Regulators
  for (var i = 0; i < chainSuffixes.length; i++) {
    var inI = element["REG_IN_" + chainSuffixes[i] + "_I"];
    var inP = prBattV * inI;

    var outI = element["REG_OUT_" + chainSuffixes[i] + "_I"];
    var outV = element["REG_OUT_" + chainSuffixes[i] + "_V"];
    var outP = outV * outI;

    pr3V3V = Math.max(pr3V3V, outV);

    prBattPOut += inP;
    pr3V3PIn += outP;

    obj     = document.getElementById("REG_V_" + chainSuffixes[i]);
    obj.num = outV;
    ehbanana.numToMetricPrefix(obj);

    obj     = document.getElementById("REG_P_" + chainSuffixes[i]);
    obj.num = outP;
    ehbanana.numToMetricPrefix(obj);

    obj     = document.getElementById("REG_N_" + chainSuffixes[i]);
    obj.num = outP / inP * 100.0;
    numToPercent(obj);
  }

  // 3.3V switching
  for (var i = 0; i < 13; i++) {
    obj     = document.getElementById("PR_3V3_" + i);
    obj.num = pr3V3V * element["PR_3V3_" + i + "_I"];
    pr3V3POut += obj.num;
    totalPOut += obj.num;
    ehbanana.numToMetricPrefix(obj);
  }
  obj     = document.getElementById("PR_3V3_PV");
  obj.num = pr3V3V * element["PR_3V3_PV_I"];
  pr3V3POut += obj.num;
  ehbanana.numToMetricPrefix(obj);

  obj     = document.getElementById("PR_3V3_EPS");
  obj.num = pr3V3V * element["PR_3V3_EPS_I"];
  pr3V3POut += obj.num;
  ehbanana.numToMetricPrefix(obj);

  obj     = document.getElementById("INPUT_P");
  obj.num = vinPOut;
  ehbanana.numToMetricPrefix(obj);

  obj     = document.getElementById("INPUT_N");
  obj.num = vinPOut / vinPIn * 100.0;
  numToPercent(obj);

  obj     = document.getElementById("VBATT_P");
  obj.num = prBattPOut;
  ehbanana.numToMetricPrefix(obj);

  obj     = document.getElementById("VBATT_N");
  obj.num = prBattPOut / prBattPIn * 100.0;
  numToPercent(obj);

  obj     = document.getElementById("PR3V3_P");
  obj.num = pr3V3POut;
  ehbanana.numToMetricPrefix(obj);

  obj     = document.getElementById("PR3V3_N");
  obj.num = pr3V3POut / pr3V3PIn * 100.0;
  numToPercent(obj);

  obj     = document.getElementById("EPS_IN");
  obj.num = totalPIn;
  ehbanana.numToMetricPrefix(obj);

  obj     = document.getElementById("EPS_OUT");
  obj.num = totalPOut;
  ehbanana.numToMetricPrefix(obj);

  obj     = document.getElementById("EPS_N");
  obj.num = totalPOut / totalPIn * 100.0;
  numToPercent(obj);

  traces.updateGeometry();
  traces.updateValues(element);
  traces.renderLine();
}

/**
 * Setup the traces
 */
function setup() {
  traces.setup();
  traces.renderLine();
}

/**
 * Adjust canvas geometry on resize
 */
function onResize() {
  traces.updateGeometry();
  traces.renderLine();
}

document.addEventListener("DOMContentLoaded", setup);
window.addEventListener("resize", onResize);