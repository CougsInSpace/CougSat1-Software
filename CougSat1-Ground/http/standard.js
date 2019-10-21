/**
 * Chooses an appropriate metric prefix and formats the number with that prefix
 *
 * @param {Number} number
 * @param {Integer} sigFigs
 */
function formatMetricPrefix(number, sigFigs) {
  var exponent = Math.floor(Math.log10(Math.abs(number)) / 3);
  exponent     = Math.min(3, Math.max(-3, exponent));
  var prefix   = [" n", " μ", " m", " ", " k", " M", " G"][exponent + 3];
  number       = number / Math.pow(1000, exponent);
  return number.toPrecision(sigFigs) + prefix;
}

/**
 * Format a number to degrees
 *
 * @param {DOMElement} element
 */
function formatDegrees(element) {
  if (element == document.activeElement)
    return; // Don't change the focused element

  var num = element["num"];
  if (String(num).length == 0) {
    element.innerHTML = "";
    element.value     = "";
    return;
  }
  if (num == "infinity") {
    element.innerHTML = "infinity";
    element.value     = "infinity";
    return;
  }

  element.innerHTML = num.toFixed(1) + " &deg;";
}

/**
 * Format a number to time countdown "T-30"
 *
 * @param {DOMElement} element
 */
function formatTime(element) {
  if (element == document.activeElement)
    return; // Don't change the focused element

  var num = element["num"];
  if (String(num).length == 0) {
    element.innerHTML = "";
    element.value     = "";
    return;
  }
  if (num == "infinity") {
    element.innerHTML = "infinity";
    element.value     = "infinity";
    return;
  }

  if (num < 0)
    element.innerHTML = "T-" + Math.abs(num).toFixed(0);
  else
    element.innerHTML = "T+" + Math.abs(num).toFixed(0);
}

/**
 * Format a number to parts per million
 *
 * @param {DOMElement} element
 */
function formatPPM(element) {
  if (element == document.activeElement)
    return; // Don't change the focused element

  var num = element["num"];
  if (String(num).length == 0) {
    element.innerHTML = "";
    element.value     = "";
    return;
  }
  if (num == "infinity") {
    element.innerHTML = "infinity";
    element.value     = "infinity";
    return;
  }

  num               = num * 1e6;
  element.innerHTML = num.toFixed(1) + " ppm";
}

/**
 * Set the innerHTML and value of the element to its num with percent formatting
 * and number of significant figures
 *
 * attributes: sigfig
 * @param {DOMElement} element to process
 */
function numToPercent(element) {
  if (element == document.activeElement)
    return; // Don't change the focused element

  var num = element["num"];
  if (String(num).length == 0) {
    element.innerHTML = "";
    element.value     = "";
    return;
  }
  if (num == "infinity") {
    element.innerHTML = "infinity";
    element.value     = "infinity";
    return;
  }
  num               = num.toPrecision(element.getAttribute("sigfig"));
  element.innerHTML = num + "%";
  element.value     = num + "%";
}