(() => {
  class Lights {
    constructor(rgb) {
      this.n_lights = rgb.length / 3;
      this.rgb = rgb;
      this.leds = null;
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

    Module._tick(20);
  };

  window.Module = {
    onRuntimeInitialized: main,
  };
})();
