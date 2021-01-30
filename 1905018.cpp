/*
	author: Mohaiminul Islam
	last modified: November 5, 2020
*/

/**
------USER GUIDE------

KEY:
    a: increase amplitude
    A: decrease amplitude
    f: increase frequency
    F: decrease frequency
    1,2,3...,n: show ball and switch color
    +: increase speed
    -: decrease speed
    s: Show curve
    S: Hide Curve
    p: pause
    r: resume
    q: quit
    END: quit

INTERFACE:
    Top-Left:
        green '+': add a sine curve
        blue '+': add a cosine curve
        red '-': remove last added curve
        yellow '?': help, 'x' to close
    Top-Right:
        Show reference circle of of specific color tracer
        Show/hide curves
    Bottom (if ref circle is on):
        Show a reference circle of respective SHM
        Change amplitude individually by sliding and show warning with change
        Change frequency individually by sliding and show warning with change

**/


# include "iGraphics.h"
# include "gl.h"

int width = 1280, height = 720;
//random colors
int red[5] = {255,153,255,153,255};
int green[5] = {153,255,255,153,70};
int blue[5] = {153,204,255,255,255};


int n = 3;


double ball_x[10] =      {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
double ball_y[10] =      {height/2.0,height/2.0,height/2.0,height/2.0,height/2.0,height/2.0,height/2.0,height/2.0,height/2.0,height/2.0};
double radius[10] =      {7.0,7.0,7.0,7.0,7.0,7.0,7.0,7.0,7.0,7.0};
double vx[10] =          {3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0};
int amplitude[10] =      {40,40,40,40,40,40,40,40,40,40};
double frequency[10] =   {1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
int type[10] =           {1,0,1,1,1,1,1,1,1,1};
int ball_red[10] =       {red[0],red[1],red[2],red[3],red[4],red[4],red[4],red[4],red[4],red[4]};
int ball_green[10] =     {green[0],green[1],green[2],green[3],green[4],green[4],green[4],green[4],green[4],green[4]};
int ball_blue[10] =      {blue[0],blue[1],blue[2],blue[3],blue[4],blue[4],blue[4],blue[4],blue[4],blue[4]};
int showRefCircle[10] =  {0,0,0,0,0,0,0,0,0,0};

double amplifier_x[10];
double freqBar_x[10];

int showCurve = 1, help = 0;
int toRight = 1, toLeft = 0;

int i, j, k;
double x, y, z;
double line_rad = 0;
double tmp_x[10], tmp_y[10];



int mnAmp()
{
    int amp = height;
    for (i = 0; i < n; i++) {
        if (amp > amplitude[i])
            amp = amplitude[i];
    }
    return amp;
}
int mxAmp()
{
    int amp = 0;
    for (i = 0; i < n; i++) {
        if (amp < amplitude[i])
            amp = amplitude[i];
    }
    return amp;
}
double mxFreq()
{
    double freq = 0;
    for (i = 0; i < n; i++) {
        if (freq < frequency[i])
            freq = frequency[i];
    }
    return freq;
}
double mnFreq()
{
    double freq = 10;
    for (i = 0; i < n; i++) {
        if (freq > frequency[i])
            freq = frequency[i];
    }
    return freq;
}
double yellowDivider(double start, double finish)
{
    return start+33.33;
}
double redDivider(double start, double finish)
{
    return start+66.67;
}
void amplifier()
{
    for (i = 0; i < n; i++) {
        amplifier_x[i] = ((double)amplitude[i]*(5.0/18.0))+(width/n)*i+(width/(2*n));
    }
}
void freqBar()
{
    for (i = 0; i < n; i++) {
        freqBar_x[i] = frequency[i]*10+(width/n)*i+(width/(2*n));
    }
}

void iDraw()
{
	iClear();

    amplifier();
	freqBar();

	//text
	for (i = 0; i < n; i++) {

        //show upper-right text
        x = width-300;
        y = height - (i*24) - 24;
        iSetColor(ball_red[i], ball_green[i], ball_blue[i]);
        iFilledCircle(x, y+8, 5);

        if (showRefCircle[i]) {
            iSetColor(255, 255, 255);
            iRectangle(x-10, y-3, 290, 20);
            iSetColor(255, 255, 255);
            iText(x+10, y+4, "Show reference circle");

            iSetColor(200, 200, 200);
            iCircle((width/n)*i+(width/(2*n)), height/4, amplitude[i]);
            iSetColor(ball_red[i], ball_green[i], ball_blue[i]);

            //draw reference circle
            double ref_ball_x;
            if (type[i]) ref_ball_x = amplitude[i]*cos((ball_x[i]*frequency[i])/57.2958);
            else ref_ball_x = amplitude[i]*sin((ball_x[i]*frequency[i])/57.2958);

            iFilledCircle(ref_ball_x+(width/n)*i+(width/(2*n)), ball_y[i]-height/4, 3);
            iSetColor(153,153,153);
            iLine((width/n)*i+(width/(2*n)), height/4, ref_ball_x+(width/n)*i+(width/(2*n)), ball_y[i]-height/4);
            iSetColor(153,153,153);
            iLine(ref_ball_x+(width/n)*i+(width/(2*n)), ball_y[i]-height/4, ball_x[i], ball_y[i]);

            //draw amplitude bar
            iSetColor(255,255,255);
            iText((width/n)*i+(width/(2*n))-100, 100, "Amplitude");
            iSetColor(255,255,255);
            iLine((width/n)*i+(width/(2*n)), 103, (width/n)*i+(width/(2*n))+110, 103);

            if (amplifier_x[i] < yellowDivider((width/n)*i+(width/(2*n)), (width/n)*i+(width/(2*n))+100)) {
                iSetColor(0,255,0);
                iFilledRectangle(amplifier_x[i], 100, 10, 10);
            }
            else if (amplifier_x[i] < redDivider((width/n)*i+(width/(2*n)), (width/n)*i+(width/(2*n))+100)) {
                iSetColor(255,255,0);
                iFilledRectangle(amplifier_x[i], 100, 10, 10);
            }
            else {
                iSetColor(255,0,0);
                iFilledRectangle(amplifier_x[i], 100, 10, 10);
            }

            //draw frequency bar
            iSetColor(255,255,255);
            iText((width/n)*i+(width/(2*n))-100, 80, "Frequency");
            iSetColor(255,255,255);
            iLine((width/n)*i+(width/(2*n)), 83, (width/n)*i+(width/(2*n))+110, 83);

            if (freqBar_x[i] < yellowDivider((width/n)*i+(width/(2*n)), (width/n)*i+(width/(2*n))+100)) {
                iSetColor(0,255,0);
                iFilledRectangle(freqBar_x[i], 80, 10, 10);
            }
            else if (freqBar_x[i] < redDivider((width/n)*i+(width/(2*n)), (width/n)*i+(width/(2*n))+100)) {
                iSetColor(255,255,0);
                iFilledRectangle(freqBar_x[i], 80, 10, 10);
            }
            else {
                iSetColor(255,0,0);
                iFilledRectangle(freqBar_x[i], 80, 10, 10);
            }
        }

        else {
            iSetColor(180, 180, 180);
            iRectangle(x-10, y-3, 290, 20);
            iSetColor(180, 180, 180);
            iText(x+10, y+4, "Show reference circle");
        }
	}

	//toggle tracing curve
	y-=24;
	if (showCurve) {
        iSetColor(255, 255, 255);
        iRectangle(x-10, y-3, 290, 20);
        iSetColor(255, 255, 255);
        iText(x+10, y+4, "Show tracing curves");
	}
	else {
        iSetColor(180, 180, 180);
        iRectangle(x-10, y-3, 290, 20);
        iSetColor(180, 180, 180);
        iText(x+10, y+4, "Show tracing curves");
    }

    //add sine
    iSetColor(153,255,153);
    iFilledRectangle(10,height-30,20,20);
    iSetColor(0,0,0);
    iText(18,height-25,"+");
    //add cosine
    iSetColor(153,153,255);
    iFilledRectangle(40,height-30,20,20);
    iSetColor(0,0,0);
    iText(48,height-25,"+");
    //remove
    iSetColor(255,153,153);
    iFilledRectangle(70,height-30,20,20);
    iSetColor(0,0,0);
    iText(78,height-25,"-");
    //help
    iSetColor(250,210,0);
    iFilledRectangle(100,height-30,20,20);
    iSetColor(0,0,0);
    iText(108,height-25,"?");

	//draw axis line
	iSetColor(255, 255, 255);
	iLine(0.0, height/2.0, width, height/2.0);

	//draw tracers
	for (i = 0; i < n; i++) {
        iSetColor(ball_red[i], ball_green[i], ball_blue[i]);
        iFilledCircle(ball_x[i], ball_y[i], radius[i]);
	}
	//draw resultant tracer
	iSetColor(ball_red[9], ball_green[9], ball_blue[9]);
    iFilledCircle(ball_x[9], ball_y[9], radius[9]);

    //draw curves
    tmp_x[9] = 0.0;
	tmp_x[9] = 0.0;
    for (i = 0; i < 9; i++) {
        tmp_x[i] = 0.0;
        tmp_y[i] = type[i];
        tmp_y[9]+=type[i];
    }

	if (showCurve) {
        for (x = 0; x <= width; x+=5) {
            double finy = 0.0;
            for (i = 0; i < n; i++) {
                //sin
                if (type[i]) {
                    y = height/2.0 + amplitude[i]*sin((x*frequency[i])/57.2958);
                    finy+=(y-height/2.0);
                    iSetColor(153, 255, 204);
                    iFilledCircle(x, y, line_rad);
                    iSetColor(153, 255, 204);
                    iLine(x, y, tmp_x[i], tmp_y[i]);
                    tmp_x[i] = x, tmp_y[i] = y;
                }
                //cos
                else {
                    y = height/2.0 + amplitude[i]*cos((x*frequency[i])/57.2958);
                    finy+=(y-height/2.0);
                    iSetColor(153, 204, 255);
                    iFilledCircle(x, y, line_rad);
                    iSetColor(153, 204, 255);
                    iLine(x, y, tmp_x[i], tmp_y[i]);
                    tmp_x[i] = x, tmp_y[i] = y;
                }
            }
            //resultant
            iSetColor(255, 153, 153);
            iFilledCircle(x, finy+height/2, line_rad);
            iSetColor(255, 153, 153);
            iLine(x, finy+height/2, tmp_x[9], tmp_y[9]+height/2);
            tmp_x[9] = x, tmp_y[9] = finy;
        }
	}

    //show help
    if (help) {
        iShowBMP(0, 0, "help.bmp");
        iPauseTimer(0);
    }
}

void iMouseMove(int mx, int my)
{
    for (i = 0; i < n; i++) {
        //amplitude bar
        if (mx >= (width/n)*i + (width/(2*n)) && mx <= ((width/n)*i)+(width/(2*n))+100 && my >= 100 && my < 110 && showRefCircle[i]) {
            amplifier_x[i] = mx;
            amplitude[i] = 3.6 * (mx - ((width/n)*i)-(width/(2*n)));
        }
        //frequency bar
        if (mx >= (width/n)*i + (width/(2*n)) && mx <= ((width/n)*i)+(width/(2*n))+100 && my >= 80 && my < 90 && showRefCircle[i]) {
            freqBar_x[i] = mx;
            frequency[i] = 0.1 * (mx - ((width/n)*i)-(width/(2*n)));
        }
    }

}

void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
	    for (i = 0; i <= n; i++) {
            x = width - 300;
            y = height - (i*24) - 24;
            //show reference circle
            if(mx >= x-10 && mx <= x+290 && my >= y-3 && my <= y+17) {
                if (i!=n && showRefCircle[i])
                    showRefCircle[i] = 0;
                else if (i!=n && !showRefCircle[i])
                    showRefCircle[i] = 1;
                else if (i==n && showCurve)
                    showCurve = 0;
                else if (i==n && !showCurve)
                    showCurve = 1;
            }
        }

        //add sine
        if (n <= 5 && mx >= 10 && mx <= 30 && my >= height-30 && my <= height-10) {
            ball_x[n] = ball_x[0];
            type[n] = 1;
            n++;
        }

        //add cosine
        if (n <= 5 && mx >= 40 && mx <= 60 && my >= height-30 && my <= height-10) {
            ball_x[n] = ball_x[0];
            type[n] = 0;
            n++;
        }

        //remove
        if (n > 0 && mx >= 70 && mx <= 90 && my >= height-30 && my <= height-10)
            n--;

        //help
        if (mx >= 100 && mx <= 120 && my >= height-30 && my <=height-10)
            help = 1;
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//place your codes here
	}
}


void iKeyboard(unsigned char key)
{
    //toggle tracer and its color
    if (key >= '1' && key < n+'1') {
        if (!radius[key-'1']) radius[key-'1'] = 7;
        else radius[key-'1'] = 0;
        ball_red[key-'1'] = red[(int)rand()%5];
        ball_green[key-'1'] = green[(int)rand()%5];
        ball_blue[key-'1'] = blue[(int)rand()%5];
    }
    //toggle resultant tracer
    if (key == n+'1') {
        if (!radius[9]) radius[9] = 7;
        else radius[9] = 0;
        ball_red[9] = red[(int)rand()%5];
        ball_green[9] = green[(int)rand()%5];
        ball_blue[9] = blue[(int)rand()%5];
    }

    //change amplitude
    if (key == 'a') {
        for (i = 0; i < n; i++) {
            if (amplitude[i] < height/2)
                amplitude[i]+=2;
            if (amplitude[i] > height/2)
                amplitude[i] = height/2;
        }
    }
    if (key == 'A') {
        for (i = 0; i < n; i++) {
            if (amplitude[i] > 0)
                amplitude[i]-=2;
            if (amplitude[i] < 0)
                amplitude[i] = 0;
        }
    }

    //change frequency
    if (key == 'f') {
        for (i = 0; i < n; i++) {
            if (frequency[i] < 10.0)
                frequency[i]+=0.05;
            if (frequency[i] > 10.0)
                frequency[i] = 10;
        }
    }
    if (key == 'F') {
        for (i = 0; i < n; i++) {
            if (frequency[i] > 0.0)
                frequency[i]-=0.05;
            if (frequency[i] < 0.0)
                frequency[i] = 0;
        }
    }

    //change speed
    if (key == '+') {
        if (toRight && vx[0] < 10) {
            for (i = 0; i < 10; i++)
                vx[i]+=0.5;
        }
        else if (toLeft && vx[0] >-10) {
            for (i = 0; i < 10; i++)
                vx[i]-=0.5;
        }
    }
    if (key == '-') {
        if (toRight && vx[0] > 0.0) {
            for (i = 0; i < 10; i++)
                vx[i]-=0.5;
        }
        else if (toLeft && vx[0] < 0.0) {
            for (i = 0; i < 10; i++)
                vx[i]+=0.5;
        }
    }

    //toggle curve
    if (key == 's' || key == 'S') {
        if (showCurve)
            showCurve = 0;
        else showCurve = 1;
    }

    //help
    if (key == 'x' && help) {
        help = 0;
        iResumeTimer(0);
    }

    //pause resume quit
	if(key == 'p' || key == 'P')
	{
		iPauseTimer(0);
	}
	if(key == 'r' || key == 'R')
	{
		iResumeTimer(0);
	}
	if (key == 'q') {
        exit(0);
    }
}

void iSpecialKeyboard(unsigned char key)
{
    //end
	if(key == GLUT_KEY_END)
	{
		exit(0);
	}
}

void ballChange(){

    ball_y[9] = 0;

    for (i = 0; i < 9; i++) {
        ball_x[i] += vx[i];
        if (type[i] && i < n)
            ball_y[i] = height/2.0 + amplitude[i]*sin(((ball_x[i])*frequency[i])/57.2958);
        else if (!type[i] && i < n)
            ball_y[i] = height/2.0 + amplitude[i]*cos((ball_x[i]*frequency[i])/57.2958);
        else ball_y[i] = height/2;

        ball_y[9]+=(ball_y[i]-height/2);

        if(ball_x[i] > width || ball_x[i] < 0) {
            vx[i] = -vx[i];

            if (toRight) {
                toRight = 0;
                toLeft = 1;
            }
            else {
                toLeft = 0;
                toRight = 1;
            }
        }
    }

    ball_y[9] += height/2;
    ball_x[9] += vx[9];
    if(ball_x[9] > width || ball_x[9] < 0) vx[9] = -vx[9];
}

int main()
{
	iSetTimer(5, ballChange);
	iInitialize(width, height, "Sinusoidal Waves");

	return 0;
}
