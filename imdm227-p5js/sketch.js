let width, height, halfWidth, halfHeight;
let x, y;

let yGradient = 1;
let color1, color2;

let fishWidth = 80, fishHeight = 60;
let fishInc = 20;
let eyeOffset = 20, eyeDiameter = 10;
let tailStart = fishWidth / 2;
let tailLength = tailStart + fishHeight / 2, tailHeight = fishHeight / 2;

let facingRight = false;
let fishColor;

let numGrass, grassHeight, grassLength, grassDivisor = 20;

let bubble;
let numBubbles = 80;
let bubbles = [];
let smlSize = 40;

let words = "Use arrow keys to move around. Click the fish to change color."
let tutorial = true;
let wordSize;

let speedSlider, slider1Offset = -75;
let inc = 6;

let sizeSlider, slider2Offset = -25;

let waterSound, colorSound;

function preload() {
  waterSound = loadSound('Underwater.mp3');
  colorSound = loadSound('pop.mp3');
}

function setup() {
  width = windowWidth;
  height = windowHeight;
  createCanvas(width, height);
  
  print(width / 6, height);
  
  // Define BG colors
  color1 = color(120, 245, 224);
  color2 = color(32, 207, 236);
  
  halfWidth = width / 2;
  halfHeight = height / 2;
  
  // fishWidth = width / 12
  
  x = halfWidth;
  y = halfHeight;
  
  wordSize = height / 25; 
  
  numGrass = width / grassDivisor;
  grassLength = numGrass * 1.5;
  grassHeight = height / 3;
  
  fishColor = color(240, 173, 71);
  // fishColor = color(random(255), random(180), random(200));
  
  // bubbles
  for (let i = 0; i < numBubbles; i++) {
    let bubble = new Bubble();
    bubbles.push(bubble);
  }
  
  waterSound.loop();
  
  // speed slider
  speedSlider = createSlider(4, 15, inc, 0)
    .style("transform","rotate(270deg)")
    .position(slider1Offset, halfHeight)
    .size(height / 3, 20);
  
  // size slider
  sizeSlider = createSlider(fishInc - fishInc / 2, fishInc + fishInc / 2, fishInc, 0)
    .style("transform","rotate(270deg)")
    .position(slider2Offset, halfHeight)
    .size(height / 3, 20);
}

function draw() {
  background(49, 171, 232);
  setGradient(0, 0, width, height, color1, color2, yGradient);
  noStroke();
  
  // text
  if (tutorial) {
    textSize(wordSize);
    textAlign(CENTER);
    textFont("Calibri");
    // textStyle(BOLD);
    fill(13, 186, 137, 220);
    text(words, halfWidth, halfHeight - height / 4);
  }
  
  // bubbles
  randomSeed(0);
  for(let i = 0; i < bubbles.length; i++) {
    let bubble = bubbles[i];
    bubble.bounce();

    newColor = color(random(30,60), random(150,230), random(150,230));
    newColor.setAlpha(100);
    fill(newColor)
    
    circle(bubble.p.x,bubble.p.y, random(smlSize / 2, smlSize));
    bubble.p.x += bubble.v.x * cos(1);
    bubble.p.y += bubble.v.y * sin(2);
  }
  
  // reset randomSeed
  randomSeed(null);

  // fish body
  fill(fishColor);
  fishWidth = sizeSlider.value() * 4;
  fishHeight = sizeSlider.value() * 3;
  tailStart = fishWidth / 2;
  tailLength = tailStart + fishHeight / 2;
  tailHeight = fishHeight / 2;
  eyeOffset = fishWidth / 4;
  eyeDiameter = eyeOffset / 2;
  
  ellipse(x, y, fishWidth, fishHeight);
  
  fill("black");
  if (facingRight) {
    // fish eye
    circle(x + eyeOffset, y, eyeDiameter);
    
    // tail
    fill(fishColor);
    triangle(x - tailStart, y, 
             x - tailLength, y + tailHeight, 
             x - tailLength, y - tailHeight);
  } else {
    // fish eye
    circle(x - eyeOffset, y, eyeDiameter);
    
    // tail
    fill(fishColor);
    triangle(x + tailStart, y, 
             x + tailLength, y + tailHeight, 
             x + tailLength, y - tailHeight);
  }
  moveFish();
  
  drawSeaweed();
  
  inc = speedSlider.value();
}

function setGradient(x, y, w, h, color1, color2, axis) {
  noFill();

  if (axis === yGradient) {
    // Top to bottom gradient
    for (let i = y; i <= y + h; i++) {
      let inter = map(i, y, y + h, 0, 1);
      let c = lerpColor(color1, color2, inter);
      stroke(c);
      line(x, i, x + w, i);
    }
  }
}

function drawSeaweed(){
  for (let i = 0; i < numGrass; i++){
    let posX = numGrass * i;
    
    fill(2, 150, 56, 90);
    triangle(posX, height - grassHeight,
             posX - grassLength, height,  
             posX + grassLength, height);
  }
}

function moveFish() {
  if (keyIsDown(LEFT_ARROW)) {
    facingRight = false;
    if (x - tailStart - inc > 0) {
      x -= inc;
    }
  }
  
  if (keyIsDown(RIGHT_ARROW)) {
    facingRight = true;
    if (x + tailStart + inc < width) {
      x += inc;
    }
  }
  
  if (keyIsDown(UP_ARROW) && y - tailHeight - inc > 0) {
    y -= inc;
  }
  
  if (keyIsDown(DOWN_ARROW) && y + tailHeight + inc < height) {
    y += inc;
  }
  
  if (keyIsDown(LEFT_ARROW) || keyIsDown(RIGHT_ARROW) || 
      keyIsDown(DOWN_ARROW) || keyIsDown(DOWN_ARROW)) {
    tutorial = false;
  }
}

function mousePressed() {
  // (x, y) is center of ellipse
  let distanceX = mouseX - x, distanceY = abs(mouseY - y);
  let tailLevel = (abs(distanceX) - tailStart) / (tailLength - tailStart);
  
  // if mouse is on fish body
  if (abs(distanceX) < fishWidth / 2 && distanceY < fishHeight / 2) {
    fishColor = color(random(255), random(180), random(200));
    colorSound.play();
  } 
  // if mouse is on fish tail (left facing)
  else if (facingRight == false &&
           distanceX > tailStart && 
           distanceX < tailLength && 
           distanceY < tailHeight * tailLevel) {
    fishColor = color(random(255), random(180), random(200));
    colorSound.play();
  }
  // if mouse is on fish tail (right facing)
  else if (facingRight == true &&
           distanceX < tailStart * -1 && 
           distanceX > tailLength * -1 && 
           distanceY < tailHeight * tailLevel) {
    fishColor = color(random(255), random(180), random(200));
    colorSound.play();
  }
}

class Bubble {
  constructor() {
    this.x = random(20,50),
    this.p = {
      x: random(width),
      y: random(height),
    }
    this.v = {
      x: random(-1,1),
      y: random(-1,1),
    }

//    this.color = (random(["#cce0ff", "#62cbf8", "#91d3ed", "#71c8eb", "#62cbf5"]))

  }
  
  // bounces off the edges of the screen
  bounce() {
		if (this.p.x + smlSize > width || this.p.x - smlSize < 0) {
			this.v.x *= -1
		}
		if (this.p.y+ smlSize > height || this.p.y- smlSize < 0) {
			this.v.y *= -1
		}
	}
  
}
