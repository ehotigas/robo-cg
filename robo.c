#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BMPImagem {
  int width;
  int height;
  char *data;
} BMPImage;

#define MAX_NO_TEXTURES 1

GLuint texture_id[MAX_NO_TEXTURES];

char *files[MAX_NO_TEXTURES] = {"./text.bmp"};

GLUquadricObj *obj;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLint direcao = 1;

void getBitmapImageData(char *pFileName, BMPImage *pImage) {
  FILE *pFile = NULL;
  unsigned short nNumPlanes;
  unsigned short nNumBPP;
  int i;

  if ((pFile = fopen(pFileName, "rb")) == NULL)
    printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

  fseek(pFile, 18, SEEK_CUR);

  if ((i = fread(&pImage->width, 4, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ",
           pFileName);

  if ((i = fread(&pImage->height, 4, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ",
           pFileName);

  if ((fread(&nNumPlanes, 2, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n",
           pFileName);

  if ((i = fread(&nNumBPP, 2, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ",
           pFileName);

  fseek(pFile, 24, SEEK_CUR);

  int nTotalImagesize = (pImage->width * pImage->height) * 3;

  pImage->data = (char *)malloc(nTotalImagesize);

  if ((i = fread(pImage->data, nTotalImagesize, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ",
           pFileName);

  char charTemp;
  for (i = 0; i < nTotalImagesize; i += 3) {
    charTemp = pImage->data[i];
    pImage->data[i] = pImage->data[i + 2];
    pImage->data[i + 2] = charTemp;
  }
}

void getTexture() {
  BMPImage textura;

  glGenTextures(MAX_NO_TEXTURES, texture_id); /* 1 = uma textura; */

  int i;
  for (i = 0; i < MAX_NO_TEXTURES; i++) {
    getBitmapImageData(files[i], &textura);
    glBindTexture(GL_TEXTURE_2D, texture_id[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, textura.width, textura.height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, textura.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
}

void initTexture(void) {
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  getTexture();
}

void lightning() {
  GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};

  GLfloat light1_pos[] = {-2.0f, 1.0f, -1.0f, 1.0f};
  glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white);
  GLfloat direction[] = {1.0f, 0.0f, 0.0f};
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, -45.0f);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.1f);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT1);
}

void init(void) {
  glEnable(GL_COLOR_MATERIAL);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(1.0, 2.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, 1.0, 2.0, 8.0);
  glViewport(0, 0, 500, 500);

  lightning();
}

void cubo() {
  glBegin(GL_POLYGON);
  glNormal3f(-0.25, 0.25, 0.25);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.25, 0.25, 0.25);
  glNormal3f(0.25, 0.25, 0.25);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.25, 0.25, 0.25);
  glNormal3f(-0.25, -0.25, 0.25);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.25, -0.25, 0.25);
  glNormal3f(-0.25, -0.25, 0.25);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.25, -0.25, 0.25);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-0.25, 0.25, -0.25);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.25, 0.25, -0.25);
  glNormal3f(0.25, 0.25, -0.25);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.25, 0.25, -0.25);
  glNormal3f(0.25, -0.25, -0.25);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.25, -0.25, -0.25);
  glNormal3f(-0.25, -0.25, -0.25);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.25, -0.25, -0.25);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0.25, -0.25, 0.25);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.25, -0.25, 0.25);
  glNormal3f(0.25, 0.25, 0.25);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.25, 0.25, 0.25);
  glNormal3f(0.25, 0.25, -0.25);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.25, 0.25, -0.25);
  glNormal3f(0.25, -0.25, -0.25);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.25, -0.25, -0.25);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-0.25, -0.25, 0.25);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.25, -0.25, 0.25);
  glNormal3f(-0.25, 0.25, 0.25);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.25, 0.25, 0.25);
  glNormal3f(-0.25, 0.25, -0.25);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.25, 0.25, -0.25);
  glNormal3f(-0.25, -0.25, -0.25);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.25, -0.25, -0.25);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-0.25, 0.25, 0.25);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.25, 0.25, 0.25);
  glNormal3f(0.25, 0.25, 0.25);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.25, 0.25, 0.25);
  glNormal3f(0.25, 0.25, -0.25);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.25, 0.25, -0.25);
  glNormal3f(-0.25, 0.25, -0.25);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.25, 0.25, -0.25);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-0.25, -0.25, 0.25);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.25, -0.25, 0.25);
  glNormal3f(0.25, -0.25, 0.25);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.25, -0.25, 0.25);
  glNormal3f(0.25, -0.25, -0.25);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.25, -0.25, -0.25);
  glNormal3f(-0.25, -0.25, -0.25);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.25, -0.25, -0.25);
  glEnd();
}

void robo() {
  glMatrixMode(GL_MODELVIEW);
  /* Cabeça */
  glBindTexture(GL_TEXTURE_2D, texture_id[0]);
  glPushMatrix();
  glTranslatef(0.0, 0.95, 0.0);
  glScalef(0.5, 0.5, 0.5);
  cubo();
  glPopMatrix();
  /* Corpo */
  glBindTexture(GL_TEXTURE_2D, texture_id[0]);
  glPushMatrix();
  glTranslatef(0.0, 0.5, 0.0);
  glScalef(1.0, 1.0, 0.5);
  cubo();
  glPopMatrix();
  /* Braço esquerdo */
  glBindTexture(GL_TEXTURE_2D, texture_id[0]);
  glPushMatrix();
  glTranslatef(-0.35, 0.45, 0.0);
  glScalef(0.25, 1.25, 0.25);
  glPushMatrix();
  glRotatef(-angleX, 1.0, 0.0, 0.0);
  cubo();
  glPopMatrix();
  glPopMatrix();
  /* Braço Direito */
  glBindTexture(GL_TEXTURE_2D, texture_id[0]);
  glPushMatrix();
  glTranslatef(0.35, 0.45, 0.0);
  glScalef(0.25, 1.25, 0.25);
  glPushMatrix();
  glRotatef(angleX, 1.0, 0.0, 0.0);
  cubo();
  glPopMatrix();
  glPopMatrix();
  /* Perna esquerda */
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texture_id[0]);
  glTranslatef(-0.125, -0.125, 0.0);
  glScalef(0.25, 1.50, 0.25);
  glPushMatrix();
  glRotatef(angleX, 1.0, 0.0, 0.0);
  cubo();
  glPopMatrix();
  glPopMatrix();
  /* Perna direita */
  glBindTexture(GL_TEXTURE_2D, texture_id[0]);
  glPushMatrix();
  glTranslatef(0.125, -0.125, 0.0);
  glScalef(0.25, 1.50, 0.25);
  glPushMatrix();
  glRotatef(-angleX, 1.0, 0.0, 0.0);
  cubo();
  glPopMatrix();
  glPopMatrix();
}

void displayHandler() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glBindTexture(GL_TEXTURE_2D, texture_id[0]);
  glPushMatrix();
  glRotatef(angleY, 0.0, 1.0, 0.0);
  robo();
  glPopMatrix();

  glFlush();
}

void keyboardHandler(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
        angleX += direcao * 2;
        break;
    case GLUT_KEY_DOWN:
        angleX += direcao * 2;
        break;
    case GLUT_KEY_LEFT:
        angleY -= 8;
        break;
    case GLUT_KEY_RIGHT:
        angleY += 8;
    default:
        break;
  }
  glutPostRedisplay();
}

void timerHandler(int value) {
  if (angleX > 30) {
    direcao = -1;
  } else if (angleX < -30) {
    direcao = 1;
  }
  glutPostRedisplay();
  glutTimerFunc(33, timerHandler, 1);
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(700, 700);
  glutCreateWindow("robo");
  glutDisplayFunc(displayHandler);
  glutSpecialFunc(keyboardHandler);
  glutTimerFunc(33, timerHandler, 1);
  init();
  initTexture();
  glutMainLoop();
  return 0;
}