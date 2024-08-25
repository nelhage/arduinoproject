(() => {
  const PHI = [3.1415926535897927, 3.1749573455210878, 3.2083312419698506, 3.2417235706508833, 3.275143605835007, 3.308600692032667, 3.3421042681704836, 3.3756638924356057, 3.4092892679720643, 3.4429902696254167, 3.4767769719470496, 3.510659678688022, 3.5446489540346477, 3.5787556558647546, 3.6129909713354227, 3.6473664551507947, 3.6818940709033914, 3.7165862359354893, 3.7514558702301306, 3.786516449916209, 3.8217820660611963, 3.85726748953149, 3.892988242827782, 3.928960679956027, 3.9652020755793864, 4.001730724920512, 4.038566056156254, 4.07572875738057, 4.113240920621977, 4.1511262059098915, 4.189410029016554, 4.228119777293687, 4.2672850590227736, 4.30693799296877, 4.347113546455238, 4.387849932382831, 4.429189078356262, 4.4711771846970265, 4.513865392925532, 4.557310592765191, 4.601576404535458, 4.646734385973632, 4.692865529584318, 4.740062140909517, 4.788430223328113, 4.838092547051852, 4.88919265863993, 4.941900209138832, 4.9964181728004045, 5.052992846739788, 5.1119280641662925, 5.173606017000367, 5.238518881385293, 5.307319000044805, 5.380902958011627, 5.4605625776902675, 5.5482823240628765, 5.647406198575428, 5.764462365062285, 5.916333783251, 8.615330671091215e-14, 0.3668515239285872, 0.5187229421173019, 0.6357791086041589, 0.7349029831167107, 0.8226227294893196, 0.9022823491679599, 0.9758663071347824, 1.0446664257942944, 1.1095792901792203, 1.1712572430132955, 1.2301924604397998, 1.2867671343791836, 1.341285098040756, 1.3939926485396592, 1.4450927601277366, 1.4947550838514765, 1.5431231662700737, 1.5903197775952727, 1.6364509212059577, 1.6816089026441317, 1.7258747144143989, 1.7693199142540585, 1.812008122482565, 1.8539962288233287, 1.89533537479676, 1.936071760724353, 1.97624731421082, 2.0159002481568162, 2.055065529885903, 2.0937752781630348, 2.1320591012696966, 2.16994438655761, 2.2074565497990175, 2.244619251023332, 2.281454582259075, 2.3179832316002003, 2.3542246272235587, 2.3901970643518045, 2.425917817648095, 2.46140324111839, 2.4966688572633764, 2.5317294369494547, 2.566599071244095, 2.601291236276195, 2.635818852028791, 2.6701943358441635, 2.7044296513148307, 2.738536353144939, 2.7725256284915645, 2.806408335232536, 2.840195037554171, 2.873896039207523, 2.9075214147439805, 2.9410810390091022, 2.9745846151469184, 3.0080417013445793, 3.041461736528703, 3.0748540652097347, 3.108227961658498];

  function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms))
  }

  class Lights {
    constructor(rgb, config) {
      this.n_lights = rgb.length / 3;
      this.rgb = rgb;
      this.leds = null;
      this.config = config;
      this.tickMS = config.tickMS;
      if (this.tickMS === null) {
        this.tickMS = 50;
      }
      this.mode = config.mode;
      if (this.mode === null) {
        this.mode = 3;
      }
      this.clock = 0;
    }

    renderHtml(container) {
      if (this.leds !== null) {
        throw new AssertionError("must only render once!");
      }
      const leds = []

      for (let i = 0; i < this.n_lights; i++) {
        const led = document.createElement('div');
        led.classList.add('led');
        led.id = "led-" + i;

        const phi = PHI[i]; //(0.5 + i / this.n_lights) * 2 * Math.PI;
        const cosphi = Math.cos(phi);
        const sinphi = Math.sin(phi);
        const y = (1 - cosphi) * cosphi;
        const x = (1 - cosphi) * sinphi;

        const xScaled = 50 - 50*(x/2);
        const yScaled = 50 - 50*((y+1)/2);

        led.style.left = xScaled + "%";
        led.style.top = yScaled + "%";

        container.appendChild(led);
        leds.push(led);
      }
      this.leds = leds;
    };

    refresh() {
      function rescale(channel) {
        let raw = channel/256;
        return 0.25 + 0.75 * channel;
      }

      let p = 0;
      for (let i = 0; i < this.n_lights; i++) {
        const g = rescale(this.rgb[p++]);
        const r = rescale(this.rgb[p++]);
        const b = rescale(this.rgb[p++]);

        this.leds[i].style.backgroundColor = `rgb(${r}%, ${g}%, ${b}%)`;
      };
    }

    async start() {
      while (true) {
        this.clock += 1;
        Module._tick(this.mode, this.clock & 0xffff);
        this.refresh();
        await sleep(this.tickMS);
      }
    }
  };

  let lights = null;

  function main() {
    const num_lights = Module._num_lights();
    const rgb = new Uint8Array(Module.HEAPU8.buffer,
                               Module._colors,
                               3*num_lights);

    let settings = {tickMS: 50, mode: 3};
    const params = new URLSearchParams(window.location.search);
    if (params.has('tick')) {
      settings.tickMS = parseInt(params.get('tick'));
    }
    if (params.has('mode')) {
      settings.mode = parseInt(params.get('mode'));
    }

    lights = new Lights(rgb, settings);
    window.lights = lights;

    const container = document.getElementById('container');
    lights.renderHtml(container);

    lights.start();
  };

  window.Module = {
    onRuntimeInitialized: main,
  };
})();
