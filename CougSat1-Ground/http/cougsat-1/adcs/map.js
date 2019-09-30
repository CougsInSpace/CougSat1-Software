// Satellite map adapted from
// https://github.com/richiecarmichael/Esri-Satellite-Map

var TRAJECTORY_SEGMENT_HR = 50;
var TRAJECTORY_STEP_MS    = (60 * 60 * 1000) / TRAJECTORY_SEGMENT_HR;
var UPDATE_INTERVAL_MS    = 500;

var satelliteRoll  = 0;
var satellitePitch = 0;
var satelliteYaw   = 0;

require(
    [
      "esri/Map", "esri/Camera", "esri/views/SceneView",
      "esri/views/3d/externalRenderers", "dojo/number", "dojo/string",
      "dojo/domReady!"
    ],
    function(Map, Camera, SceneView, ExternalRenderers, number, string) {
      var view = new SceneView({
        map: new Map({basemap: "satellite"}),
        container: "map",
        ui: {components: ["zoom", "compass"]},
        environment: {
          lighting: {
            date: new Date(),
            directShadowsEnabled: false,
            ambientOcclusionEnabled: false,
            cameraTrackingEnabled: false
          },
          atmosphereEnabled: true,
          atmosphere: {quality: "high"},
          starsEnabled: true
        },
        constraints: {altitude: {max: 250000000}},
        camera: {
          position: {latitude: 46.73, longitude: -117.17, altitude: 25000000}
        }
      });

      var satRenderer = {
        view: null,
        renderer: null,
        camera: null,
        scene: null,
        vbo: null,
        satellite: null,
        trajectory: null,
        setup: function(context) {
          this.view = context.view;

          // Create the THREE.js webgl renderer
          this.renderer = new THREE.WebGLRenderer(
              {context: context.gl, premultipliedAlpha: false});

          // Set renderer pixel settings
          this.renderer.setPixelRatio(window.devicePixelRatio);
          this.renderer.setSize(this.view.size[0], this.view.size[1]);

          // Make sure it does not clear anything before rendering
          this.renderer.autoClearDepth   = false;
          this.renderer.autoClearColor   = false;
          this.renderer.autoClearStencil = false;

          // The ArcGIS JS API renders to custom offscreen buffers, and not to
          // the default framebuffer. We have to inject this bit of code into
          // the three.js runtime in order for it to bind those buffers instead
          // of the default ones.
          var originalSetRenderTarget =
              this.renderer.setRenderTarget.bind(this.renderer);

          this.renderer.setRenderTarget = function(target) {
            originalSetRenderTarget(target);
            if (target === null) {
              context.bindRenderTarget();
            }
          };

          // Create THREE.js scene
          this.scene  = new THREE.Scene();
          this.camera = new THREE.PerspectiveCamera();

          // Create both a directional light, as well as an ambient light
          this.directionalLight = new THREE.DirectionalLight(0xFFFFFF, 0.5);
          this.ambientLight     = new THREE.AmbientLight(0xFFFFFF, 0.5);
          this.scene.add(this.directionalLight, this.ambientLight);

          // Create objects and add them to the scene
          this.satellite = new THREE.Points(
              new THREE.BufferGeometry(), new THREE.PointsMaterial({
                color: 0xFF9640,
                size: 5,
                sizeAttenuation: false,
                vertexColors: THREE.NoColors,
                fog: false
              }));

          this.trajectory = new THREE.Line(
              new THREE.BufferGeometry(), new THREE.LineBasicMaterial({
                color: 0xFFFFFF,
                linewidth: 2,
                vertexColors: THREE.NoColors,
                fog: false
              }));
          this.trajectory.frustumCulled = false;

          // Add to scene
          this.scene.add(this.satellite, this.trajectory);

          // Refresh the screen
          ExternalRenderers.requestRender(this.view);
        },
        render: function(context) {
          // Get Esri's camera
          var c = context.camera;

          // Update three.js camera
          this.camera.position.set(c.eye[0], c.eye[1], c.eye[2]);
          this.camera.up.set(c.up[0], c.up[1], c.up[2]);
          this.camera.lookAt(
              new THREE.Vector3(c.center[0], c.center[1], c.center[2]));
          this.camera.projectionMatrix.fromArray(c.projectionMatrix);

          // Get Esri's current sun settings
          this.view.environment.lighting.date = this.date;

          // Update lighting
          var direction = context.sunLight.direction;
          var diffuse   = context.sunLight.diffuse;
          var ambient   = context.sunLight.ambient;

          // Update the directional light color, intensity and position
          this.directionalLight.color.setRGB(
              diffuse.color[0], diffuse.color[1], diffuse.color[2]);
          this.directionalLight.intensity = diffuse.intensity;
          this.directionalLight.position.set(
              direction[0], direction[1], direction[2]);

          // Update the ambient light color and intensity
          this.ambientLight.color.setRGB(
              ambient.color[0], ambient.color[1], ambient.color[2]);
          this.ambientLight.intensity = ambient.intensity;

          // Render the scene
          this.renderer.resetGLState();
          this.renderer.render(this.scene, this.camera);

          // Cleanup
          context.resetWebGLState();
        },
        getSatelliteLocation: function(satrec, date) {
          var position_and_velocity = satellite.propagate(satrec,
              date.getUTCFullYear(), date.getUTCMonth() + 1, date.getUTCDate(),
              date.getUTCHours(), date.getUTCMinutes(), date.getUTCSeconds());

          var ecf = satellite.eciToEcf(position_and_velocity.position,
              satellite.gstimeFromDate(date.getUTCFullYear(),
                  date.getUTCMonth() + 1, date.getUTCDate(), date.getUTCHours(),
                  date.getUTCMinutes(), date.getUTCSeconds()));
          return ecf;
        },
        updateSatellite: function(satrec, date, orbitHours, enabledElements) {
          // Satellite position
          var vertex = [];
          var pos    = this.getSatelliteLocation(satrec, date);
          if (pos === null || pos === undefined || isNaN(pos.x) ||
              isNaN(pos.y) || isNaN(pos.z)) {
            console.log("Bad satellite location from: ", satrec);
            return;
          }
          vertex.push(pos.x * 1000, pos.y * 1000, pos.z * 1000);

          this.satellite.geometry.removeAttribute("position");
          this.satellite.geometry.addAttribute(
              "position", new THREE.Float32Attribute(vertex, 3));

          // Orbit projection
          var segments = TRAJECTORY_SEGMENT_HR * orbitHours;
          vertex       = [];
          for (var i = 0; i <= segments; i++) {
            var trajectoryDate =
                new Date(date.valueOf() + i * TRAJECTORY_STEP_MS);
            pos = this.getSatelliteLocation(satrec, trajectoryDate);
            if (pos === null || pos === undefined || isNaN(pos.x) ||
                isNaN(pos.y) || isNaN(pos.z)) {
              console.log("Bad satellite location from: ", satrec);
              continue;
            }
            vertex.push(pos.x * 1000, pos.y * 1000, pos.z * 1000);
          }
          this.trajectory.geometry.removeAttribute("position");
          this.trajectory.geometry.addAttribute(
              "position", new THREE.Float32Attribute(vertex, 3));

          // Immediately request a new redraw
          ExternalRenderers.requestRender(this.view);
        }
      };
      ExternalRenderers.add(view, satRenderer);
      view.when(function() {
        var satelliteTime = document.getElementById("satellite-time");
        var date          = new Date();
        var timeStep      = UPDATE_INTERVAL_MS;
        var orbitHours    = 1.5;

        var enabled = {
          cameraWide: false,
          cameraNarrow: false,
          antennaLow: false,
          antennaHigh: false,
          magnetorquer: false,
          sun: false,
          ground: false,
          gps: false
        };

        var update = function() {
          if (satelliteTime != document.activeElement)
            satelliteTime.value = date.toDatetimeLocal();
          satRenderer.updateSatellite(satrec, date, orbitHours, enabled);
        };

        document.getElementById("time-speed")
            .addEventListener("input", function(event) {
              timeStep = event.target.value * UPDATE_INTERVAL_MS;
              update();
            });

        document.getElementById("time-projection")
            .addEventListener("input", function(event) {
              orbitHours = event.target.value;
              update();
            });

        document.getElementById("en-cam-wide")
            .addEventListener("input", function(event) {
              enabled.cameraWide = event.target.checked;
              update();
            });

        document.getElementById("en-cam-wide")
            .addEventListener("input", function(event) {
              enabled.cameraWide = event.target.checked;
              update();
            });

        document.getElementById("en-cam-narrow")
            .addEventListener("input", function(event) {
              enabled.cameraNarrow = event.target.checked;
              update();
            });

        document.getElementById("en-antenna-low")
            .addEventListener("input", function(event) {
              enabled.antennaLow = event.target.checked;
              update();
            });

        document.getElementById("en-antenna-high")
            .addEventListener("input", function(event) {
              enabled.antennaHigh = event.target.checked;
              update();
            });

        document.getElementById("en-magnetorquer")
            .addEventListener("input", function(event) {
              enabled.magnetorquer = event.target.checked;
              update();
            });

        document.getElementById("en-sun-vector")
            .addEventListener("input", function(event) {
              enabled.sun = event.target.checked;
              update();
            });

        document.getElementById("en-ground")
            .addEventListener("input", function(event) {
              enabled.ground = event.target.checked;
              update();
            });

        document.getElementById("en-gps").addEventListener(
            "input", function(event) {
              enabled.gps = event.target.checked;
              update();
            });

        satelliteTime.addEventListener("input", function(event) {
          date = new Date(event.target.value);
          console.log(event.target.value);
          update();
        });

        var satrec = satellite.twoline2satrec(
            "1 25544U 98067A   19272.17332272  .00000706  00000-0  20336-4 0  9999",
            "2 25544  51.6426 205.5801 0007437  95.6316 228.4969 15.50113298191385")
        setInterval(function() {
          if (satelliteTime != document.activeElement) {
            date = new Date(date.valueOf() + timeStep);
            update();
          }
        }, UPDATE_INTERVAL_MS);
      });
    });

/**
 * Convert a date to a string in the format "YYY-MM-DDTHH:mm:ss"
 * From WebReflection:
 * https://gist.github.com/WebReflection/6076a40777b65c397b2b9b97247520f0
 */
Date.prototype.toDatetimeLocal = function toDatetimeLocal() {
  var date = this,
      ten =
          function(i) {
        return (i < 10 ? "0" : "") + i;
      },
      YYYY = date.getFullYear(), MM = ten(date.getMonth() + 1),
      DD = ten(date.getDate()), HH = ten(date.getHours()),
      II = ten(date.getMinutes()), SS = ten(date.getSeconds());
  return YYYY + "-" + MM + "-" + DD + "T" + HH + ":" + II + ":" + SS;
};
