/*---------- Import */
import processing.serial.*;

/*---------- Variables */
Serial port;
int m=0;
String value=  "empty";
PFont font;

String test = "2000.0"; 

float mass = 0 ;
float max_mass = 10000;

/*--------- Functions*/

void draw_indicator(float ang)
{
  background(0);
  fill(255);
  circle(width/2, 400, 600);
  fill(0);
  square(0, 400, width);
  translate(width/2, 395.5);
  rotate(ang);
  fill(200, 0, 0);
  rect(-300, 0, 300, 5);
  rotate(-ang);
  translate(-width/2, -392.5);
  fill(0);
  circle(width/2, 400, 400);
}

void draw_numbers(float mass)
{
  float r = 350;
  
  fill(255);
  textAlign(CENTER);
  textFont(font, 18);
  fill(255);
   for (float ang = 0; ang <= PI+PI/10; ang+=PI/10)
  {
    float x = -cos(ang) * r + 400, y = -sin(ang) * r  + 400;  
    text(str(ang/PI * max_mass), x, y);
  }
  text(str(mass), width/2, 450);
}


/*---------- SETUP */
void setup()
{
  size(800, 600);
    font  = createFont("Arial", 16, true);
   port =  new Serial(this, "/dev/ttyACM0", 115200);
 
  draw_indicator(mass);
  draw_numbers(mass);
  delay(500);
}

/*---------- DRAW */
void draw()
{
  //float ang = ;
  //mass+=1;
  port.write('r');
  if(port.available() > 0)
   {
    value = port.readString();
    mass = float(value);
    draw_indicator(PI* mass/max_mass);
    draw_numbers(mass);
   }
}
