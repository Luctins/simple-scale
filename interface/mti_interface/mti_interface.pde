/*---------- Import */
import processing.serial.*;

/*---------- Variables */
Serial port;

String value= "0.0";
PFont font;

float mass = 0 ;
float max_mass = 10000;
int start=0, now=0;
/*--------- Functions*/

void draw_indicator(float ang)
{
  background(0);
  fill(255);
  circle(width/2, 400, 600);
  translate(width/2, 395.5);
  rotate(ang);
  fill(200, 0, 0);
  rect(-300, 0, 300, 5);
  rotate(-ang);
  translate(-width/2, -392.5);
  fill(0);
  circle(width/2, 400, 400);
  fill(0,200,0);
  square(0, 400, width/2);
  fill(0);
  text("Tara",width/4,500);
  fill(0);
  square(width/2, 400, width/2);
}

void draw_numbers(float mass)
{
  float r = width/2-75;
  
  fill(255);
  textAlign(CENTER);
  textFont(font, 18);
  fill(255);
   for (float ang = 0; ang <= PI+PI/10; ang+=PI/10)
  {
    float x = -cos(ang) * r + width/2, y = -sin(ang) * r  + 400;  
    text(str(int(ang/PI * max_mass))+" g ", x, y);
  }
  text(str(mass)+" g", width/2, height/2);
  text(value,200,30);
}


/*---------- SETUP */
void setup()
{
  size(900, 600);
    font  = createFont("Arial", 16, true);
   port =  new Serial(this, "/dev/ttyACM0", 115200);
 
  draw_indicator(0);
  draw_numbers(0);
  port.bufferUntil('\n');
  delay(500);
  

}


/*------------- Events */
/* this function is called whenever the mouse is clicked*/
void mouseClicked()
{
  if(mouseY > 400)
  {
  if(mouseX<width/2)
  {
      port.write(116);
  }
  else
  {
  }
  port.write(10);
  }
}

/* this function is called when it receives data from the serial and has found a \n*/
void serialEvent(Serial p)
{
  value = port.readString();
  fill(255);
  
  if(value != null)
     {
       
       float tmp=float(value);
        mass = tmp != Float.NaN? tmp: mass;
        
        /*
       switch(value.charAt(0))
       { 
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
          case '-':
             //<>//
          break;
         }*/
       }
} 


/*---------- Draw*/
void draw()
{
  float ang = PI * mass/max_mass;
  draw_indicator(ang);
  draw_numbers(mass);
}
