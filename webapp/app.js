/**
 * TinkerSpace Modular LED Display — Web App Client Logic
 * 50x30 Matrix Visualizer, Multi-Line Space Signs, Image Uploader & Live Paint
 */

// =============================================================================
// Constants & Geometry
// =============================================================================
const MATRIX_WIDTH = 50;
const MATRIX_HEIGHT = 30;
const TOTAL_LEDS = MATRIX_WIDTH * MATRIX_HEIGHT;

// Basic 5x7 Font Definition for Client-Side Preview
const FONT_5X7 = {
  ' ': [0x00, 0x00, 0x00, 0x00, 0x00],
  '!': [0x00, 0x00, 0x5F, 0x00, 0x00],
  '"': [0x00, 0x07, 0x00, 0x07, 0x00],
  '#': [0x14, 0x7F, 0x14, 0x7F, 0x14],
  '$': [0x24, 0x2A, 0x7F, 0x2A, 0x12],
  '%': [0x23, 0x13, 0x08, 0x64, 0x62],
  '&': [0x36, 0x49, 0x55, 0x22, 0x50],
  '\'': [0x00, 0x05, 0x03, 0x00, 0x00],
  '(': [0x00, 0x1C, 0x22, 0x41, 0x00],
  ')': [0x00, 0x41, 0x22, 0x1C, 0x00],
  '*': [0x14, 0x08, 0x3E, 0x08, 0x14],
  '+': [0x08, 0x08, 0x3E, 0x08, 0x08],
  ',': [0x00, 0x50, 0x30, 0x00, 0x00],
  '-': [0x08, 0x08, 0x08, 0x08, 0x08],
  '.': [0x00, 0x60, 0x60, 0x00, 0x00],
  '/': [0x20, 0x10, 0x08, 0x04, 0x02],
  '0': [0x3E, 0x51, 0x49, 0x45, 0x3E],
  '1': [0x00, 0x42, 0x7F, 0x40, 0x00],
  '2': [0x42, 0x61, 0x51, 0x49, 0x46],
  '3': [0x21, 0x41, 0x45, 0x4B, 0x31],
  '4': [0x18, 0x14, 0x12, 0x7F, 0x10],
  '5': [0x27, 0x45, 0x45, 0x45, 0x39],
  '6': [0x3C, 0x4A, 0x49, 0x49, 0x30],
  '7': [0x01, 0x71, 0x09, 0x05, 0x03],
  '8': [0x36, 0x49, 0x49, 0x49, 0x36],
  '9': [0x06, 0x49, 0x49, 0x29, 0x1E],
  ':': [0x00, 0x36, 0x36, 0x00, 0x00],
  ';': [0x00, 0x56, 0x36, 0x00, 0x00],
  '<': [0x08, 0x14, 0x22, 0x41, 0x00],
  '=': [0x14, 0x14, 0x14, 0x14, 0x14],
  '>': [0x00, 0x41, 0x22, 0x14, 0x08],
  '?': [0x02, 0x01, 0x51, 0x09, 0x06],
  '@': [0x32, 0x49, 0x79, 0x41, 0x3E],
  'A': [0x7E, 0x11, 0x11, 0x11, 0x7E],
  'B': [0x7F, 0x49, 0x49, 0x49, 0x36],
  'C': [0x3E, 0x41, 0x41, 0x41, 0x22],
  'D': [0x7F, 0x41, 0x41, 0x22, 0x1C],
  'E': [0x7F, 0x49, 0x49, 0x49, 0x41],
  'F': [0x7F, 0x09, 0x09, 0x09, 0x01],
  'G': [0x3E, 0x41, 0x49, 0x49, 0x7A],
  'H': [0x7F, 0x08, 0x08, 0x08, 0x7F],
  'I': [0x00, 0x41, 0x7F, 0x41, 0x00],
  'J': [0x20, 0x40, 0x41, 0x3F, 0x01],
  'K': [0x7F, 0x08, 0x14, 0x22, 0x41],
  'L': [0x7F, 0x40, 0x40, 0x40, 0x40],
  'M': [0x7F, 0x02, 0x0C, 0x02, 0x7F],
  'N': [0x7F, 0x04, 0x08, 0x10, 0x7F],
  'O': [0x3E, 0x41, 0x41, 0x41, 0x3E],
  'P': [0x7F, 0x09, 0x09, 0x09, 0x06],
  'Q': [0x3E, 0x41, 0x51, 0x21, 0x5E],
  'R': [0x7F, 0x09, 0x19, 0x29, 0x46],
  'S': [0x46, 0x49, 0x49, 0x49, 0x31],
  'T': [0x01, 0x01, 0x7F, 0x01, 0x01],
  'U': [0x3F, 0x40, 0x40, 0x40, 0x3F],
  'V': [0x1F, 0x20, 0x40, 0x20, 0x1F],
  'W': [0x3F, 0x40, 0x38, 0x40, 0x3F],
  'X': [0x63, 0x14, 0x08, 0x14, 0x63],
  'Y': [0x07, 0x08, 0x70, 0x08, 0x07],
  'Z': [0x61, 0x51, 0x49, 0x45, 0x43]
};

// 24 Curated LED Palette Swatches
const PALETTE_COLORS = [
  '#ff0055', '#ff3366', '#ff5500', '#ffaa00', '#ffd700', '#ffff00',
  '#00ff66', '#00ffcc', '#00f0ff', '#0099ff', '#0044ff', '#3b82f6',
  '#7c3aed', '#a855f7', '#d946ef', '#f43f5e', '#ffffff', '#e2e8f0',
  '#94a3b8', '#64748b', '#334155', '#1e293b', '#0f172a', '#000000'
];

// Presets data
const PRESETS = {
  'space-open': {
    line1: 'TINKERSPACE', color1: '#38bdf8',
    line2: 'SPACE IS OPEN', color2: '#34d399',
    line3: 'WELCOME MAKERS', color3: '#facc15',
    align: 'center', effect: 'pulse'
  },
  'space-closed': {
    line1: 'TINKERSPACE', color1: '#f43f5e',
    line2: 'CLOSED FOR NOW', color2: '#f43f5e',
    line3: 'OPENS 10:00 AM', color3: '#94a3b8',
    align: 'center', effect: 'static'
  },
  'workshop': {
    line1: 'WORKSHOP', color1: '#facc15',
    line2: 'IN PROGRESS', color2: '#fbbf24',
    line3: 'QUIET PLEASE', color3: '#f8fafc',
    align: 'center', effect: 'static'
  },
  'printer-active': {
    line1: '3D PRINTERS', color1: '#38bdf8',
    line2: 'PRINTING NOW', color2: '#0284c7',
    line3: 'DO NOT TOUCH', color3: '#f43f5e',
    align: 'center', effect: 'blink'
  },
  'laser-active': {
    line1: 'LASER CUTTER', color1: '#a855f7',
    line2: 'ACTIVE BEAM', color2: '#e11d48',
    line3: 'WEAR GOGGLES', color3: '#facc15',
    align: 'center', effect: 'blink'
  },
  'hackathon': {
    line1: 'HACKATHON', color1: '#ec4899',
    line2: 'NIGHT SPRINT', color2: '#8b5cf6',
    line3: 'SHIP OR SLEEP', color3: '#06b6d4',
    align: 'center', effect: 'rainbow'
  },
  'coffee-break': {
    line1: 'COFFEE BREAK', color1: '#f59e0b',
    line2: 'WARM CUPS UP', color2: '#d97706',
    line3: 'KITCHEN LOUNGE', color3: '#fed7aa',
    align: 'center', effect: 'static'
  },
  'pizza-time': {
    line1: 'PIZZA TIME!', color1: '#f43f5e',
    line2: 'FRESH SLICES', color2: '#facc15',
    line3: 'CENTER TABLE', color3: '#22c55e',
    align: 'center', effect: 'pulse'
  }
};

// =============================================================================
// App State
// =============================================================================
const state = {
  // Pixel buffer: 50 * 30 * 3 (RGB values 0..255)
  pixels: new Uint8ClampedArray(TOTAL_LEDS * 3),
  brightness: 128,
  powerOn: true,
  currentMode: 'text',
  textLayout: '3line',
  textAlign: 'center',
  textEffect: 'static',
  textSpeed: 40,
  activeTool: 'pencil',
  drawColor: '#ff0055',
  isDrawing: false,
  streamLive: true,
  lastDrawCoord: null,
  uploadedImage: null
};

// =============================================================================
// DOM References
// =============================================================================
const matrixCanvas = document.getElementById('matrix-canvas');
const matrixCtx = matrixCanvas.getContext('2d');
const paintCanvas = document.getElementById('paint-canvas');
const paintCtx = paintCanvas.getContext('2d');
const toastEl = document.getElementById('toast');

// =============================================================================
// Initialization
// =============================================================================
document.addEventListener('DOMContentLoaded', () => {
  initTabs();
  initColorSwatches();
  initTextControls();
  initPaintStudio();
  initImageStudio();
  initPresetButtons();
  initAnimationCards();
  initGlobalControls();
  checkConnection();

  // Render initial demo space notice
  renderTextToMatrix();
  renderMatrixCanvas();
  renderPaintCanvas();

  // Polling connection and simulator animation
  setInterval(checkConnection, 5000);
  startSimulatorLoop();
});

// =============================================================================
// UI Feedback & Toast
// =============================================================================
function showToast(message, type = 'info') {
  const toast = document.getElementById('toast');
  if (!toast) return;
  toast.textContent = message;
  toast.className = 'toast show toast-' + type;
  clearTimeout(showToast.timer);
  showToast.timer = setTimeout(() => {
    toast.classList.remove('show');
  }, 2600);
}

// =============================================================================
// Global Controls & Connection Status
// =============================================================================
function checkConnection() {
  const pill = document.getElementById('conn-status');
  const text = document.getElementById('conn-text');

  fetch('/api/status', { method: 'GET', signal: AbortSignal.timeout(2000) })
    .then(res => res.json())
    .then(data => {
      pill.classList.remove('offline');
      pill.classList.add('online');
      text.textContent = 'ESP32 Connected';
    })
    .catch(() => {
      // Offline / Desktop Mock mode
      pill.classList.remove('online');
      pill.classList.add('offline');
      text.textContent = 'Offline / Preview';
    });
}

function initGlobalControls() {
  const bSlider = document.getElementById('global-brightness');
  const bLabel = document.getElementById('brightness-label');
  const btnPower = document.getElementById('btn-power');
  const btnClear = document.getElementById('btn-clear-matrix');
  const btnSync = document.getElementById('btn-sync-preview');
  const btnCornerTest = document.getElementById('btn-corner-test');

  bSlider.addEventListener('input', (e) => {
    state.brightness = parseInt(e.target.value);
    const pct = Math.round((state.brightness / 255) * 100);
    bLabel.textContent = `Brightness: ${pct}%`;
    sendApi('/api/brightness', { brightness: state.brightness });
    renderMatrixCanvas();
  });

  btnPower.addEventListener('click', () => {
    state.powerOn = !state.powerOn;
    btnPower.style.color = state.powerOn ? '#34d399' : '#f43f5e';
    showToast(state.powerOn ? 'Display Power ON' : 'Display Power OFF', 'info');
    sendApi('/api/power', { power: state.powerOn });
    renderMatrixCanvas();
  });

  btnClear.addEventListener('click', () => {
    clearMatrix();
    renderMatrixCanvas();
    renderPaintCanvas();
    showToast('Display Cleared', 'info');
    sendApi('/api/clear', {});
  });

  if (btnCornerTest) {
    btnCornerTest.addEventListener('click', () => {
      clearMatrix();
      // BL (0, 0): Red (255, 0, 0)
      setPixel(0, 0, 255, 0, 0);
      // BR (49, 0): Green (0, 255, 0)
      setPixel(MATRIX_WIDTH - 1, 0, 0, 255, 0);
      // TR (49, 29): White (255, 255, 255)
      setPixel(MATRIX_WIDTH - 1, MATRIX_HEIGHT - 1, 255, 255, 255);
      // TL (0, 29): Blue (0, 0, 255)
      setPixel(0, MATRIX_HEIGHT - 1, 0, 0, 255);

      renderMatrixCanvas();
      renderPaintCanvas();
      sendFramePayload();
      showToast('Corner Test: BL=Red, BR=Green, TR=White, TL=Blue', 'success');
    });
  }

  btnSync.addEventListener('click', () => {
    fetch('/api/frame')
      .then(res => res.json())
      .then(data => {
        if (data && data.frame) {
          const raw = atob(data.frame);
          for (let i = 0; i < TOTAL_LEDS && i * 3 + 2 < raw.length; i++) {
            state.pixels[i * 3]     = raw.charCodeAt(i * 3);
            state.pixels[i * 3 + 1] = raw.charCodeAt(i * 3 + 1);
            state.pixels[i * 3 + 2] = raw.charCodeAt(i * 3 + 2);
          }
          renderMatrixCanvas();
          renderPaintCanvas();
          showToast('Simulator Synced with Display', 'success');
        }
      })
      .catch(() => showToast('Display sync failed', 'error'));
  });
}

// =============================================================================
// Tab Switching
// =============================================================================
function initTabs() {
  const tabBtns = document.querySelectorAll('.tab-btn');
  const tabPanes = document.querySelectorAll('.tab-pane');

  tabBtns.forEach(btn => {
    btn.addEventListener('click', () => {
      tabBtns.forEach(b => b.classList.remove('active'));
      tabPanes.forEach(p => p.classList.remove('active'));

      btn.classList.add('active');
      const targetId = btn.getAttribute('data-tab');
      const pane = document.getElementById(targetId);
      if (pane) pane.classList.add('active');

      if (targetId === 'tab-draw') {
        renderPaintCanvas();
      }
    });
  });
}

// =============================================================================
// Pixel Buffer Helper Functions
// =============================================================================
function setPixel(x, y, r, g, b) {
  if (x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT) return;
  const idx = (y * MATRIX_WIDTH + x) * 3;
  state.pixels[idx] = r;
  state.pixels[idx + 1] = g;
  state.pixels[idx + 2] = b;
}

function getPixel(x, y) {
  if (x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT) return [0, 0, 0];
  const idx = (y * MATRIX_WIDTH + x) * 3;
  return [state.pixels[idx], state.pixels[idx + 1], state.pixels[idx + 2]];
}

function clearMatrix() {
  state.pixels.fill(0);
}

function hexToRgb(hex) {
  const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? [
    parseInt(result[1], 16),
    parseInt(result[2], 16),
    parseInt(result[3], 16)
  ] : [255, 255, 255];
}

function rgbToHex(r, g, b) {
  return '#' + [r, g, b].map(x => x.toString(16).padStart(2, '0')).join('');
}

// =============================================================================
// Live 50x30 Matrix Simulator Canvas Rendering
// =============================================================================
function renderMatrixCanvas() {
  const canvas = matrixCanvas;
  const ctx = matrixCtx;
  const width = canvas.width;
  const height = canvas.height;

  // Background
  ctx.fillStyle = '#05070a';
  ctx.fillRect(0, 0, width, height);

  const dotRadius = 3.6;
  const stepX = width / MATRIX_WIDTH;
  const stepY = height / MATRIX_HEIGHT;

  let totalR = 0, totalG = 0, totalB = 0;

    for (let y = 0; y < MATRIX_HEIGHT; y++) {
    for (let x = 0; x < MATRIX_WIDTH; x++) {
      const cx = x * stepX + stepX / 2;
      const cy = (MATRIX_HEIGHT - 1 - y) * stepY + stepY / 2;

      let [r, g, b] = getPixel(x, y);

      if (!state.powerOn) {
        r = 0; g = 0; b = 0;
      } else {
        // Scale by global brightness
        const scale = state.brightness / 255;
        r = Math.round(r * scale);
        g = Math.round(g * scale);
        b = Math.round(b * scale);
      }

      totalR += r;
      totalG += g;
      totalB += b;

      // Dark LED socket ring
      ctx.beginPath();
      ctx.arc(cx, cy, dotRadius + 0.8, 0, Math.PI * 2);
      ctx.fillStyle = '#111827';
      ctx.fill();

      // Lit pixel
      if (r > 5 || g > 5 || b > 5) {
        // Subtle glow aura
        ctx.beginPath();
        ctx.arc(cx, cy, dotRadius * 1.5, 0, Math.PI * 2);
        ctx.fillStyle = `rgba(${r}, ${g}, ${b}, 0.2)`;
        ctx.fill();

        // Main LED emitter core
        ctx.beginPath();
        ctx.arc(cx, cy, dotRadius, 0, Math.PI * 2);
        ctx.fillStyle = `rgb(${r}, ${g}, ${b})`;
        ctx.fill();
      } else {
        // Off LED
        ctx.beginPath();
        ctx.arc(cx, cy, dotRadius, 0, Math.PI * 2);
        ctx.fillStyle = '#0a0f1d';
        ctx.fill();
      }
    }
  }

  // Update Estimated Current Draw Stat
  const totalColorSum = totalR + totalG + totalB;
  const currentEst = (1.5 + (totalColorSum / (255 * 3)) * 0.055).toFixed(1);
  const curLabel = document.getElementById('est-current');
  if (curLabel) {
    curLabel.innerHTML = `<strong>Est. Current:</strong> ~${state.powerOn ? currentEst : '0.1'} A @ 5V`;
  }
}

// =============================================================================
// Multi-Line Space Signs & Text Engine
// =============================================================================
function drawChar(startX, startY, char, r, g, b, scale = 1) {
  const upper = char.toUpperCase();
  const fontData = FONT_5X7[upper] || FONT_5X7[' '];

  for (let col = 0; col < 5; col++) {
    const colByte = fontData[col];
    for (let row = 0; row < 7; row++) {
      if ((colByte & (1 << row)) !== 0) {
        const px = startX + col * scale;
        const py = startY + (6 - row) * scale;
        if (scale === 1) {
          setPixel(px, py, r, g, b);
        } else {
          for (let dx = 0; dx < scale; dx++) {
            for (let dy = 0; dy < scale; dy++) {
              setPixel(px + dx, py + dy, r, g, b);
            }
          }
        }
      }
    }
  }
}

function getTextWidth(text, scale = 1) {
  return text.length * (6 * scale) - (1 * scale);
}

function drawLineText(y, text, colorHex, align = 'center', scale = 1, xOffset = 0) {
  const [r, g, b] = hexToRgb(colorHex);
  const textWidth = getTextWidth(text, scale);
  let startX = 1;

  if (align === 'center') {
    startX = Math.floor((MATRIX_WIDTH - textWidth) / 2);
  } else if (align === 'right') {
    startX = MATRIX_WIDTH - textWidth - 1;
  }
  startX += xOffset;

  let cursorX = startX;
  for (let i = 0; i < text.length; i++) {
    drawChar(cursorX, y, text[i], r, g, b, scale);
    cursorX += (6 * scale);
  }
}

function drawBadgeIcon(iconName, x = 1, y = 10) {
  // 8x8 Mini Icon Bitmaps for Space Badge mode
  const ICONS = {
    check: [
      [0,0,0,0,0,0,0,1],
      [0,0,0,0,0,0,1,1],
      [0,0,0,0,0,1,1,0],
      [1,0,0,0,1,1,0,0],
      [1,1,0,1,1,0,0,0],
      [0,1,1,1,0,0,0,0],
      [0,0,1,0,0,0,0,0],
      [0,0,0,0,0,0,0,0]
    ],
    cross: [
      [1,1,0,0,0,0,1,1],
      [0,1,1,0,0,1,1,0],
      [0,0,1,1,1,1,0,0],
      [0,0,0,1,1,0,0,0],
      [0,0,1,1,1,1,0,0],
      [0,1,1,0,0,1,1,0],
      [1,1,0,0,0,0,1,1],
      [0,0,0,0,0,0,0,0]
    ],
    gear: [
      [0,0,1,1,1,1,0,0],
      [0,1,1,0,0,1,1,0],
      [1,1,0,0,0,0,1,1],
      [1,0,0,1,1,0,0,1],
      [1,0,0,1,1,0,0,1],
      [1,1,0,0,0,0,1,1],
      [0,1,1,0,0,1,1,0],
      [0,0,1,1,1,1,0,0]
    ],
    warning: [
      [0,0,0,1,1,0,0,0],
      [0,0,1,1,1,1,0,0],
      [0,0,1,0,0,1,0,0],
      [0,1,1,0,0,1,1,0],
      [0,1,0,0,0,0,1,0],
      [1,1,0,1,1,0,1,1],
      [1,1,1,1,1,1,1,1],
      [0,0,0,0,0,0,0,0]
    ],
    rocket: [
      [0,0,0,1,1,0,0,0],
      [0,0,1,1,1,1,0,0],
      [0,0,1,0,0,1,0,0],
      [0,1,1,0,0,1,1,0],
      [0,1,1,1,1,1,1,0],
      [1,1,1,1,1,1,1,1],
      [1,0,1,0,0,1,0,1],
      [0,0,1,1,1,1,0,0]
    ],
    coffee: [
      [0,1,1,1,1,1,0,0],
      [0,1,0,0,0,1,1,0],
      [0,1,0,0,0,1,0,1],
      [0,1,0,0,0,1,1,0],
      [0,0,1,1,1,0,0,0],
      [0,0,0,0,0,0,0,0],
      [1,1,1,1,1,1,1,0],
      [0,0,0,0,0,0,0,0]
    ]
  };

  const grid = ICONS[iconName] || ICONS['check'];
  for (let r = 0; r < 8; r++) {
    for (let c = 0; c < 8; c++) {
      if (grid[r][c]) {
        setPixel(x + c, y + (7 - r), 56, 189, 248);
      }
    }
  }
}

function renderTextToMatrix(scrollX = 0) {
  clearMatrix();

  const l1 = document.getElementById('input-line1').value.trim();
  const l2 = document.getElementById('input-line2').value.trim();
  const l3 = document.getElementById('input-line3').value.trim();

  const c1 = document.getElementById('color-line1').value;
  const c2 = document.getElementById('color-line2').value;
  const c3 = document.getElementById('color-line3').value;

  if (state.textLayout === '3line') {
    // In bottom-left coordinates:
    // Line 1 (Top header) at y = 21 (rows 21..27)
    // Line 2 (Middle)     at y = 11 (rows 11..17)
    // Line 3 (Bottom)     at y = 2  (rows 2..8)
    if (l1) drawLineText(21, l1, c1, state.textAlign, 1, scrollX);
    if (l2) drawLineText(11, l2, c2, state.textAlign, 1, scrollX);
    if (l3) drawLineText(2,  l3, c3, state.textAlign, 1, scrollX);
  } else if (state.textLayout === 'bigheader') {
    // 2x Scaled Header on top (y = 14) + 1x Subtitle on bottom (y = 3)
    if (l1) drawLineText(14, l1, c1, state.textAlign, 2, scrollX);
    if (l2) drawLineText(3,  l2, c2, state.textAlign, 1, scrollX);
  } else if (state.textLayout === 'badge') {
    // Left Badge Icon at y = 11 + Line 1 at y = 17, Line 2 at y = 6
    const iconName = document.getElementById('select-badge-icon').value;
    drawBadgeIcon(iconName, 2, 11);
    if (l1) drawLineText(17, l1, c1, 'left', 1, 14);
    if (l2) drawLineText(6,  l2, c2, 'left', 1, 14);
  } else if (state.textLayout === 'marquee') {
    // Single Large Scrolling Banner
    const fullMsg = [l1, l2, l3].filter(Boolean).join(' • ');
    drawLineText(11, fullMsg, c1, 'left', 1, scrollX);
  }
}

function initTextControls() {
  // Layout Chips
  const chips = document.querySelectorAll('#text-layout-selector .chip');
  const badgeGroup = document.getElementById('badge-icon-group');

  chips.forEach(chip => {
    chip.addEventListener('click', () => {
      chips.forEach(c => c.classList.remove('active'));
      chip.classList.add('active');
      state.textLayout = chip.getAttribute('data-layout');
      badgeGroup.style.display = (state.textLayout === 'badge') ? 'block' : 'none';
      renderTextToMatrix();
      renderMatrixCanvas();
    });
  });

  // Alignment Radio
  const alignRadios = document.querySelectorAll('input[name="text-align"]');
  alignRadios.forEach(radio => {
    radio.parentElement.addEventListener('click', () => {
      alignRadios.forEach(r => r.parentElement.classList.remove('active'));
      radio.parentElement.classList.add('active');
      state.textAlign = radio.value;
      renderTextToMatrix();
      renderMatrixCanvas();
    });
  });

  // Text Inputs
  ['input-line1', 'input-line2', 'input-line3'].forEach(id => {
    document.getElementById(id).addEventListener('input', () => {
      renderTextToMatrix();
      renderMatrixCanvas();
    });
  });

  ['color-line1', 'color-line2', 'color-line3'].forEach(id => {
    document.getElementById(id).addEventListener('input', () => {
      renderTextToMatrix();
      renderMatrixCanvas();
    });
  });

  document.getElementById('select-text-effect').addEventListener('change', (e) => {
    state.textEffect = e.target.value;
  });

  document.getElementById('input-text-speed').addEventListener('input', (e) => {
    state.textSpeed = parseInt(e.target.value);
    document.getElementById('speed-label').textContent = `Scroll / Effect Speed: ${state.textSpeed}`;
  });

  // Action Buttons
  document.getElementById('btn-preview-text').addEventListener('click', () => {
    renderTextToMatrix();
    renderMatrixCanvas();
    showToast('Preview Updated');
  });

  document.getElementById('btn-send-text').addEventListener('click', () => {
    renderTextToMatrix();
    renderMatrixCanvas();
    sendTextPayload();
  });
}

async function sendTextPayload() {
  const payload = {
    layout: state.textLayout,
    line1: document.getElementById('input-line1').value.trim(),
    line2: document.getElementById('input-line2').value.trim(),
    line3: document.getElementById('input-line3').value.trim(),
    color1: document.getElementById('color-line1').value,
    color2: document.getElementById('color-line2').value,
    color3: document.getElementById('color-line3').value,
    align: state.textAlign,
    effect: state.textEffect,
    speed: state.textSpeed,
    badgeIcon: document.getElementById('select-badge-icon') ? document.getElementById('select-badge-icon').value : 'check'
  };

  showToast('Sending text to matrix...', 'info');
  const res = await sendApi('/api/text', payload);
  if (res && res.ok) {
    showToast('Display Updated: Text Active!', 'success');
  } else {
    showToast('Failed to update text', 'error');
  }
}

// =============================================================================
// Quick Space Presets
// =============================================================================
function initPresetButtons() {
  const presetCards = document.querySelectorAll('.preset-card');

  presetCards.forEach(card => {
    card.addEventListener('click', () => {
      const presetKey = card.getAttribute('data-preset');
      const data = PRESETS[presetKey];
      if (!data) return;

      document.getElementById('input-line1').value = data.line1;
      document.getElementById('input-line2').value = data.line2;
      document.getElementById('input-line3').value = data.line3;

      document.getElementById('color-line1').value = data.color1;
      document.getElementById('color-line2').value = data.color2;
      document.getElementById('color-line3').value = data.color3;

      state.textAlign = data.align;
      state.textEffect = data.effect;
      document.getElementById('select-text-effect').value = data.effect;

      // Select 3-line layout
      document.querySelectorAll('#text-layout-selector .chip').forEach(c => {
        c.classList.toggle('active', c.getAttribute('data-layout') === '3line');
      });
      state.textLayout = '3line';

      renderTextToMatrix();
      renderMatrixCanvas();

      // Card active feedback
      presetCards.forEach(c => c.classList.remove('selected'));
      card.classList.add('selected');

      // Send to physical display immediately
      sendTextPayload();
      showToast(`Applied Preset: ${card.querySelector('h4').textContent}`, 'success');
    });
  });
}

// =============================================================================
// Image Studio (Upload, Crop, Dither to 50x30)
// =============================================================================
function initImageStudio() {
  const dropzone = document.getElementById('image-dropzone');
  const fileInput = document.getElementById('file-input');
  const btnUpload = document.getElementById('btn-upload-image');

  dropzone.addEventListener('click', () => fileInput.click());

  dropzone.addEventListener('dragover', (e) => {
    e.preventDefault();
    dropzone.classList.add('drag-over');
  });

  dropzone.addEventListener('dragleave', () => dropzone.classList.remove('drag-over'));

  dropzone.addEventListener('drop', (e) => {
    e.preventDefault();
    dropzone.classList.remove('drag-over');
    if (e.dataTransfer.files.length) {
      processImageFile(e.dataTransfer.files[0]);
    }
  });

  fileInput.addEventListener('change', (e) => {
    if (e.target.files.length) {
      processImageFile(e.target.files[0]);
    }
  });

  // Adjustments
  ['image-brightness-adj', 'image-contrast-adj', 'check-dither'].forEach(id => {
    document.getElementById(id).addEventListener('input', () => {
      if (state.uploadedImage) renderImageToMatrix(state.uploadedImage);
    });
  });

  document.querySelectorAll('input[name="image-fit"]').forEach(r => {
    r.parentElement.addEventListener('click', () => {
      document.querySelectorAll('input[name="image-fit"]').forEach(x => x.parentElement.classList.remove('active'));
      r.parentElement.classList.add('active');
      if (state.uploadedImage) renderImageToMatrix(state.uploadedImage);
    });
  });

  btnUpload.addEventListener('click', () => {
    sendImagePayload();
  });
}

function processImageFile(file) {
  const reader = new FileReader();
  reader.onload = (e) => {
    const img = new Image();
    img.onload = () => {
      state.uploadedImage = img;
      document.getElementById('btn-upload-image').disabled = false;
      renderImageToMatrix(img);
      showToast('Image loaded & processed to 50x30');
    };
    img.src = e.target.result;
  };
  reader.readAsDataURL(file);
}

function renderImageToMatrix(img) {
  clearMatrix();

  const tempCanvas = document.createElement('canvas');
  tempCanvas.width = MATRIX_WIDTH;
  tempCanvas.height = MATRIX_HEIGHT;
  const tempCtx = tempCanvas.getContext('2d');

  const fitMode = document.querySelector('input[name="image-fit"]:checked').value;
  const bAdj = parseInt(document.getElementById('image-brightness-adj').value);
  const cAdj = parseInt(document.getElementById('image-contrast-adj').value);
  const useDither = document.getElementById('check-dither').checked;

  document.getElementById('brightness-adjust-label').textContent = `Brightness Boost: ${bAdj}`;
  document.getElementById('contrast-adjust-label').textContent = `Contrast Boost: ${cAdj}`;

  tempCtx.fillStyle = '#000000';
  tempCtx.fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT);

  if (fitMode === 'stretch') {
    tempCtx.drawImage(img, 0, 0, MATRIX_WIDTH, MATRIX_HEIGHT);
  } else if (fitMode === 'contain') {
    const scale = Math.min(MATRIX_WIDTH / img.width, MATRIX_HEIGHT / img.height);
    const nw = img.width * scale;
    const nh = img.height * scale;
    const nx = (MATRIX_WIDTH - nw) / 2;
    const ny = (MATRIX_HEIGHT - nh) / 2;
    tempCtx.drawImage(img, nx, ny, nw, nh);
  } else {
    // Cover / Crop to fill
    const scale = Math.max(MATRIX_WIDTH / img.width, MATRIX_HEIGHT / img.height);
    const nw = img.width * scale;
    const nh = img.height * scale;
    const nx = (MATRIX_WIDTH - nw) / 2;
    const ny = (MATRIX_HEIGHT - nh) / 2;
    tempCtx.drawImage(img, nx, ny, nw, nh);
  }

  const imgData = tempCtx.getImageData(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT);
  const d = imgData.data;

  // Apply Brightness & Contrast
  const contrastFactor = (259 * (cAdj + 255)) / (255 * (259 - cAdj));

  for (let i = 0; i < d.length; i += 4) {
    for (let c = 0; c < 3; c++) {
      let val = d[i + c] + bAdj;
      val = contrastFactor * (val - 128) + 128;
      d[i + c] = Math.max(0, Math.min(255, val));
    }
  }

  if (useDither) {
    applyFloydSteinbergDither(d, MATRIX_WIDTH, MATRIX_HEIGHT);
  }

  // Copy to state.pixels (mapping canvas top-to-bottom to logical bottom-to-top)
  for (let cy = 0; cy < MATRIX_HEIGHT; cy++) {
    const ly = MATRIX_HEIGHT - 1 - cy;
    for (let cx = 0; cx < MATRIX_WIDTH; cx++) {
      const srcIdx = (cy * MATRIX_WIDTH + cx) * 4;
      setPixel(cx, ly, d[srcIdx], d[srcIdx + 1], d[srcIdx + 2]);
    }
  }

  renderMatrixCanvas();
  renderPaintCanvas();
}

function applyFloydSteinbergDither(data, w, h) {
  for (let y = 0; y < h; y++) {
    for (let x = 0; x < w; x++) {
      const idx = (y * w + x) * 4;
      for (let c = 0; c < 3; c++) {
        const oldVal = data[idx + c];
        const newVal = oldVal < 128 ? 0 : 255;
        data[idx + c] = newVal;
        const err = oldVal - newVal;

        if (x + 1 < w) data[((y) * w + (x + 1)) * 4 + c] += (err * 7) >> 4;
        if (x - 1 >= 0 && y + 1 < h) data[((y + 1) * w + (x - 1)) * 4 + c] += (err * 3) >> 4;
        if (y + 1 < h) data[((y + 1) * w + (x)) * 4 + c] += (err * 5) >> 4;
        if (x + 1 < w && y + 1 < h) data[((y + 1) * w + (x + 1)) * 4 + c] += (err * 1) >> 4;
      }
    }
  }
}

function uint8ToBase64(bytes) {
  let binary = '';
  const len = bytes.byteLength;
  const chunk = 1024;
  for (let i = 0; i < len; i += chunk) {
    const sub = bytes.subarray(i, Math.min(i + chunk, len));
    binary += String.fromCharCode.apply(null, sub);
  }
  return btoa(binary);
}

async function sendFramePayload() {
  const b64 = uint8ToBase64(state.pixels);
  showToast('Sending frame to LED matrix...', 'info');
  const res = await sendApi('/api/frame', { frame: b64 });
  if (res && res.ok) {
    showToast('Display Updated: Frame Loaded!', 'success');
  } else {
    showToast('Failed to send frame', 'error');
  }
}

function sendImagePayload() {
  sendFramePayload();
}

// =============================================================================
// Pixel Art Paint Studio
// =============================================================================
function initColorSwatches() {
  const container = document.getElementById('palette-swatches');
  container.innerHTML = '';

  PALETTE_COLORS.forEach(hex => {
    const sw = document.createElement('div');
    sw.className = 'swatch';
    sw.style.backgroundColor = hex;
    if (hex === state.drawColor) sw.classList.add('active');

    sw.addEventListener('click', () => {
      document.querySelectorAll('.swatch').forEach(s => s.classList.remove('active'));
      sw.classList.add('active');
      state.drawColor = hex;
      document.getElementById('draw-color-picker').value = hex;
    });

    container.appendChild(sw);
  });

  document.getElementById('draw-color-picker').addEventListener('input', (e) => {
    state.drawColor = e.target.value;
  });

  document.getElementById('check-stream-live').addEventListener('change', (e) => {
    state.streamLive = e.target.checked;
  });
}

function initPaintStudio() {
  const tools = document.querySelectorAll('.tool-btn');
  tools.forEach(btn => {
    btn.addEventListener('click', () => {
      tools.forEach(t => t.classList.remove('active'));
      btn.classList.add('active');
      state.activeTool = btn.getAttribute('data-tool');
    });
  });

  const canvas = paintCanvas;

  canvas.addEventListener('mousedown', (e) => {
    state.isDrawing = true;
    handlePaintAction(e);
  });

  window.addEventListener('mouseup', () => {
    state.isDrawing = false;
    state.lastDrawCoord = null;
  });

  canvas.addEventListener('mousemove', (e) => {
    if (state.isDrawing) handlePaintAction(e);
  });

  // Touch support for phones and tablets
  canvas.addEventListener('touchstart', (e) => {
    e.preventDefault();
    state.isDrawing = true;
    handlePaintAction(e.touches[0]);
  }, { passive: false });

  canvas.addEventListener('touchmove', (e) => {
    e.preventDefault();
    if (state.isDrawing) handlePaintAction(e.touches[0]);
  }, { passive: false });

  window.addEventListener('touchend', () => {
    state.isDrawing = false;
    state.lastDrawCoord = null;
  });

  document.getElementById('btn-draw-clear').addEventListener('click', () => {
    clearMatrix();
    renderMatrixCanvas();
    renderPaintCanvas();
  });

  document.getElementById('btn-draw-push').addEventListener('click', () => {
    sendImagePayload();
  });
}

function getCanvasCoordinates(e) {
  const rect = paintCanvas.getBoundingClientRect();
  const clientX = e.clientX;
  const clientY = e.clientY;
  const x = Math.floor(((clientX - rect.left) / rect.width) * MATRIX_WIDTH);
  const canvasY = Math.floor(((clientY - rect.top) / rect.height) * MATRIX_HEIGHT);
  const y = (MATRIX_HEIGHT - 1) - canvasY;
  return {
    x: Math.max(0, Math.min(MATRIX_WIDTH - 1, x)),
    y: Math.max(0, Math.min(MATRIX_HEIGHT - 1, y))
  };
}

function handlePaintAction(e) {
  const { x, y } = getCanvasCoordinates(e);
  const [r, g, b] = hexToRgb(state.drawColor);

  if (state.activeTool === 'pencil') {
    setPixel(x, y, r, g, b);
    if (state.streamLive) sendPixelDirect(x, y, r, g, b);
  } else if (state.activeTool === 'eraser') {
    setPixel(x, y, 0, 0, 0);
    if (state.streamLive) sendPixelDirect(x, y, 0, 0, 0);
  } else if (state.activeTool === 'picker') {
    const [pr, pg, pb] = getPixel(x, y);
    const hex = rgbToHex(pr, pg, pb);
    state.drawColor = hex;
    document.getElementById('draw-color-picker').value = hex;
  } else if (state.activeTool === 'bucket') {
    floodFill(x, y, r, g, b);
    if (state.streamLive) sendImagePayload();
  }

  renderMatrixCanvas();
  renderPaintCanvas();
}

function floodFill(startX, startY, targetR, targetG, targetB) {
  const [startR, startG, startB] = getPixel(startX, startY);
  if (startR === targetR && startG === targetG && startB === targetB) return;

  const queue = [[startX, startY]];
  const visited = new Uint8Array(TOTAL_LEDS);

  while (queue.length > 0) {
    const [cx, cy] = queue.pop();
    const idx = cy * MATRIX_WIDTH + cx;
    if (visited[idx]) continue;
    visited[idx] = 1;

    const [cr, cg, cb] = getPixel(cx, cy);
    if (cr === startR && cg === startG && cb === startB) {
      setPixel(cx, cy, targetR, targetG, targetB);

      if (cx + 1 < MATRIX_WIDTH) queue.push([cx + 1, cy]);
      if (cx - 1 >= 0) queue.push([cx - 1, cy]);
      if (cy + 1 < MATRIX_HEIGHT) queue.push([cx, cy + 1]);
      if (cy - 1 >= 0) queue.push([cx, cy - 1]);
    }
  }
}

function renderPaintCanvas() {
  const imgData = paintCtx.createImageData(MATRIX_WIDTH, MATRIX_HEIGHT);
  const d = imgData.data;

  for (let cy = 0; cy < MATRIX_HEIGHT; cy++) {
    const ly = MATRIX_HEIGHT - 1 - cy;
    for (let cx = 0; cx < MATRIX_WIDTH; cx++) {
      const canvasIdx = (cy * MATRIX_WIDTH + cx) * 4;
      const [r, g, b] = getPixel(cx, ly);
      d[canvasIdx]     = r;
      d[canvasIdx + 1] = g;
      d[canvasIdx + 2] = b;
      d[canvasIdx + 3] = 255;
    }
  }

  paintCtx.putImageData(imgData, 0, 0);
}

function sendPixelDirect(x, y, r, g, b) {
  sendApi('/api/pixel', { x, y, r, g, b });
}

// =============================================================================
// Animation Cards
// =============================================================================
function initAnimationCards() {
  const cards = document.querySelectorAll('.effect-card');
  cards.forEach(card => {
    card.addEventListener('click', () => {
      cards.forEach(c => c.classList.remove('active'));
      card.classList.add('active');
      const modeId = parseInt(card.getAttribute('data-effect'));
      sendApi('/api/mode', { mode: modeId })
        .then(() => showToast(`Animation: ${card.querySelector('h4').textContent}`, 'success'))
        .catch(() => showToast('Simulated Animation Mode', 'info'));
    });
  });
}

// =============================================================================
// Live Simulator Loop
// =============================================================================
let scrollStep = MATRIX_WIDTH;
let blinkPhase = false;

function startSimulatorLoop() {
  setInterval(() => {
    if (state.textEffect === 'scroll') {
      scrollStep -= 1;
      if (scrollStep < -150) scrollStep = MATRIX_WIDTH;
      renderTextToMatrix(scrollStep);
      renderMatrixCanvas();
    } else if (state.textEffect === 'blink') {
      blinkPhase = !blinkPhase;
      if (blinkPhase) {
        renderTextToMatrix();
      } else {
        clearMatrix();
      }
      renderMatrixCanvas();
    }
  }, 100);
}

// =============================================================================
// API Communication Helper
// =============================================================================
async function sendApi(endpoint, data) {
  try {
    const res = await fetch(endpoint, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(data),
      signal: AbortSignal.timeout(3000)
    });
    return await res.json();
  } catch (err) {
    // Graceful offline mock handling
    console.debug(`[API Mock] ${endpoint}:`, data);
    return { ok: true, offline: true };
  }
}
