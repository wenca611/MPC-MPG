/*
*	MPC-MPG 2023 - Zaverecny projekt
* 
*	Autor:		Václav Pastušek
*	Student ID:	204437
*	Email:		xpastu02@vut.cz
*	Název: Fraktálový svìt
* 
*	Klávesy: w, a, s, d, r, t, f, g, o, mezerník, PgUp, PgDn, Ctrl, Shift, šipky
*
*	Seznam vypracovanych ukolu:
*   1) Modelování objektù (3 b) - splnìno
*	- 30 draèích køivek v rùzných iteracích => jiné útvary (je zde využito rekurzivní volání funkce)
*	- 5 krychlí
*   2) Animace (1 b) - splnìno
*	- draèí køivky se toèí i hýbají, to stejné krychle na støedu plátna, bezierovy pláty, atd.
*	- pro zapnuti/vypnuti buï v menu nebo 't'
*	- pro zrychlení lze držet 't'
*	3) Osvìtlení (1 b) – splnìno
*	- barevné osvìtlení se neustále mìní a zapne/vypne se klávesou 'o' nebo z menu
*	- zmìna osvìtlení jde zastavit vypnutím èasu 't' nebo v menu a opaènì
*   4) Volný pohyb v horizontální rovinì (1 b) - splnìno
*	- šipky pohyb z pohledu kamery jako v Minecraftu
*	- WASD také pohyb, ale jen v ose x nebo z, nezávisí na pohledu kamery
*	5) Menu (2 b) - splìno
*	- Reset pohybu a pozice nebo 'r'
*	- Timer nebo 't'
*	- Letani (nahoru/dolu) nebo 'Ctrl'
*	- Osvetleni nebo 'o'
*	- Textura (generovaná) nebo 'g'
*	- Ukonèení programu nebo 'x'
*	presum = 8b
*	------------------------------------------------------
*	6) Výpis textu (2 b) - splnìno
*	- výpis je vlevém dolním rohu
*	- vypisuje se zde pohyb, pauza, reset, osvìtlení, creative/survival, naražení do krychle, atd.
*	- výpis v levém horním rohu, reaguje na zmìnu okna
*	- vypisuje se zde pozice vyhozeného míèe
*	7) Ruèní svítilna (2 b) – X
*	8) Blender model (2 b) - X
*	9) Létání (2 b) – X
*	10) Stoupání, klesání (1 b) - splnìno
*	- 'Page up/down' funguje vždy
*	- nebo je potøeba zapnout creativ mode pøes Ctrl nebo Menu a
*	nahoru se lítá pøes mezerník a dolù pøes shift+mezerník
*	presum = 8 + 3 = 11b
*	------------------------------------------------------
*	11) Hod pøedmìtu (2 b) - splnìno
*	- pøes klávesu 'f' se vyhodí míèek z pohledu kamery, který skáèe
*	- míèek se kutálí/skáèe jen nìjakou dobu nebo jde vypnout stejnou klávesou 'f'
*	- míèek lze vyhodnit odkudkoliv i ze vzduchu
*	12) Simulace krokù (2 b) - splnìno
*	- pøi pohybu pøes šipky i WASD je nadskakování, tedy kroky
*	- (vylepšení, double jump, pokud døív kamera nespadne)
*	- pokud neni zastaven èas, tak kamera po chvili klesne na puvodni uroven
*	13) Tlaèítka (2 b) – splnìno
*	- obrazovka je rozdìlena na 4 èásti a vypisuje do které èásti bylo kliknuto,
*	výpis je v dolním levém rohu
*	14) Prùhlednost (1 b) – splnìno
*	- poloprùhledná rùznobarevná krychle na støedu a bezierovy pláty
*	15) Projekèní paprsek (1 b) – X
*	presum = 11 + 7 = 18 b
*	------------------------------------------------------
*	16) Neprùchozí objekt (2 b) – splnìno
*	- u statických krychlí nejde jít dovnitø a vrací to kameru na pøedchozí pozici
*	- pøi viletìní na krychlí a pøepnutí na survival kameraman zaène skákat
*	17) Texturování (2 b) – jen generován kódem -> 1b
*	- u Beziérových plátù, dá se zapnout/vypnout pøes menu nebo 'g'
*	18) Bézierovy pláty (2 b) – splnìno
*	- nìkolik plátù jako kopeèky, rùznobarevné, poloprùhlené
*	- utváøí jakoby pohybující se týpí u ohništì, které reprezentuje pohybující se kostky
*	19) Estetika scény - -1e-3b
*	20) Vlastní rozšíøení – fraktály, double jump +1e-3b
*	Suma celkem = 18+5 = 23 b
*	------------------------------------------------------
*   Neco vlastniho co by mohlo byt take hodnoceno:
*
*	Ocekavany pocet bodu: 23b
*
*/
#include "imageLoad.h"
#include <cmath>
#include <GL/glut.h>
#include <string>
#include <stdlib.h>
#include <GLFW/glfw3.h>

// konstanty a makra pridejte zde
// PI/180
#define PI        3.141592653589793f
#define PIover180 0.017453292519943f
#define M_PI_4 0.78539816339744830962f // pi/4 rad = 45°

// globalni promenne pro transformace
float xnew = 0, ynew = 0;
float xold = 0, yold = 0;
float xx = 0, yy = 0;
bool tocime = false;
float uhel = 0;
float tranz = -100.0f, tranx = 0.f, trany = 0.f;
float tranz_pred = -100.0f, tranx_pred = 0.f, trany_pred = 0.f;
float radius = 150.f;

//okno
int windowWidth = 640;
int windowHeight = 480;

// rychlost
float rychlost = 1.;
bool increasing = true;
bool timerOn = true;

// rotace a posuny
float rOffset = 0;
float xOffset = 0;
float dragonMin = -1;
float yOffset = 0;

int rotWidth;
int dragonWidth;

bool stoupani = true;

#define MENU_RESET  1001
#define MENU_EXITOK 1002
#define MENU_EXITNO 1003 
#define MENU_TIMERON 1004
#define MENU_TIMEROFF 1005
#define MENU_FLYON 1006
#define MENU_FLYOFF 1007
#define MENU_LIGHTON 1008
#define MENU_LIGHTOFF 1009
#define MENU_TEXTUREON 1010
#define MENU_TEXTUREOFF 1011

// nastaveni projekce
float fov = 60.0f;
float nearPlane = 0.1f;
float farPlane = 200.0f;

//kamera
float cameraSpeed = 1.0f; // rychlost pohybu kamery
float maxHeight = 2.0f; // maximální výška kamery
float minHeight = 0.0f; // minimální výška kamery
float maxHoverTime = 2.f; // maximální doba, po kterou kamera mùže zùstat ve vzduchu
float hoverTime = 0.0f; // aktuální doba, po kterou kamera zùstává ve vzduchu

//creative
bool creative = false;

// Osvìtlení
GLfloat light_position[] = { 0.0, 10.0, 0.0, 10.0 }; // umístìní svìtla
GLfloat light_ambient[] = { 0.8, 0.8, 0.2, 1.0 }; // ambientní barva svìtla
GLfloat light_diffuse[] = { 1., 1.0, 1.0, 0.5 }; // difuzní barva svìtla
bool svetlo_je = false; // Osvìtlení
int color_index = 0;

// text v rohu
char last_command[50] = "Stisknete libovolnou klavesu"; // Text posledního pøíkazu uživatelského rozhraní
bool draw_text = true; // Pøíznak pro vykreslování textu
char ball_text[200] = "Pozice koule: Chybi";

// camera a míè pro vyhození po balistice
GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;
GLfloat cameraZ = 0.0f;

GLfloat ballX = 0.0f;
GLfloat ballY = 0.0f;
GLfloat ballZ = 0.0f;

GLfloat ballSpeedX = 0.f;
GLfloat ballSpeedY = 0.1f;
GLfloat ballSpeedZ = 0.f;
bool objectFlying = false;
int ballTimer = 0;
bool ball_go_up = true;

// èásti okna pro kliknutí
bool clickedTopLeft = false;
bool clickedTopRight = false;
bool clickedBottomLeft = false;
bool clickedBottomRight = false;

// Definice velikosti textury
const int textureWidth = 256;
const int textureHeight = 256;

// Textura
bool texture_is = true;

// timer
long int timer = 0;

// Globální pole pro uchování textury
GLubyte textureData[textureHeight][textureWidth][3];

// bezieruv plat jako kopecek
GLfloat controlPoints[4][4][3] = {
	{{-1.5, -1.5, -2.0}, {-0.5, -1.5, 0.0}, {0.5, -1.5, 0.0}, {1.5, -1.5, -2.0}},
	{{-1.5, -0.5, 0.0}, {-0.5, -0.5, 4.0}, {0.5, -0.5, 4.0}, {1.5, -0.5, -0.0}},
	{{-1.5, 0.5, 0.0}, {-0.5, 0.5, 4.0}, {0.5, 0.5, 4.0}, {1.5, 0.5, 0.0}},
	{{-1.5, 1.5, -2.0}, {-0.5, 1.5, 0.0}, {0.5, 1.5, 0.0}, {1.5, 1.5, -2.0}}
};

// zde vlastnosti svetel materialu, menu, popripade jinych vlastnosti
void changeLightAmbient(int value)
{
	if(timerOn)
		switch (color_index)
		{
		case 0:
			light_ambient[0] += 0.1f;
			if (light_ambient[0] >= 1.0f)
			{
				light_ambient[0] = 0.0f;
				color_index++;
			}
			break;
		case 1:
			light_ambient[1] += 0.1f;
			if (light_ambient[1] >= 1.0f)
			{
				light_ambient[1] = 0.0f;
				color_index++;
			}
			break;
		case 2:
			light_ambient[2] += 0.1f;
			if (light_ambient[2] >= 1.0f)
			{
				light_ambient[2] = 0.0f;
				color_index++;
			}
			break;
		case 3:
			light_ambient[0] += 0.1f;
			light_ambient[1] += 0.1f;

			if (light_ambient[0] >= 1.0f)
			{
				light_ambient[0] = 0.0f;
				light_ambient[1] = 0.0f;
				color_index++;
			}
			break;
		case 4:
			light_ambient[1] += 0.1f;
			light_ambient[2] += 0.1f;
			if (light_ambient[1] >= 1.0f)
			{
				light_ambient[1] = 0.0f;
				light_ambient[2] = 0.0f;
				color_index++;
			}
			break;
		case 5:
			light_ambient[2] += 0.1f;
			light_ambient[0] += 0.1f;
			if (light_ambient[2] >= 1.0f)
			{
				light_ambient[2] = 0.0f;
				light_ambient[0] = 0.0f;
				color_index = 0;
			}
			break;
		default:
			break;
		}
	glutPostRedisplay();
	glutTimerFunc(500, changeLightAmbient, 0);
}

// funkce volana pri zmene velikosti sceny
void OnReshape(int w, int h)            // event handler pro zmenu velikosti okna
{
	// Aktualizace globálních promìnných s novými rozmìry okna
	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, w, h);             // OpenGL: nastaveni rozmenu viewportu
	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();                   // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	gluPerspective(fov, (double)w/h, nearPlane, farPlane); // perspektivni projekce
}

// funkce pro inicializaci
// do teto funkce vlozit vsechny vlastnosti a inicializace, ktere se nebudou menit pri behu aplikace
void OnInit(void)
{
	// steny
	glFrontFace(GL_CW);					// nastaveni front == clockwise
	glPolygonMode(GL_FRONT, GL_FILL);   // mod predni steny fill
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}

// Funkce pro generování vlastní textury
void generateTexture() {
	for (int i = 0; i < textureHeight; i++) {
		for (int j = 0; j < textureWidth; j++) {
			// Generování hodnot barev pixelù textury
			GLubyte r = static_cast<GLubyte>(i % 256);
			GLubyte g = static_cast<GLubyte>(j % 256);
			GLubyte b = static_cast<GLubyte>((i + j) % 256);

			// Nastavení barev pixelù textury
			textureData[i][j][0] = r;
			textureData[i][j][1] = g;
			textureData[i][j][2] = b;
		}
	}
}

// Funkce pro naètení textury a nastavení parametrù texturování
void loadTexture() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

// Funkce pro vykreslení textu
void drawText(const char* text, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor4f(0.0, 0.0, 1.0, 0.8);
	glRasterPos2i(x, y);
	for (const char* c = text; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// doporucene vykresleni objektu 
// (1) pouzijte PUSH/POP pro GL_MODELVIEW matice, abyste si nerozhodili transformace cele sceny
// (2) vykreslujte vlastni objekty okolo pocatku (0,0,0) pro snazsi provadeni transformaci a umisteni ve scene
// (3) pro ziskaji aktulanich stavovych promennych v OpenGL muzete pouzit funkce glGetBooleanv(...)
void DrawPlane(int size)
{
	// (1)
	glPushMatrix();
	
	// nastaveni transformace (pokud je treba)
	glTranslatef(0, -1., 0);
	
	// (3) nastaveni vlastnosti OpenGL
	GLboolean isLightingEnabled;
	glGetBooleanv(GL_LIGHTING, &isLightingEnabled);

	if(isLightingEnabled) {
		glDisable(GL_LIGHTING);
	}

	if (svetlo_je)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, light_position); // nastavení pozice svìtla
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // nastavení ambientní barvy svìtla
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); // nastavení difuzní barvy svìtla
		glEnable(GL_LIGHT0); // zapnutí svìtla

		glEnable(GL_LIGHTING); // zapnutí osvìtlení
		glEnable(GL_DEPTH_TEST); // zapnutí testování hloubky

	}
	glColor3f(0.0f, 0.7f, 0.2f);

	// (2) vykresleni objektu
	glBegin( GL_QUADS);
	glVertex3i(-size, 0, size);
	glVertex3i(-size, 0, -size);
	glVertex3i(size , 0, -size);
	glVertex3i(size , 0, size);
	glEnd();

	glPopMatrix();

	// v pripade, ze jste vypnuli/zapnuli nejaky stav, vratte ho do puvodniho
	if (isLightingEnabled) {
		glEnable(GL_LIGHTING);
	}
}

void checkCollisionWithCube(float cubeX, float cubeY, float cubeZ, float cubeSize, float& cameraX, float& cameraY, float& cameraZ) {
	float CubeSize = cubeSize / 1.0;

	// Pokud je kamera uvnitø krychle, vrátit ji na pøedchozí platnou pozici
	if (tranx < -cubeX + CubeSize && tranx > - cubeX - CubeSize &&
		trany > cubeY - CubeSize && trany < cubeY + CubeSize &&
		tranz < -cubeZ + CubeSize && tranz > -cubeZ - CubeSize
		) {
		
		strcpy_s(last_command, sizeof last_command, "Narazil jsi do krychle");;
		// Vrácení na pøedchozí platnou pozici
		
		tranx = tranx_pred;
		trany = trany_pred;
		tranz = tranz_pred;
	}
}

void drawFractalTree(float length, int iteration, float angle)
{
	if (iteration == 0)
		return;

	glColor3f(0.5f, 0.35f, 0.05f);
	glLineWidth(iteration);

	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, length);
	glEnd();

	glTranslatef(0.0f, length, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	drawFractalTree(length * 0.7f, iteration - 1, angle);
	glRotatef(-2 * angle, 0.0f, 0.0f, 1.0f);
	drawFractalTree(length * 0.7f, iteration - 1, angle);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -length, 0.0f);
}

void DrawCube(float x, float y, float z, float size, bool move) {

	if (svetlo_je) 
	{
		glLightfv(GL_LIGHT0, GL_POSITION, light_position); // nastavení pozice svìtla
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // nastavení ambientní barvy svìtla
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); // nastavení difuzní barvy svìtla
		glEnable(GL_LIGHT0); // zapnutí svìtla

		glEnable(GL_LIGHTING); // zapnutí osvìtlení
		glEnable(GL_DEPTH_TEST); // zapnutí testování hloubky
	}

	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat rgba[] = { 1.0f, 1.0f, 1.0f, 0.8f }; // bílá barva s 50% prùhledností
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rgba);
	glPushMatrix();
	yOffset = (xOffset + dragonWidth) / 16.;
	checkCollisionWithCube(x, y + size / 2., z, size, cameraX, cameraY, cameraZ);
	glTranslatef(x, y + size / 2., z);
	if(move)
	{
		glTranslatef(0., yOffset, 0.);
		glRotatef(rOffset, 0., 1., 0.);
	}
	
	glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	// Top face (y = size)
	glNormal3f(0.0f, size, 0.0f);
	red(); glVertex3f(size, size, -size);
	yellow(); glVertex3f(-size, size, -size);
	magenta(); glVertex3f(-size, size, size);
	white(); glVertex3f(size, size, size);

	// Bottom face (y = -size)
	glNormal3f(0.0f, -size, 0.0f);
	cyan(); glVertex3f(size, -size, size);
	blue(); glVertex3f(-size, -size, size);
	green(); glVertex3f(-size, -size, -size);
	black(); glVertex3f(size, -size, -size);

	// Front face  (z = size)
	glNormal3f(0.0f, 0.0f, size);
	white(); glVertex3f(size, size, size);
	magenta(); glVertex3f(-size, size, size);
	blue(); glVertex3f(-size, -size, size);
	cyan(); glVertex3f(size, -size, size);

	// Back face (z = -size)
	glNormal3f(0.0f, 0.0f, -size);
	black(); glVertex3f(size, -size, -size);
	green(); glVertex3f(-size, -size, -size);
	yellow(); glVertex3f(-size, size, -size);
	red(); glVertex3f(size, size, -size);

	// Left face (x = -size)
	glNormal3f(-size, 0.0f, 0.0f);
	magenta(); glVertex3f(-size, size, size);
	yellow(); glVertex3f(-size, size, -size);
	green(); glVertex3f(-size, -size, -size);
	blue(); glVertex3f(-size, -size, size);

	// Right face (x = size)
	glNormal3f(size, 0.0f, 0.0f);
	red(); glVertex3f(size, size, -size);
	white(); glVertex3f(size, size, size);
	cyan(); glVertex3f(size, -size, size);
	black(); glVertex3f(size, -size, -size);
	glEnd();  // End of drawing color-cube

	glPopMatrix();
}



void bitmapText(float x, float y, void* font, string string)
{
	glRasterPos2f(x, y);
	int delka = string.size();

	for (int i = 0; i < delka; i++)
		glutBitmapCharacter(font, string[i]);
}

void DrawDragonCurve(int iterations, float x1, float y1, float x2, float y2, float z, int direction) {
	if (iterations == 0) {
		// Base case: draw a line segment
		glBegin(GL_LINE_STRIP);
		glVertex3f(x1, y1, z);
		glVertex3f(x2, y2, z);
		glEnd();
	}
	else {
		// Recursive case: divide the curve and draw two smaller curves
		float dx = x2 - x1;
		float dy = y2 - y1;
		float x3 = x1 + (dx + direction * dy) / 2;
		float y3 = y1 + (dy - direction * dx) / 2;
		DrawDragonCurve(iterations - 1, x1, y1, x3, y3, z, 1);
		DrawDragonCurve(iterations - 1, x3, y3, x2, y2, z, -1);
	}
}

void DragonCurve(int sign, float offset) {

	glPushMatrix();

	// Kreslení draèí køivky
	glLineWidth(2);
	glTranslatef(sign*30, 23, 0);
	glTranslatef(sign*xOffset, 0, 0);
	glRotatef(sign*rOffset, 0, 0, 1);
	glTranslatef(-30* sign, -23, 0);
	for (size_t i = 0; i < 15; i++)
	{
		if(sign>0)
			glColor3f(1. * i / 20., 0.2f + i / 100., 1.f - i / 20.);
		else
			glColor3f(0.2f + i / 100., 1.f - i / 20., 1. * i / 20.);

		DrawDragonCurve(i, sign*20, 10, sign*40, 30, i * 5+offset, -1);
	}

	
	glPopMatrix();
	glEnd();
}

void renderBezierSurface(float x, float y, float z) {
	int u, v;

	glPushMatrix(); // Uložení transformaèní matice
	glTranslatef(x, y+0.5, z); // Posunutí plátna
	glRotatef(-90., 1., 0., 0.);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &controlPoints[0][0][0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_TEXTURE_2D); // Zapnutí texturování
	GLuint textureID; // Deklarace promìnné pro identifikátor textury
	// Generování texturového identifikátoru
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID); // Vazba textury
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Použití plných ploch

	// Vygenerování rùznobarevného plátna
	for (u = 0; u < 30; u++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (v = 0; v <= 30; v++) {
			// Výpoèet barevného pøechodu
			GLfloat color[4];
			color[0] = u / 30.0;
			color[1] = v / 30.0;
			color[2] = 1.0 - (u / 30.0);
			color[3] = 0.5;
			glColor4fv(color); // Nastavení barvy

			glEvalCoord2f(u / 30.0, v / 30.0);
			glEvalCoord2f((u + 1) / 30.0, v / 30.0);

			if(texture_is)
			{
			// Výpoèet texturovacích souøadnic
			GLfloat texCoordU = u / 30.0;
			GLfloat texCoordV = v / 30.0;

			glTexCoord2f(texCoordU, texCoordV);
			glEvalCoord2f(u / 30.0, v / 30.0);

			glTexCoord2f(texCoordU, (v + 1) / 30.0);
			glEvalCoord2f(u / 30.0, (v + 1) / 30.0);
			}
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D); // Vypnutí texturování
	glPopMatrix(); // Obnovení transformaèní matice
}

// funkce volana pri prekresleni sceny
void OnDisplay(void)				   
{
    // nastaveni parametru pro defaultni mazani rovin
    glClearColor(0.8, 0.8, 0.8, 0.0);
    glClearDepth(1.0f);

    // mazani rovin
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);      
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);   

    // inicializace matice MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	if (draw_text) {
		drawText(last_command, 10, 10);
		drawText(ball_text, 10, windowHeight-20);
	}

	// provedeni transformaci sceny (pro rotaci mysi a pohyb klavesami)
	///glTranslatef(tranx, trany, tranz);				// relativni zmena v z
	glRotatef(-180 * ynew / PI, 1.0f, 0.0f, 0.0f);	// rotace podle osy x
	glRotatef(180 * xnew / PI, 0.0f, 1.0f, 0.0f);	// rotace podle osy y
	glTranslatef(tranx, -trany, tranz);				// relativni zmena v z

	// souradnicove osy
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 50, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 50);
	glEnd();
	glEnable(GL_LIGHTING);

	GLboolean isLightingEnabled;
	glGetBooleanv(GL_LIGHTING, &isLightingEnabled);

	if (isLightingEnabled) {
		glDisable(GL_LIGHTING);
	}

	//Draèí køivky
	DragonCurve(1, 0);
	DragonCurve(-1, 2.5);
	
	//krychle
	int cube_p = 3;
	int cube_s = 1;
	DrawCube(0, 0, 0, cube_s, true);
	DrawCube(cube_p,0, cube_p, cube_s, false);
	DrawCube(cube_p, 1, -cube_p, cube_s, true);
	DrawCube(-cube_p, 1, cube_p, cube_s, true);
	DrawCube(-cube_p, 0, -cube_p, cube_s, false);

	// Hození koule po eliptice s odrazem
	if (objectFlying)
	{
		glPushMatrix();
		glTranslatef(-ballX, ballY, -ballZ);
		glColor3f(0.0f, 1.0f, 1.0f);
		glutSolidSphere(0.5, 20, 20);
		glPopMatrix();
	}

	//glTranslatef(0., 0., 5.);
	//drawFractalTree(2., 10, 30.);
	//glTranslatef(0., 0., -5.);


	// text
	glColor3f(0.1, 0.8, 0.8);
	glTranslatef(10. + xOffset / 10., 10., 0.);
	glRotatef(rOffset, 1., 0., 0.);
	bitmapText(10.+xOffset/10., 10., GLUT_BITMAP_HELVETICA_18, "Fraktalovy svet");
	glRotatef(-rOffset, 1., 0., 0.);
	glTranslatef(-10. - xOffset / 10., -10., 0.);

	// Bezieruv Plát
	int numObjects = 50; // Poèet objektù
	float radius = (150.0 + xOffset) /3.; // Polomìr kruhu
	glRotatef(rOffset, 0., 1., 0.);
	for (int i = 0; i < numObjects; i++) {
		float angle = (2.0 * PI * i) / numObjects; // Úhel pro umístìní objektu na kružnici
		float x = radius * cos(angle); // Výpoèet souøadnice x
		float y = 0.0; // Souøadnice y (v tomto pøípadì zùstává konstantní)
		float z = radius * sin(angle); // Výpoèet souøadnice z

		renderBezierSurface(x, y, z);
	}
	glRotatef(-rOffset, 0., 1., 0.);

	// podlaha
	DrawPlane(200);

    glFlush();		
    glutSwapBuffers(); 
}

// èítaè
void onTimer(int value)
{
	timer++;
	static int counter = 0;
	if (!creative)
		if (trany > 0)
			if (counter == 2) {
				trany -= 1;
				counter = 0;
			}
			else
				counter++;

	if (rOffset < rotWidth)
		rOffset += 1;
	else
		rOffset = 0;


	if (increasing)
		xOffset += 1;
	else
		xOffset -= 1;

	if (xOffset > dragonWidth)
	{
		xOffset = dragonWidth;
		increasing = false;
	}
	else if (xOffset < -dragonWidth)
	{
		xOffset = -dragonWidth;
		increasing = true;
	};

	glutPostRedisplay();

	if (timerOn)
		glutTimerFunc(100, onTimer, value);
}

// obsluha menu
void onMenu(int value)
{
	switch (value)
	{
	case MENU_RESET:
	{
		xOffset = 0; yOffset = 0; rOffset = 0;
		tranz = -100.0f; tranx = 0; trany = 0;
		creative = false;
		radius = 150.f;
		strcpy_s(last_command, sizeof last_command, "Reset pohybu");
		break;
	}
	case MENU_EXITOK: exit(1); break;
	case MENU_EXITNO: strcpy_s(last_command, sizeof last_command, "Jeste nechci spat"); break;
	case MENU_TIMERON:
		// doplnte kod
		if (!timerOn)
		{
			timerOn = true;
			glutTimerFunc(5, onTimer, value);
		}
		strcpy_s(last_command, sizeof last_command, "Zapnuti casu");
		break;
	case MENU_TIMEROFF:
		// doplnte kod
		timerOn = false;
		strcpy_s(last_command, sizeof last_command, "Zastaveni casu");
		break;
	case MENU_FLYON:
		creative = true;
		strcpy_s(last_command, sizeof last_command, "Creative");
		break;
	case MENU_FLYOFF:
		creative = false;
		strcpy_s(last_command, sizeof last_command, "Survival");
		break;
	case MENU_LIGHTON:
		svetlo_je = true;
		strcpy_s(last_command, sizeof last_command, "Osvetleni zapnuto");
		break;
	case MENU_LIGHTOFF:
		svetlo_je = false;
		strcpy_s(last_command, sizeof last_command, "Osvetleni vypnuto");
		break;
	case MENU_TEXTUREON:
		texture_is = true;
		strcpy_s(last_command, sizeof last_command, "Textura zapnuta");
		break;
	case MENU_TEXTUREOFF:
		texture_is = false;
		strcpy_s(last_command, sizeof last_command, "Textura vypnuta");
		break;
	}

	glutPostRedisplay();
}

void createMenu()
{
	// vytvoreni podmenu
	int idSub = glutCreateMenu(onMenu);
	glutAddMenuEntry("Ano", MENU_EXITOK);
	glutAddMenuEntry("Ne", MENU_EXITNO);

	// doplnte kod
	int idTimerSub = glutCreateMenu(onMenu);
	glutAddMenuEntry("Ano", MENU_TIMERON);
	glutAddMenuEntry("Ne", MENU_TIMEROFF);

	// létání nahoru a dolù
	int idFly = glutCreateMenu(onMenu);
	glutAddMenuEntry("Ano", MENU_FLYON);
	glutAddMenuEntry("Ne", MENU_FLYOFF);

	// Osvìtlení
	int idLight = glutCreateMenu(onMenu);
	glutAddMenuEntry("Ano", MENU_LIGHTON);
	glutAddMenuEntry("Ne", MENU_LIGHTOFF);

	// Textura
	int idTexture = glutCreateMenu(onMenu);
	glutAddMenuEntry("Ano", MENU_TEXTUREON);
	glutAddMenuEntry("Ne", MENU_TEXTUREOFF);

	// vytvoreni hlavniho menu
	glutCreateMenu(onMenu);
	glutAddMenuEntry("Reset pohybu", MENU_RESET);
	glutAddSubMenu("Timer", idTimerSub);
	glutAddSubMenu("Letani", idFly);
	glutAddSubMenu("Osvetleni", idLight);
	glutAddSubMenu("Textura", idTexture);
	glutAddSubMenu("Konec", idSub);

	// prirazeni hlavniho menu na tlacitko mysi
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// funkce volana pri stisku tlacitka
void OnMouseButton(int button, int state, int x, int y)
{ 
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			tocime = true;
			xx = x;
			yy = y;
		}
		else {
			tocime = false;
			xold = xnew;
			yold = ynew;
		}
	}

	// rozdeleni obrazovky na 4 èásti, kde se klikne
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Získání normalizovaných souøadnic myši
		float normalizedX = static_cast<float>(x) / windowWidth;
		float normalizedY = static_cast<float>(y) / windowHeight;

		// Zjištìní do které èásti obrazovky bylo kliknuto
		if (normalizedX < 0.5f && normalizedY >= 0.5f) {
			clickedTopLeft = !clickedTopLeft;
			strcpy_s(last_command, sizeof last_command, "Kliknuto nahore vlevo");
		}
		else if (normalizedX >= 0.5f && normalizedY >= 0.5f) {
			clickedTopRight = !clickedTopRight;
			strcpy_s(last_command, sizeof last_command, "Kliknuto nahore vpravo");
		}
		else if (normalizedX < 0.5f && normalizedY < 0.5f) {
			clickedBottomLeft = !clickedBottomLeft;
			strcpy_s(last_command, sizeof last_command, "Kliknuto dole vlevo");
		}
		else if (normalizedX >= 0.5f && normalizedY < 0.5f) {
			clickedBottomRight = !clickedBottomRight;
			strcpy_s(last_command, sizeof last_command, "Kliknuto dole vpravo");
		}
	}

	glutPostRedisplay();
}

// funkce volana pri pohybu mysi a stistenem tlacitku mysi
void OnMouseMotion(int x, int y)
{ 
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	if (tocime) {
		xnew = xold + (x - xx) / radius;
		ynew = yold + (y - yy) / radius;
		uhel = xnew * PIover180;
		glutPostRedisplay();
	}
}

// funkce volana pri stisku specialnich klaves
void OnSpecial(int key, int mx, int my)
{
	switch (key)
	{
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		tranx_pred = tranx;
		trany_pred = trany;
		tranz_pred = tranz;
		break;
	}

	// detekce modifikatoru
	switch (glutGetModifiers()) {
	case GLUT_ACTIVE_SHIFT:stoupani = false; break;
	case GLUT_ACTIVE_CTRL: {
		creative = !creative;
		if (creative)
			strcpy_s(last_command, sizeof last_command, "Creative");
		else
			strcpy_s(last_command, sizeof last_command, "Survival");
		break;
	}
		//case GLUT_ACTIVE_ALT:	jump = 20; break;
	default: stoupani = true;
	}
	
	GLfloat m[16];
	switch(key)
	{
		case GLUT_KEY_DOWN:
		{
			GLfloat forward[3] = { 0.0f, 0.0f, -1.0f };
			GLfloat dir[3];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			dir[0] = m[0] * forward[0] + m[4] * forward[1] + m[8] * forward[2];
			dir[1] = m[1] * forward[0] + m[5] * forward[1] + m[9] * forward[2];
			dir[2] = m[2] * forward[0] + m[6] * forward[1] + m[10] * forward[2];
			tranz += 2 * dir[2];
			tranx -= 2 * dir[0];
			strcpy_s(last_command, sizeof last_command, "Pohyb dozadu");
		}
		break;
		case GLUT_KEY_UP:
		{
			GLfloat forward[3] = { 0.0f, 0.0f, -1.0f };
			GLfloat dir[3];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			dir[0] = m[0] * forward[0] + m[4] * forward[1] + m[8] * forward[2];
			dir[1] = m[1] * forward[0] + m[5] * forward[1] + m[9] * forward[2];
			dir[2] = m[2] * forward[0] + m[6] * forward[1] + m[10] * forward[2];
			tranz -= 2 * dir[2];
			tranx += 2 * dir[0];
			strcpy_s(last_command, sizeof last_command, "Pohyb dopredu");
		}
		break;
		case GLUT_KEY_RIGHT:
		{
			GLfloat right[3] = { 1.0f, 0.0f, 0.0f };
			GLfloat dir[3];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			dir[0] = m[0] * right[0] + m[4] * right[1] + m[8] * right[2];
			dir[1] = m[1] * right[0] + m[5] * right[1] + m[9] * right[2];
			dir[2] = m[2] * right[0] + m[6] * right[1] + m[10] * right[2];
			tranz += 2 * dir[2];
			tranx -= 2 * dir[0];
			strcpy_s(last_command, sizeof last_command, "Pohyb doprava");
		}
		break;
		case GLUT_KEY_LEFT:
		{
			GLfloat right[3] = { 1.0f, 0.0f, 0.0f };
			GLfloat dir[3];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			dir[0] = m[0] * right[0] + m[4] * right[1] + m[8] * right[2];
			dir[1] = m[1] * right[0] + m[5] * right[1] + m[9] * right[2];
			dir[2] = m[2] * right[0] + m[6] * right[1] + m[10] * right[2];
			tranz -= 2 * dir[2];
			tranx += 2 * dir[0];
			strcpy_s(last_command, sizeof last_command, "Pohyb doleva");
		}
		break;
		case GLUT_KEY_PAGE_UP: // Page up 
		{
			trany += 1;
			strcpy_s(last_command, sizeof last_command, "Stoupani");
			break;
		}
		case GLUT_KEY_PAGE_DOWN: // Page down	
		{
			trany -= 1;
			strcpy_s(last_command, sizeof last_command, "Klesani");
		}
		default:
			break;
	}

	if (!creative) {
		switch (key)
		{
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT:
			if (trany < maxHeight) // pokud kamera není pøíliš vysoko, pohybuje se nahoru
			{
				trany += cameraSpeed;
				hoverTime = 0.0f; // resetuje dobu zùstávání ve vzduchu
			}
			else // pokud dosáhla maximální výšky, zaène se snižovat
			{
				trany -= cameraSpeed;
			}
			break;
		default:
			break;
		}

		// omezení výšky kamery na minimální hodnoty
		if (trany < minHeight)
		{
			trany = minHeight;
		}

		// pokud kamera zùstává ve vzduchu pøíliš dlouho, spadne dolù
		if (trany > minHeight) // kamera je nahoøe
		{
			hoverTime += 1.f; // zvýší dobu zùstávání ve vzduchu
			if (hoverTime >= maxHoverTime) // pokud je doba pøekroèena, spadne dolù
			{
				trany -= cameraSpeed;
				hoverTime = 0.0f; // resetuje dobu zùstávání ve vzduchu
			}
		}
	}
	

	glutPostRedisplay();
}

void onKeyboard(unsigned char key, int mx, int my)
{
	// prevod na mala pismena
	key = (key > 'A' && key <= 'Z') ? key + 'a' - 'A' : key;

	switch (key)
	{
	case 'w':
	case 'a':
	case 's':
	case 'd':
		tranx_pred = tranx;
		trany_pred = trany;
		tranz_pred = tranz;
		break;
	}


	// detekce modifikatoru
	switch (glutGetModifiers()) {
	case GLUT_ACTIVE_SHIFT: stoupani = false; break;
		//case GLUT_ACTIVE_CTRL:	jump = 10; break;
		//case GLUT_ACTIVE_ALT:	jump = 20; break;
	default: stoupani = true;
	}

	// zpracovani klaves
	switch (key)
	{
	case 'g':
	{
		texture_is = !texture_is;
		if (texture_is)
			strcpy_s(last_command, sizeof last_command, "Textura zapnuta");
		else
			strcpy_s(last_command, sizeof last_command, "Textura vypnuta");
		break;
	}
	case 'f':
	{
		objectFlying = !objectFlying;
		if (objectFlying)
			strcpy_s(last_command, sizeof last_command, "Vyhozeni objektu");
		else
			strcpy_s(last_command, sizeof last_command, "Objekt zmizel");
		break;
	}
	case 't':
	{
		
		if (!timerOn)
		{
			glutTimerFunc(5, onTimer, MENU_TIMERON);
			strcpy_s(last_command, sizeof last_command, "Zapnuti casu");
		}
		else
			strcpy_s(last_command, sizeof last_command, "Zastaveni casu");
		timerOn = !timerOn;
		break;
	}
	case 'o':
	{
		svetlo_je = !svetlo_je;
		if(svetlo_je)
			strcpy_s(last_command, sizeof last_command, "Osvetleni zapnuto");
		else
			strcpy_s(last_command, sizeof last_command, "Osvetleni vypnuto");
		break;
	}
	case 'x':
	{
		exit(1);
		break;
	}
	case 'r':
	{
		xOffset = 0; yOffset = 0; rOffset = 0;
		tranz = -100.0f; tranx = 0; trany = 0;
		creative = false;
		radius = 150.f;
		strcpy_s(last_command, sizeof last_command, "Reset pohybu");
		break;
	}
	case ' ':
	{
		if (creative)
			if(stoupani){
				trany += 1;
				strcpy_s(last_command, sizeof last_command, "Stoupani");
			}
			else
			{
				trany -= 1;
				strcpy_s(last_command, sizeof last_command, "Klesani");
			}
		break;
	}
	case 'w':
	{
		tranz += 2 * cos(uhel);
		tranx -= 2 * sin(uhel);
		strcpy_s(last_command, sizeof last_command, "W: Pohyb po ose z");
		break;
	}
	case 's':
	{
		tranz -= 2 * cos(uhel);
		tranx += 2 * sin(uhel);
		strcpy_s(last_command, sizeof last_command, "S: Pohyb po ose z");
		break;
	}
	case 'a':
	{
		tranx += 2 * cos(uhel);
		tranz += 2 * sin(uhel);
		strcpy_s(last_command, sizeof last_command, "A: Pohyb po ose x");
		break;
	}
	case 'd':
	{
		tranx -= 2 * cos(uhel);
		tranz -= 2 * sin(uhel);
		strcpy_s(last_command, sizeof last_command, "D: Pohyb po ose x");
		break;
	}
	default:
		break;
	}


	if (!creative) {
		switch (key)
		{
		case 'w':
		case 'a':
		case 's':
		case 'd':
			if (trany < maxHeight) // pokud kamera není pøíliš vysoko, pohybuje se nahoru
			{
				trany += cameraSpeed;
				hoverTime = 0.0f; // resetuje dobu zùstávání ve vzduchu
			}
			else // pokud dosáhla maximální výšky, zaène se snižovat
			{
				trany -= cameraSpeed;
			}
			break;
		default:
			break;
		}

		// omezení výšky kamery na minimální hodnoty
		if (trany < minHeight)
		{
			trany = minHeight;
		}


		// pokud kamera zùstává ve vzduchu pøíliš dlouho, spadne dolù
		if (trany > minHeight) // kamera je nahoøe
		{
			hoverTime += 1.f; // zvýší dobu zùstávání ve vzduchu
			if (hoverTime >= maxHoverTime) // pokud je doba pøekroèena, spadne dolù
			{
				trany -= cameraSpeed;
				hoverTime = 0.0f; // resetuje dobu zùstávání ve vzduchu
			}
		}
	}




	// vynuceni prekresleni
	glutPostRedisplay();

}

// hod koule
void update(int value)
{
	if(objectFlying)
	{
		ballTimer++;
		if (ballTimer == 5000)
			objectFlying = false;
		string ballInfo = "Pozice koule (X, Y, Z): " + to_string(ballX) + " "
			+ to_string(ballY) + " " + to_string(ballZ);
		strcpy_s(ball_text, sizeof ball_text, ballInfo.c_str());
		
		ballX += ballSpeedX;
		ballY += ballSpeedY;
		ballZ += ballSpeedZ;
		// zpomalení
		ballSpeedX *= 0.999;
		ballSpeedZ *= 0.999;

		if (ballY > -1.0) {
			// Utlumování skoku
			ballSpeedY -= 0.001;
		}
		else {
			// Skákání nahoru a dolù
			if (ballSpeedY < -0.01)
				ballSpeedY *= -0.9;
			else if (ballSpeedY > 0.01)
				ballSpeedY *= -0.9;
			else
				ballSpeedY = 0.0f;
		}

		if (ballY < -0.0f) {
			// Odrážení od podlahy
			ballSpeedY *= -0.9f;
		}
	}
	else
	{
		ballTimer = 0;
		strcpy_s(ball_text, sizeof ball_text, "Pozice koule: Chybi");
		GLfloat m[16];
		GLfloat forward[3] = { 0.0f, 0.0f, -1.0f };
		GLfloat dir[3];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);
		dir[0] = m[0] * forward[0] + m[4] * forward[1] + m[8] * forward[2];
		dir[1] = m[1] * forward[0] + m[5] * forward[1] + m[9] * forward[2];
		dir[2] = m[2] * forward[0] + m[6] * forward[1] + m[10] * forward[2];
		ballX = tranx;
		ballY = trany;
		ballZ = tranz;
		ballSpeedX = dir[0] / 40.;
		ballSpeedY = 0.1;
		ballSpeedZ = -dir[2] / 40.;
	}
	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

// funkce main
int main(int argc, char* argv[])
{
	dragonWidth = 50;
	rotWidth = 360;

	glutInit(&argc, argv);					// inicializace knihovny GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // init double buffer
	glutInitWindowSize(windowWidth, windowHeight);			// nastaveni pocatecni velikosti dale oteviranych oken
    glutInitWindowPosition(200, 200);		// nastaveni pocatecniho umisteni dale oteviranych oken
    
	glutCreateWindow("Projekt 2023 – Autor: xpastu02");	// vytvoreni okna

	generateTexture(); // Generování vlastní textury

	// registracni funkce
	glutDisplayFunc(OnDisplay);				// registrace funkce volane pri prekreslovani aktualniho okna
    glutReshapeFunc(OnReshape);				// registrace funkce volane pri zmene velikosti aktualniho okna
    glutMouseFunc(OnMouseButton);			// registrace funkce pro stisk tlacitka mysi
    glutMotionFunc(OnMouseMotion);			// registrace funkce pro pohyb mysi pri stisknutem tlacitku
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(OnSpecial);				// registrace funkce pro zachytavani specialnich klaves

	// pripadne dalsi udalosti...
	createMenu();
	glutTimerFunc(5, onTimer, 1);

	// inicializace sceny
	OnInit();

	// spuštìní èasovaèe pro zmìnu ambientního svìtla
	glutTimerFunc(1000, changeLightAmbient, 0);

	// hozeni koule
	glutTimerFunc(0, update, 0);
	
	glutMainLoop();

    return 0;
}
