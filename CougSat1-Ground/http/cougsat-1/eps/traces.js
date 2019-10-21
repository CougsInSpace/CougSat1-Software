const Line =
    function() {
  return {
    x1: 0, y1: 0, x2: 0, y2: 0, color: "green", current: 0, currentStep: 0,
        dotSpacing: 15, update: function(x1, y1, x2, y2) {
          this.x1 = (x1);
          this.y1 = (y1);
          this.x2 = (x2);
          this.y2 = (y2);
        }, renderLine: function(ctx) {
          ctx.lineWidth = "4";
          ctx.beginPath();
          ctx.strokeStyle = this.color;
          ctx.moveTo(this.x1, this.y1);
          ctx.lineTo(this.x2, this.y2);
          ctx.stroke();
        }, length: function() {
          return (Math.sqrt(Math.pow((this.x2 - this.x1), 2) +
                            Math.pow((this.y2 - this.y1), 2)))
        }, renderCurrent: function(ctx) {
          ctx.save();
          this.currentStep = (this.currentStep + this.current * 15.0);
          if (this.currentStep > this.dotSpacing)
            this.currentStep = this.currentStep - this.dotSpacing;
          if (this.currentStep < 0.0)
            this.currentStep = this.currentStep + this.dotSpacing;
          ctx.fillStyle = "yellow";
          ctx.beginPath();
          ctx.setTransform(1, 0, 0, 1, this.x1, this.y1);
          ctx.rotate(Math.atan2(this.y2 - this.y1, this.x2 - this.x1));
          var numDots =
              Math.ceil((this.length() - this.currentStep) / this.dotSpacing)
          for (var i = 0; i < numDots; i++) {
            ctx.arc(
                i * this.dotSpacing + this.currentStep, 0, 3, 0, 2 * Math.PI);
          }
          ctx.fill();
          ctx.restore();
        }
  }
}

const Cell =
    function() {
  return {
    x1: 0, y1: 0, x2: 0, y2: 0, charge: 0, color: "green",
        update: function(x1, y1, x2, y2) {
          this.x1 = (x1);
          this.y1 = (y1);
          this.x2 = (x2);
          this.y2 = (y2);
        }, render: function(ctx) {
          ctx.beginPath();
          ctx.fillStyle  = this.color;
          var cellHeight = (this.y2 - this.y1);
          var cellWidth  = (this.x2 - this.x1);
          var height = cellHeight * 120 / 144 * this.charge / 3.2; // 3.2Ah max
          var width  = cellWidth * 480 / 520;
          var x      = this.x1 + 12 / 520 * cellWidth;
          var y      = this.y2 - height - 12 / 144 * cellHeight;
          ctx.rect(x, y, width, height);
          ctx.fill();
        }
  }
}

var critical = {
  CELL_H: 4.2,
  CELL_L: -0.3,
  BATT_H: 4.5,
  BATT_L: -0.3,
  REG_H: 3.6,
  REG_L: -0.3
};

var warning = {
  CELL_H: 3.0,
  CELL_L: 0.0,
  BATT_H: 4.2,
  BATT_L: 3.3,
  REG_H: 3.5,
  REG_L: 3.1
};

var traces = {
  canvas: null,
  ctx: null,
  canvasCurrent: null,
  ctxCurrent: null,
  pvIDSuffixes: ["0A", "0B", "1A", "1B", "2A", "2B", "3A", "3B"],
  chainIDSuffixes: ["A", "B"],
  lines: [],
  cells: [],
  setup: function() {
    this.canvas        = document.getElementById("overlay-traces");
    this.ctx           = this.canvas.getContext("2d");
    this.canvasCurrent = document.getElementById("overlay-current");
    this.ctxCurrent    = this.canvasCurrent.getContext("2d");
    for (var i = 0; i < 56; i++) {
      this.lines.push(new Line());
    }
    for (var i = 0; i < 2; i++) {
      this.cells.push(new Cell());
    }
    setInterval(function() {
      traces.renderCurrent();
    }, 50);
  },
  updateGeometry: function() {
    // Adjust the canvas to cover the body
    var body          = document.getElementById("main").getBoundingClientRect();
    this.canvas.width = body.width;
    this.canvas.height        = body.height;
    this.canvasCurrent.width  = body.width;
    this.canvasCurrent.height = body.height;

    var busIn   = document.getElementById("BUS_INPUT").getBoundingClientRect();
    var busBatt = document.getElementById("BUS_BATT").getBoundingClientRect();
    var bus3V3  = document.getElementById("BUS_3V3").getBoundingClientRect();

    var lineCount = 0;

    for (var i = 0; i < this.pvIDSuffixes.length; i++) {
      var mppt = document.getElementById("MPPT_" + this.pvIDSuffixes[i])
                     .getBoundingClientRect();

      var pv = document.getElementById("PV_" + Math.floor(i / 2))
                   .getBoundingClientRect();
      var y = (mppt.top + mppt.bottom) / 2 - body.y;

      // MPPT in
      if (pv.x < mppt.x) {
        // PV on left of MPPT
        this.lines[lineCount++].update(
            pv.right - body.x, y, mppt.left - body.x, y);
        this.lines[lineCount++].update(
            mppt.right - body.x, y, busIn.left - body.x, y);
      } else {
        // PV on right of MPPT
        this.lines[lineCount++].update(
            pv.left - body.x, y, mppt.right - body.x, y);
        this.lines[lineCount++].update(
            mppt.left - body.x, y, busIn.right - body.x, y);
      }
    }
    {
      var batteryA = document.getElementById("BATT_A").getBoundingClientRect();
      var batteryB = document.getElementById("BATT_B").getBoundingClientRect();
      this.cells[0].update(batteryA.left - body.x, batteryA.top - body.y,
          batteryA.right - body.x, batteryA.bottom - body.y);
      this.cells[1].update(batteryB.left - body.x, batteryB.top - body.y,
          batteryB.right - body.x, batteryB.bottom - body.y);

      var regA = document.getElementById("REG_A").getBoundingClientRect();
      var regB = document.getElementById("REG_B").getBoundingClientRect();

      var busX     = busIn.left + busIn.width / 3.0 - body.x;
      var busY     = (busIn.bottom + busBatt.top) / 2.0 - body.y;
      var batteryX = (batteryA.right + busIn.left) / 2.0 - body.x;
      var batteryY = (batteryA.top + batteryA.bottom) / 2.0 - body.y;
      this.lines[lineCount++].update(busX, busIn.bottom - body.y, busX, busY);
      this.lines[lineCount++].update(busX, busY, busX, busBatt.top - body.y);
      this.lines[lineCount++].update(busX, busY, batteryX, busY);
      this.lines[lineCount++].update(batteryX, busY, batteryX, batteryY);
      this.lines[lineCount++].update(
          batteryX, batteryY, batteryA.right - body.x, batteryY);

      busX = (busBatt.left + regA.right) / 2.0 - body.x;
      this.lines[lineCount++].update(
          busX, busBatt.bottom - body.y, busX, regA.top - body.y);
      this.lines[lineCount++].update(
          busX, regA.bottom - body.y, busX, bus3V3.top - body.y);

      busX     = busIn.right - busIn.width / 3 - body.x;
      batteryX = (batteryB.left + busIn.right) / 2.0 - body.x;
      this.lines[lineCount++].update(busX, busIn.bottom - body.y, busX, busY);
      this.lines[lineCount++].update(busX, busY, busX, busBatt.top - body.y);
      this.lines[lineCount++].update(busX, busY, batteryX, busY);
      this.lines[lineCount++].update(batteryX, busY, batteryX, batteryY);
      this.lines[lineCount++].update(
          batteryX, batteryY, batteryB.left - body.x, batteryY);

      busX = (busBatt.right + regB.left) / 2.0 - body.x;
      this.lines[lineCount++].update(
          busX, busBatt.bottom - body.y, busX, regB.top - body.y);
      this.lines[lineCount++].update(
          busX, regB.bottom - body.y, busX, bus3V3.top - body.y);

      var heaterA = document.getElementById("HEATER_A").getBoundingClientRect();
      var heaterB = document.getElementById("HEATER_B").getBoundingClientRect();
      var y       = (heaterA.top + heaterA.bottom) / 2.0 - body.y;
      this.lines[lineCount].color = "grey";
      this.lines[lineCount++].update(
          busBatt.left - body.x, y, heaterA.right - body.x, y);
      this.lines[lineCount].color = "grey";
      this.lines[lineCount++].update(
          busBatt.right - body.x, y, heaterB.left - body.x, y);
    }
    var outputs = document.getElementsByClassName("output");
    for (var i = 0; i < outputs.length; i++) {
      var rect                    = outputs[i].getBoundingClientRect();
      var y                       = (rect.top + rect.bottom) / 2.0 - body.y;
      this.lines[lineCount].color = "grey";
      if (rect.x < busBatt.x) {
        // Output is left of bus
        this.lines[lineCount++].update(
            busBatt.left - body.x, y, rect.right - body.x, y);
      } else {
        // Output is right of bus
        this.lines[lineCount++].update(
            busBatt.right - body.x, y, rect.left - body.x, y);
      }
    }
  },
  adjustColor(lineCount, prefix, voltage) {
    if (voltage > critical[prefix + "H"] || voltage < critical[prefix + "L"])
      this.lines[lineCount].color = "red";
    else if (voltage > warning[prefix + "H"] || voltage < warning[prefix + "L"])
      this.lines[lineCount].color = "yellow";
    else
      this.lines[lineCount].color = "green";
  },
  updateValues: function(values) {
    var lineCount = 0;

    for (var i = 0; i < this.pvIDSuffixes.length; i++) {
      this.lines[lineCount].current =
          values["PV_CELL_" + this.pvIDSuffixes[i] + "_I"];
      this.adjustColor(
          lineCount, "CELL", values["PV_CELL_" + this.pvIDSuffixes[i] + "_V"]);
      lineCount++;

      this.lines[lineCount].current =
          values["PV_IN_" + this.pvIDSuffixes[i] + "_I"];
      this.adjustColor(
          lineCount, "BATT", values["PV_IN_" + this.pvIDSuffixes[i] + "_V"]);
      lineCount++;
    }
    for (var i = 0; i < this.chainIDSuffixes.length; i++) {
      this.lines[lineCount].current =
          values["BATT_IN_" + this.chainIDSuffixes[i] + "_I"];
      this.adjustColor(lineCount, "BATT",
          values["BATT_IN_" + this.chainIDSuffixes[i] + "_V"]);
      lineCount++;

      this.lines[lineCount].current =
          values["BATT_OUT_" + this.chainIDSuffixes[i] + "_I"];
      this.adjustColor(lineCount, "BATT",
          values["BATT_OUT_" + this.chainIDSuffixes[i] + "_V"]);
      lineCount++;

      var current = values["BATT_" + this.chainIDSuffixes[i] + "_I"];
      var voltage = values["BATT_" + this.chainIDSuffixes[i] + "_V"];
      this.lines[lineCount].current = current;
      this.adjustColor(lineCount++, "BATT", voltage);
      this.lines[lineCount].current = current;
      this.adjustColor(lineCount++, "BATT", voltage);
      this.lines[lineCount].current = current;
      this.adjustColor(lineCount++, "BATT", voltage);

      this.cells[i].charge = values["BATT_" + this.chainIDSuffixes[i] + "_C"];

      this.lines[lineCount].current =
          values["REG_IN_" + this.chainIDSuffixes[i] + "_I"];
      this.adjustColor(lineCount, "BATT", voltage);
      lineCount++;

      this.lines[lineCount].current =
          values["REG_OUT_" + this.chainIDSuffixes[i] + "_I"];
      this.adjustColor(lineCount, "REG",
          values["REG_OUT_" + this.chainIDSuffixes[i] + "_V"]);
      lineCount++;
    }
    for (var i = 0; i < 2; i++) {
      this.lines[lineCount++].current = values["PR_BH_" + i + "_I"];
    }
    for (var i = 0; i < 2; i++) {
      this.lines[lineCount++].current = values["PR_DEPLOY_" + i + "_I"];
    }
    for (var i = 5; i < 7; i++) {
      this.lines[lineCount++].current = values["PR_BATT_" + i + "_I"];
    }
    for (var i = 0; i < 5; i++) {
      this.lines[lineCount++].current = values["PR_BATT_" + i + "_I"];
    }
    this.lines[lineCount++].current = values["PR_3V3_PV_I"];
    for (var i = 0; i < 7; i++) {
      this.lines[lineCount++].current = values["PR_3V3_" + i + "_I"];
    }
    this.lines[lineCount++].current = values["PR_3V3_EPS_I"];
    for (var i = 7; i < 13; i++) {
      this.lines[lineCount++].current = values["PR_3V3_" + i + "_I"];
    }
  },
  renderLine: function() {
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    for (var i = 0; i < this.lines.length; i++) {
      this.lines[i].renderLine(this.ctx);
    }
    for (var i = 0; i < this.cells.length; i++) {
      this.cells[i].render(this.ctx);
    }
  },
  renderCurrent: function() {
    this.ctxCurrent.clearRect(
        0, 0, this.canvasCurrent.width, this.canvasCurrent.height);
    for (var i = 0; i < this.lines.length; i++) {
      this.lines[i].renderCurrent(this.ctxCurrent);
    }
  }
}