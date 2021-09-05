document.oncontextmenu = function() { return false; }

let initialized = false
Module.onRuntimeInitialized = function() {
  initialized = true
}

const matrixWidth = 384;
const matrixHeight = 216;
const scale = 4;

let img;


function setup() {
  img = createImage(matrixWidth, matrixHeight);
  createCanvas(matrixWidth * scale, matrixHeight * scale); // 1920x1080
  noSmooth();
}

function draw() {
  if(initialized) {
    print(frameRate());
    //-- process
    handleInputs();
    Module._update();

    //-- draw
    img.loadPixels();
    for(let x = 0; x < img.width; x++) {
      for(let y = 0; y < img.height; y++)
      {
        setPixel(x, y, Module._getCellR(x, y), Module._getCellG(x, y), Module._getCellB(x, y), Module._getCellA(x, y))
      }
    }
    img.updatePixels();
  
    image(img, 0, 0, width, height);

    fill(255)
    text(Module._getBrushSize(), 5, 15);
    text(Module._getSelectedIndex(), 5, 30);
  }
}


function setPixel(x, y, r, g, b, a) {
  let index = (img.width * y + x) * 4;
  img.pixels[index + 0] = r;
  img.pixels[index + 1] = g;
  img.pixels[index + 2] = b;
  img.pixels[index + 3] = a;
}


function handleInputs() {
  if(mouseIsPressed) {
    if(mouseButton === LEFT) {
      Module._matrixDraw(mouseX / scale, mouseY / scale)
    }
    if(mouseButton === RIGHT) {
      Module._matrixErase(mouseX / scale, mouseY / scale)
    }
  }
}

function keyPressed() {
  if(keyCode === RIGHT_ARROW) {
    Module._increaseSelectedIndex(1);
  }
  if(keyCode === LEFT_ARROW) {
    Module._decreaseSelectedIndex(1);
  }
}

function mouseWheel(event) {
  if(event.delta < 0) {
    Module._increaseBrushSize(1);
  }
  else if(event.delta > 0) {
    Module._decreaseBrushSize(1);
  }
}