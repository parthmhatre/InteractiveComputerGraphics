#define GLEW_STATIC

#include "Angel.h"
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;
void buffer_function (struct vec4 value[], int value_size, struct vec4 value_color[], int value_color_size );
void display(void);
void read_file(string FileName);
void quad( int a, int b, int c );
vec3 normalize_function(int x, int y, int z);
void perspective_function();
void translate_scale_to_origin();


GLuint program;


static int menu_id;
static int window;
mat4 model_view, projection;
int parallel_perspective=1;
float xme, yme, zme, a=90, r=2.0, h=0.0;
float x_min, y_min, z_min;
float x_max, y_max, z_max;

mat4 mtranslate, mscale;
mat4 multiply_matrix;


vec4 location_of_me;
std::vector<vec4> vertices;
std::vector<vec4> colors;
std::vector<vec4> faces;
std::vector<vec4> normalized_values;
std::vector<vec4> points;
std::string line;
vec3 temp;
vec3 value;

#define PI 3.14159265

//------------------------------------------------_rotate---------------------_rotate-----

void
init( void )
{
    program = InitShader("vshader21.glsl", "fshader21.glsl" );
    translate_scale_to_origin();
}


void
idle( void ) {

    glutPostRedisplay();
}

void read_file ( string FileName) {
    std::ifstream smfFile(FileName.c_str());
    cout << "You can right click on the screen to get a menu which has 2 options (Parallel and Perspective). The default choice is Perspective. Choose one of the choices. Keys: a and z is used to increase and decrease the angle respectively. s and x are used to increase or decrease the height respectively. d and c are used to increase and decrease the radius of the cylinder respectively. r is used to reset all the values to default. q is used to quit.";
    while(!smfFile.eof())
    {
        //Get input
        smfFile >> line >> value.x >> value.y >> value.z;

        //Print input

        if (line.compare("v") == 0) {
            vertices.push_back(vec4(value, 1.0));
        }

        else if (line.compare("f") == 0) {
            temp = normalize_function(value.x, value.y, value.z);
            temp.x=fabs(temp.x);
            temp.y=fabs(temp.y);
            temp.z=fabs(temp.z);
            normalized_values.push_back(vec4(temp, 1.0));
            quad((int)(value.x-1),(int)(value.y-1),(int)(value.z-1));
            faces.push_back(vec4(value, 1.0));

        }

    }

    //Close file
    smfFile.close();

}
vec3 normalize_function(int x, int y, int z){
    vec3 final;
    vec4 temp1 = vertices[y-1]-vertices[x-1];
    vec4 temp2 = vertices[z-1]-vertices[x-1];
    final = normalize(cross(temp1, temp2));

    return final;
}

void quad( int a, int b, int c ){

    colors.push_back(vec4(temp, 1.0));
    points.push_back(vertices[a]);

    colors.push_back(vec4(temp, 1.0));
    points.push_back(vertices[b]);

    colors.push_back(vec4(temp, 1.0));
    points.push_back(vertices[c]);

    }


void menu(int num){
    if (num == 1 ) {
        parallel_perspective = 1;
    }
    if (num == 2 ) {
        parallel_perspective = 2;
    }

  glutPostRedisplay();
}

void createMenu(void){
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Perspective", 1);
    glutAddMenuEntry("Parallel", 2);
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
    GLint abcd = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(abcd , 1 , GL_TRUE , &projection[0][0]);

    GLint abcd1 = glGetUniformLocation(program, "model_view");
    glUniformMatrix4fv(abcd1 , 1 , GL_TRUE , &model_view[0][0]);
}
void perspective_function(){
    xme = cos(a*M_PI/180.0) * r;
    yme = h;
    zme = sin(a*M_PI/180.0) * r;
    location_of_me = vec4(xme , yme , zme , 1.0);
    model_view = LookAt( location_of_me , vec4 ( 0.0 , 0.0 , 0.0 , 0.0 ) , vec4 ( 0.0 , 1.0 , 0.0 , 0.0 ) );
    }




void translate_scale_to_origin(){

    float temp_x_min = points[0].x;
    float temp_y_min = points[0].y;
    float temp_z_min = points[0].z;
    float temp_x_max = points[0].x;
    float temp_y_max = points[0].y;
    float temp_z_max = points[0].z;

    for( int i = 1; i < points.size(); i++ ) {
        if ( points[i].x < temp_x_min ) {
            temp_x_min = points[i].x;
        }
        if ( points[i].y < temp_y_min ) {
            temp_y_min = points[i].y;
        }
        if ( points[i].z < temp_z_min ) {
            temp_z_min = points[i].z;
        }
        if ( points[i].x > temp_x_max ) {
            temp_x_max = points[i].x;
        }
        if ( points[i].y > temp_y_max ) {
            temp_y_max = points[i].y;
        }
        if ( points[i].z > temp_z_max ) {
            temp_z_max = points[i].z;
        }
    }
    x_min=temp_x_min;
    y_min=temp_y_min;
    z_min=temp_z_min;
    x_max=temp_x_max;
    y_max=temp_y_max;
    z_max=temp_z_max;

    vec3 center_of_object = vec3((x_min+x_max)/2.0, (y_min+y_max)/2.0, (z_min+z_max)/2.0);

    mtranslate = Translate(-center_of_object.x , -center_of_object.y , -center_of_object.z);

    float range;
    if(x_max>y_max&&x_max>z_max){
        range = x_max-x_min;
    }
    else if(y_max>x_max&&y_max>z_max){
        range = y_max-y_min;
    }
    else if(z_max>y_max&&z_max>x_max){
        range = z_max-z_min;
    }

    mscale = Scale(vec3((1.0/range) , (1.0/range) , (1.0/range)));

    multiply_matrix = mscale * mtranslate;

    for(int i=0 ; i<points.size(); i++)
        points[i] = multiply_matrix * points[i];
}
//----------------------------------------------------------------------------

void
display( void )
{
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    perspective_function();
    if( parallel_perspective == 1) {
        projection = Perspective(100.0 , 1.0 , 0.5 , 12);
    }
    else if (parallel_perspective == 2) {
        projection = Ortho(-1.0 , 1.0 , -1.0 , 1.0 , 0.01 , 10.0);
    }
    vec4 arr1[points.size()];
    std::copy(points.begin(), points.end(), arr1);
    vec4 arr2[colors.size()];
    std::copy(colors.begin(), colors.end(), arr2);
    buffer_function ( arr1, sizeof(arr1), arr2, sizeof(arr2) );
    glDrawArrays( GL_TRIANGLES, 0, points.size() );

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
        a+=2;
        if(a>360){
            a=0;
        }
    break;

    case 's':
        h+=0.2;
    break;

    case 'd':
        r+=1;
    break;

    case 'z':
        a-=2;
        if(a<-360){
            a=0;
        }
    break;

    case 'x':
        h-=0.2;
    break;

    case 'c':
        r-=1;
        if( r <= 0.5){
            r=0.5;
        }
    break;

    case 'r':
        a=90;
        r=2.0;
        h=0.0;
        parallel_perspective=1;
    break;
    }
}

//----------------------------------------------------------------------------
int
main( int argc, char **argv) {
    
        glutInit( &argc, argv );
    #ifdef _APPLE_
        glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA );
    #else
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    #endif
        glutInitWindowSize( 500, 500 );

        window = glutCreateWindow( "Animation" );

    if (argv[1] == NULL) {
        read_file("bunny.smf");
    }
    else {
        read_file(argv[1]);
    }


    #ifndef _APPLE_
        GLenum err = glewInit();
        if (GLEW_OK != err)
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    #endif


    init();


    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    createMenu();
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}

