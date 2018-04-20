#define GLEW_STATIC

#include "Angel.h"
#include<math.h>

vec3 change_square_color[3] = { vec3(1.0,1.0,1.0), vec3(1.0,0.0,0.0), vec3(0.0,1.0,0.0),};
vec3 change_ellipse_color[5] = { vec3(1.0,1.0,1.0),vec3(0.0,0.0,1.0),vec3(0.0,1.0,0.0),vec3(1.0,0.0,0.0),vec3(0.0,0.0,0.0)};
int color_of_ellipse_background = 0;
void buffer_function (struct vec2 value[], int value_size, struct vec3 value_color[], int value_color_size, GLuint ProgramName );
void multiple_square(double rsquare);
void display(void);
void display2(void);
void display3(void);
void circle ( double rcircle, double x, double y, struct vec3 circle_given_color);
static int submenu_id;
static int menu_id;
static int window1;
static int window2;
static int subWindow1;
int value=0;
#define PI 3.14159265
int animate;
int color_of_square=0;
vec3 color = vec3( 1.0,0.0,0.0 );
int menu2_id;
vec3 circle_given_color;

int s = 0;
int t = 0;
double c = 0;
double counter = 0;
int click_point = 0;
vec2 click_circle[25];
vec3 random_color[25];
//--------------------------------------------------------------------------

void
init( void )
{
    // Create a vertex array object
    GLuint vao[1];
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );
//    random_color = vec3 ( rand()%100, rand()%100, rand()%100 );

    // glGenBuffers( 1, &buffer );
    // glBindBuffer( GL_ARRAY_BUFFER, buffer );
    // glBufferData( … );

    display();

}

void menu(int num){
    if(num == 1){
        if (animate == 1){
            animate=0;
        }
    }
    if(num == 2){
        if (animate == 0){
            animate=1;
        }
    }
    if(num == 3){
        color_of_square = 0;
    }
    if(num == 4){
        color_of_square = 1;
    }
    if(num == 5){
        color_of_square = 2;
    }
    if(num == 6){
        color_of_ellipse_background = 3;
    }
    if(num == 7){
        color_of_ellipse_background = 1;
    }
    if(num == 8){
        color_of_ellipse_background = 2;
    }
    if(num == 9){
        color_of_ellipse_background = 0;
    }
    if(num == 10){
        color_of_ellipse_background = 4;
    }
    // else if() {
  //   value = num;
  // }
  glutPostRedisplay();
}

void mymouse(GLint btn, GLint state, GLint x, GLint y) {
    double xmouse;
    double ymouse;
    xmouse = ((x-250.0)/250.0);
    ymouse = ((250.0-y)/250.0);


    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
        exit(0);
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        random_color[click_point] = vec3 ( rand()%100/100.0, rand()%100/100.0, rand()%100/100.0 );
        click_circle[click_point] = vec2( xmouse, ymouse );
        click_point++;
    }
}

void myidle() {
    if(animate == 0) {
        s+=5;
        t-=5;

        if (c < 0){
            counter = 0;
        }
        if (c>=0.20){
            counter = 1;
        }

        if (counter == 0){
                c+=0.01;
        }
        if (counter == 1){
                c-=0.01;
        }
    }
    glutPostRedisplay();
}

void triangle( void ) {

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    //  glUseProgram( program );  // This is called in InitShader

    double xtriangle = 0.0;
    double ytriangle = 0.0;
    double rtriangle = 0.2;
    int i=1;
    int angle = 120+t;
    vec2 triangle_vertices[5];
    vec3 triangle_color[5];

    triangle_color[0]=color;
    triangle_color[1]=color;
    triangle_color[2]=color;
    triangle_color[3]=color;
    triangle_color[4]=color;
    triangle_vertices[0] = vec2 ( 0.0, 0.8 );
    while(i<5){
        ytriangle = ((rtriangle*sin(angle*PI/180)+0.8));
        xtriangle = (rtriangle*cos(angle*PI/180));
        triangle_vertices[i] = vec2 ( xtriangle, ytriangle );
        angle=angle+120;
        i++;
    }

    buffer_function(triangle_vertices, sizeof(triangle_vertices), triangle_color, sizeof(triangle_color), program);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 5);    // draw the triangle
}

void multiple_square(double rsquare, int color) {

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    //  glUseProgram( program );  // This is called in InitShader


    double xsquare = 0.0;
    double ysquare = 0.0;
    int i=1;
    int angle = 45+s;
    vec2 square_vertices[6];
    vec3 square_color[6];
    square_vertices[0] = vec2 ( 0.0, -0.3 );
    if (color==0){
        square_color[0]= vec3( 0.0, 0.0, 0.0 );
    }
    else {
        square_color[0]= change_square_color[color_of_square];
    }
    while(i<6){

        ysquare = ((rsquare*sin(angle*PI/180)-0.3));
        xsquare = (rsquare*cos(angle*PI/180));
        square_vertices[i] = vec2 ( xsquare, ysquare );
        if (color==0){
            square_color[0]= vec3( 0.0, 0.0, 0.0 );
        }
        else {
            square_color[i] = change_square_color[color_of_square];
        }

        angle=angle+90;
        i++;

    }

    buffer_function ( square_vertices, sizeof(square_vertices), square_color, sizeof(square_color), program );
    glDrawArrays( GL_TRIANGLE_FAN, 0, 6);    // draw the square
}

void circle(double rcircle, double x, double y, struct vec3 circle_given_color) {

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    //  glUseProgram( program );  // This is called in InitShader
    rcircle = rcircle + c;
    double xcircle;
    double ycircle;
    int i=1;
    vec2 circle_vertices[362];
    vec3 circle_color[362];
    circle_vertices[0] = vec2 ( x, y );
    circle_color[0]=  circle_given_color;
    while(i<362){
        ycircle = rcircle*sin(i*PI/180.0) + y;
        xcircle = rcircle*cos(i*PI/180.0) + x;
        circle_vertices[i] = vec2 ( xcircle, ycircle );
        circle_color[i] = circle_given_color;
        i++;
    }

    buffer_function(circle_vertices, sizeof(circle_vertices), circle_color, sizeof(circle_color), program);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 362);    // draw the circle
}

void ellipse(double rellipse) {

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    //  glUseProgram( program );  // This is called in InitShader

    double xellipse = 0.0;
    double yellipse = 0.0;
    int i=1;
    vec2 ellipse_vertices[362];
    vec3 ellipse_color[362];
    ellipse_vertices[0] = vec2 ( 0.0, 0.0 );
    ellipse_color[0]= vec3 ( 1.0, 0.0, 0.0 );
    while(i<362){
        yellipse = ((0.6*rellipse*sin(i*PI/180)));
        xellipse = ((rellipse*cos(i*PI/180)));
        ellipse_vertices[i] = vec2 ( xellipse, yellipse );
        ellipse_color[i] = vec3 ( 1.0, 0.0, 0.0 );
        i++;
    }

    buffer_function(ellipse_vertices, sizeof(ellipse_vertices), ellipse_color, sizeof(ellipse_color), program);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 362);    // draw the ellipse
}


//----------------------------------------------------------------------------
void buffer_function (struct vec2 value[], int value_size, struct vec3 value_color[], int value_color_size, GLuint ProgramName )
{

    // Create and inintialize a buffer object for color
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, value_size, value, GL_STATIC_DRAW );
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( ProgramName, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );


    // Create and inintialize a buffer object for color
    GLuint buffer_color;
    glGenBuffers( 1, &buffer_color );
    glBindBuffer( GL_ARRAY_BUFFER, buffer_color );
    glBufferData( GL_ARRAY_BUFFER, value_color_size, value_color, GL_STATIC_DRAW );
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc_col = glGetAttribLocation( ProgramName, "vColor" );
    glEnableVertexAttribArray( loc_col );
    glVertexAttribPointer( loc_col, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
}

void createMenu(void){
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("White", 3);
    glutAddMenuEntry("Red", 4);
    glutAddMenuEntry("Green", 5);
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Start Animation", 1);
    glutAddMenuEntry("Stop Animation", 2);
    glutAddSubMenu("Change Color", submenu_id);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void createMenu2(void){
    menu2_id = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 6);
    glutAddMenuEntry("Blue", 7);
    glutAddMenuEntry("Green", 8);
    glutAddMenuEntry("White", 9);
    glutAddMenuEntry("Black", 10);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


//----------------------------------------------------------------------------

void
display( void )
{
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

//    vec3 random_color = vec3 ( rand()%100, rand()%100, rand()%100 );

    for(int i = 0; i<click_point;i++) {
        circle (0.03, click_circle[i].x, click_circle[i].y, random_color[i]);
    }

    multiple_square(0.9, 1.0);
    multiple_square(0.75, 0.0);
    multiple_square(0.6, 1.0);
    multiple_square(0.45, 0.0);
    multiple_square(0.3, 1.0);
    multiple_square(0.15, 0.0);

    glutSwapBuffers();
    glutPostRedisplay();
}

void display2 ( void ) {
    glClearColor( change_ellipse_color[color_of_ellipse_background].x, change_ellipse_color[color_of_ellipse_background].y, change_ellipse_color[color_of_ellipse_background].z, 1.0 ); // black background
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

    ellipse(0.5);
    //

    glutSwapBuffers();
    glutPostRedisplay();
}

void display3 ( void ) {
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

    triangle();
    circle(0.03, 0.7, -0.7, color);
    //

    glutSwapBuffers();
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y ) {
    switch ( key ) {
    case 'q':
        exit( EXIT_SUCCESS );
        break;

    case 'w':
        color = vec3(1.0,1.0,1.0);
        break;

    case 'r':
        color = vec3(1.0,0.0,0.0);
        break;

    case 'g':
        color = vec3(0.0,1.0,0.0);
        break;

    case 'b':
        color = vec3(0.0,0.0,1.0);
        break;

    case 'y':
        color = vec3(1.0,1.0,0.0);
        break;

    case 'o':
        color = vec3(1.0,0.5,0.0);
        break;

    case 'p':
        color = vec3(0.5,0.0,0.5);
        break;
    }
}

//----------------------------------------------------------------------------
void myReshape (int w, int h){
    glViewport(0,0,w,h);
}

int
main( int argc, char **argv )
{

    std::cout << "Once the application has started you can right click on the Animation screen which will give you an option to start/stop the application and you can even change the color of the rectangle. Left clicking on the animation window will generate a random colored circle. The sub window consists of an ellipse. Right clicking on the sub window will give a drop down menu consisting of colors that change the background of the sub window. The second window consists of a triangle and circle. On pressing keys such as w,r,b,p,y,g and o, the program will change the color of the triangle and circle to white,red,blue,purple,yellow,green and orange respectively. You can use 'q' to quit.";
    glutInit( &argc, argv );
#ifdef _APPLE_
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA );
#else
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE);
#endif
    glutInitWindowSize( 500, 500 );

    window1 = glutCreateWindow( "Animation" );



#ifndef _APPLE_
    GLenum err = glewInit();
    if (GLEW_OK != err)
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
#endif

createMenu();

init();

glutDisplayFunc( display );
glutKeyboardFunc( keyboard );
glutIdleFunc( myidle );
glutMouseFunc( mymouse );


subWindow1 = glutCreateSubWindow(window1,1,1,200,200);

//window2 = glutCreateWindow( "Create Window" );
glutDisplayFunc( display2 );
glutKeyboardFunc( keyboard );
createMenu2();
glutInitWindowPosition(600,100);
window2 = glutCreateWindow ( "Another Window" );

glutDisplayFunc( display3 );
glutKeyboardFunc( keyboard );
glutReshapeFunc(myReshape);
glutMainLoop();
return 0;

}
