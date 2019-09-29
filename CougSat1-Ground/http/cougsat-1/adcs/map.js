// Satellite map adapted from
// https://github.com/richiecarmichael/Esri-Satellite-Map

var TRAJECTORY_SEGMENTS = 1000;

require(
    [
      "esri/Map", "esri/Camera", "esri/views/SceneView",
      "esri/views/3d/externalRenderers", "dojo/number", "dojo/string",
      "dojo/domReady!"
    ],
    function(Map, Camera, SceneView, ExternalRenderers, number, string) {
      var view        = new SceneView({
        map: new Map({basemap: "satellite"}),
        container: "map",
        ui: {components: ["zoom", "compass"]},
        environment: {
          lighting: {
            date: new Date(),
            directShadowsEnabled: true,
            ambientOcclusionEnabled: false,
            cameraTrackingEnabled: false
          },
          atmosphereEnabled: true,
          atmosphere: {quality: "high"},
          starsEnabled: true
        },
        constraints: {altitude: {max: 12000000000}},
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
        satellites: [],
        normal: null,
        trajectory: null,
        setup: function(context) { // Store view
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
          // the default framebuffers. We have to inject this bit of code into
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

          this.scene  = new THREE.Scene();
          this.camera = new THREE.PerspectiveCamera();

          // Create both a directional light, as well as an ambient light
          this.directionalLight = new THREE.DirectionalLight(0xffffff, 0.5);
          this.ambientLight     = new THREE.AmbientLight(0xffffff, 0.5);
          this.scene.add(this.directionalLight, this.ambientLight);

          // Create objects and add them to the scene
          this.normal = new THREE.Points(
              new THREE.BufferGeometry(), new THREE.PointsMaterial({
                color: 0xff7f00,
                size: 6,
                sizeAttenuation: false,
                vertexColors: THREE.NoColors,
                fog: false
              }));

          this.trajectory = new THREE.Line(
              new THREE.BufferGeometry(), new THREE.LineBasicMaterial({
                color: 0xffffff,
                linewidth: 1,
                vertexColors: THREE.NoColors,
                fog: false
              }));
          this.trajectory.frustumCulled = false;

          // Add all satellites to normal layer.
          var vertex = [];
          var valid  = [];
          var date   = new Date();
          for (var i = 0; i < this.satellites.length; i++) {
            var satellite = this.satellites[i];
            var eci       = this.getSatelliteLocation(satellite, date);
            if (eci === null || eci === undefined || isNaN(eci.x) ||
                isNaN(eci.y) || isNaN(eci.z)) {
              continue;
            }
            vertex.push(eci.x * 1000, eci.y * 1000, eci.z * 1000);
            valid.push(satellite);
          }

          // Satellites that returned a computable location.
          this.satellites = valid;

          // Assign buffer.
          var position = new THREE.Float32Attribute(vertex, 3);
          position.setDynamic(true);
          this.normal.geometry.addAttribute("position", position);

          // Add to scene
          this.scene.add(this.normal, this.trajectory);

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

          // Request a re-render
          ExternalRenderers.requestRender(this.view);

          // Cleanup
          context.resetWebGLState();
        },
        getSatelliteLocation: function(satrec, date) {
          var position_and_velocity = satellite.propagate(satrec,
              date.getUTCFullYear(), date.getUTCMonth() + 1, date.getUTCDate(),
              date.getUTCHours(), date.getUTCMinutes(), date.getUTCSeconds());
          return position_and_velocity.position;
        },
        showOrbit: function() {
          // Time between orbital vertex
          var ms = 114.87 * 60000;
          ms /= TRAJECTORY_SEGMENTS;

          // Show orbit
          var vertex = [];
          var start  = new Date();
          for (var i = 0; i <= TRAJECTORY_SEGMENTS; i++) {
            var date = new Date(start.valueOf() + i * ms);
            var eci  = this.getSatelliteLocation(this.satellites[0], date);
            if (eci === null || eci === undefined || isNaN(eci.x) ||
                isNaN(eci.y) || isNaN(eci.z)) {
              continue;
            }
            vertex.push(eci.x * 1000, eci.y * 1000, eci.z * 1000);
          }
          this.trajectory.geometry.removeAttribute("position");
          this.trajectory.geometry.addAttribute(
              "position", new THREE.Float32Attribute(vertex, 3));

          // Immediately request a new redraw
          ExternalRenderers.requestRender(this.view);
        }
      };
      var satrec = satellite.twoline2satrec(
          "1  7530U 74089B   19271.85075181 -.00000044  00000-0  71821-5 0  9991",
          "2  7530 101.7568 238.5079 0012202 148.2872 267.9139 12.53640078 53324");
      satRenderer.satellites.push(satrec);
      ExternalRenderers.add(view, satRenderer);
      console.log(satRenderer.trajectory);
      setTimeout(function() {
        satRenderer.showOrbit();
      }, 2000);
    });