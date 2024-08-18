(() => {
  function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms))
  }

  class Lights {
    constructor(rgb) {
      this.n_lights = rgb.length / 3;
      this.rgb = rgb;
      this.leds = null;
      this.tickMS = 100;
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

        const phi = (0.5 + i / this.n_lights) * 2 * Math.PI;
        const cosphi = Math.cos(phi);
        const sinphi = Math.sin(phi);
        const y = (1 - cosphi) * cosphi;
        const x = (1 - cosphi) * sinphi;

        const xScaled = 50 + 50*(x/2);
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
        Module._tick(this.clock & 0xffff);
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

    lights = new Lights(rgb);
    window.lights = lights;

    const container = document.getElementById('container');
    lights.renderHtml(container);

    lights.start();

    Module._tick(20);
  };

  window.Module = {
    onRuntimeInitialized: main,
  };
})();
