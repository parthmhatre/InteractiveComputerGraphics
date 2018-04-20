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
    // // Load shaders and use the resulting shader program
    // GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    // //  glUseProgram( program );  // This is called in InitShader

    

    // double x = 0.0;
    // double y = 0.0;
    // double r = 0.3;
    // int i=0;
    // vec2 circle_vertices[361];
    // vec3 circle_color[361];
    // while(i<362){
    //     y = ((r*sin(i*PI/180)) + 0.70);
    //     x = ((r*cos(i*PI/180)) + 0.70);
    //     circle_vertices[i] = vec2 ( x, y );
    //     circle_color[i] = vec3 ((1.0*(i/360)), 0.0, 0.0 );
    //     i++;
    // }

    // // Specify the vertices for a triangle
    
    // vec2 triangle_vertices[3] = {
    //     vec2 ( -0.20, 0.60 ), vec2 ( 0.20, 0.60 ), vec2 ( 0.0, 1.0 ) 
    // };


    // vec3 triangle_color[3] = {
    //     vec3 ( 0.0, 1.0, 0.0 ), vec3 ( 0.0, 0.0, 1.0 ), vec3 ( 1.0, 0.0, 0.0 )
    // };

    

    // // Create a vertex array object
    // GLuint vao[1];
    // glGenVertexArrays( 1, vao );
    // glBindVertexArray( vao[0] );
    

    

    // buffer_function(triangle_vertices, sizeof(triangle_vertices), triangle_color, sizeof(triangle_color), program);
    display();
    


    // buffer_function(circle_vertices, sizeof(circle_vertices), circle_color, sizeof(circle_color), program);
    // glDrawArrays( GL_TRIANGLE_FAN, 0, 360);    // draw the circle
    


    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
}

void multiple_square(double rsquare, double color_of_square) {
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    //  glUseProgram( program );  // This is called in InitShader


    double xsquare = 0.0;
    double ysquare = 0.0;
    int i=1;
    int angle = 45;
    vec2 square_vertices[6];
    vec3 square_color[6];
    square_vertices[0] = vec2 ( 0.0, -0.3 );
    square_color[0]= vec3 ( color_of_square, color_of_square, color_of_square );
    while(i<6){

        ysquare = ((rsquare*sin(angle*PI/180)-0.3));
        xsquare = (rsquare*cos(angle*PI/180));
        square_vertices[i] = vec2 ( xsquare, ysquare );
        square_color[i] = vec3 ( color_of_square, color_of_square, color_of_square );
        angle=angle+90;
        i++;

    }

    buffer_function ( square_vertices, sizeof(square_vertices), square_color, sizeof(square_color), program );
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
    

    double xcircle = 0.0;
    double ycircle = 0.0;
    double rcircle = 0.2;
    int i=1;
    vec2 circle_vertices[361];
    vec3 circle_color[361];
    circle_vertices[0] = vec2 ( 0.7, 0.7 );
    circle_color[0]= vec3 ( 0.0, 0.0, 0.0 );
    while(i<361){
        ycircle = ((rcircle*sin(i*PI/180)) + 0.70);
        xcircle = ((rcircle*cos(i*PI/180)) + 0.70);
        circle_vertices[i] = vec2 ( xcircle, ycircle );
        circle_color[i] = vec3 ( i*(1.0/361), 0.0, 0.0 );
        i++;
    }

    double xellipse = 0.0;
    double yellipse = 0.0;
    double rellipse = 0.2;
    i=1;
    vec2 ellipse_vertices[362];
    vec3 ellipse_color[362];
    ellipse_vertices[0] = vec2 ( -0.7, 0.7 );
    ellipse_color[0]= vec3 ( 1.0, 0.0, 0.0 );
    while(i<362){
        yellipse = ((0.6*rcircle*sin(i*PI/180)) + 0.70);
        xellipse = ((rcircle*cos(i*PI/180)) - 0.70);
        ellipse_vertices[i] = vec2 ( xellipse, yellipse );
        ellipse_color[i] = vec3 ( 1.0, 0.0, 0.0 );
        i++;
    }

    // Specify the vertices for a triangle
    
    vec2 triangle_vertices[3] = {
        vec2 ( -0.20, 0.60 ), vec2 ( 0.20, 0.60 ), vec2 ( 0.0, 1.0 ) 
    };

    vec3 triangle_color[3] = {
        vec3 ( 0.0, 1.0, 0.0 ), vec3 ( 0.0, 0.0, 1.0 ), vec3 ( 1.0, 0.0, 0.0 )
    };

    

    // Create a vertex array object
    GLuint vao[1];
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );
    

    multiple_square(0.9, 1.0);
    multiple_square(0.75, 0.0);
    multiple_square(0.6, 1.0);
    multiple_square(0.45, 0.0);
    multiple_square(0.3, 1.0);
    multiple_square(0.15, 0.0);


    buffer_function(triangle_vertices, sizeof(triangle_vertices), triangle_color, sizeof(triangle_color), program);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 3);    // draw the triangle


    buffer_function(circle_vertices, sizeof(circle_vertices), circle_color, sizeof(circle_color), program);
    glDrawArrays( GL_TRIANGLE_FAN, 0, 361);    // draw the circle


    buffer_function(ellipse_vertices, sizeof(ellipse_vertices), ellipse_color, sizeof(ellipse_color), program);
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
