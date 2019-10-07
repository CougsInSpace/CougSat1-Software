// Satellite map adapted from
// https://github.com/richiecarmichael/Esri-Satellite-Map

var TRAJECTORY_SEGMENT_HR = 100;
var TRAJECTORY_STEP_MS    = (60 * 60 * 1000) / TRAJECTORY_SEGMENT_HR;
var UPDATE_INTERVAL_MS    = 500;

var attitude           = new THREE.Euler(0, 0, 0, "XYZ");
var vectorCamWide      = new THREE.Vector3(0, 0, -1);
var vectorCamNarrow    = new THREE.Vector3(0, 0, -1);
var vectorAntennaHigh  = new THREE.Vector3(0, 0, -1);
var vectorMagnetorquer = new THREE.Vector3(1, 0, 0);
var vectorSun          = new THREE.Vector3(-0.3, -3, 6);

var fovCamWide     = 142 / 180 * Math.PI;
var fovCamNarrow   = 21 / 180 * Math.PI;
var fovAntennaHigh = 40 / 180 * Math.PI;

var ground     = null;
var gps        = null;
var antennaLow = null;

require(
    [
      "esri/Map", "esri/Camera", "esri/views/SceneView",
      "esri/views/3d/externalRenderers", "esri/layers/GraphicsLayer",
      "esri/Graphic", "dojo/number", "dojo/string", "dojo/domReady!"
    ],
    function(Map, Camera, SceneView, ExternalRenderers, GraphicsLayer, Graphic,
        number, string) {
      var view = new SceneView({
        map: new Map({basemap: "satellite"}),
        container: "map",
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

      graphicsLayer = new GraphicsLayer();
      view.map.add(graphicsLayer);

      var groundPoint = {
        type: "point", // autocasts as new Point()
        x: -117.17,
        y: 46.73,
        z: 0
      };
      var groundSymbol = {
        type: "simple-marker",
        color: "#2BEEF5",
        size: 6,
        outline: {width: 0}
      };
      ground = new Graphic({geometry: groundPoint, symbol: groundSymbol})
      graphicsLayer.add(ground);

      var gpsPoint = {
        type: "point", // autocasts as new Point()
        x: -117.17,
        y: 46.73,
        z: 0
      };
      var gpsSymbol = {
        type: "simple-marker",
        color: "#18FF65",
        size: 6,
        outline: {width: 0}
      };
      gps = new Graphic({geometry: gpsPoint, symbol: gpsSymbol})
      graphicsLayer.add(gps);

      var satRenderer = {
        view: null,
        renderer: null,
        camera: null,
        scene: null,
        vbo: null,
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
          this.satellite.frustumCulled = false;

          this.trajectory = new THREE.Line(
              new THREE.BufferGeometry(), new THREE.LineBasicMaterial({
                color: 0xFFFFFF,
                linewidth: 2,
                vertexColors: THREE.NoColors,
                fog: false
              }));
          this.trajectory.frustumCulled = false;

          this.magnetorquer = new THREE.ArrowHelper(
              new THREE.Vector3(), new THREE.Vector3(), 1000000, 0xFF2626);
          this.magnetorquer.frustumCulled = false;

          this.sun = new THREE.ArrowHelper(
              new THREE.Vector3(), new THREE.Vector3(), 1000000, 0xFFFB16);
          this.sun.frustumCulled = false;

          this.cameraWide = new THREE.Mesh(
              new THREE.ConeGeometry(
                  Math.tan(fovCamWide / 2) * 1000000, 1000000, 32),
              new THREE.MeshBasicMaterial({
                color: 0x31B431,
                transparent: true,
                opacity: 0.25,
                depthWrite: false
              }));
          this.cameraWide.geometry.translate(0, -500000, 0);
          this.cameraWide.geometry.rotateX(-Math.PI / 2);
          this.cameraWide.renderOrder   = 3;
          this.cameraWide.frustumCulled = false;

          this.cameraNarrow = new THREE.Mesh(
              new THREE.ConeGeometry(
                  Math.tan(fovCamNarrow / 2) * 1000000, 1000000, 32),
              new THREE.MeshBasicMaterial({
                color: 0x5782C2,
                transparent: true,
                opacity: 0.25,
                depthWrite: false
              }));
          this.cameraNarrow.geometry.translate(0, -500000, 0);
          this.cameraNarrow.geometry.rotateX(-Math.PI / 2);
          this.cameraNarrow.renderOrder   = 1;
          this.cameraNarrow.frustumCulled = false;

          this.antennaHigh = new THREE.Mesh(
              new THREE.ConeGeometry(
                  Math.tan(fovAntennaHigh / 2) * 1000000, 1000000, 32),
              new THREE.MeshBasicMaterial({
                color: 0xA149F3,
                transparent: true,
                opacity: 0.25,
                depthWrite: false
              }));
          this.antennaHigh.geometry.translate(0, -500000, 0);
          this.antennaHigh.geometry.rotateX(-Math.PI / 2);
          this.antennaHigh.renderOrder   = 2;
          this.antennaHigh.frustumCulled = false;

          // Add to scene
          this.scene.add(this.satellite, this.trajectory, this.magnetorquer,
              this.sun, this.cameraWide, this.cameraNarrow, this.antennaHigh);

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
          this.camera.near - c.near;

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
        /**
         * Get the position and velocity of the satellite at a date in Earth
         * fixed coordinates
         *
         * @param {satellite.satrec} satrec satellite record from the TLE
         * @param {Date} date to know where the earth is rotated
         */
        getSatelliteDynamics: function(satrec, date) {
          var result = satellite.propagate(satrec, date.getUTCFullYear(),
              date.getUTCMonth() + 1, date.getUTCDate(), date.getUTCHours(),
              date.getUTCMinutes(), date.getUTCSeconds());
          return this.eciToEcf(result, date);
        },
        /**
         * Convert Earth inertial coordinates to Earth fixed coordinates
         *
         * @param {set} dynamics position and velocity, each are sets of x, y, z
         * @param {Date} date to know where the earth is rotated
         */
        eciToEcf: function(dynamics, date) {
          var omega       = 7.29211514668855e-5; // rad/sec of earth
          var gmst        = satellite.gstimeFromDate(date.getUTCFullYear(),
              date.getUTCMonth() + 1, date.getUTCDate(), date.getUTCHours(),
              date.getUTCMinutes(), date.getUTCSeconds());
          var ecfPosition = satellite.eciToEcf(dynamics.position, gmst);
          var ecfVelocity = satellite.eciToEcf(dynamics.velocity, gmst);
          ecfVelocity.x += omega * ((-dynamics.position.x * Math.sin(gmst)) +
                                       (dynamics.position.y * Math.cos(gmst)));
          ecfVelocity.y += omega * ((-dynamics.position.x * Math.cos(gmst)) -
                                       (dynamics.position.y * Math.sin(gmst)));
          ecfPosition.x *= 1000.0; // km to m
          ecfPosition.y *= 1000.0;
          ecfPosition.z *= 1000.0;
          ecfVelocity.x *= 1000.0; // km/s to m/s
          ecfVelocity.y *= 1000.0;
          ecfVelocity.z *= 1000.0;
          return {position: ecfPosition, velocity: ecfVelocity};
        },
        /**
         * Set the properties of an arrow, transforming from satellite's XYZ to
         * world direction coordinates
         *
         * @param {THREE.ArrowHelper} arrow
         * @param {THREE.Vector3} vector relative to the satellite's XYZ
         * @param {THREE.Vector3} attitude of the satellite's XYZ to its
         *     trajectory vector (aka roll, pitch, yaw)
         * @param {THREE.Vector3} position of the satellite origin
         * @param {THREE.Vector3} x basis of trajectory vector
         * @param {THREE.Vector3} y basis of trajectory vector
         * @param {THREE.Vector3} z basis of trajectory vector
         */
        updateArrow: function(arrow, vector, attitude, position, x, y, z) {
          // Rotate the satellite vector to the trajectory frame (roll, pitch,
          // yaw)
          var trajectoryFrame = vector.clone().applyEuler(attitude);

          // Project trajectory frame onto the world frame given the trajectory
          // frame's basis vectors
          var worldFrame = new THREE.Vector3();
          worldFrame.addScaledVector(x, trajectoryFrame.x);
          worldFrame.addScaledVector(y, trajectoryFrame.y);
          worldFrame.addScaledVector(z, trajectoryFrame.z);
          worldFrame.normalize();
          arrow.position.copy(position);
          arrow.setDirection(worldFrame);
        },
        /**
         * Set the properties of an obj, transforming from satellite's XYZ to
         * world direction coordinates
         *
         * @param {THREE.Mesh} obj
         * @param {THREE.Vector3} vector relative to the satellite's XYZ
         * @param {THREE.Vector3} attitude of the satellite's XYZ to its
         *     trajectory vector (aka roll, pitch, yaw)
         * @param {THREE.Vector3} position of the satellite origin
         * @param {THREE.Vector3} x basis of trajectory vector
         * @param {THREE.Vector3} y basis of trajectory vector
         * @param {THREE.Vector3} z basis of trajectory vector
         */
        updateMesh: function(obj, vector, attitude, position, x, y, z) {
          // Rotate the satellite vector to the trajectory frame (roll, pitch,
          // yaw)
          var trajectoryFrame = vector.clone().applyEuler(attitude);

          // Project trajectory frame onto the world frame given the trajectory
          // frame's basis vectors
          var worldFrame = new THREE.Vector3();
          worldFrame.addScaledVector(x, trajectoryFrame.x);
          worldFrame.addScaledVector(y, trajectoryFrame.y);
          worldFrame.addScaledVector(z, trajectoryFrame.z);
          worldFrame.normalize().scale;
          var matrix = new THREE.Matrix4().lookAt(worldFrame,
              new THREE.Vector3(0, 0, 0), new THREE.Vector3(0, 0, 1));

          obj.position.copy(position);
          obj.setRotationFromMatrix(matrix);
        },
        /**
         * Update the map based on the trajectory, date, and desired elements
         *
         * @param {satellite.satrec} satrec satellite record from the TLE
         * @param {Date} date to calculate everything for
         * @param {float} orbitHours to project the trajectory for
         * @param {set} enabledElements to display: cameraWide, cameraNarrow,
         *     antennaLow, antennaHigh, magnetorquer, sun
         */
        updateSatellite: function(satrec, date, orbitHours, enabledElements) {
          // Satellite position
          var vertex   = [];
          var dynamics = this.getSatelliteDynamics(satrec, date);
          vertex.push(
              dynamics.position.x, dynamics.position.y, dynamics.position.z);
          this.satellite.geometry.removeAttribute("position");
          this.satellite.geometry.addAttribute(
              "position", new THREE.Float32Attribute(vertex, 3));

          var position = new THREE.Vector3(
              dynamics.position.x, dynamics.position.y, dynamics.position.z);

          // Generate the coordinate system for roll pitch yaw
          // x = velocity, y = z cross x, z = position
          var x = (new THREE.Vector3(dynamics.velocity.x, dynamics.velocity.y,
                       dynamics.velocity.z))
                      .normalize();
          var z = position.clone().normalize();
          var y = (new THREE.Vector3()).crossVectors(z, x);

          this.updateArrow(this.magnetorquer, vectorMagnetorquer, attitude,
              position, x, y, z);
          this.magnetorquer.visible = enabledElements.magnetorquer;

          this.updateArrow(this.sun, vectorSun, attitude, position, x, y, z);
          this.sun.visible = enabledElements.sun;

          this.updateMesh(
              this.cameraWide, vectorCamWide, attitude, position, x, y, z);
          this.cameraWide.visible = enabledElements.cameraWide;

          this.updateMesh(
              this.cameraNarrow, vectorCamNarrow, attitude, position, x, y, z);
          this.cameraNarrow.visible = enabledElements.cameraNarrow;

          this.updateMesh(
              this.antennaHigh, vectorAntennaHigh, attitude, position, x, y, z);
          this.antennaHigh.visible = enabledElements.antennaHigh;

          // Orbit projection
          var segments = TRAJECTORY_SEGMENT_HR * orbitHours;
          vertex       = [];
          for (var i = 0; i <= segments; i++) {
            var trajectoryDate =
                new Date(date.valueOf() + i * TRAJECTORY_STEP_MS);
            dynamics = this.getSatelliteDynamics(satrec, trajectoryDate);
            vertex.push(
                dynamics.position.x, dynamics.position.y, dynamics.position.z);
          }
          this.trajectory.geometry.removeAttribute("position");
          this.trajectory.geometry.addAttribute(
              "position", new THREE.Float32Attribute(vertex, 3));

          view.environment.lighting.date = date;

          // Immediately request a new redraw
          ExternalRenderers.requestRender(this.view);
        }
      };
      ExternalRenderers.add(view, satRenderer);
      view.when(function() {
        var satelliteTime = document.getElementById("satellite-time");
        var date          = new Date();
        var timeStep      = UPDATE_INTERVAL_MS;
        var orbitHours    = 1.7;

        var enabled = {
          cameraWide: true,
          cameraNarrow: true,
          antennaLow: true,
          antennaHigh: true,
          magnetorquer: true,
          sun: true
        };

        var update = function() {
          if (satelliteTime != document.activeElement)
            satelliteTime.value = date.toDatetimeLocal();
          gps.geometry = { // TODO Replace with getting GPS from telemetry
            type: "point", // autocasts as new Point()
            x: gps.geometry.x + 1,
            y: 46.73,
            z: 0
          };
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
              ground.visible = event.target.checked;
            });

        document.getElementById("en-gps").addEventListener(
            "input", function(event) {
              gps.visible = event.target.checked;
            });

        satelliteTime.addEventListener("input", function(event) {
          date = new Date(event.target.value);
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
