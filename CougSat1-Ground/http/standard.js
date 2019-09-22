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