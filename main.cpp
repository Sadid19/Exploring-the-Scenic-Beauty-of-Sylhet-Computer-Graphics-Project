#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.14159265358979323846

/* =====================================================================
   GLOBAL SCENE SELECTOR
   0 = Cover Page
   1 = Guidebook
   2 = Sylhet Railway Station
   3 = Srimongol Tea Garden
   4 = Jaflong Riverside View
   5 = Utmachora Waterfall
   6 = Sylhet City
   ===================================================================== */
int currentScene = 0;

bool isMuted = false;

void stopAllSounds()
{
    PlaySound(NULL, NULL, 0);
}

// SHARED UTILITY
void renderBitmapString(float x, float y, void *font, const char *string)
{
    const char *c;
    glRasterPos2f(x, y);
    for(c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

// drawCircle – used by Tea Garden
void drawCircle(float radius)
{
    int triangleAmount = 100;
    float twicePi = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f
        (
         radius * cos(i * twicePi / triangleAmount),
         radius * sin(i * twicePi / triangleAmount)
        );
    }
    glEnd();
}

// ellipse – used by Jaflong
void ellipse(float cx, float cy, float rx, float ry)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + rx * cos(angle), cy + ry * sin(angle));
    }
    glEnd();
}

// circle – used by Utmachora and City
void circle(float cx, float cy, float r, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

// circleOutline – used by City
void circleOutline(float cx, float cy, float r, int segments)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

//_________________________________________________________________________

//  RAILWAY STATION – global variables & functions  (Scene 2)
bool  rw_nightMode = false;
bool  rw_animating = false;

float rw_sunY = 0.375f;
float rw_moonY = -1.179f;

float rw_trainX = 0.0f;
float rw_trainSpeed = 0.0f;

float rw_cloudX = 0.0f;
float rw_cloudSpeed = 0.004f;

bool  rw_isTrainSoundPlaying = false;

void rw_trainSound()
{
    if(currentScene == 2 && rw_trainSpeed != 0.0f)
    {
        if(!rw_isTrainSoundPlaying && !isMuted)
        {
            PlaySound(TEXT("train.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            rw_isTrainSoundPlaying = true;
        }
    }
    else
    {
        if(rw_isTrainSoundPlaying)
        {
            stopAllSounds();
            rw_isTrainSoundPlaying = false;
        }
    }
}

void rw_circle(float x, float y, float r, float red, float green, float blue)
{
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for(int i = 0; i <= 100; i++)
    {
        float angle = 2.0f * 3.14159265f * i / 100.0f;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void rw_emptyCircle(float x, float y, float r)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i <= 100; i++)
    {
        float angle = 2.0f * 3.14159265f * i / 100.0f;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void rw_text(float x, float y, const char *str)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for(int i = 0; str[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
}

void rw_rectangle(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.3f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void rw_line(float x1, float y1, float x2, float y2)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void rw_drawStars()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(-0.791f, 0.571f);
    glVertex2f(-0.565f, 0.643f);
    glVertex2f(-0.339f, 0.464f);
    glVertex2f(-0.043f, 0.696f);
    glVertex2f(0.252f, 0.571f);
    glVertex2f(0.739f, 0.643f);
    glVertex2f(0.600f, 0.393f);
    glVertex2f(-0.739f, 0.321f);
    glVertex2f(-0.165f, 0.286f);
    glEnd();
}

void rw_drawTrain()
{
    float t = rw_trainX;

    rw_rectangle(0.078f + t, -0.679f, 0.426f + t, -0.554f, 0.10f, 0.30f, 0.85f);
    rw_rectangle(0.426f + t, -0.679f, 0.478f + t, -0.554f, 1.00f, 0.85f, 0.10f);
    rw_rectangle(0.496f + t, -0.679f, 0.843f + t, -0.554f, 0.85f, 0.10f, 0.10f);

    glColor3f(1.00f, 0.85f, 0.10f);
    glBegin(GL_POLYGON);
    glVertex2f(0.843f + t, -0.679f);
    glVertex2f(0.913f + t, -0.679f);
    glVertex2f(0.896f + t, -0.554f);
    glVertex2f(0.843f + t, -0.554f);
    glEnd();

    for(float x = 0.113f; x <= 0.322f; x += 0.070f)
    {
        rw_rectangle(x + t, -0.625f, x + 0.052f + t, -0.589f, 0.60f, 0.88f, 1.00f);
    }

    for(float x = 0.530f; x <= 0.739f; x += 0.070f)
    {
        rw_rectangle(x + t, -0.625f, x + 0.052f + t, -0.589f, 0.60f, 0.88f, 1.00f);

    }
    rw_emptyCircle(0.478f + t, -0.589f, 0.007f);
    rw_emptyCircle(0.510f + t, -0.589f, 0.007f);
    rw_line(0.478f + t, -0.589f, 0.510f + t, -0.589f);

    rw_circle(0.113f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);
    rw_circle(0.165f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);
    rw_circle(0.391f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);
    rw_circle(0.443f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);

    rw_circle(0.530f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);
    rw_circle(0.583f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);
    rw_circle(0.774f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);
    rw_circle(0.843f + t, -0.696f, 0.020f, 0.03f, 0.03f, 0.03f);

    rw_line(0.113f + t, -0.696f, 0.443f + t, -0.696f);
    rw_line(0.530f + t, -0.696f, 0.843f + t, -0.696f);

    if (rw_nightMode)
    {
        glColor3f(1.00f, 1.00f, 0.35f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.913f + t, -0.607f);
        glVertex2f(1.052f + t, -0.554f);
        glVertex2f(1.052f + t, -0.661f);
        glEnd();
        rw_circle(0.913f + t, -0.607f, 0.032f, 1.00f, 1.00f, 0.20f);
    }
    else
    {
        rw_circle(0.913f + t, -0.607f, 0.013f, 1.00f, 1.00f, 0.55f);
    }
}

void rw_drawClouds()
{
    float c = rw_cloudX;
    float cloudColor = rw_nightMode ? 0.55f : 1.00f;

    rw_circle(-0.078f + c, 0.732f, 0.114f, cloudColor, cloudColor, cloudColor);
    rw_circle(-0.148f + c, 0.536f, 0.129f, cloudColor, cloudColor, cloudColor);
    rw_circle(0.009f + c, 0.589f, 0.114f, cloudColor, cloudColor, cloudColor);
    rw_circle(-0.252f + c, 0.714f, 0.121f, cloudColor, cloudColor, cloudColor);
    rw_circle(-0.403f + c, 0.691f, 0.107f, cloudColor, cloudColor, cloudColor);
    rw_circle(-0.334f + c, 0.554f, 0.121f, cloudColor, cloudColor, cloudColor);

    rw_circle(0.565f + c, 0.696f, 0.139f, cloudColor, cloudColor, cloudColor);
    rw_circle(0.722f + c, 0.679f, 0.100f, cloudColor, cloudColor, cloudColor);
    rw_circle(0.652f + c, 0.518f, 0.113f, cloudColor, cloudColor, cloudColor);
    rw_circle(0.443f + c, 0.554f, 0.130f, cloudColor, cloudColor, cloudColor);
    rw_circle(0.355f + c, 0.679f, 0.121f, cloudColor, cloudColor, cloudColor);
    rw_circle(0.419f + c, 0.787f, 0.121f, cloudColor, cloudColor, cloudColor);
}

void rw_drawPeople()
{
    rw_circle(-0.687f, -0.509f, 0.014f, 0.98f, 0.78f, 0.60f);
    rw_line(-0.687f, -0.527f, -0.687f, -0.598f);
    rw_line(-0.687f, -0.527f, -0.711f, -0.571f);
    rw_line(-0.687f, -0.527f, -0.663f, -0.571f);
    rw_line(-0.687f, -0.598f, -0.701f, -0.661f);
    rw_line(-0.687f, -0.598f, -0.673f, -0.661f);

    rw_circle(-0.478f, -0.509f, 0.014f, 0.98f, 0.78f, 0.60f);
    rw_line(-0.478f, -0.527f, -0.478f, -0.598f);
    rw_line(-0.478f, -0.527f, -0.502f, -0.571f);
    rw_line(-0.478f, -0.527f, -0.454f, -0.571f);
    rw_line(-0.478f, -0.598f, -0.492f, -0.661f);
    rw_line(-0.478f, -0.598f, -0.464f, -0.661f);
}

void rw_drawScene()
{
    if (rw_nightMode)
    {
        rw_drawStars();
    }

    if (rw_sunY > -1.179f)
    {
        rw_circle(0.600f, rw_sunY, 0.089f, 1.00f, 0.85f, 0.00f);
        rw_line(0.357f, rw_sunY, 0.493f, rw_sunY);
        rw_line(0.450f, rw_sunY + 0.130f, 0.522f, rw_sunY + 0.079f);
        rw_line(0.605f, rw_sunY + 0.121f, 0.605f, rw_sunY + 0.241f);
        rw_line(0.761f, rw_sunY + 0.143f, 0.692f, rw_sunY + 0.091f);
        rw_line(0.739f, rw_sunY, 0.865f, rw_sunY);
        rw_line(0.697f, rw_sunY - 0.066f, 0.772f, rw_sunY - 0.116f);
        rw_line(0.610f, rw_sunY - 0.118f, 0.610f, rw_sunY - 0.214f);
        rw_line(0.515f, rw_sunY - 0.082f, 0.450f, rw_sunY - 0.134f);
    }

    if(rw_moonY <= 0.786f)
    {
        rw_circle(0.739f, rw_moonY, 0.089f, 0.95f, 0.95f, 0.90f);

    }
    rw_drawClouds();

    // birds
    rw_line(-0.652f, 0.143f, -0.617f, 0.179f); rw_line(-0.617f, 0.179f, -0.583f, 0.143f);
    rw_line(-0.443f, 0.071f, -0.409f, 0.107f); rw_line(-0.409f, 0.107f, -0.374f, 0.071f);
    rw_line(-0.270f, 0.161f, -0.235f, 0.196f); rw_line(-0.235f, 0.196f, -0.200f, 0.161f);
    rw_line(-0.461f, 0.161f, -0.426f, 0.196f); rw_line(-0.426f, 0.196f, -0.391f, 0.161f);
    rw_line(-0.600f, 0.036f, -0.548f, 0.089f); rw_line(-0.548f, 0.089f, -0.496f, 0.036f);

    // ground
    rw_rectangle(-1.000f, -1.000f, 1.000f, -0.786f, 0.20f, 0.70f, 0.25f);
    rw_rectangle(-1.000f, -0.821f, 1.000f, -0.768f, 0.35f, 0.35f, 0.35f);
    rw_rectangle(-1.000f, -0.768f, 1.000f, -0.714f, 0.12f, 0.12f, 0.12f);

    // sleeper rails
    for(float x = -0.965f; x <= 0.843f; x += 0.070f)
    {
        rw_rectangle(x, -0.768f, x + 0.026f, -0.714f, 0.55f, 0.32f, 0.16f);
    }

    // platform
    rw_rectangle(-0.826f, -0.714f, 0.043f, -0.589f, 0.75f, 0.65f, 0.50f);

    // building
    rw_rectangle(-0.757f, -0.589f, -0.200f, -0.232f, 0.902f, 0.902f, 0.980f);

    // roof
    glColor3f(0.65f, 0.20f, 0.18f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.757f, -0.232f);
    glVertex2f(-0.478f, 0.000f);
    glVertex2f(-0.200f, -0.232f);
    glEnd();

    // roof border
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.757f, -0.232f);
    glVertex2f(-0.478f, 0.000f);
    glVertex2f(-0.200f, -0.232f);
    glEnd();

    // nameboard
    glColor3f(0.15f, 0.35f, 0.80f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.670f, -0.161f);
    glVertex2f(-0.287f, -0.161f);
    glVertex2f(-0.374f, -0.089f);
    glVertex2f(-0.583f, -0.089f);
    glEnd();

    rw_text(-0.600f, -0.143f, "SYLHET STATION");
    rw_rectangle(-0.530f, -0.589f, -0.426f, -0.321f, 0.45f, 0.22f, 0.10f);
    rw_rectangle(-0.652f, -0.339f, -0.583f, -0.250f, 0.60f, 0.88f, 1.00f);
    rw_rectangle(-0.374f, -0.339f, -0.304f, -0.250f, 0.60f, 0.88f, 1.00f);

    if(rw_nightMode)
    {
         rw_drawStars();
    }

    rw_drawPeople();
    rw_drawTrain();
}

void displayRailwayStation()
{
    if(rw_nightMode)
        glClearColor(0.02f, 0.03f, 0.18f, 1.00f);
    else
        glClearColor(0.67f, 0.88f, 1.00f, 1.00f);

    glClear(GL_COLOR_BUFFER_BIT);
    rw_drawScene();
    glFlush();
}

void rw_update(int value)
{
    if(currentScene == 2)
    {
        if(rw_animating)
        {
            if(rw_nightMode)
            { // Transitioning to Night
                if(rw_sunY > -1.179f)
                {
                    rw_sunY -= 0.009f;
                }
                else
                {
                    rw_sunY = -1.179f;
                }

                if(rw_moonY < 0.375f)
                {
                    rw_moonY += 0.009f;
                }
                else
                {
                    rw_moonY = 0.375f;
                }

                if(rw_sunY <= -1.179f && rw_moonY >= 0.375f)
                {
                    rw_animating = false;
                }
            }
            else
            { // Transitioning to Day
                if(rw_sunY < 0.375f)
                {
                    rw_sunY += 0.009f;
                }
                else
                {
                    rw_sunY = 0.375f;
                }

                if(rw_moonY > -1.179f)
                {
                    rw_moonY -= 0.009f;
                }
                else
                {
                    rw_moonY = -1.179f;
                }

                if(rw_sunY >= 0.375f && rw_moonY <= -1.179f)
                {
                     rw_animating = false;

                }
            }
        }

        rw_trainX += rw_trainSpeed;

        if (rw_trainX > 0.783f)
            rw_trainSpeed = 0.0f;

        if (rw_trainX < -1.739f)
            rw_trainSpeed = 0.0f;

        rw_trainSound();

        rw_cloudX += rw_cloudSpeed;

        if (rw_cloudX > 2.087f)
            rw_cloudX = -2.087f;
    }
    glutPostRedisplay();
    glutTimerFunc(30, rw_update, 0);
}


// __________________________________________________________________

//  TEA GARDEN – global variables & functions  (Scene 3)

float birdPos    = -1.2f;
float birdScale  =  1.0f;
bool  isStarted  = false;

float rainY_move = 0.0f;
bool  isRaining  = false;

float cloudPos   = -0.6f;
bool  cloudMoving = false;

void updateBirds_Tea(int value) {
    if (isStarted) {
        birdPos += 0.01f;
        if (birdPos > 1.1f) birdPos = -1.1f;
        birdScale = 1.0f - (birdPos + 1.0f) * 0.4f;
    }
    glutPostRedisplay();
    glutTimerFunc(50, updateBirds_Tea, 0);
}

void updateRain(int value) {
    if (isRaining) {
        rainY_move += 0.08f;
        if (rainY_move > 0.3f) rainY_move = 0.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(40, updateRain, 0);
}

void updateClouds_Tea(int value) {
    if (currentScene == 3 && cloudMoving) {
        cloudPos += 0.0008f;
        if (cloudPos > 1.8f) cloudPos = -1.8f;
    }
    glutPostRedisplay();
    glutTimerFunc(20, updateClouds_Tea, 0);
}

void drawteaRain() {
    if (!isRaining) return;
    glPushMatrix();
    glTranslatef(0.0f, -rainY_move, 0.0f);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    for (float x = -1.0f; x <= 1.0f; x += 0.1f) {
        glVertex2f(x,  1.1f); glVertex2f(x,  1.0f);
        glVertex2f(x,  0.7f); glVertex2f(x,  0.6f);
        glVertex2f(x,  0.4f); glVertex2f(x,  0.3f);
        glVertex2f(x,  0.1f); glVertex2f(x,  0.0f);
        glVertex2f(x, -0.2f); glVertex2f(x, -0.3f);
        glVertex2f(x, -0.5f); glVertex2f(x, -0.6f);
        glVertex2f(x, -0.8f); glVertex2f(x, -0.9f);
    }
    glEnd();
    glPopMatrix();
}

void teaSky() {
    glBegin(GL_QUADS);
    if (!isRaining) glColor3f(0.529f, 0.808f, 0.922f);
    else            glColor3f(0.2f,   0.2f,   0.2f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f(-1.0f,  0.2f);
    glVertex2f( 1.0f,  0.2f);
    glVertex2f( 1.0f,  1.0f);
    glEnd();
}

void teaBirdsShape() {
    glLineWidth(2.0f);
    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_LINES);
    glVertex2f(-0.05f,  0.05f); glVertex2f( 0.0f,   0.0f);
    glVertex2f( 0.0f,   0.0f);  glVertex2f( 0.05f,  0.05f);
    glVertex2f(-0.15f, -0.05f); glVertex2f(-0.10f, -0.10f);
    glVertex2f(-0.10f, -0.10f); glVertex2f(-0.05f, -0.05f);
    glVertex2f( 0.05f, -0.05f); glVertex2f( 0.10f, -0.10f);
    glVertex2f( 0.10f, -0.10f); glVertex2f( 0.15f, -0.05f);
    glEnd();
}

void teaHill() {
    glColor3f(0.01f, 0.05f, 0.01f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, 0.6f); glVertex2f(-0.2f, 0.0f);
    glVertex2f(-1.0f, 0.0f); glVertex2f(-1.0f, 0.3f);
    glEnd();

    glColor3f(0.02f, 0.07f, 0.02f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(0.8f, 0.3f); glVertex2f(1.0f, 0.6f); glVertex2f(1.0f, 0.0f);
    glEnd();

    glColor3f(0.03f, 0.12f, 0.03f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(0.1f, 0.0f); glVertex2f(0.6f, 0.6f); glVertex2f(1.0f, 0.0f);
    glEnd();

    glColor3f(0.05f, 0.18f, 0.05f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-0.2f, 0.0f); glVertex2f(0.3f, 0.5f); glVertex2f(0.7f, 0.0f);
    glEnd();

    glColor3f(0.08f, 0.25f, 0.08f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-0.7f, 0.0f); glVertex2f(-0.1f, 0.7f); glVertex2f(0.5f, 0.0f);
    glEnd();
}

void drawTeaTree() {
    glColor3f(0.1f, 0.05f, 0.0f);
    glLineWidth(5.0f);
    glBegin(GL_LINE_STRIP);
        glVertex2f( 0.0f,  0.0f);  glVertex2f( 0.0f, -0.1f);
        glVertex2f(-0.05f, 0.0f);  glVertex2f( 0.05f, 0.0f);
        glVertex2f( 0.0f, -0.1f);
    glEnd();

    glColor3f(0.0f, 0.15f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f( 0.05f, 0.0f);  glVertex2f(-0.05f, 0.0f);
    glVertex2f(-0.15f, 0.05f); glVertex2f(-0.13f, 0.12f);
    glVertex2f(-0.1f,  0.15f); glVertex2f( 0.1f,  0.15f);
    glVertex2f( 0.13f, 0.12f); glVertex2f( 0.15f, 0.05f);
    glEnd();

    glLineWidth(2.5f);
    glColor3f(0.0f, 0.4f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f( 0.05f, 0.0f);  glVertex2f(-0.05f, 0.0f);
    glVertex2f(-0.15f, 0.05f); glVertex2f(-0.13f, 0.12f);
    glVertex2f(-0.1f,  0.15f); glVertex2f( 0.1f,  0.15f);
    glVertex2f( 0.13f, 0.12f); glVertex2f( 0.15f, 0.05f);
    glEnd();
}

void renderClouds() {
    glPushMatrix();
    glTranslatef(cloudPos, 0.88f, 0.0f);
    glPushMatrix(); glTranslatef( 0.0f,  0.0f, 0.0f); drawCircle(0.08f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.08f, 0.0f, 0.0f); drawCircle(0.07f); glPopMatrix();
    glPushMatrix(); glTranslatef( 0.08f, 0.0f, 0.0f); drawCircle(0.07f); glPopMatrix();
    glPushMatrix(); glTranslatef( 0.0f,  0.05f,0.0f); drawCircle(0.08f); glPopMatrix();
    glPushMatrix(); glTranslatef( 0.05f, 0.04f,0.0f); drawCircle(0.06f); glPopMatrix();
    glPopMatrix();
}

void drawBigTree() {
    glColor3f(0.36f, 0.25f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(-0.01f,-0.1f); glVertex2f(0.01f,-0.1f);
    glVertex2f(0.01f, 0.0f);  glVertex2f(-0.01f, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.35f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.12f,0.0f); glVertex2f(0.12f,0.0f); glVertex2f(0.0f,0.08f);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.12f,0.0f); glVertex2f(0.12f,0.0f); glVertex2f(0.0f,0.08f);
    glEnd();

    glColor3f(0.0f, 0.40f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.09f,0.05f); glVertex2f(0.09f,0.05f); glVertex2f(0.0f,0.14f);
    glEnd();
    glColor3f(0.0f, 0.55f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.09f,0.05f); glVertex2f(0.09f,0.05f); glVertex2f(0.0f,0.14f);
    glEnd();

    glColor3f(0.0f, 0.45f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.05f,0.11f); glVertex2f(0.05f,0.11f); glVertex2f(0.0f,0.19f);
    glEnd();
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.05f,0.11f); glVertex2f(0.05f,0.11f); glVertex2f(0.0f,0.19f);
    glEnd();
}

void teaSun() {
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(0.4f, 0.85f, 0.0f);
    drawCircle(0.1f);
    glLoadIdentity();
}

void teaRoad() {
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.26f, 0.13f);
    glVertex2f( 1.0f, -0.4f); glVertex2f( 1.0f, -0.7f);
    glVertex2f( 0.2f, -1.0f); glVertex2f(-0.4f, -1.0f);
    glEnd();
}

void teaSurface() {
    glBegin(GL_QUADS);
    glColor3f(0.30f, 0.32f, 0.20f);
    glVertex2f( 1.0f,  0.0f); glVertex2f( 1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f); glVertex2f(-1.0f,  0.0f);
    glEnd();
}

void displayTeaGarden() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    teaSky();
    if (!isRaining) {
        teaSun();
        glColor3f(1.0f, 1.0f, 1.0f);
        renderClouds();
    } else {
        drawteaRain();
    }

    teaHill();
    teaSurface();
    teaRoad();

    // col 11
    glPushMatrix(); glTranslatef(0.9f,-0.02f,0.0f); glScalef(0.40f,0.40f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.98f,-0.05f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    // col 10
    glPushMatrix(); glTranslatef(0.75f,-0.02f,0.0f); glScalef(0.43f,0.43f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.8f,-0.05f,0.0f); glScalef(0.43f,0.43f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.88f,-0.1f,0.0f); glScalef(0.48f,0.48f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.94f,-0.15f,0.0f); glScalef(0.5f,0.5f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.98f,-0.2f,0.0f); glScalef(0.52f,0.52f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(1.0f,-0.24f,0.0f); glScalef(0.52f,0.52f,0.0f); drawTeaTree(); glPopMatrix();
    // col 9
    glPushMatrix(); glTranslatef(0.6f,-0.02f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.7f,-0.05f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.75f,-0.1f,0.0f); glScalef(0.48f,0.48f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.82f,-0.18f,0.0f); glScalef(0.49f,0.49f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.9f,-0.24f,0.0f); glScalef(0.5f,0.5f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.94f,-0.29f,0.0f); glScalef(0.52f,0.52f,0.0f); drawTeaTree(); glPopMatrix();
    // col 8
    glPushMatrix(); glTranslatef(0.45f,-0.02f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.52f,-0.05f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.62f,-0.12f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.68f,-0.19f,0.0f); glScalef(0.48f,0.48f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.75f,-0.25f,0.0f); glScalef(0.5f,0.5f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.83f,-0.32f,0.0f); glScalef(0.52f,0.52f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.9f,-0.38f,0.0f); glScalef(0.55f,0.55f,0.0f); drawTeaTree(); glPopMatrix();
    // big tree
    glPushMatrix(); glTranslatef(0.4f,0.1f,0.0f); glScalef(0.7f,1.3f,0.0f); drawBigTree(); glPopMatrix();
    // col 7
    glPushMatrix(); glTranslatef(0.29f,-0.02f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.38f,-0.08f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.46f,-0.14f,0.0f); glScalef(0.48f,0.48f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.52f,-0.2f,0.0f); glScalef(0.5f,0.5f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.62f,-0.28f,0.0f); glScalef(0.54f,0.54f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.68f,-0.34f,0.0f); glScalef(0.58f,0.58f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.73f,-0.4f,0.0f); glScalef(0.6f,0.6f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.78f,-0.43f,0.0f); glScalef(0.62f,0.62f,0.0f); drawTeaTree(); glPopMatrix();
    // col 6
    glPushMatrix(); glTranslatef(0.15f,-0.05f,0.0f); glScalef(0.45f,0.45f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.2f,-0.1f,0.0f); glScalef(0.5f,0.5f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.28f,-0.15f,0.0f); glScalef(0.5f,0.5f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.32f,-0.2f,0.0f); glScalef(0.55f,0.55f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.38f,-0.25f,0.0f); glScalef(0.58f,0.58f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.44f,-0.3f,0.0f); glScalef(0.6f,0.6f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.50f,-0.38f,0.0f); glScalef(0.65f,0.65f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.55f,-0.44f,0.0f); glScalef(0.68f,0.68f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.62f,-0.5f,0.0f); glScalef(0.7f,0.7f,0.0f); drawTeaTree(); glPopMatrix();
    // col 5
    glPushMatrix(); glTranslatef(-0.05f,-0.02f,0.0f); glScalef(0.50f,0.50f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.03f,-0.05f,0.0f); glScalef(0.53f,0.53f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.01f,-0.1f,0.0f); glScalef(0.55f,0.55f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.1f,-0.2f,0.0f); glScalef(0.65f,0.65f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.18f,-0.25f,0.0f); glScalef(0.68f,0.68f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.22f,-0.3f,0.0f); glScalef(0.70f,0.70f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.32f,-0.4f,0.0f); glScalef(0.75f,0.75f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.4f,-0.5f,0.0f); glScalef(0.8f,0.8f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.45f,-0.56f,0.0f); glScalef(0.8f,0.8f,0.0f); drawTeaTree(); glPopMatrix();
    // big tree 3
    glPushMatrix(); glTranslatef(-0.3f,-0.01f,0.0f); glScalef(1.0f,1.5f,0.0f); drawBigTree(); glPopMatrix();
    // col 4
    glPushMatrix(); glTranslatef(-0.25f,-0.02f,0.0f); glScalef(0.53f,0.53f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.18f,-0.1f,0.0f); glScalef(0.57f,0.57f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.12f,-0.2f,0.0f); glScalef(0.68f,0.68f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.04f,-0.3f,0.0f); glScalef(0.73f,0.73f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.06f,-0.4f,0.0f); glScalef(0.85f,0.85f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.15f,-0.5f,0.0f); glScalef(0.9f,0.9f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.23f,-0.6f,0.0f); glScalef(1.0f,1.0f,0.0f); drawTeaTree(); glPopMatrix();
    // col 3
    glPushMatrix(); glTranslatef(-0.48f,-0.05f,0.0f); glScalef(0.68f,0.68f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.44f,-0.1f,0.0f); glScalef(0.7f,0.7f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.40f,-0.2f,0.0f); glScalef(0.8f,0.8f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.33f,-0.3f,0.0f); glScalef(0.83f,0.83f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.25f,-0.4f,0.0f); glScalef(0.85f,0.85f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.2f,-0.5f,0.0f); glScalef(1.0f,1.0f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.15f,-0.6f,0.0f); glScalef(1.1f,1.1f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.05f,-0.7f,0.0f); glScalef(1.2f,1.2f,0.0f); drawTeaTree(); glPopMatrix();
    // col 2
    glPushMatrix(); glTranslatef(-0.72f,-0.05f,0.0f); glScalef(0.68f,0.68f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.69f,-0.1f,0.0f); glScalef(0.7f,0.7f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.65f,-0.2f,0.0f); glScalef(0.8f,0.8f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.63f,-0.3f,0.0f); glScalef(0.83f,0.83f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.58f,-0.4f,0.0f); glScalef(0.85f,0.85f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.52f,-0.5f,0.0f); glScalef(1.0f,1.0f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.50f,-0.6f,0.0f); glScalef(1.1f,1.1f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.48f,-0.7f,0.0f); glScalef(1.2f,1.2f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.44f,-0.8f,0.0f); glScalef(1.3f,1.3f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.42f,-0.85f,0.0f); glScalef(1.32f,1.32f,0.0f); drawTeaTree(); glPopMatrix();
    // col 1
    glPushMatrix(); glTranslatef(-1.0f,-0.05f,0.0f); glScalef(0.68f,0.68f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.98f,-0.1f,0.0f); glScalef(0.7f,0.7f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.96f,-0.2f,0.0f); glScalef(0.8f,0.8f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.93f,-0.3f,0.0f); glScalef(0.83f,0.83f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.91f,-0.4f,0.0f); glScalef(0.85f,0.85f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.9f,-0.5f,0.0f); glScalef(1.0f,1.0f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.88f,-0.6f,0.0f); glScalef(1.1f,1.1f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.87f,-0.7f,0.0f); glScalef(1.2f,1.2f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.86f,-0.8f,0.0f); glScalef(1.3f,1.3f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.84f,-0.88f,0.0f); glScalef(1.35f,1.35f,0.0f); drawTeaTree(); glPopMatrix();
    // big tree 1 road side
    glPushMatrix(); glTranslatef(0.8f,-0.4f,0.0f); glScalef(3.0f,5.0f,0.0f); drawBigTree(); glPopMatrix();
    // road trees
    glPushMatrix(); glTranslatef(0.9f,-0.75f,0.0f); glScalef(1.2f,1.2f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.5f,-0.95f,0.0f); glScalef(1.4f,1.4f,0.0f); drawTeaTree(); glPopMatrix();
    glPushMatrix(); glTranslatef(0.85f,-0.95f,0.0f); glScalef(1.4f,1.4f,0.0f); drawTeaTree(); glPopMatrix();
    // big tree 2
    glPushMatrix(); glTranslatef(-0.78f,-0.1f,0.0f); glScalef(1.0f,2.0f,0.0f); drawBigTree(); glPopMatrix();

    drawteaRain();

    if (!isRaining && isStarted) {
        glPushMatrix();
        glTranslatef(birdPos, 0.85f, 0.0f);
        glScalef(birdScale, birdScale, 1.0f);
        teaBirdsShape();
        glPopMatrix();
    }

    glFlush();
}


// _________________________________________________________________________________

//  JAFLONG – global variables & functions  (Scene 4)
GLfloat boatMove  = 0.0f;
GLfloat birdMove  = 0.0f;
GLfloat cloudMove = 0.0f;
GLfloat waveMove  = 0.0f;
GLfloat jaf_speed = 0.01f;
GLfloat sunAngle  = 0.0f;

bool    isNight   = false;
GLfloat sunY      = 0.72f;
GLfloat moonY_J   = 1.25f;
bool    cloudRun  = true;

void drawText(float x, float y, const char *str) {
    glRasterPos2f(x, y);
    for (int i = 0; str[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}

void drawSun_J() {
    glColor3f(1.0f, 0.85f, 0.0f);
    ellipse(-0.80f, sunY, 0.08f, 0.08f);

    glPushMatrix();
    glTranslatef(-0.80f, sunY, 0.0f);
    glRotatef(sunAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.80f, -sunY, 0.0f);

    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.85f, 0.0f);
    glVertex2f(-0.80f, sunY+0.14f); glVertex2f(-0.80f, sunY+0.26f);
    glVertex2f(-0.80f, sunY-0.14f); glVertex2f(-0.80f, sunY-0.24f);
    glVertex2f(-0.94f, sunY);       glVertex2f(-1.00f, sunY);
    glVertex2f(-0.66f, sunY);       glVertex2f(-0.58f, sunY);
    glVertex2f(-0.90f, sunY+0.10f); glVertex2f(-0.96f, sunY+0.18f);
    glVertex2f(-0.70f, sunY+0.10f); glVertex2f(-0.64f, sunY+0.18f);
    glVertex2f(-0.90f, sunY-0.10f); glVertex2f(-0.96f, sunY-0.18f);
    glVertex2f(-0.70f, sunY-0.10f); glVertex2f(-0.64f, sunY-0.18f);
    glEnd();
    glPopMatrix();
}

void drawMoon_J() {
    glColor3f(0.95f, 0.95f, 0.85f);
    ellipse(-0.80f, moonY_J, 0.08f, 0.08f);
}

void drawStars_J() {
    if (isNight) {
        glPointSize(4);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(-0.50f, 0.82f); glVertex2f(-0.25f, 0.65f);
        glVertex2f( 0.05f, 0.78f); glVertex2f( 0.35f, 0.63f);
        glVertex2f( 0.60f, 0.85f); glVertex2f( 0.85f, 0.70f);
        glVertex2f( 0.15f, 0.90f); glVertex2f(-0.65f, 0.55f);
        glEnd();
    }
}

void drawClouds_J() {
    glColor3f(1.0f, 1.0f, 1.0f);
    ellipse(-0.45f, 0.58f, 0.07f, 0.04f); ellipse(-0.38f, 0.61f, 0.08f, 0.05f);
    ellipse(-0.30f, 0.58f, 0.07f, 0.04f); ellipse(-0.23f, 0.57f, 0.04f, 0.03f);
    ellipse( 0.62f, 0.70f, 0.08f, 0.05f); ellipse( 0.70f, 0.73f, 0.09f, 0.06f);
    ellipse( 0.80f, 0.70f, 0.08f, 0.05f); ellipse( 0.88f, 0.68f, 0.04f, 0.03f);
}

void drawBirds_J() {
    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f( 0.08f, 0.62f); glVertex2f( 0.13f, 0.58f);
    glVertex2f( 0.13f, 0.58f); glVertex2f( 0.18f, 0.62f);
    glVertex2f(-0.08f, 0.50f); glVertex2f(-0.03f, 0.46f);
    glVertex2f(-0.03f, 0.46f); glVertex2f( 0.02f, 0.50f);
    glVertex2f( 0.28f, 0.53f); glVertex2f( 0.33f, 0.49f);
    glVertex2f( 0.33f, 0.49f); glVertex2f( 0.38f, 0.53f);
    glEnd();
}

void drawMountains() {
    glBegin(GL_TRIANGLES);
    glColor3f(0.28f, 0.58f, 0.36f);
    glVertex2f(-1.0f, 0.05f); glVertex2f(-0.62f, 0.48f); glVertex2f(-0.22f, 0.05f);
    glColor3f(0.34f, 0.66f, 0.43f);
    glVertex2f(-0.75f, 0.05f); glVertex2f(-0.35f, 0.45f); glVertex2f( 0.12f, 0.05f);
    glColor3f(0.26f, 0.56f, 0.36f);
    glVertex2f( 0.00f, 0.05f); glVertex2f( 0.48f, 0.50f); glVertex2f( 0.95f, 0.05f);
    glColor3f(0.36f, 0.68f, 0.45f);
    glVertex2f( 0.30f, 0.05f); glVertex2f( 0.86f, 0.58f); glVertex2f( 1.18f, 0.05f);
    glEnd();
}

void drawBushLine() {
    glColor3f(0.08f, 0.40f, 0.12f);
    for (float x = -1.0f; x <= 1.05f; x += 0.07f)
        ellipse(x, 0.08f, 0.045f, 0.035f);
}

void drawLand() {
    glBegin(GL_QUADS);
    glColor3f(0.35f, 0.70f, 0.10f);
    glVertex2f(-1.0f, 0.05f); glVertex2f(1.0f, 0.05f);
    glVertex2f( 1.0f,-0.05f); glVertex2f(-1.0f,-0.05f);
    glEnd();
}

void drawRiver() {
    glBegin(GL_QUADS);
    glColor3f(0.04f, 0.55f, 0.90f);
    glVertex2f(-1.0f,-0.05f); glVertex2f(1.0f,-0.05f);
    glColor3f(0.02f, 0.40f, 0.75f);
    glVertex2f( 1.0f,-0.70f); glVertex2f(-1.0f,-0.70f);
    glEnd();
}

void drawSand() {
    glBegin(GL_QUADS);
    glColor3f(0.78f, 0.65f, 0.42f);
    glVertex2f(-1.0f,-0.70f); glVertex2f(1.0f,-0.70f);
    glVertex2f( 1.0f,-1.0f);  glVertex2f(-1.0f,-1.0f);
    glEnd();
}

void drawWaterWaves() {
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.80f, 0.95f, 1.0f);
    glVertex2f(-0.90f+waveMove,-0.35f); glVertex2f(-0.75f+waveMove,-0.35f);
    glVertex2f(-0.62f+waveMove,-0.42f); glVertex2f(-0.45f+waveMove,-0.42f);
    glVertex2f(-0.48f+waveMove,-0.25f); glVertex2f(-0.30f+waveMove,-0.25f);
    glVertex2f(-0.10f+waveMove,-0.17f); glVertex2f( 0.08f+waveMove,-0.17f);
    glVertex2f( 0.16f+waveMove,-0.23f); glVertex2f( 0.34f+waveMove,-0.23f);
    glVertex2f( 0.45f+waveMove,-0.38f); glVertex2f( 0.63f+waveMove,-0.38f);
    glVertex2f( 0.70f+waveMove,-0.55f); glVertex2f( 0.88f+waveMove,-0.55f);
    glEnd();
}

void drawWaterStones() {
    glColor3f(0.75f, 0.72f, 0.65f);
    ellipse(-0.75f,-0.25f,0.04f,0.02f); ellipse(-0.60f,-0.30f,0.03f,0.015f);
    ellipse(-0.45f,-0.22f,0.035f,0.018f); ellipse(-0.10f,-0.28f,0.03f,0.015f);
    ellipse( 0.10f,-0.20f,0.035f,0.02f); ellipse( 0.45f,-0.30f,0.04f,0.02f);
    ellipse( 0.60f,-0.25f,0.035f,0.018f); ellipse( 0.80f,-0.32f,0.03f,0.015f);
    glColor3f(0.55f, 0.50f, 0.45f);
    ellipse(-0.30f,-0.35f,0.03f,0.015f); ellipse(0.25f,-0.33f,0.035f,0.018f);
}

void drawRiverSideRocks() {
    glColor3f(0.55f, 0.53f, 0.45f);
    ellipse(-0.95f,-0.10f,0.06f,0.035f); ellipse(-0.85f,-0.08f,0.08f,0.045f);
    ellipse(-0.73f,-0.07f,0.08f,0.045f); ellipse(-0.61f,-0.06f,0.07f,0.040f);
    ellipse(-0.50f,-0.05f,0.06f,0.035f); ellipse(-0.40f,-0.04f,0.055f,0.030f);
    ellipse( 0.62f,-0.10f,0.07f,0.040f); ellipse( 0.72f,-0.12f,0.08f,0.045f);
    ellipse( 0.84f,-0.14f,0.08f,0.045f); ellipse( 0.95f,-0.16f,0.08f,0.045f);
}

void drawBoat() {
    glColor3f(0.0f, 0.18f, 0.35f);
    ellipse(0.08f,-0.455f,0.26f,0.045f);
    glBegin(GL_POLYGON);
    glColor3f(0.40f, 0.22f, 0.05f);
    glVertex2f(-0.22f,-0.35f); glVertex2f( 0.38f,-0.35f);
    glVertex2f( 0.31f,-0.42f); glVertex2f(-0.15f,-0.42f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.70f, 0.45f, 0.12f);
    glVertex2f(0.05f,-0.32f); glVertex2f(0.23f,-0.32f);
    glVertex2f(0.23f,-0.38f); glVertex2f(0.05f,-0.38f);
    glEnd();
}

void drawMan() {
    glColor3f(1.0f, 0.20f, 0.10f);
    ellipse(-0.09f,-0.30f,0.035f,0.07f);
    glColor3f(0.95f, 0.72f, 0.45f);
    ellipse(-0.09f,-0.20f,0.035f,0.035f);
    glBegin(GL_TRIANGLES);
    glColor3f(0.95f, 0.70f, 0.20f);
    glVertex2f(-0.16f,-0.18f); glVertex2f(-0.02f,-0.18f); glVertex2f(-0.09f,-0.10f);
    glEnd();
    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.25f, 0.12f, 0.02f);
    glVertex2f(-0.03f,-0.22f); glVertex2f(-0.12f,-0.47f);
    glEnd();
}

void drawTree_J() {
    glBegin(GL_QUADS);
    glColor3f(0.45f, 0.22f, 0.05f);
    glVertex2f(0.77f,-0.05f); glVertex2f(0.84f,-0.05f);
    glVertex2f(0.84f, 0.27f); glVertex2f(0.77f, 0.27f);
    glEnd();
    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.28f, 0.12f, 0.02f);
    glVertex2f(0.805f,0.15f); glVertex2f(0.70f,0.25f);
    glVertex2f(0.805f,0.15f); glVertex2f(0.91f,0.25f);
    glVertex2f(0.805f,0.18f); glVertex2f(0.805f,0.35f);
    glEnd();
    glColor3f(0.24f, 0.64f, 0.02f);
    ellipse(0.68f,0.25f,0.11f,0.08f); ellipse(0.78f,0.34f,0.13f,0.09f);
    ellipse(0.90f,0.30f,0.12f,0.08f); ellipse(0.83f,0.22f,0.14f,0.08f);
    ellipse(0.72f,0.18f,0.10f,0.07f); ellipse(0.94f,0.18f,0.09f,0.06f);
}

void drawSignboard() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.55f,0.00f); glVertex2f(0.73f,0.00f);
    glVertex2f(0.73f,0.11f); glVertex2f(0.55f,0.11f);
    glEnd();
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.55f,0.00f); glVertex2f(0.73f,0.00f);
    glVertex2f(0.73f,0.11f); glVertex2f(0.55f,0.11f);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(0.565f, 0.040f, "JAFLONG");
    glBegin(GL_LINES);
    glVertex2f(0.58f,0.00f); glVertex2f(0.58f,-0.08f);
    glVertex2f(0.70f,0.00f); glVertex2f(0.70f,-0.08f);
    glEnd();
}

void drawForegroundRocks() {
    glColor3f(0.52f, 0.50f, 0.43f);
    ellipse(-0.88f,-0.82f,0.17f,0.12f); ellipse(-0.66f,-0.80f,0.14f,0.10f);
    ellipse(-0.54f,-0.90f,0.10f,0.06f); ellipse( 0.62f,-0.82f,0.15f,0.11f);
    ellipse( 0.82f,-0.80f,0.17f,0.12f); ellipse( 0.95f,-0.90f,0.10f,0.06f);
    glColor3f(0.65f, 0.62f, 0.55f);
    ellipse(-0.78f,-0.93f,0.12f,0.06f); ellipse(-0.38f,-0.91f,0.09f,0.05f);
    ellipse( 0.42f,-0.91f,0.09f,0.05f); ellipse( 0.76f,-0.93f,0.12f,0.06f);
}

void drawGrass_J() {
    glBegin(GL_TRIANGLES);
    glColor3f(0.10f, 0.55f, 0.05f);
    glVertex2f(-0.95f,-0.95f); glVertex2f(-0.90f,-0.77f); glVertex2f(-0.85f,-0.95f);
    glVertex2f(-0.82f,-0.95f); glVertex2f(-0.76f,-0.80f); glVertex2f(-0.70f,-0.95f);
    glVertex2f( 0.80f,-0.95f); glVertex2f( 0.86f,-0.77f); glVertex2f( 0.92f,-0.95f);
    glEnd();
}

void updateJaflong(int value) {
    if (boatMove > 1.3f)  boatMove = -1.3f;
    boatMove += jaf_speed;

    if (birdMove > 1.4f)  birdMove = -1.4f;
    birdMove += 0.015f;

    if (cloudRun) {
        if (cloudMove > 1.6f) cloudMove = -1.6f;
        cloudMove += 0.006f;
    }

    if (waveMove > 0.20f) waveMove = 0.0f;
    waveMove += 0.01f;

    sunAngle += 2.0f;
    if (sunAngle > 360.0f) sunAngle = 0.0f;

    if (isNight) {
        if (sunY   > -0.20f)  sunY   -= 0.01f;
        if (moonY_J >  0.72f) moonY_J -= 0.01f;
    } else {
        if (sunY   <  0.72f)  sunY   += 0.01f;
        if (moonY_J <  1.25f) moonY_J += 0.01f;
    }

    glutPostRedisplay();
    glutTimerFunc(100, updateJaflong, 0);
}

void displayJaflong() {
    if (isNight) glClearColor(0.02f, 0.05f, 0.18f, 1.0f);
    else         glClearColor(0.45f, 0.78f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSun_J();
    drawMoon_J();
    drawStars_J();

    glPushMatrix();
    glTranslatef(cloudMove, 0.0f, 0.0f);
    drawClouds_J();
    glPopMatrix();

    if (!isNight) {
        glPushMatrix();
        glTranslatef(birdMove, 0.0f, 0.0f);
        drawBirds_J();
        glPopMatrix();
    }

    drawMountains();
    drawBushLine();
    drawLand();
    drawRiver();
    drawSand();
    drawWaterWaves();
    drawWaterStones();
    drawRiverSideRocks();

    glPushMatrix();
    glTranslatef(boatMove, 0.0f, 0.0f);
    drawBoat();
    drawMan();
    glPopMatrix();

    drawTree_J();
    drawSignboard();
    drawForegroundRocks();
    drawGrass_J();

    glFlush();
}


// _________________________________________________________________________
//  UTMACHORA WATERFALL – global variables & functions  (Scene 5)

bool ut_isNightMode = false;

GLfloat ut_carPosition = -0.8f;
GLfloat ut_carSpeed = 0.01f;
bool ut_carMoving = true;

GLfloat ut_moonY = -0.2f;
GLfloat ut_moonTargetY  = 0.82f;
bool ut_moonRising = false;

GLfloat ut_cloudX = -0.5375f;
GLfloat ut_cloudSpeed = 0.005f;
bool ut_cloudMoving = true;

GLfloat ut_dropX[] = {-0.05f, 0.05f, -0.10f, 0.10f};
GLfloat ut_dropY[] = {0.60f, 0.60f,  0.60f, 0.60f};
GLfloat ut_dropSpeed = 0.02f;

GLfloat ut_birdX[] = {-0.80f, -0.40f, 0.10f};
GLfloat ut_birdY[] = {0.88f,  0.93f, 0.85f};
GLfloat ut_birdSpeed = 0.004f;

void ut_sky()
{
    if(ut_isNightMode)
    {
        glColor3f(0.05f, 0.05f, 0.18f);
    }
    else
    {
        glColor3f(0.53f, 0.81f, 0.92f);
    }

    glBegin(GL_QUADS);
    glVertex2f(-1.00f, 0.25f);
    glVertex2f(1.00f, 0.25f);
    glVertex2f(1.00f, 1.00f);
    glVertex2f(-1.00f, 1.00f);
    glEnd();
}

void ut_sun()
{
    if (ut_isNightMode)
    {
        return;
    }

    glColor3f(1.0f, 0.85f, 0.0f);
    circle(-0.7812f, 0.8125f, 0.0875f, 40);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
    glVertex2f(-0.7812f, 0.9458f);
    glVertex2f(-0.7812f, 0.9917f);
    glVertex2f(-0.7812f, 0.6792f);
    glVertex2f(-0.7812f, 0.6333f);
    glVertex2f(-0.8812f, 0.8125f);
    glVertex2f(-0.9156f, 0.8125f);
    glVertex2f(-0.6813f, 0.8125f);
    glVertex2f(-0.6469f, 0.8125f);
    glVertex2f(-0.8438f, 0.8958f);
    glVertex2f(-0.8688f, 0.9292f);
    glVertex2f(-0.7188f, 0.8958f);
    glVertex2f(-0.6937f, 0.9292f);
    glVertex2f(-0.8438f, 0.7292f);
    glVertex2f(-0.8688f, 0.6958f);
    glVertex2f(-0.7188f, 0.7292f);
    glVertex2f(-0.6937f, 0.6958f);
    glEnd();
}

void ut_moon()
{
    if(!ut_isNightMode)
    {
        return;
    }

    glColor3f(0.95f, 0.95f, 0.80f);
    circle(0.7812f, ut_moonY, 0.0875f, 40);
    glColor3f(0.80f, 0.80f, 0.65f);
    circle(0.745f, ut_moonY + 0.025f, 0.018f, 20);
    circle(0.810f, ut_moonY - 0.020f, 0.013f, 20);
    circle(0.770f, ut_moonY - 0.035f, 0.010f, 20);
    if(ut_moonY > 0.50f)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        circle(-0.60f,0.90f,0.012f,12);
        circle(-0.20f,0.95f,0.009f,12);
        circle(0.10f,0.88f,0.011f,12);
        circle(0.40f,0.93f,0.008f,12);
        circle(0.20f,0.72f,0.010f,12);
        circle(-0.50f,0.75f,0.009f,12);
        circle(0.55f,0.78f,0.011f,12);
        circle(-0.85f,0.65f,0.008f,12);
        circle(0.90f,0.62f,0.009f,12);
        circle(-0.30f,0.82f,0.007f,12);
    }
}

void ut_cloud(float cx, float cy)
{
    if(ut_isNightMode)
    {
        glColor3f(0.55f, 0.55f, 0.62f);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    circle(cx, cy, 0.0688f, 30);
    circle(cx + 0.0781f, cy, 0.0875f, 30);
    circle(cx + 0.1656f, cy, 0.0688f, 30);
    circle(cx + 0.0406f, cy+0.0667f, 0.0688f, 30);
    circle(cx + 0.1156f, cy + 0.0750f, 0.0688f, 30);
}

void ut_leftCliff()
{
    glColor3f(0.50f, 0.38f, 0.28f);
    glBegin(GL_QUADS);

    glVertex2f(-1.0000f,0.0625f);
    glVertex2f(-0.0938f,0.0625f);
    glVertex2f(-0.0938f,0.6667f);
    glVertex2f(-1.0000f,0.6667f);
    glEnd();

    glColor3f(0.38f, 0.28f, 0.18f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    glVertex2f(-0.7812f,0.6667f);
    glVertex2f(-0.7344f,0.0625f);
    glVertex2f(-0.5625f,0.6667f);
    glVertex2f(-0.5156f,0.0625f);
    glVertex2f(-0.3594f,0.6667f);
    glVertex2f(-0.3063f,0.0625f);
    glVertex2f(-0.1937f,0.6667f);
    glVertex2f(-0.1500f,0.0625f);
    glEnd();

    glColor3f(0.15f, 0.55f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0000f,0.6250f);
    glVertex2f(-0.0938f,0.6250f);
    glVertex2f(-0.0938f,0.7500f);
    glVertex2f(-1.0000f,0.7500f);
    glEnd();
}

void ut_rightCliff()
{
    glColor3f(0.50f, 0.38f, 0.28f);
    glBegin(GL_QUADS);

    glVertex2f(0.0938f,0.0625f);
    glVertex2f(1.0000f,0.0625f);
    glVertex2f(1.0000f,0.6667f);
    glVertex2f(0.0938f,0.6667f);
    glEnd();

    glColor3f(0.38f, 0.28f, 0.18f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    glVertex2f(0.1938f,0.6667f);
    glVertex2f(0.1500f,0.0625f);
    glVertex2f(0.3906f,0.6667f);
    glVertex2f(0.3375f,0.0625f);
    glVertex2f(0.5781f,0.6667f);
    glVertex2f(0.5156f,0.0625f);
    glVertex2f(0.7750f,0.6667f);
    glVertex2f(0.7125f,0.0625f);
    glEnd();

    glColor3f(0.15f, 0.55f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(0.0938f,0.6250f);
    glVertex2f(1.0000f,0.6250f);
    glVertex2f(1.0000f,0.7500f);
    glVertex2f(0.0938f,0.7500f);

    glEnd();
}

void ut_waterfallBody()
{
    glColor3f(0.72f, 0.92f, 0.97f);
    glBegin(GL_QUADS);
    glVertex2f(-0.0938f,0.0625f);
    glVertex2f(0.0938f,0.0625f);
    glVertex2f(0.0938f,0.6667f);
    glVertex2f(-0.0938f,0.6667f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.0687f,0.6667f);
    glVertex2f(-0.0781f,0.2292f);
    glVertex2f(-0.0375f,0.6667f);
    glVertex2f(-0.0312f,0.2083f);
    glVertex2f(0.0000f,0.6667f);
    glVertex2f(0.0000f,0.1875f);
    glVertex2f(0.0375f,0.6667f);
    glVertex2f(0.0312f,0.2083f);
    glVertex2f(0.0688f,0.6667f);
    glVertex2f(0.0781f,0.2292f);
    glEnd();
}

void ut_waterfallSplash()
{
    glColor3f(0.72f, 0.92f, 0.97f);

    circle(0.0000f, 0.0750f, 0.0875f, 30);
    circle(-0.0563f, 0.0917f, 0.0500f, 30);
    circle(0.0562f, 0.0917f, 0.0500f, 30);
    glColor3f(1.0f, 1.0f, 1.0f);
    circle(0.0000f, 0.0958f, 0.0563f, 30);
    circle(-0.0375f, 0.1042f, 0.0313f, 30);
    circle(0.0375f, 0.1042f, 0.0313f, 30);
}

void ut_waterDrop(GLfloat x, GLfloat y)
{
    glColor3f(0.72f, 0.92f, 0.97f);
    circle(x, y, 0.025f, 20);
}

void ut_waterfallWithDrops()
{
    ut_waterfallBody();
    ut_waterfallSplash();
    for(int i = 0; i < 4; i++)
    {
       ut_waterDrop(ut_dropX[i], ut_dropY[i]);
    }

}

void ut_river()
{
    if(ut_isNightMode)
    {
        glColor3f(0.10f, 0.30f, 0.55f);
    }
    else
    {
        glColor3f(0.20f, 0.55f, 0.85f);
    }

    glBegin(GL_POLYGON);
    glVertex2f(-1.0000f,-0.2917f);
    glVertex2f(1.0000f,-0.2917f);
    glVertex2f(1.0000f,-0.0417f);
    glVertex2f(0.5625f, 0.0208f);
    glVertex2f(0.0000f, 0.0500f);
    glVertex2f(-0.5625f, 0.0208f);
    glVertex2f(-1.0000f,-0.0417f);
    glEnd();

    glColor3f(0.8f, 0.95f, 1.0f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    glVertex2f(-0.7500f,-0.1250f);
    glVertex2f(-0.5312f,-0.1250f);
    glVertex2f(-0.6875f,-0.1875f);
    glVertex2f(-0.4688f,-0.1875f);
    glVertex2f(0.4375f,-0.1250f);
    glVertex2f(0.6875f,-0.1250f);
    glVertex2f(0.4062f,-0.1875f);
    glVertex2f(0.6250f,-0.1875f);
    glVertex2f(-0.2812f,-0.1458f);
    glVertex2f(-0.0625f,-0.1458f);
    glVertex2f(0.0625f,-0.1458f);
    glVertex2f(0.2812f,-0.1458f);
    glVertex2f(-0.2188f,-0.2167f);
    glVertex2f(-0.0312f,-0.2167f);
    glVertex2f(0.0312f,-0.2167f);
    glVertex2f(0.2188f,-0.2167f);
    glEnd();
}

void ut_leftGreenBank()
{
    glColor3f(0.20f, 0.65f, 0.10f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0000f,-0.2917f);
    glVertex2f(-1.0000f, 0.0625f);
    glVertex2f(-0.0938f, 0.0625f);
    glVertex2f(-0.0938f, 0.0000f);
    glVertex2f(-0.3750f,-0.0500f);
    glVertex2f(-0.6875f,-0.0833f);
    glVertex2f(-1.0000f,-0.0417f);
    glEnd();

    glColor3f(0.15f, 0.55f, 0.08f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0000f, 0.0625f);
    glVertex2f(-0.0938f, 0.0625f);
    glVertex2f(-0.0938f, 0.0333f);
    glVertex2f(-0.5312f, 0.0167f);
    glVertex2f(-1.0000f, 0.0083f);
    glEnd();

    glColor3f(0.25f, 0.70f, 0.12f);
    circle(-0.8125f, 0.0750f, 0.0625f, 20);
    circle(-0.5469f, 0.0792f, 0.0688f, 20);
    circle(-0.2656f, 0.0750f, 0.0563f, 20);
}

void ut_rightGreenBank()
{
    glColor3f(0.20f, 0.65f, 0.10f);
    glBegin(GL_POLYGON);
    glVertex2f(1.0000f,-0.2917f);
    glVertex2f(1.0000f, 0.0625f);
    glVertex2f(0.0938f, 0.0625f);
    glVertex2f(0.0938f, 0.0000f);
    glVertex2f(0.3750f,-0.0500f);
    glVertex2f(0.6875f,-0.0833f);
    glVertex2f(1.0000f,-0.0417f);
    glEnd();

    glColor3f(0.15f, 0.55f, 0.08f);
    glBegin(GL_POLYGON);
    glVertex2f(1.0000f, 0.0625f);
    glVertex2f(0.0938f, 0.0625f);
    glVertex2f(0.0938f, 0.0333f);
    glVertex2f(0.5312f, 0.0167f);
    glVertex2f(1.0000f, 0.0083f);
    glEnd();

    glColor3f(0.25f, 0.70f, 0.12f);
    circle(0.2500f, 0.0750f, 0.0625f, 20);
    circle(0.5312f, 0.0792f, 0.0688f, 20);
    circle(0.7969f, 0.0750f, 0.0563f, 20);
}

void ut_bottomGreenBanks()
{
    glColor3f(0.20f, 0.62f, 0.10f);
    glBegin(GL_POLYGON);

    glVertex2f(-1.0000f,-0.6667f);
    glVertex2f(-1.0000f,-0.2917f);
    glVertex2f(-0.5625f,-0.2917f);
    glVertex2f(-0.6875f,-0.6667f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(1.0000f,-0.6667f);
    glVertex2f(1.0000f,-0.2917f);
    glVertex2f(0.5625f,-0.2917f);
    glVertex2f(0.6875f,-0.6667f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-0.6875f,-0.6667f);
    glVertex2f(-0.5625f,-0.2917f);
    glVertex2f(0.5625f,-0.2917f);
    glVertex2f(0.6875f,-0.6667f);
    glEnd();

    glColor3f(0.15f, 0.50f, 0.08f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    glVertex2f(-1.0000f,-0.6583f);
    glVertex2f(1.0000f,-0.6583f);
    glEnd();
}

void ut_road()
{
    glColor3f(0.62f, 0.50f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-1.00f,-1.0000f);
    glVertex2f(1.00f,-1.0000f);
    glVertex2f(1.00f,-0.6667f);
    glVertex2f(-1.00f,-0.6667f);
    glEnd();

    glColor3f(0.95f, 0.90f, 0.75f);
    glLineWidth(2.5f);

    glBegin(GL_LINES);
    glVertex2f(-1.00f,-0.8333f);
    glVertex2f(-0.7812f, -0.8333f);
    glVertex2f(-0.7188f, -0.8333f);
    glVertex2f(-0.4688f, -0.8333f);
    glVertex2f(-0.4062f, -0.8333f);
    glVertex2f(-0.1562f, -0.8333f);
    glVertex2f(-0.0938f, -0.8333f);
    glVertex2f(0.1562f, -0.8333f);
    glVertex2f(0.2188f, -0.8333f);
    glVertex2f(0.4688f, -0.8333f);
    glVertex2f(0.5312f, -0.8333f);
    glVertex2f(0.7812f, -0.8333f);
    glVertex2f(0.8438f, -0.8333f);
    glVertex2f(1.0000f, -0.8333f);
    glEnd();

    glColor3f(0.45f, 0.35f, 0.22f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    glVertex2f(-1.0000f,-0.6750f);
    glVertex2f(1.0000f,-0.6750f);
    glVertex2f(-1.0000f,-0.9875f);
    glVertex2f(1.0000f,-0.9875f);
    glEnd();
}

void ut_tree(float x, float y)
{
    glColor3f(0.55f, 0.35f, 0.15f);
    glBegin(GL_QUADS);

    glVertex2f(x-0.028f,y);
    glVertex2f(x+0.028f,y);
    glVertex2f(x+0.028f,y+0.25f);
    glVertex2f(x-0.028f,y+0.25f);
    glEnd();

    glColor3f(0.40f, 0.22f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(x+0.010f,y);
    glVertex2f(x+0.028f,y);
    glVertex2f(x+0.028f,y+0.25f);
    glVertex2f(x+0.010f,y+0.25f);
    glEnd();

    glColor3f(0.55f, 0.35f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(x-0.028f,y+0.18f);
    glVertex2f(x-0.005f,y+0.18f);
    glVertex2f(x-0.075f,y+0.34f);
    glVertex2f(x-0.095f,y+0.33f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x+0.005f,y+0.18f);
    glVertex2f(x+0.028f,y+0.18f);
    glVertex2f(x+0.095f,y+0.33f);
    glVertex2f(x+0.075f,y+0.34f);
    glEnd();

    glColor3f(0.40f, 0.22f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(x+0.016f,y+0.19f);
    glVertex2f(x+0.028f,y+0.19f);
    glVertex2f(x+0.095f,y+0.33f);
    glVertex2f(x+0.085f,y+0.335f);
    glEnd();

    glColor3f(0.25f, 0.70f, 0.20f);
    circle(x-0.100f,y+0.33f,0.100f,30);
    circle(x+0.100f,y+0.33f,0.100f,30);
    circle(x,y+0.43f,0.115f,30);
    circle(x, y+0.32f,0.085f,30);
}

void ut_smallTree(float x, float y)
{
    glColor3f(0.45f, 0.28f, 0.10f);
    glBegin(GL_QUADS);

    glVertex2f(x-0.008f,y);
    glVertex2f(x+0.008f,y);
    glVertex2f(x+0.008f,y+0.13f);
    glVertex2f(x-0.008f,y+0.13f);
    glEnd();

    glColor3f(0.20f, 0.55f, 0.25f);
    circle(x-0.060f,y+0.12f,0.045f,25);
    circle(x-0.020f,y+0.13f,0.050f,25);
    circle(x+0.020f,y+0.13f,0.050f,25);
    circle(x+0.060f,y+0.12f,0.045f,25);
    circle(x-0.040f,y+0.17f,0.045f,25);
    circle(x+0.000f,y+0.18f,0.050f,25);
    circle(x+0.040f,y+0.17f,0.045f,25);
    circle(x-0.020f,y+0.21f,0.040f,25);
    circle(x+0.020f,y+0.21f,0.040f,25);
    circle(x+0.000f,y+0.235f,0.038f,25);
}

void ut_smallBush(float x, float y)
{
    glColor3f(0.20f, 0.55f, 0.25f);
    circle(x-0.045f,y, 0.035f,25);
    circle(x-0.015f,y+0.005f,0.040f,25);
    circle(x+0.015f,y+0.005f,0.040f,25);
    circle(x+0.045f,y, 0.035f,25);
    circle(x-0.020f,y+0.030f,0.035f,25);
    circle(x+0.020f,y+0.030f,0.035f,25);
    circle(x+0.000f,y+0.045f,0.030f,25);
}

void ut_stone(float x, float y, float size)
{
    glColor3f(0.55f, 0.47f, 0.35f);
    circle(x, y, size, 20);
}

void ut_bird(float x, float y)
{
    glColor3f(0.10f, 0.10f, 0.10f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x-0.060f,y);
    glVertex2f(x-0.030f,y+0.022f);
    glVertex2f(x,y);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(x,y);
    glVertex2f(x+0.030f,y+0.022f);
    glVertex2f(x+0.060f,y);
    glEnd();
}

void ut_car()
{
    glPushMatrix();
    glTranslatef(ut_carPosition, -0.80f, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.28f,0.0f);
    glVertex2f(0.28f,0.0f);
    glVertex2f(0.28f,0.13f);
    glVertex2f(-0.28f,0.13f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    glVertex2f(-0.17f,0.13f);
    glVertex2f(0.17f,0.13f);
    glVertex2f(0.17f,0.24f);
    glVertex2f(-0.17f,0.24f);
    glEnd();

    glColor3f(0.6f, 0.8f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.13f,0.14f);
    glVertex2f(0.13f,0.14f);
    glVertex2f(0.13f,0.23f);
    glVertex2f(-0.13f,0.23f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.24f,0.04f);
    glVertex2f(0.28f,0.04f);
    glVertex2f(0.28f,0.09f);
    glVertex2f(0.24f,0.09f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    circle(-0.17f,0.0f,0.065f,20);
    circle(0.17f,0.0f,0.065f,20);
    glColor3f(1.0f, 1.0f, 1.0f);
    circle(-0.17f,0.0f,0.028f,20);
    circle(0.17f,0.0f,0.028f,20);

    glPopMatrix();
}

void ut_bench()
{
    glColor3f(0.55f, 0.35f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.12f,-0.35f);
    glVertex2f(-0.10f,-0.35f);
    glVertex2f(-0.10f,-0.25f);
    glVertex2f(-0.12f,-0.25f);
    glVertex2f(0.10f,-0.35f);
    glVertex2f(0.12f,-0.35f);
    glVertex2f(0.12f,-0.25f);
    glVertex2f(0.10f,-0.25f);
    glVertex2f(-0.12f,-0.45f);
    glVertex2f(-0.10f,-0.45f);
    glVertex2f(-0.10f,-0.35f);
    glVertex2f(-0.12f,-0.35f);
    glVertex2f(0.10f,-0.45f);
    glVertex2f(0.12f,-0.45f);
    glVertex2f(0.12f,-0.35f);
    glVertex2f(0.10f,-0.35f);
    glEnd();


    glColor3f(0.65f, 0.45f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.13f,-0.35f);
    glVertex2f(0.13f,-0.35f);
    glVertex2f(0.13f,-0.30f);
    glVertex2f(-0.13f,-0.30f);
    glEnd();

    glColor3f(0.65f, 0.45f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.13f,-0.25f);
    glVertex2f(0.13f,-0.25f);
    glVertex2f(0.13f,-0.18f);
    glVertex2f(-0.13f,-0.18f);
    glEnd();

    glColor3f(0.40f, 0.25f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(0.855f,-0.42f);
    glVertex2f(0.875f,-0.42f);
    glVertex2f(0.875f,-0.30f);
    glVertex2f(0.855f,-0.30f);
    glEnd();

    glColor3f(0.85f, 0.70f, 0.45f);
    glBegin(GL_QUADS);
    glVertex2f(0.78f,-0.30f);
    glVertex2f(0.95f,-0.30f);
    glVertex2f(0.95f,-0.20f);
    glVertex2f(0.78f,-0.20f);
    glEnd();

    glColor3f(0.40f, 0.25f, 0.10f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.78f,-0.30f);
    glVertex2f(0.95f,-0.30f);
    glVertex2f(0.95f,-0.20f);
    glVertex2f(0.78f,-0.20f);
    glEnd();

    glColor3f(0.20f, 0.10f, 0.05f);
    glRasterPos2f(0.79f,-0.235f);
    const char* line1 = "Utmachora";
    for(int i = 0; line1[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, line1[i]);
    }

    glRasterPos2f(0.81f,-0.275f);
    const char* line2 = "Jhorna";
    for(int i = 0; line2[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, line2[i]);
    }
}

void ut_drawBirds()
{
    if(ut_isNightMode)
    {
        return;
    }

    for(int i = 0; i < 3; i++)
    {
        ut_bird(ut_birdX[i], ut_birdY[i]); // Fixed copying bug to render birds instead of water drops
    }

}

void ut_drawInstructions()
{
    glColor3f(0.15f, 0.10f, 0.05f);
    glBegin(GL_QUADS);

    glVertex2f(-1.00f,-1.00f);
    glVertex2f(1.00f,-1.00f);
    glVertex2f( 1.00f,-0.95f);
    glVertex2f(-1.00f,-0.95f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.99f,-0.982f);
    const char* instructions = "Press -> n=Day/Night | UP/DOWN=Waterfall Speed | f=Fast Car | g=Slow Car | c=Normal Speed/continue | Space=Stop Car | Mouse Right Click=Cloud | m=Mute | u=Unmute";
    for(int i = 0; instructions[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, instructions[i]);

    }

}

void ut_updateCloud(int value)
{
    if(currentScene == 5 && ut_cloudMoving)
    {
        ut_cloudX += ut_cloudSpeed;
        if(ut_cloudX > 1.2f)
        {
            ut_cloudX = -1.3f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, ut_updateCloud, 0);
}

void ut_updateWaterDrops(int value)
{
    if(currentScene == 5)
    {
        for(int i = 0; i < 4; i++)
        {
            ut_dropY[i] -= ut_dropSpeed;
            if(ut_dropY[i] < -0.5f)
            {
                ut_dropY[i] = 0.60f;

            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, ut_updateWaterDrops, 0);
}

void ut_moonRiseUpdate(int value)
{
    if(!ut_moonRising)
    {
        return;
    }
    if(ut_moonY < ut_moonTargetY)
    {
        ut_moonY += 0.012f;
        glutPostRedisplay();
        glutTimerFunc(30, ut_moonRiseUpdate, 0);
    }
    else
    {
        ut_moonY = ut_moonTargetY;
        ut_moonRising = false;
        glutPostRedisplay();
    }
}

void ut_updateBirds(int value)
{
    if(currentScene == 5 && !ut_isNightMode)
    {
        for (int i = 0; i < 3; i++)
        {
            ut_birdX[i] += ut_birdSpeed;
            if(ut_birdX[i] > 1.2f)
            {
                ut_birdX[i] = -1.2f;
            }
        }
        glutPostRedisplay();
    }
    glutTimerFunc(30, ut_updateBirds, 0);
}

void ut_updateCar(int value)
{
    if(currentScene == 5 && ut_carMoving)
    {
        ut_carPosition += ut_carSpeed;
        if(ut_carPosition > 1.1f)
        {
            ut_carPosition = -1.1f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, ut_updateCar, 0);
}

void displayUtmachora()
{
    glClear(GL_COLOR_BUFFER_BIT);

    ut_sky();
    ut_sun();
    ut_moon();

    ut_cloud(ut_cloudX, 0.7500f);
    ut_cloud(0.4219f,0.7583f);

    ut_leftCliff();
    ut_rightCliff();
    ut_waterfallWithDrops();
    ut_leftGreenBank();
    ut_rightGreenBank();
    ut_river();
    ut_waterfallSplash();
    ut_bottomGreenBanks();
    ut_road();

    ut_tree(-0.6719f, 0.0333f);
    ut_tree(0.6406f, 0.0333f);

    ut_stone(-0.8f,-0.15f,0.05f);
    ut_stone(-0.5f,-0.30f,0.07f);
    ut_stone(0.3f,-0.35f,0.06f);
    ut_stone(0.6f,-0.25f,0.04f);

    ut_bench();
    ut_smallTree(-0.85f,-0.55f);
    ut_smallBush(0.80f,-0.43f);
    ut_smallBush(0.86f,-0.45f);
    ut_smallBush(0.92f,-0.43f);

    ut_car();
    ut_drawBirds();
    ut_drawInstructions();

    glFlush();
}


//_____________________________________________________________________

//  SYLHET CITY – global variables & functions  (Scene 6)

GLfloat cy_carPosition = 0.0f;
GLfloat cy_carSpeed = 0.008f;
GLfloat cy_birdPosition = -1.35f;
GLfloat cy_skyPosition = 0.0f;
bool cy_isCarMoving = true;
bool cy_cloudMoving = true;

GLfloat cy_clockMinuteAngle = 0.0f;
GLfloat cy_clockHourAngle = 0.0f;
GLfloat cy_clockTimer = 0.0f;

int cy_trafficLightState = 2;
float cy_trafficLightTimer = 0.0f;

void cy_Car1Move() {
    if (cy_isCarMoving) {
        cy_carPosition += cy_carSpeed;
        if (cy_carPosition > 1.95f)
            cy_carPosition = -0.45f;
    }
}

void cy_BirdMove() {
    cy_birdPosition += 0.006f;
    if (cy_birdPosition > 1.35f)
        cy_birdPosition = -1.35f;
}

void cy_SkyMove() {
    if (cy_cloudMoving) { // only translate the sky offset if active
        cy_skyPosition -= 0.0015f;
        if (cy_skyPosition < -1.0f)
            cy_skyPosition = 0.0f;
    }
}

void cy_sky() {
    glColor3f(0.76f, 0.89f, 0.93f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.10f);
    glVertex2f(1.0f, -0.10f);
    glVertex2f(1.0f,  1.00f);
    glVertex2f(-1.0f,  1.00f);
    glEnd();

    glPushMatrix();
    glTranslatef(cy_skyPosition, 0.0f, 0.0f);

    for (int pass = 0; pass < 2; pass++) {
        float shift = pass * 1.0f;

        glColor3f(0.82f, 0.93f, 0.96f);
        circle(-0.70f + shift, 0.82f, 0.055f, 24);
        circle(-0.63f + shift, 0.83f, 0.075f, 24);
        circle(-0.55f + shift, 0.82f, 0.050f, 24);

        circle(0.15f + shift, 0.72f, 0.050f, 24);
        circle(0.23f + shift, 0.73f, 0.070f, 24);
        circle(0.31f + shift, 0.72f, 0.045f, 24);

        glColor3f(0.70f, 0.86f, 0.90f);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        glVertex2f(-0.95f + shift, 0.77f); glVertex2f(-0.25f + shift, 0.72f);
        glVertex2f( 0.05f + shift, 0.64f); glVertex2f( 0.75f + shift, 0.59f);
        glEnd();
    }

    glPopMatrix();
}

void cy_powerLines() {
    glColor3f(0.66f, 0.82f, 0.86f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.00f, 0.82f); glVertex2f( 0.34f, 0.69f);
    glVertex2f(-1.00f, 0.78f); glVertex2f( 0.34f, 0.65f);
    glVertex2f(-1.00f, 0.74f); glVertex2f( 0.34f, 0.61f);
    glEnd();
}

void cy_trees() {
    glColor3f(0.04f, 0.20f, 0.06f);
    glBegin(GL_QUADS);
    glVertex2f(-1.00f, -0.10f);
    glVertex2f(0.44f, -0.10f);
    glVertex2f(0.44f,  0.31f);
    glVertex2f(-1.00f,  0.38f);
    glEnd();

    glColor3f(0.07f, 0.33f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-1.00f,  0.04f);
    glVertex2f(0.44f,  0.04f);
    glVertex2f(0.44f,  0.33f);
    glVertex2f(-1.00f,  0.36f);
    glEnd();

    glColor3f(0.08f, 0.38f, 0.11f);
    circle(-0.96f, 0.31f, 0.15f, 24); circle(-0.80f, 0.35f, 0.15f, 24);
    circle(-0.64f, 0.32f, 0.16f, 24); circle(-0.48f, 0.35f, 0.14f, 24);
    circle(-0.32f, 0.33f, 0.16f, 24); circle(-0.16f, 0.36f, 0.14f, 24);
    circle(0.00f, 0.34f, 0.15f, 24); circle( 0.16f, 0.33f, 0.13f, 24);
    circle(0.32f, 0.31f, 0.11f, 24);

    glColor3f(0.10f, 0.48f, 0.14f);
    circle(-0.90f, 0.39f, 0.09f, 20); circle(-0.70f, 0.40f, 0.08f, 20);
    circle(-0.54f, 0.38f, 0.10f, 20); circle(-0.38f, 0.40f, 0.09f, 20);
    circle(-0.22f, 0.39f, 0.10f, 20); circle(-0.06f, 0.40f, 0.09f, 20);
    circle(0.10f, 0.37f, 0.08f, 20); circle( 0.27f, 0.36f, 0.07f, 20);

    glColor3f(0.03f, 0.16f, 0.05f);
    circle(-0.96f, 0.40f, 0.07f, 16); circle(-0.78f, 0.42f, 0.06f, 16);
    circle(-0.62f, 0.41f, 0.07f, 16); circle(-0.46f, 0.42f, 0.06f, 16);
    circle(-0.30f, 0.41f, 0.07f, 16); circle(-0.14f, 0.42f, 0.06f, 16);
}

void cy_fence() {
    float fLeft = -0.36f;
    float fRight =  0.52f;
    float fBot = -0.10f;
    float fTop=  0.11f;

    glColor3f(0.90f, 0.90f, 0.88f);
    glBegin(GL_QUADS);
    glVertex2f(fLeft,  fBot); glVertex2f(-0.10f, fBot);
    glVertex2f(-0.10f, fTop); glVertex2f(fLeft,  fTop);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.10f,  fBot); glVertex2f(fRight, fBot);
    glVertex2f(fRight, fTop); glVertex2f(0.10f,  fTop);
    glEnd();

    glColor3f(0.18f, 0.18f, 0.17f);
    float railY[] = {-0.02f, 0.045f};
    for (int i = 0; i < 2; i++) {
        glLineWidth(5.0f);
        glBegin(GL_LINES);
        glVertex2f(fLeft + 0.04f, railY[i]); glVertex2f(-0.14f, railY[i]);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(0.15f, railY[i]); glVertex2f(fRight - 0.04f, railY[i]);
        glEnd();
    }

    glColor3f(0.94f, 0.94f, 0.92f);
    glLineWidth(6.0f);
    float leftPosts[] = {-0.36f, -0.25f, -0.14f};
    for (int i = 0; i < 3; i++) {
        glBegin(GL_LINES);
        glVertex2f(leftPosts[i], fBot); glVertex2f(leftPosts[i], fTop);
        glEnd();
    }
    float rightPosts[] = {0.10f, 0.22f, 0.34f, 0.46f, 0.52f};
    for (int i = 0; i < 5; i++) {
        glBegin(GL_LINES);
        glVertex2f(rightPosts[i], fBot); glVertex2f(rightPosts[i], fTop);
        glEnd();
    }
}

void cy_pavilionRoof() {
    float rLeft = -0.33f;
    float rRight=  0.23f;
    float rBase =  0.18f;
    float rMid =  0.30f;
    float peakX = -0.05f;
    float peakY =  0.44f;

    glColor3f(0.82f, 0.38f, 0.12f);
    glBegin(GL_TRIANGLES);
    glVertex2f(rLeft,  rBase); glVertex2f(peakX, peakY); glVertex2f(-0.18f, rMid);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(rRight, rBase); glVertex2f(peakX, peakY); glVertex2f( 0.05f, rMid);
    glEnd();

    glColor3f(0.88f, 0.49f, 0.19f);
    glBegin(GL_QUADS);
    glVertex2f(rLeft,  rBase); glVertex2f(rRight, rBase);
    glVertex2f( 0.08f, rMid);  glVertex2f(-0.22f, rMid);
    glEnd();

    glColor3f(0.73f, 0.56f, 0.34f);
    glBegin(GL_QUADS);
    glVertex2f(rLeft,  rBase - 0.025f); glVertex2f(rRight, rBase - 0.025f);
    glVertex2f(rRight, rBase);          glVertex2f(rLeft,  rBase);
    glEnd();

    glColor3f(0.79f, 0.31f, 0.09f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.22f, rMid); glVertex2f( 0.08f, rMid); glVertex2f(peakX, peakY);
    glEnd();

    glColor3f(0.75f, 0.31f, 0.10f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(rLeft, rBase); glVertex2f(rRight, rBase);
    glEnd();
}

void cy_clockTower() {
    float tLeft = -0.14f;
    float tRight =  0.08f;
    float tBot = -0.10f;
    float tTop =  0.63f;

    glColor3f(0.75f, 0.23f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(tLeft,  tBot); glVertex2f(tRight, tBot);
    glVertex2f(tRight, tTop); glVertex2f(tLeft,  tTop);
    glEnd();

    glColor3f(0.56f, 0.16f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f( 0.00f, tBot); glVertex2f(tRight, tBot);
    glVertex2f(tRight, tTop); glVertex2f( 0.00f, tTop);
    glEnd();

    glColor3f(0.94f, 0.81f, 0.67f);
    glBegin(GL_QUADS);
    glVertex2f(tLeft - 0.025f, 0.58f); glVertex2f(tRight + 0.025f, 0.58f);
    glVertex2f(tRight + 0.025f, 0.63f); glVertex2f(tLeft - 0.025f, 0.63f);
    glEnd();

    float cx = -0.03f;
    float cy_val = 0.48f;
    float cr = 0.055f;

    glColor3f(0.82f, 0.80f, 0.74f);
    circle(cx, cy_val, cr, 36);

    glColor3f(0.70f, 0.24f, 0.08f);
    glLineWidth(1.8f);
    circleOutline(cx, cy_val, cr, 36);

    glColor3f(0.28f, 0.28f, 0.26f);
    glLineWidth(1.4f);
    // Minute hand
    glPushMatrix();
    glTranslatef(cx, cy_val, 0.0f);
    glRotatef(-cy_clockMinuteAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f); glVertex2f(0.0f, cr * 0.80f);
    glEnd();
    glPopMatrix();
    // Hour hand
    glPushMatrix();
    glTranslatef(cx, cy_val, 0.0f);
    glRotatef(-cy_clockHourAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f); glVertex2f(0.0f, cr * 0.60f);
    glEnd();
    glPopMatrix();

    float spireBase = 0.63f;
    float spireTop = 0.91f;
    float spireL = tLeft  - 0.025f;
    float spireR = tRight + 0.025f;
    float spireX = (spireL + spireR) * 0.5f;

    glColor3f(0.90f, 0.37f, 0.15f);
    glBegin(GL_TRIANGLES);
    glVertex2f(spireL, spireBase); glVertex2f(spireR, spireBase); glVertex2f(spireX, spireTop);
    glEnd();

    glColor3f(0.70f, 0.20f, 0.07f);
    glBegin(GL_TRIANGLES);
    glVertex2f(spireX, spireBase + 0.02f); glVertex2f(spireR, spireBase); glVertex2f(spireX, spireTop);
    glEnd();

    glColor3f(0.93f, 0.80f, 0.66f);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex2f(spireL, spireBase); glVertex2f(spireR, spireBase);
    glEnd();
}

void cy_overpass() {
    float x0 = -0.37f, y0b = 0.22f, y0t = 0.28f;
    float x1 = 1.00f, y1b = 0.48f, y1t = 0.55f;

    glColor3f(0.61f, 0.19f, 0.06f);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0b); glVertex2f(x1, y1b);
    glVertex2f(x1, y1t); glVertex2f(x0, y0t);
    glEnd();

    glColor3f(0.74f, 0.25f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0t); glVertex2f(x1, y1t);
    glVertex2f(x1, y1t + 0.03f); glVertex2f(x0, y0t + 0.03f);
    glEnd();

    glColor3f(0.48f, 0.54f, 0.56f);
    glLineWidth(2.2f);
    glBegin(GL_LINES);
    glVertex2f(x0, y0t + 0.045f); glVertex2f(x1, y1t + 0.045f);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x0, y0t + 0.010f); glVertex2f(x1, y1t + 0.010f);
    glEnd();

    glLineWidth(1.5f);
    int nPosts = 12;
    for (int i = 0; i <= nPosts; i++) {
        float t  = (float)i / nPosts;
        float px = x0 + t * (x1 - x0);
        float py = y0t + t * (y1t - y0t);
        glBegin(GL_LINES);
        glVertex2f(px, py); glVertex2f(px, py + 0.045f);
        glEnd();
    }
}

void cy_trussTower() {
    float tx = 0.84f;
    float tBot = -0.30f;
    float tTop = 0.48f;

    glColor3f(0.64f, 0.18f, 0.06f);
    glBegin(GL_QUADS);
    glVertex2f(tx - 0.015f, tBot); glVertex2f(tx + 0.015f, tBot);
    glVertex2f(tx + 0.015f, tTop); glVertex2f(tx - 0.015f, tTop);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(tx + 0.06f, tBot); glVertex2f(tx + 0.09f, tBot);
    glVertex2f(tx + 0.09f, tTop); glVertex2f(tx + 0.06f, tTop);
    glEnd();

    glColor3f(0.72f, 0.22f, 0.08f);
    glLineWidth(2.5f);
    float bY[] = {-0.30f, -0.15f, 0.00f, 0.15f, 0.30f, 0.45f};
    int nb = 5;
    for (int i = 0; i < nb; i++) {
        glBegin(GL_LINES);
        glVertex2f(tx - 0.015f, bY[i]); glVertex2f(tx + 0.09f, bY[i+1]);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(tx + 0.09f, bY[i]); glVertex2f(tx - 0.015f, bY[i+1]);
        glEnd();
    }

    glColor3f(0.58f, 0.15f, 0.05f);
    glLineWidth(2.0f);
    for (int i = 0; i < 6; i++) {
        glBegin(GL_LINES);
        glVertex2f(tx - 0.015f, bY[i]); glVertex2f(tx + 0.09f, bY[i]);
        glEnd();
    }

    glColor3f(0.50f, 0.50f, 0.50f);
    glBegin(GL_QUADS);
    glVertex2f(tx - 0.06f, tBot - 0.02f); glVertex2f(tx + 0.14f, tBot - 0.02f);
    glVertex2f(tx + 0.14f, tBot);         glVertex2f(tx - 0.06f, tBot);
    glEnd();
}

void cy_streetLightPole() {
    float px = 0.58f;

    glColor3f(0.70f, 0.70f, 0.62f);
    glBegin(GL_QUADS);
    glVertex2f(px - 0.008f, -0.10f); glVertex2f(px + 0.008f, -0.10f);
    glVertex2f(px + 0.008f,  0.72f); glVertex2f(px - 0.008f,  0.72f);
    glEnd();

    glColor3f(0.12f, 0.12f, 0.11f);
    circle(px - 0.015f, 0.70f, 0.013f, 16);
    circle(px - 0.005f, 0.66f, 0.013f, 16);
    circle(px + 0.005f, 0.62f, 0.013f, 16);

    glColor3f(0.88f, 0.88f, 0.72f);
    circle(px - 0.018f, 0.702f, 0.006f, 12);
    circle(px - 0.008f, 0.662f, 0.006f, 12);
    circle(px + 0.002f, 0.622f, 0.006f, 12);

    glColor3f(0.70f, 0.70f, 0.62f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(px, 0.70f); glVertex2f(px + 0.022f, 0.62f);
    glEnd();
}

void cy_road() {
    glColor3f(0.16f, 0.16f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.00f); glVertex2f( 1.0f, -1.00f);
    glVertex2f(1.0f, -0.10f); glVertex2f(-1.0f, -0.10f);
    glEnd();

    glColor3f(0.20f, 0.20f, 0.19f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.18f); glVertex2f( 1.0f, -0.18f);
    glVertex2f(1.0f, -0.10f); glVertex2f(-1.0f, -0.10f);
    glEnd();

    glColor3f(0.82f, 0.82f, 0.80f);
    glLineWidth(2.8f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, -0.76f); glVertex2f( 1.0f, -0.43f);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(-1.0f, -0.88f); glVertex2f( 0.95f, -0.52f);
    glEnd();
}

void cy_car() {
    float x = -0.95f + cy_carPosition;
    float y = -0.76f + (cy_carPosition * 0.165f);
    float scale = 1.05f - (cy_carPosition * 0.12f);
    if (scale < 0.72f) scale = 0.72f;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(9.0f, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, 1.0f);

    glColor3f(0.08f, 0.08f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(-0.27f, -0.08f); glVertex2f( 0.25f, -0.08f);
    glVertex2f(0.21f, -0.12f); glVertex2f(-0.23f, -0.12f);
    glEnd();

    glColor3f(0.78f, 0.16f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(-0.25f, -0.04f); glVertex2f( 0.25f, -0.04f);
    glVertex2f(0.20f,  0.08f); glVertex2f(-0.22f,  0.08f);
    glEnd();

    glColor3f(0.86f, 0.20f, 0.09f);
    glBegin(GL_QUADS);
    glVertex2f(0.20f, -0.035f); glVertex2f( 0.29f, -0.020f);
    glVertex2f(0.25f,  0.055f); glVertex2f( 0.19f,  0.080f);
    glEnd();

    glColor3f(0.70f, 0.12f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(-0.14f, 0.08f); glVertex2f( 0.11f, 0.08f);
    glVertex2f(0.04f, 0.18f); glVertex2f(-0.08f, 0.18f);
    glEnd();

    glColor3f(0.70f, 0.90f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(-0.110f, 0.095f); glVertex2f(-0.015f, 0.095f);
    glVertex2f(-0.035f, 0.155f); glVertex2f(-0.080f, 0.155f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.005f, 0.095f); glVertex2f(0.085f, 0.095f);
    glVertex2f(0.035f, 0.155f); glVertex2f(-0.005f, 0.155f);
    glEnd();

    glColor3f(0.96f, 0.88f, 0.46f);
    glBegin(GL_QUADS);
    glVertex2f(0.235f, 0.000f); glVertex2f(0.285f, 0.010f);
    glVertex2f(0.275f, 0.038f); glVertex2f(0.230f, 0.030f);
    glEnd();

    glColor3f(0.45f, 0.03f, 0.03f);
    glBegin(GL_QUADS);
    glVertex2f(-0.250f, 0.000f); glVertex2f(-0.215f, 0.000f);
    glVertex2f(-0.210f, 0.035f); glVertex2f(-0.245f, 0.035f);
    glEnd();

    glColor3f(0.03f, 0.03f, 0.03f);
    circle(-0.14f, -0.05f, 0.055f, 28);
    circle(0.14f, -0.05f, 0.055f, 28);

    glColor3f(0.55f, 0.55f, 0.52f);
    circle(-0.14f, -0.05f, 0.025f, 24);
    circle(0.14f, -0.05f, 0.025f, 24);

    glPopMatrix();
}

void cy_trafficLight() {
    float tlx = -0.72f;
    float tly = -0.10f;

    glColor3f(0.25f, 0.25f, 0.24f);
    glBegin(GL_QUADS);
    glVertex2f(tlx - 0.008f, tly);         glVertex2f(tlx + 0.008f, tly);
    glVertex2f(tlx + 0.008f, tly + 0.40f); glVertex2f(tlx - 0.008f, tly + 0.40f);
    glEnd();

    float bx1 = tlx - 0.028f;
    float bx2 = tlx + 0.028f;
    float by1 = tly + 0.30f;
    float by2 = tly + 0.52f;

    glColor3f(0.12f, 0.12f, 0.11f);
    glBegin(GL_QUADS);
    glVertex2f(bx1, by1); glVertex2f(bx2, by1);
    glVertex2f(bx2, by2); glVertex2f(bx1, by2);
    glEnd();

    float lightCenters[3];
    lightCenters[0] = by1 + (by2 - by1) * 0.83f;
    lightCenters[1] = by1 + (by2 - by1) * 0.50f;
    lightCenters[2] = by1 + (by2 - by1) * 0.17f;

    float dimR[3] = {0.25f, 0.20f, 0.05f};
    float dimG[3] = {0.03f, 0.18f, 0.18f};
    float dimB[3] = {0.03f, 0.00f, 0.04f};
    for (int i = 0; i < 3; i++) {
        glColor3f(dimR[i], dimG[i], dimB[i]);
        circle(tlx, lightCenters[i], 0.016f, 20);
    }

    if (cy_trafficLightState == 0) {
        glColor3f(1.0f, 0.10f, 0.05f);
        circle(tlx, lightCenters[0], 0.018f, 20);
    } else if (cy_trafficLightState == 1) {
        glColor3f(1.0f, 0.85f, 0.00f);
        circle(tlx, lightCenters[1], 0.018f, 20);
    } else {
        glColor3f(0.10f, 0.95f, 0.15f);
        circle(tlx, lightCenters[2], 0.018f, 20);
    }

    glLineWidth(1.2f);
    if (cy_trafficLightState == 0) {
        glColor3f(0.8f, 0.1f, 0.05f);
        circleOutline(tlx, lightCenters[0], 0.022f, 20);
    } else if (cy_trafficLightState == 1) {
        glColor3f(0.8f, 0.7f, 0.0f);
        circleOutline(tlx, lightCenters[1], 0.022f, 20);
    } else {
        glColor3f(0.1f, 0.75f, 0.1f);
        circleOutline(tlx, lightCenters[2], 0.022f, 20);
    }
}

//City timer callback
void cy_update(int value) {
    if (currentScene == 6) {
        cy_Car1Move();
        cy_BirdMove();
        cy_SkyMove();

        cy_clockTimer += 16.0f;
        cy_clockMinuteAngle = fmod(cy_clockTimer / 60000.0f * 360.0f, 360.0f);
        cy_clockHourAngle   = fmod(cy_clockMinuteAngle / 12.0f, 360.0f);

        cy_trafficLightTimer += 16.0f;
        float greenDur = 5000.0f;
        float yellowDur = 1500.0f;
        float redDur = 4000.0f;
        float cycleDur = greenDur + yellowDur + redDur;
        float phase = fmod(cy_trafficLightTimer, cycleDur);
        if (phase < greenDur)
            cy_trafficLightState = 2;
        else if (phase < greenDur + yellowDur)
            cy_trafficLightState = 1;
        else
            cy_trafficLightState = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, cy_update, 0);
}

// -- City main display --
void displayCity() {
    glClearColor(0.76f, 0.89f, 0.93f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    cy_sky();
    cy_powerLines();
    cy_overpass();
    cy_trees();
    cy_trussTower();
    cy_streetLightPole();
    cy_fence();
    cy_pavilionRoof();
    cy_clockTower();
    cy_road();
    cy_trafficLight();
    cy_car();

    glFlush();
}


//_________________________________________________________

//  COVER PAGE  (Scene 0)
void coverPage() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.8f);
    renderBitmapString(-0.62f, 0.82f, GLUT_BITMAP_TIMES_ROMAN_24, "American International University Of Bangladesh (AIUB)");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.38f, 0.68f, GLUT_BITMAP_HELVETICA_18, "Department of Computer Science (CS)");
    renderBitmapString(-0.3f,  0.55f, GLUT_BITMAP_HELVETICA_18, "Semester: Spring 2025-26");
    renderBitmapString(-0.5f,  0.47f, GLUT_BITMAP_HELVETICA_18, "Course code: 00501 Course: COMPUTER GRAPHICS");
    renderBitmapString(-0.13f, 0.30f, GLUT_BITMAP_HELVETICA_18, "Submitted To:");
    renderBitmapString(-0.15f, 0.22f, GLUT_BITMAP_HELVETICA_18, "Mahfujur Rahman");
    renderBitmapString(-0.18f, 0.14f, GLUT_BITMAP_HELVETICA_18, "Assistant Professor");
    renderBitmapString(-0.3f,  0.06f, GLUT_BITMAP_HELVETICA_18, "Department of Computer Science");
    renderBitmapString(-0.09f,-0.05f, GLUT_BITMAP_HELVETICA_18, "Section: I");
    renderBitmapString(-0.1f, -0.20f, GLUT_BITMAP_HELVETICA_18, "Submitted By:");
    glColor3f(0.0f, 0.0f, 0.7f);
    renderBitmapString(-0.5f, -0.32f, GLUT_BITMAP_HELVETICA_12, "SL");
    renderBitmapString(-0.35f,-0.32f, GLUT_BITMAP_HELVETICA_12, "Name");
    renderBitmapString(0.28f,-0.32f, GLUT_BITMAP_HELVETICA_12, "ID");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.5f, -0.40f, GLUT_BITMAP_HELVETICA_12, "10");
    renderBitmapString(-0.35f,-0.40f, GLUT_BITMAP_HELVETICA_12, "MEHEDI HASAN");
    renderBitmapString(0.25f,-0.40f, GLUT_BITMAP_HELVETICA_12, "22-48502-3");
    renderBitmapString(-0.5f, -0.47f, GLUT_BITMAP_HELVETICA_12, "13");
    renderBitmapString(-0.35f,-0.47f, GLUT_BITMAP_HELVETICA_12, "MD. IBTIHAZZAMAN");
    renderBitmapString(0.25f,-0.47f, GLUT_BITMAP_HELVETICA_12, "22-49153-3");
    renderBitmapString(-0.5f, -0.54f, GLUT_BITMAP_HELVETICA_12, "28");
    renderBitmapString(-0.35f,-0.54f, GLUT_BITMAP_HELVETICA_12, "MD. MUSHFIQUR RAHMAN");
    renderBitmapString(0.25f,-0.54f, GLUT_BITMAP_HELVETICA_12, "23-51548-1");
    renderBitmapString(-0.5f, -0.61f, GLUT_BITMAP_HELVETICA_12, "29");
    renderBitmapString(-0.35f,-0.61f, GLUT_BITMAP_HELVETICA_12, "MD. AHSANUL KABIR");
    renderBitmapString(0.25f,-0.61f, GLUT_BITMAP_HELVETICA_12, "23-51602-2");
    renderBitmapString(-0.5f, -0.68f, GLUT_BITMAP_HELVETICA_12, "38");
    renderBitmapString(-0.35f,-0.68f, GLUT_BITMAP_HELVETICA_12, "MD. SAYEEM");
    renderBitmapString(0.25f,-0.68f, GLUT_BITMAP_HELVETICA_12, "23-55321-3");
    renderBitmapString(-0.35f,-0.85f, GLUT_BITMAP_HELVETICA_18, "Submission Date: 07-05-2026");
    renderBitmapString(0.4f, -0.95f, GLUT_BITMAP_HELVETICA_12, "Press 1 for guide book");
    glFlush();
}


// ___________________________________________________________________________________________________________

//  GUIDEBOOK  (Scene 1)
void guidebook() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.8f);
    renderBitmapString(-0.45f, 0.90f, GLUT_BITMAP_TIMES_ROMAN_24, "Guidebook of Scenarios");
    glBegin(GL_LINES);
    glVertex2f(-0.85f, 0.82f); glVertex2f(0.85f, 0.82f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-0.85f, 0.72f, GLUT_BITMAP_HELVETICA_18, "Scenario 1: Press 0 to go to the cover page");
    renderBitmapString(-0.85f, 0.64f, GLUT_BITMAP_HELVETICA_18, "Scenario 2: Press 1 to go to Guidebook");
    renderBitmapString(-0.85f, 0.56f, GLUT_BITMAP_HELVETICA_18, "Scenario 3: Press 2 to go to Sylhet Railway Station");
    renderBitmapString(-0.80f, 0.49f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Click Left Mouse Button to toggle between Day and Night");
    renderBitmapString(-0.80f, 0.43f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press right arrow to move train right side");
    renderBitmapString(-0.80f, 0.37f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press left arrow to move train left side");
    renderBitmapString(-0.85f, 0.29f, GLUT_BITMAP_HELVETICA_18, "Scenario 4: Press 3 to go to Srimongol Tea Garden");
    renderBitmapString(-0.80f, 0.22f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press B or b for bird animation");
    renderBitmapString(-0.80f, 0.16f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press R or r for rain animation");
    renderBitmapString(-0.80f, 0.10f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press the mouse button to move the cloud");
    renderBitmapString(-0.85f, 0.02f, GLUT_BITMAP_HELVETICA_18, "Scenario 5: Press 4 to go to Jaflong Riverside View");
    renderBitmapString(-0.80f,-0.05f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press Enter key to change Day to Night");
    renderBitmapString(-0.80f,-0.11f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press right mouse button to start/stop cloud movement");
    renderBitmapString(-0.80f,-0.17f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press Up Arrow key to increase boat speed");
    renderBitmapString(-0.80f,-0.23f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press Down Arrow key to decrease boat speed");
    renderBitmapString(-0.85f,-0.31f, GLUT_BITMAP_HELVETICA_18, "Scenario 6: Press 5 to go to Utmachora Waterfall View");
    renderBitmapString(-0.80f,-0.38f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press n key to change Day to Night or Night to Day");
    renderBitmapString(-0.80f,-0.44f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press Up/Down Arrow key to control waterfall speed");
    renderBitmapString(-0.80f,-0.50f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press f key to move fast the car");
    renderBitmapString(-0.80f,-0.56f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press g key to slow the car");
    renderBitmapString(-0.80f,-0.62f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press spacebar key to stop the car");
    renderBitmapString(-0.80f,-0.68f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Press c key to resume car at normal speed");
    renderBitmapString(-0.80f,-0.74f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Right click mouse to stop/start cloud");
    renderBitmapString(-0.85f,-0.82f, GLUT_BITMAP_HELVETICA_18, "Scenario 7: Press 6 to go to Sylhet City View");
    renderBitmapString(-0.80f,-0.88f, GLUT_BITMAP_TIMES_ROMAN_10, "  - Space=Stop/Start Car | UP/DOWN=Speed | Left/Right=Move | R=Reset | Left Click=Stop/Start Cloud");

    glColor3f(0.8f, 0.0f, 0.0f);
    renderBitmapString(0.30f, 0.72f, GLUT_BITMAP_HELVETICA_18, "Sound Controls:");
    renderBitmapString(0.30f, 0.64f, GLUT_BITMAP_HELVETICA_18, "  - Press M or m to MUTE all sounds");
    renderBitmapString(0.30f, 0.56f, GLUT_BITMAP_HELVETICA_18, "  - Press U or u to UNMUTE all sounds");

    glFlush();
}


//_________________________________________________________________________________________________________

//  MASTER DISPLAY
void display()
{
    if(currentScene == 0)
    {
        coverPage();
    }
    else if(currentScene == 1)
    {
        guidebook();
    }
    else if(currentScene == 2)
    {
        displayRailwayStation();
    }
    else if(currentScene == 3)
    {
        displayTeaGarden();
    }
    else if(currentScene == 4)
    {
        displayJaflong();
    }
    else if(currentScene == 5)
    {
        displayUtmachora();
    }
    else if(currentScene == 6)
    {
        displayCity();
    }
}


//_______________________________________________________________________________

//MASTER KEYBOARD
void keyboard(unsigned char key, int x, int y)
{

    //Global: Mute / Unmute
    if(key == 'm' || key == 'M')
    {
        isMuted = true;
        stopAllSounds();
        rw_isTrainSoundPlaying = false;
        glutPostRedisplay();
        return;
    }
    if(key == 'u' || key == 'U')
    {
        isMuted = false;
        if (currentScene == 2)
        {
            if (rw_trainSpeed != 0.0f)
            {
                PlaySound("train.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                rw_isTrainSoundPlaying = true;
            }
        }
    else if(currentScene == 3)
    {
            if (isRaining)
            {
                PlaySound("raincg.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

            }
            else if (isStarted)
            {
                 PlaySound("teabird.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            }

    }
    else if(currentScene == 4 && !isNight)
    {
        PlaySound("JAFLONG.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }
    else if(currentScene == 5)
    {
        PlaySound("waterfall.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }
    else if(currentScene == 6)
    {
        PlaySound("Traffic.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }

    glutPostRedisplay();
    return;
    }

    //Scene navigation
    if(key == '0')
    {
        currentScene = 0;
        stopAllSounds();
        isStarted = false; isRaining = false; cloudMoving = false;
        rw_isTrainSoundPlaying = false;
    }
    else if(key == '1')
    {
        currentScene = 1;
        stopAllSounds();
        rw_isTrainSoundPlaying = false;
    }
    else if(key == '2')
    {
        currentScene = 2;
        stopAllSounds();
        rw_isTrainSoundPlaying = false;
        if (rw_trainSpeed != 0.0f && !isMuted) {
            PlaySound("train.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            rw_isTrainSoundPlaying = true;
        }
    }
    else if(key == '3')
    {
        currentScene = 3;
        stopAllSounds();
        isRaining = false; isStarted = false; cloudMoving = false;
        rw_isTrainSoundPlaying = false;
    }
    else if(key == '4')
    {
        currentScene = 4;
        stopAllSounds();
        rw_isTrainSoundPlaying = false;
        if (!isMuted && !isNight)
            PlaySound("JAFLONG.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }
    else if(key == '5')
    {
        currentScene = 5;
        stopAllSounds();
        rw_isTrainSoundPlaying = false;
        if(!isMuted)
        {
            PlaySound("waterfall.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

        }
    }
    else if(key == '6')
    {
        currentScene = 6;
        stopAllSounds();
        rw_isTrainSoundPlaying = false;
        if(!isMuted)
        {
             PlaySound("Traffic.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
        }
    }

    //Tea Garden scene-specific (Scene 3)
    else if (currentScene == 3) {
        if (key == 'r' || key == 'R') {
            isRaining = !isRaining;
            stopAllSounds();
            if (isRaining) {
                isStarted = false;
                if (!isMuted) PlaySound("raincg.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            }
        }
        else if (key == 'b' || key == 'B') {
            if (!isRaining) {
                isStarted = !isStarted;
                stopAllSounds();
                if (isStarted && !isMuted)
                    PlaySound("teabird.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            }
        }
    }

    //Jaflong scene-specific (Scene 4)
    else if (currentScene == 4)
    {
        if (key == 13)
        {
            // Enter
            isNight = !isNight;
            stopAllSounds();
            if (!isNight && !isMuted)
                PlaySound("JAFLONG.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
        }
    }

    //Utmachora scene-specific (Scene 5)
    else if (currentScene == 5)
    {
        if (key == 'n' || key == 'N')
        {
            ut_isNightMode = !ut_isNightMode;
            if (ut_isNightMode)
            {
                ut_moonY = 0.22f;
                ut_moonRising = true;
                glutTimerFunc(30, ut_moonRiseUpdate, 0);
            }
            else
            {
                ut_moonRising = false;
                ut_moonY = 0.22f;
            }
        }
        else if(key == 'f' || key == 'F')
        {
            ut_carSpeed = 0.05f; ut_carMoving = true;
        }
        else if(key == 'g' || key == 'G')
        {
            ut_carSpeed = 0.005f; ut_carMoving = true;
        }
        else if(key == ' ')
        {
            ut_carMoving = false;
        }
        else if(key == 'c' || key == 'C')
        {
            ut_carMoving = true; ut_carSpeed = 0.01f;
        }
    }

    //City scene-specific (Scene 6)
    else if (currentScene == 6) {
        if (key == ' ') {
            cy_isCarMoving = !cy_isCarMoving;
        }
        else if (key == 'r' || key == 'R') {
            cy_carPosition = 0.0f;
            cy_isCarMoving = true;
        }
    }

    glutPostRedisplay();
}


// _______________________________________________________________________________________________

// MASTER SPECIAL (Arrow keys)

void SpecialInput(int key, int x, int y) {
    if (currentScene == 2) {
        if (key == GLUT_KEY_RIGHT)
            rw_trainSpeed = 0.009f;
        if (key == GLUT_KEY_LEFT)
            rw_trainSpeed = -0.009f;
    }
    else if (currentScene == 4) {
        if (key == GLUT_KEY_UP)   jaf_speed += 0.002f;
        if (key == GLUT_KEY_DOWN) jaf_speed -= 0.002f;
    }
    else if (currentScene == 5) {
        if (key == GLUT_KEY_UP) {
            ut_dropSpeed += 0.005f;
            if (ut_dropSpeed > 0.08f) ut_dropSpeed = 0.08f;
        }
        if (key == GLUT_KEY_DOWN) {
            ut_dropSpeed -= 0.005f;
            if (ut_dropSpeed < 0.005f) ut_dropSpeed = 0.005f;
        }
    }
    else if (currentScene == 6) {
        switch (key) {
            case GLUT_KEY_UP:
                cy_carSpeed += 0.002f;
                break;
            case GLUT_KEY_DOWN:
                cy_carSpeed -= 0.002f;
                if (cy_carSpeed < 0.002f) cy_carSpeed = 0.002f;
                break;
            case GLUT_KEY_LEFT:
                cy_carPosition -= 0.05f;
                break;
            case GLUT_KEY_RIGHT:
                cy_carPosition += 0.05f;
                break;
        }
    }
    glutPostRedisplay();
}


//_____________________________________________________________________________________

//MASTER MOUSE

void mouse(int button, int state, int x, int y) {
    if (currentScene == 2) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            rw_nightMode = !rw_nightMode;
            rw_animating = true;
        }
    }
    else if (currentScene == 3) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            cloudMoving = true;
    }
    else if (currentScene == 4) {
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
            cloudRun = !cloudRun;
    }
    else if (currentScene == 5) {
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
            ut_cloudMoving = !ut_cloudMoving;
    }
    else if (currentScene == 6) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            cy_cloudMoving = !cy_cloudMoving;
        }
    }
    glutPostRedisplay();
}


//______________________________________________________________________________

//Main

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(950, 680);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("AIUB Computer Graphics Project");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialInput);
    glutMouseFunc(mouse);

    // Railway Station update
    glutTimerFunc(30, rw_update, 0);

    // Tea Garden timer
    glutTimerFunc(50, updateBirds_Tea,0);
    glutTimerFunc(40, updateRain, 0);
    glutTimerFunc(20, updateClouds_Tea, 0);

    // Jaflong timer
    glutTimerFunc(100, updateJaflong, 0);

    // Utmachora timer
    glutTimerFunc(30, ut_updateCloud, 0);
    glutTimerFunc(30, ut_updateWaterDrops, 0);
    glutTimerFunc(30, ut_updateBirds, 0);
    glutTimerFunc(30, ut_updateCar,0);

    // City timer
    glutTimerFunc(16, cy_update,0);

    glutMainLoop();
    return 0;
}
