#define GLEW_STATIC

#include "Angel.h"
#include <math.h>

void buffer_function (struct vec4 value[], int value_size, struct vec4 value_color[], int value_color_size );
void display(void);
void colorcube();
void quad( int a, int b, int c, int d );
int Index = 0;

int dx_rotate = 0;
int dy_rotate = 0;
int dz_rotate = 0;

float dx_scale = 1.0;
float dy_scale = 1.0;
float dz_scale = 1.0;

float dx_translate = 0.0;
float dy_translate = 0.0;
float dz_translate = 0.0;

float scale_value=0.1;
float translate_value=0.1;
float rotate_value=1;


GLuint program;
int scale_translate_rotate= 1;
int NumVertices = 36;
mat4 translate_matrix;
mat4 scale_matrix;
mat4 rotate_matrix;
mat4 multiply_matrix;
// float thetaLoc[36];
static int menu_id;
static int window;
vec4 colors[36];
vec4 points[36];
vec4 vertices[8] = {
    vec4( -0.5, -0.5, 0.5, 1.0 ),
    vec4( -0.5, 0.5, 0.5, 1.0 ),
    vec4( 0.5, 0.5, 0.5, 1.0 ),
    vec4( 0.5, -0.5, 0.5, 1.0 ),
    vec4( -0.5, -0.5, -0.5, 1.0 ),
    vec4( -0.5, 0.5, -0.5, 1.0 ),
    vec4( 0.5, 0.5, -0.5, 1.0 ),
    vec4( 0.5, -0.5, -0.5, 1.0 )
};

// RGBA_rotate colors
vec4 vertex_colors[8] = {
    vec4( 0.0, 0.0, 0.0, 1.0 ), // black
    vec4( 1.0, 0.0, 0.0, 1.0 ), // red
    vec4( 1.0, 1.0, 0.0, 1.0 ), // yellow
    vec4( 0.0, 1.0, 0.0, 1.0 ), // green
    vec4( 0.0, 0.0, 1.0, 1.0 ), // blue
    vec4( 1.0, 0.0, 1.0, 1.0 ), // magenta
    vec4( 1.0, 1.0, 1.0, 1.0 ), // white
    vec4( 0.0, 1.0, 1.0, 1.0 )  // cyan
};
#define PI 3.14159265

//------------------------------------------------_rotate---------------------_rotate-----

void
init( void )
{
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    colorcube();
}


void
idle( void ) {

    glutPostRedisplay();
}


void menu(int num){
    if(num == 1){
        scale_translate_rotate=1;
    }
    if(num == 2){
        scale_translate_rotate=2;
    }
    if(num == 3){
        scale_translate_rotate=3;
    }
    // else if() {
  //   value = num;
  // }
  glutPostRedisplay();
}

void createMenu(void){
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Scale", 1);
    glutAddMenuEntry("Translate", 2);
    glutAddMenuEntry("Rotate", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//----------------------------------------------------------------------------
void buffer_function (struct vec4 value[], int value_size, struct vec4 value_color[], int value_color_size ) {


    // GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    // Create and inintialize a buffer object for color
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, value_size, value, GL_STATIC_DRAW );
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );


    // Create and inintialize a buffer object for color
    GLuint buffer_color;
    glGenBuffers( 1, &buffer_color );
    glBindBuffer( GL_ARRAY_BUFFER, buffer_color );
    glBufferData( GL_ARRAY_BUFFER, value_color_size, value_color, GL_STATIC_DRAW );
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc_col = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( loc_col );
    glVertexAttribPointer( loc_col, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLint abcd = glGetUniformLocation(program, "rot");
    glUniformMatrix4fv(abcd , 1 , GL_TRUE , &multiply_matrix[0][0]);
}

void colorcube( ) {
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
} 

void quad( int a, int b, int c, int d ){
    colors[Index] = vertex_colors[a]; 
    points[Index] = vertices[a]; 
    Index++;

    colors[Index] = vertex_colors[b]; 
    points[Index] = vertices[b]; 
    Index++;

    colors[Index] = vertex_colors[c]; 
    points[Index] = vertices[c]; 
    Index++;

    colors[Index] = vertex_colors[a]; 
    points[Index] = vertices[a]; 
    Index++;

    colors[Index] = vertex_colors[c]; 
    points[Index] = vertices[c]; 
    Index++;

    colors[Index] = vertex_colors[d]; 
    points[Index] = vertices[d]; 
    Index++;
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // glUniform3fv( thetaLoc, 1, theta );
    buffer_function ( points, sizeof(points), colors, sizeof(colors) );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
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

    case 'a':
    if(scale_translate_rotate==1){
        dx_scale+=scale_value;
    }
    else if(scale_translate_rotate==2){
        dx_translate+=translate_value;
    }
    else if(scale_translate_rotate==3){
        dx_rotate+=rotate_value;

    }
    break;

    case 's':
    if(scale_translate_rotate==1){
        dy_scale+=scale_value;
    }
    else if(scale_translate_rotate==2){
        dy_translate+=translate_value;
    }
    else if(scale_translate_rotate==3){
        dy_rotate+=rotate_value;
    }
    break;

    case 'd':
    if(scale_translate_rotate==1){
        dz_scale+=scale_value;
    }
    else if(scale_translate_rotate==2){
        dz_translate+=translate_value;
    }
    else if(scale_translate_rotate==3){
        dz_rotate+=rotate_value;

    }
    break;

    case 'z':
    if(scale_translate_rotate==1){
        if(dx_scale<0.1)
            dx_scale=0;
        else{
            dx_scale-=scale_value;
        }
    }
    else if(scale_translate_rotate==2){
        dx_translate-=translate_value;
    }
    else if(scale_translate_rotate==3){
        dx_rotate-=rotate_value;
    }
    break;

    case 'x':
    if(scale_translate_rotate==1){
        if(dy_scale<0.1)
            dy_scale=0;
        else {
            dy_scale-=scale_value;
        }
    }
    else if(scale_translate_rotate==2){
        dy_translate-=translate_value;
    }
    else if(scale_translate_rotate==3){
        dy_rotate-=rotate_value;
    }
    break;

    case 'c':
    if(scale_translate_rotate==1){
        if(dz_scale<0.1)
            dz_scale=0;
        else{
            dz_scale-=scale_value;
        }
    }
    else if(scale_translate_rotate==2){
        dz_translate-=translate_value;
    }
    else if(scale_translate_rotate==3){
        dz_rotate-=rotate_value;
    }
    break;

    case'w':
    if (scale_translate_rotate==1){
        scale_value+=0.1;
    }
    else if(scale_translate_rotate==2){
        translate_value+=0.1;
    }
    else if(scale_translate_rotate==3){
        if (rotate_value<360)
            rotate_value++;
    }
    break;

    case 'e':
    if (scale_translate_rotate==1){
        if(scale_value>0.1)
            scale_value-=0.1;
    }
    else if(scale_translate_rotate==2){
        if(translate_value>0.1)
            translate_value-=0.1;
    }
    else if(scale_translate_rotate==3){
        if(rotate_value>1)
            rotate_value--;
    }
    break;

    case 'r':
    dx_rotate = 0;
    dy_rotate = 0;
    dz_rotate = 0;

    dx_scale = 1.0;
    dy_scale = 1.0;
    dz_scale = 1.0;

    dx_translate = 0.0;
    dy_translate = 0.0;
    dz_translate = 0.0;
    break;
    }

    scale_matrix = Scale( dx_scale, dy_scale, dz_scale );
    translate_matrix = Translate ( dx_translate, dy_translate, dz_translate );
    rotate_matrix = RotateX ( dx_rotate )*RotateY ( dy_rotate )*RotateZ ( dz_rotate );

    multiply_matrix = translate_matrix*rotate_matrix*scale_matrix;
    buffer_function ( points, sizeof(points), colors, sizeof(colors) );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    //glutSwapBuffers();
    glutPostRedisplay();

}

//----------------------------------------------------------------------------
int
main( int argc, char **argv ) {

    std::cout << "";
        glutInit( &argc, argv );
    #ifdef _APPLE_
        glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA );
    #else
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    #endif
        glutInitWindowSize( 500, 500 );

        window = glutCreateWindow( "Extra Credits" );



    #ifndef _APPLE_
        GLenum err = glewInit();
        if (GLEW_OK != err)
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    #endif


    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    createMenu();
    // glutIdleFunc( myidle );
    // glutMouseFunc( mymouse );
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}