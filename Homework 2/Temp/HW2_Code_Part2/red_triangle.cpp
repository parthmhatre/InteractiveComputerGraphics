// First new GL program
// Just makes a red triangle

#include "Angel.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
void buffer_function (struct vec2 value[], int value_size, struct vec3 value_color[], int value_color_size, GLuint ProgramName );
void multiple_square(double rsquare, double square_color);
void display(void);
#define PI 3.14159265





//--------------------------------------------------------------------------

void
init( void )
{
    display();

    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
}

void multiple_square(double rsquare, double color_of_square) {
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    //  glUseProgram( program );  // This is called in InitShader


    // double xsquare = 0.0;
    // double ysquare = 0.0;
    // int i=1;
    // int angle = 60;
    // vec2 square_vertices[6];
    // vec3 square_color[6];
    // square_vertices[0] = vec2 ( 0.0, -0.5 );
    // square_color[0]= vec3 ( color_of_square, color_of_square, color_of_square );
    // while(i<6){

    //     ysquare = (rsquare*sin(angle*PI/180)-0.5);
    //     xsquare = (rsquare*cos(angle*PI/180)-0.5);
    //     square_vertices[i] = vec2 ( xsquare, ysquare );
    //     square_color[i] = vec3 ( color_of_square, color_of_square, color_of_square );
    //     angle=angle+60;
    //     i++;

    // }

    vec2 triangle_vertices[3] = {
        vec2 ( -0.6, 0 ), vec2 ( -0.8, -0.2 ), vec2 ( -0.4, -0.2 ) 
    };

    vec3 triangle_color[3] = {
        vec3 ( 0.0, 1.0, 1.0 ), vec3 ( 0.0, 1.0, 1.0 ), vec3 ( 0.0, 1.0, 1.0 )
    };

    vec2 square_vertices[6] = {
        vec2 ( -0.8, -0.2 ), vec2 ( -0.4, -0.2 ), vec2 ( -0.8, -0.8 ), vec2 ( -0.4, -0.8 ), vec2 ( -0.4, -0.2 ), vec2 ( -0.8, -0.8 )
    };

    vec3 square_color[6] = {
        vec3 ( 0.0, 0.0, 1.0 ), vec3 ( 0.0, 0.0, 1.0 ), vec3 ( 0.0, 0.0, 1.0 ), vec3 ( 0.0, 0.0, 1.0 ), vec3 ( 0.0, 0.0, 1.0 ), vec3 ( 0.0, 0.0, 1.0 )
    };

    vec2 rectangle_vertices[6] = {
        vec2 ( -0.7, -0.5 ), vec2 ( -0.5, -0.5 ), vec2 ( -0.7, -0.8 ), vec2 ( -0.5, -0.8 ), vec2 ( -0.5, -0.5 ), vec2 ( -0.7, -0.8 )
    };

    vec3 rectangle_color[6] = {
        vec3 ( 0.545, 0.271, 0.075 ), vec3 ( 0.545, 0.271, 0.075 ), vec3 ( 0.545, 0.271, 0.075 ), vec3 ( 0.545, 0.271, 0.075 ), vec3 ( 0.545, 0.271, 0.075 ), vec3 ( 0.545, 0.271, 0.075 )
    };

    buffer_function ( triangle_vertices, sizeof(triangle_vertices), triangle_color, sizeof(triangle_color), program );
    glDrawArrays( GL_TRIANGLE_FAN, 0, 3);    // draw the square

    buffer_function ( square_vertices, sizeof(square_vertices), square_color, sizeof(square_color), program );
    glDrawArrays( GL_TRIANGLE_FAN, 0, 6);    // draw the square

    buffer_function ( rectangle_vertices, sizeof(rectangle_vertices), rectangle_color, sizeof(rectangle_color), program );
    glDrawArrays( GL_TRIANGLE_FAN, 0, 6);    // draw the square
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


//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

   
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    //  glUseProgram( program );  // This is called in InitShader
    

    double xsemicircle = 0.0;
    double ysemicircle = 0.0;
    double rsemicircle = 0.3;
    int i=0;
    vec2 semicircle_vertices[183];
    vec3 semicircle_color[183];
    semicircle_vertices[0] = vec2 ( 0.5, 0.5 );
    semicircle_color[0]= vec3 ( 0.0, 0.0, 1.0 );
    while(i<182) {
        ysemicircle = ((rsemicircle*sin(i*PI/180)) + 0.50);
        xsemicircle = ((rsemicircle*cos(i*PI/180)) + 0.50);
        semicircle_vertices[i+1] = vec2 ( xsemicircle, ysemicircle );
        semicircle_color[i+1] = vec3 ( 0.0, 0.0, 1.0 );
        i++;
    }
    semicircle_vertices[182]= vec2 ( 0.5, 0.5 );

    double xrhombus = 0.0;
    double yrhombus = 0.0;
    double rrhombus = 0.5;
    i=1;
    int angle = 0;
    vec2 rhombus_vertices[6];
    vec3 rhombus_color[6];
    rhombus_vertices[0] = vec2 ( -0.0, 0.0 );
    rhombus_color[0]= vec3 ( 0.0, 1.0, 0.0 );
    while(i<6){

        yrhombus = ((0.6*rrhombus*sin(angle*PI/180)));
        xrhombus = ((rrhombus*cos(angle*PI/180)));
        rhombus_vertices[i] = vec2 ( xrhombus, yrhombus );
        rhombus_color[i] = vec3 ( 0.0, 1.0, 0.0 );
        angle=angle+90;
        i++;

    }

    // Create a vertex array object
    GLuint vao[1];
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );
    

    multiple_square(0.2, 1.0);


    buffer_function(semicircle_vertices, sizeof(semicircle_vertices), semicircle_color, sizeof(semicircle_color), program);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 361);    // draw the circle


    buffer_function(rhombus_vertices, sizeof(rhombus_vertices), rhombus_color, sizeof(rhombus_color), program);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 362);    // draw the ellipse

    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 'q':
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA );
#else
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE);
#endif     
    glutInitWindowSize( 500, 500 );

    glutCreateWindow( "4 Images" );

#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err)
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
#endif

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
