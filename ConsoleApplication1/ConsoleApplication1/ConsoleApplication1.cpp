#define STB_IMAGE_IMPLEMENTATION
#define glGenerateMipmaps ;
#include<cmath>
#include<Windows.h>
#include <gl/stb_image.h>
#include<gl/glut.h>
#include<iostream>
#include<string>

//............................ GLOBAL VARIABLES .............................
unsigned int texture;  //texture 
int width, height, nrChannels;  //texture
unsigned char* data = NULL;  //texture
bool fullScreen, carbumbed = false;    //specialKeyboard 
float ratio /*reshape*/, angle, eyey = 60, eyez = 5, eyex = 50, upx = 50, upy = 0, upz = -70; // mydraw
///////ball variables and score
int ball_x = 50, ball_y = 2, ball_z = -5, attemptss = 3;
//level counter
float level = 1;
///////cars
int fast = 30;
GLfloat x1, x2;
float car1, car2, car3; // animation variable
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 15;        // refresh interval in milliseconds [NEW]
const float PI = 3.1415927;
///////collision
float C_x1 = -2.0, C_x2 = 2.0, C_x1s, C_x2s, speed = 0.005;
static int flag = 1;
bool pause = false;
float xd, yd, Distance;

//............................... FUNCTIONS PROTOTYPES ............................
void background();
void mydraw();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void load(int imgnum);
void check(unsigned char* data);
void initRendering();
void drawBall(int, int, int);
void car();
void update();
void attempts();
void startscreen();

//................................. MAIN FUNCTION ......................................
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	//glutInitWindowSize(GetSystemMetrics(SM_CXSCREEN),
	//	GetSystemMetrics(SM_CYSCREEN));
	glutCreateWindow(" Cross the Road Game");
	glutFullScreen();
	background();
	initRendering();
	glutDisplayFunc(startscreen);
	glutIdleFunc(startscreen);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMainLoop();

}
//....................................BACKGROUND FUNCTION .......................................
void background() {
	glClearColor(0.96, 0.96, 0.86, 0);  // Set background color to black and opaque
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glEnable(GL_TEXTURE_2D);   // Enable smooth shading


}

//................................... game start screen FUNCTION .................................
void startscreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.7, 0.1, 0.9);
	glRasterPos3f(-20, 40, -30);
	char Title[] = "Cross The Road Game";
	for (int i = 0; i <= strlen(Title); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Title[i]);
	}

	glRasterPos3f(-20, 10, -30);
	char msg3[] = "********WELCOME TO CROSS THE ROAD GAME********";
	for (int i = 0; i <= strlen(msg3); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg3[i]);
	}

	glRasterPos3f(-20, 0, -30);
	char msg1[] = "* Press space to start the game";
	for (int i = 0; i <= strlen(msg1); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg1[i]);
	}

	glRasterPos3f(-20, -15, -30);
	char msg2[] = "* Or Press ESC to exit the game";
	for (int i = 0; i <= strlen(msg2); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg2[i]);
	}
	glRasterPos3f(-15, -5, -30);
	char msg4[] = "->you can move using the keyboard arrows";
	for (int i = 0; i <= strlen(msg4); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg4[i]);
	}
	glutSwapBuffers();

}

//................................... game enviroment FUNCTION .................................
void mydraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez,
		upx, upy, upz,
		0, 1, 0);

	glTranslatef(0, 0, 0); //orinin point of the game box

	drawBall(ball_x, ball_y, ball_z);
	car();
	update();
	attempts();

	//........................road
	load(1);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(100, 0, 0);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(0, 0, -70);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(100, 0, -70);
	glEnd();

	//........................roof
	load(1);
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.2);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(100, 100, 0);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(0, 100, 0);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(0, 100, -70);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(100, 100, -70);
	glEnd();

	//........................background
	load(1);
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.2);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(0, 0, -70);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(100, 0, -70);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(100, 100, -70);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(0, 100, -70);
	glEnd();

	//.......................left_wall
	load(1);
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.2);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(0, 0, -70);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(0, 100, 0);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(0, 100, -70);
	glEnd();

	//.........................right-wall
	load(1);
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.2);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(100, 0, -70);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(100, 0, 0);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(100, 100, 0);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(100, 100, -70);
	glEnd();

	glutSwapBuffers();
}

//................................... BALL FUNCTION .................................
/*void drawBall(int ball_x, int ball_y, int ball_z)
{
	// Enable lighting
	glEnable(GL_LIGHTING);
	glColor3f(0.6, 0.2, 0.6);

	GLfloat amb[] = { 0.25f,0.20725f,0.20725f,1.0f };
	GLfloat diff[] = { 1.0f,0.829f,0.829f,1.0f };
	GLfloat spec[] = { 0.296648f,0.296648f,0.296648f,1.0f };
	GLfloat shine = 11.264f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);


	glPushMatrix();

	glTranslatef(ball_x, ball_y, ball_z);

	glutSolidSphere(2, 50, 50);

	glPopMatrix();
	glDisable(GL_LIGHTING);

}*/
void drawBall(int ball_x, int ball_y, int ball_z)
{
	// Enable lighting
	glEnable(GL_LIGHTING);

	// Set up light source properties
	GLfloat light_pos[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	GLfloat light_diff[] = { .5f, .5f, .5f, .5f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glEnable(GL_LIGHT0);

	// Set material properties
	GLfloat amb[] = { 0.25f, 0.20725f, 0.20725f, 1.0f };
	GLfloat diff[] = { 1.0f, 0.829f, 0.829f, 1.0f };
	GLfloat spec[] = { 0.296648f, 0.296648f, 0.296648f, 1.0f };
	GLfloat shine = 11.264f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

	// Draw the ball
	glPushMatrix();

	glTranslatef(ball_x, ball_y, ball_z);

	// Calculate and set normals (or enable automatic normalization)
	glEnable(GL_NORMALIZE);

	glutSolidSphere(2, 50, 50);

	glPopMatrix();

	// Disable lighting after drawing the ball
	glDisable(GL_LIGHTING);
}

//................................... collision update FUNCTION .................................
void update()
{
	if (ball_z < -10 && ball_z > -60)
	{
		//car1  &  car2
		if ((ball_z / 10) % 2 == 0)
		{
			// Is the ball in the way of the car or not
			if (((ball_z % 10) < -1) && ((ball_z % 10) > -7))
			{
				if ((car1 + 12) - ball_x >= 0 && car1 - ball_x <= 0) /* ball in front of the car1 */
				{
					/*std::cout << "Car1" << "\n";
					std::cout << (ball_x) << "\n";
					std::cout << ball_z << "\n";
					std::cout << (car1) << "\t";
					std::cout << (car1 + 12) << "\n";*/
					//return to the origin point
					ball_x = 50, ball_y = 2, ball_z = -5;
					eyey = 60, eyez = 5, eyex = 50;
					upx = 50, upy = 0, upz = -70;
					attemptss -= 1; //1 attempt has been lost
					attempts(); //call the function to check if this is the third collision

					std::cout << attemptss << "\n";
					//
				}
				else if ((car2 + 35) - ball_x >= 0 && (car2 + 24) - ball_x <= 0) /* ball in front of the car2 */
				{
					/*std::cout << "Car2" << "\n";
					std::cout << (ball_x) << "\n";
					std::cout << ball_z << "\n";
					std::cout << (car2 + 24) << "\t";
					std::cout << (car2 + 36) << "\n";*/
					//return to the origin point
					ball_x = 50, ball_y = 2, ball_z = -5;
					eyey = 60, eyez = 5, eyex = 50;
					upx = 50, upy = 0, upz = -70;
					attemptss -= 1; //1 attempt has been lost
					attempts(); //call the function to check if this is the third collision

					std::cout << attemptss << "\n";
					//
				}
			}
		}

		//car3
		else
		{
			//x1 = 100 - 0 - car3, x2 = 100 - 12 - car3
			// Is the ball in the way of the car or not

			if (((ball_z % 10) < -1) && ((ball_z % 10) > -7))
			{
				if ((50 - 15 - car3) - ball_x <= 0 && (50 - car3) - ball_x >= 0) /*ball in front of the car3*/
				{
					/*std::cout << "Car3" << "\n";
					std::cout << (ball_x + 2) << "\n";
					std::cout << ball_z << "\n";
					std::cout << (100 - car3) << "\t";
					std::cout << (100 - 12 - car3) << "\n";*/
					//return to the origin point
					ball_x = 50, ball_y = 2, ball_z = -5;
					eyey = 60, eyez = 5, eyex = 50;
					upx = 50, upy = 0, upz = -70;
					attemptss -= 1; //1 attempt has been lost
					attempts(); //call the function to check if this is the third collision

				}
			}
		}
	}

}

//................................... car drawing FUNCTION .................................
void car() {

	for (int i = 0; i < 5; i++)
	{
		int y1, y2, z1, z2;
		// Roads 1,3,5 direction: left -> right
		if (i % 2)
		{

			//.................................... Car1 ..................................
			//------------------Top part-------------------//
			x1 = 2 + car1, x2 = 10 + car1, y1 = 6.0, y2 = 10.0, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(0.4f, 0.3f, 0.8f);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);     ////////////
			glVertex3f(x1 + 1, y2, z1);      ///////////// 
			glVertex3f(x2 - 1, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2 - 1, y2, z1);      //top 
			glVertex3f(x1 + 1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1 + 1, y2, z1);
			glVertex3f(x1 + 1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x2 - 1, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

			//------------------Bottom cube-------------------//

			x1 = 0 + car1, x2 = 12 + car1, y1 = 1, y2 = 6, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(0.05f, 0.02f, 0.3f);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);     ////////////
			glVertex3f(x1, y2, z1);      ///////////// 
			glVertex3f(x2, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2, y2, z1);      //top 
			glVertex3f(x1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube


			//.................................... Car2 ..................................
			//------------------Top part-------------------//
			x1 = 36 + car2, x2 = 44 + car2, y1 = 6.0, y2 = 10.0, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(0.8f, 0.5f, 0.2f);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);     ////////////
			glVertex3f(x1 + 1, y2, z1);      ///////////// 
			glVertex3f(x2 - 1, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2 - 1, y2, z1);      //top 
			glVertex3f(x1 + 1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1 + 1, y2, z1);
			glVertex3f(x1 + 1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x2 - 1, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

			//------------------Bottom cube-------------------//
			x1 = 34 + car2, x2 = 46 + car2, y1 = 1, y2 = 6, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(0.7f, 0.6f, 0.2f);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);     ////////////
			glVertex3f(x1, y2, z1);      ///////////// 
			glVertex3f(x2, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2, y2, z1);      //top 
			glVertex3f(x1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

		}

		// Roads 0,2,6 direction: right -> left
		else
		{
			//.................................... Car3 ..................................
			//------------------Top part-------------------//
			x1 = 50 - 10 - car3, x2 = 50 - 2 - car3, y1 = 6, y2 = 10, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(0.6f, 0.4f, 0.2f);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);     ////////////
			glVertex3f(x1 + 1, y2, z1);      ///////////// 
			glVertex3f(x2 - 1, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2 - 1, y2, z1);      //top 
			glVertex3f(x1 + 1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1 + 1, y2, z1);
			glVertex3f(x1 + 1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x2 - 1, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube


			//------------------Bottom cube-------------------//

			x1 = 50 - 0 - car3, x2 = 50 - 12 - car3, y1 = 1, y2 = 6, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(0.85f, 0.7f, 0.4f);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);     ////////////
			glVertex3f(x1, y2, z1);      ///////////// 
			glVertex3f(x2, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2, y2, z1);      //top 
			glVertex3f(x1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

		}
	}


}

//.................................... RESHAPE FUNCTION ..................................
void reshape(int w, int h) {
	if (h == 0) h = 1;
	ratio = w / (float)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(88, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

//.................................... RENDERING FUNCTION ..................................
void initRendering()
{
	glEnable(GL_DEPTH_TEST);
}

//................................... TIMER FUNCTION ...................................
void timer(int v) {
	// screen repaint
	glutPostRedisplay();
	glutTimerFunc(1000 / fast, timer, 0);
	car1 += level;
	car2 += level;
	car3 += level;
	if (car1 > 100)
		car1 = 0;
	if (car2 > 66)
		car2 = -34;
	if (car3 > 50)
		car3 = -50;

}
//................................... KEYBOARED FUNCTION ................................
void keyboard(unsigned char key, int x, int y) {
	angle += 4;
	if (key == 27)//esc
		exit(0);

	//////////////start new game/////////////////////
	if (key == 32)//space
	{
		attemptss = 3;
		glutDisplayFunc(mydraw); //recall the game drawing fuction
		glutIdleFunc(mydraw);
	}
	glutPostRedisplay();

	//////////////start new game/////////////////////
	if (key == 'n')
	{
		fast = 30;
		attemptss = 3;
		glutDisplayFunc(mydraw); //recall the game drawing fuction
		glutIdleFunc(mydraw);
	}
	glutPostRedisplay();
	///////////////go to the main page////////////////////////
	if (key == 'b')
	{
		attemptss = 3;
		glutDisplayFunc(startscreen);
		glutIdleFunc(startscreen);
	}
	glutPostRedisplay();

	//////////////start new level/////////////////////
	if (key == 't')
	{
		ball_x = 50, ball_y = 2, ball_z = -5; //return to the origin
		eyey = 60, eyez = 5, eyex = 50;
		upx = 50, upy = 0, upz = -70;
		fast += 30;
		attemptss = 3;
		glutDisplayFunc(mydraw); //recall the game drawing fuction
		glutIdleFunc(mydraw);
	}
	glutPostRedisplay();

	if (key == 'a')
	{
		upx -= .9 * cos(.1);
		upz -= .9 * sin(.1);
	}
	if (key == 'd') {
		upx += .9 * cos(.1);
		upz += .9 * sin(.1);
	}
	if (key == 'w')
	{
		eyez -= 2;
		eyey += .3 * abs(cos(angle));
	}
	if (key == 's') {
		eyez += 2;
		eyey += .3 * abs(cos(angle));
	}
}

//.................................. SPECIALKEYBOARED FUNCTION ............................
void specialKeyboard(int key, int x, int y) {

	if (key == GLUT_KEY_F1) {
		fullScreen = !fullScreen;
		if (fullScreen)
			glutFullScreen();
		else {
			glutPositionWindow(10, 10);
			glutReshapeWindow(600, 600);
		}
	}

	//............moving the ball..............
	if (key == GLUT_KEY_RIGHT) {
		if (ball_x < 100)
		{
			ball_x += 2;
		}
	}
	if (key == GLUT_KEY_LEFT) {
		if (ball_x > 0)
		{
			ball_x -= 2;
		}
	}
	if (key == GLUT_KEY_DOWN) {
		if (ball_z < -5)
		{
			ball_z += 2;
			eyez += 1;
		}
	}
	if (key == GLUT_KEY_UP) {
		if (ball_z > -65)
		{
			ball_z -= 2;
			eyez -= 1;
		}

	}

}

//.................................... LAOD FUNCTION ....................................
void load(int imgnum)
{
	if (imgnum == 1) {

		data = stbi_load("wall.bmp", &width, &height, &nrChannels, 0);
		check(data);
	}
}

void check(unsigned char* data) {
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
//................................ attempts FUNCTION .........................................
void attempts() {

	if (attemptss == 0) {
		ball_x = 50, ball_y = 2, ball_z = -5; //return to the origin
		eyey = 60, eyez = 5, eyex = 50;
		upx = 50, upy = 0, upz = -70;

		glColor3f(1, 1, 1);
		glRasterPos3f(10, 40, -68); //writing position on the screen
		char scoremsg[] = { "Game over" };
		for (int i = 0; i <= strlen(scoremsg); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, scoremsg[i]); //this function takes only one character ber time so it needs for loop to read all the string
		}

		glColor3f(1, 1, 1);
		glRasterPos3f(10, 30, -68);
		char scoremsg2[] = "press b -> main page";
		for (int i = 0; i <= strlen(scoremsg2); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, scoremsg2[i]);
		}

		glColor3f(1, 1, 1);
		glRasterPos3f(10, 20, -68);
		char scoremsg3[] = "press n -> new game";
		for (int i = 0; i <= strlen(scoremsg3); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, scoremsg3[i]);
		}
	}
	else if (ball_z <= -60) //the ball now in the last line from -60 to -70
	{

		glColor3f(1, 1, 1);
		glRasterPos3f(10, 40, -69);
		char winmsg[] = { "You Won, Gongrats!" };
		for (int i = 0; i <= strlen(winmsg); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, winmsg[i]);
		}
		glColor3f(1, 1, 1);
		char Levelmsg[] = { "Press t to move to the next level!" };
		for (int i = 0; i <= strlen(Levelmsg); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, Levelmsg[i]);
		}

	}
}
