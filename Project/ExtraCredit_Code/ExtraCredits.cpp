#define GLEW_STATIC

#include "Angel.h"
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <sstream>
using namespace std;
void buffer_function ( struct vec4 value[], int value_size, struct vec4 value_color[], int value_color_size );
void display(void);
void read_file(string FileName);
void perspective_function();
void translate_scale_to_origin1(int, int);
void translate_scale_to_origin2(int, int);
void translate_scale_to_origin3(int, int);
void translate_scale_to_origin4(int, int);
void translate_scale_to_origin5(int, int);
void normalize_function();


GLuint program;
GLuint fb;

static int menu_id;
static int window;
mat4 model_view, projection;
float xme, yme, zme, a=90, r=3.0, h=0.0;
float xme1, yme1, zme1;
float x_min, y_min, z_min;
float x_max, y_max, z_max;

mat4 mtranslate, mscale, mrotate;
mat4 multiply_matrix;

int size1;
int size2;
int size3;
int size4;
int size5;

double move_object_3 = 0;
double move_object_4 = 0;
double move_object_5 = 0;

int constraint1 = 0;
int constraint2 = 0;

vec4 location_of_me;
std::vector<vec4> vertices;
std::vector<vec4> colors;
std::vector<vec4> faces;
std::vector<vec4> normalized_values;
std::vector<vec4> points;
std::string line;

vec4 temp;
vec4 temp1;
vec4 temp2;
vec4 temp_normal1;
vec4 temp_normal2;
vec4 temp_normal3;
vec4 t;
vec4 value;

vec3 point;

vec4 First_Light_Ambient = vec4(0.2, 0.2, 0.2, 0.0);
vec4 First_Light_Diffuse = vec4(1.0, 1.0, 1.0, 0.0);
vec4 First_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);

vec4 Material_Ambient = vec4(0.0, 0.0, 0.0, 0.0);
vec4 Material_Specular = vec4(1.0, 1.0, 1.0, 0.0);


vec4 First_AmbientProduct;
vec4 First_DiffuseProduct;
vec4 First_SpecularProduct;

int parallel_perspective = 0;

int gouraud_phong = 1;
int trans = 2;
vec4 LightPosition;
vec4 EyePosition;
float Shininess = 100;

vec4 colorObject1 = vec4(1.0 , 0.0 , 0.0 , 1.0);
vec4 colorObject2 = vec4(0.0 , 1.0 , 0.0 , 1.0);
vec4 colorObject3 = vec4(0.0 , 0.0 , 1.0 , 1.0);

#define PI 3.14159265

//------------------------------------------------_rotate---------------------_rotate-----
void getRender() {
    GLuint color_rb;
    GLuint depth_rb;

   //RGBA8 RenderBuffer, 24 bit depth RenderBuffer, 512x512
   glGenFramebuffers(1, &fb);
   glBindFramebuffer(GL_FRAMEBUFFER, fb);

   //Create and attach a color buffer
   glGenRenderbuffers(1, &color_rb);

   //We must bind color_rb before we call glRenderbufferStorage
   glBindRenderbuffer(GL_RENDERBUFFER, color_rb);

   //The storage format is RGBA8
   glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 512, 512);

   //Attach color buffer to FBO
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                                 GL_RENDERBUFFER, color_rb);

   //-------------------------
   glGenRenderbuffers(1, &depth_rb);
   glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);

   //-------------------------
   //Attach depth buffer to FBO
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
						     GL_RENDERBUFFER, depth_rb);
}

void
init( void )
{
    program = InitShader("vshader21.glsl", "fshader21.glsl" );
                      
    getRender();
}

void read_file ( string FileName) {
    vertices.clear();
    faces.clear();
    normalized_values.clear();
    std::ifstream smfFile(FileName.c_str());
    while(!smfFile.eof()) {
        //Get input
        smfFile >> line >> value.x >> value.y >> value.z;

        //Print input

        if (line.compare("v") == 0)
            vertices.push_back(value);

        else if (line.compare("f") == 0)
            faces.push_back(value);
    }
    //Close file
    smfFile.close();
}

void normalize_function(){
    for (int i=0; i<vertices.size(); i++){
        normalized_values.push_back(vec4(0.0, 0.0, 0.0, 0.0));
    }

    for (int i=0; i<faces.size(); i++){

        vertices[(int)faces[i].x-1].w = 1.0;
        vertices[(int)faces[i].y-1].w = 1.0;
        vertices[(int)faces[i].z-1].w = 1.0;

        points.push_back(vertices[(int)faces[i].x-1]);
        points.push_back(vertices[(int)faces[i].y-1]);
        points.push_back(vertices[(int)faces[i].z-1]);

        temp1 = vertices[(int)faces[i].y-1]-vertices[(int)faces[i].x-1];
        temp2 = vertices[(int)faces[i].z-1]-vertices[(int)faces[i].x-1];
        temp = normalize(cross(temp1, temp2));

        colors.push_back(vec4(temp.x, temp.y, temp.z, temp.w));
        colors.push_back(vec4(temp.x, temp.y, temp.z, temp.w));
        colors.push_back(vec4(temp.x, temp.y, temp.z, temp.w));
    }
}


//----------------------------------------------------------------------------
void buffer_function (struct vec4 value[], int value_size, struct vec4 value_color[], int value_color_size ) {

    First_AmbientProduct = First_Light_Ambient * Material_Ambient;
    First_SpecularProduct = First_Light_Specular * Material_Specular;

    LightPosition = location_of_me;

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
    GLint proj = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(proj , 1 , GL_TRUE , &projection[0][0]);

    GLint view = glGetUniformLocation(program, "model_view");
    glUniformMatrix4fv(view , 1 , GL_TRUE , &model_view[0][0]);

    GLint Ambient = glGetUniformLocation(program, "AmbientProduct1");
    glUniform4f(Ambient, First_AmbientProduct.x, First_AmbientProduct.y, First_AmbientProduct.z, First_AmbientProduct.w);

    GLint Specular = glGetUniformLocation(program, "SpecularProduct1");
    glUniform4f(Specular, First_SpecularProduct.x, First_SpecularProduct.y, First_SpecularProduct.z, First_SpecularProduct.w);

    GLint Light1 = glGetUniformLocation(program, "LightPosition1");
    glUniform4f(Light1 , LightPosition.x, LightPosition.y, LightPosition.z, LightPosition.w);

    GLint Shine = glGetUniformLocation(program, "Shininess");
    glUniform1f(Shine, Shininess);

    GLint Eye = glGetUniformLocation(program, "EyePosition");
    glUniform4f(Eye , EyePosition.x, EyePosition.y, EyePosition.z, EyePosition.w);

}
void perspective_function(){
    xme = cos(a*M_PI/180.0) * r;
    yme = h;
    zme = sin(a*M_PI/180.0) * r;
    location_of_me = vec4( xme ,yme ,zme , 1.0 );
    LightPosition = model_view * location_of_me;
    EyePosition = location_of_me;
    model_view = LookAt( location_of_me , vec4 ( 0.0 , 0.0 , 0.0 , 1.0 ) , vec4 ( 0.0 , 1.0 , 0.0 , 1.0 ) );
}



void translate_scale_to_origin1( int startObject, int endObject ){

    float temp_x_min = points[startObject].x;
    float temp_y_min = points[startObject].y;
    float temp_z_min = points[startObject].z;
    float temp_x_max = points[startObject].x;
    float temp_y_max = points[startObject].y;
    float temp_z_max = points[startObject].z;

    for( int i = startObject+1; i < endObject; i++ ) {
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

    mtranslate = Translate( 0 , -10 , 0);


    float range = 0.0;
    if((x_max - x_min) > (y_max - y_min) && (x_max - x_min) > (z_max - z_min)){
        range = x_max-x_min;
    }
    else if((y_max - y_min) > (x_max - x_min) && (y_max - y_min) > (z_max - z_min)){
        range = y_max-y_min;
    }
    else if((z_max - z_min) > (x_max - x_min) && (z_max - z_min) > (y_max - y_min)){
        range = z_max-z_min;
    }

    mscale = Scale(vec3((1.0/range) , (1.0/range) , (1.0/range)));

    mrotate = RotateX ( 90 ) * RotateY ( 0 )*RotateZ ( 0 );

    multiply_matrix = mscale * mtranslate * mrotate;

    for(int i=startObject ; i<endObject; i++) {
        points[i] = multiply_matrix * points[i];
    }

    trans--;
}

void translate_scale_to_origin2( int startObject, int endObject ){

    float temp_x_min = points[startObject].x;
    float temp_y_min = points[startObject].y;
    float temp_z_min = points[startObject].z;
    float temp_x_max = points[startObject].x;
    float temp_y_max = points[startObject].y;
    float temp_z_max = points[startObject].z;

    for( int i = startObject+1; i < endObject; i++ ) {
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

    mtranslate = Translate( 0 , -1.2 , 0);


    float range = 0.0;
    if((x_max - x_min) > (y_max - y_min) && (x_max - x_min) > (z_max - z_min)){
        range = x_max-x_min;
    }
    else if((y_max - y_min) > (x_max - x_min) && (y_max - y_min) > (z_max - z_min)){
        range = y_max-y_min;
    }
    else if((z_max - z_min) > (x_max - x_min) && (z_max - z_min) > (y_max - y_min)){
        range = z_max-z_min;
    }

    mscale = Scale(vec3((1.0/range) * 1.5 , (1.0/range) * 1.5 , (1.0/range) * 1.5));

    mrotate = RotateX ( 90 )*RotateY ( 0 )*RotateZ ( 0 );
    multiply_matrix = mscale * mtranslate * mrotate;

    for(int i=startObject ; i<endObject; i++) {
        points[i] = multiply_matrix * points[i];
    }

    trans--;
}

void translate_scale_to_origin3( int startObject, int endObject ){

    float temp_x_min = points[startObject].x;
    float temp_y_min = points[startObject].y;
    float temp_z_min = points[startObject].z;
    float temp_x_max = points[startObject].x;
    float temp_y_max = points[startObject].y;
    float temp_z_max = points[startObject].z;

    for( int i = startObject+1; i < endObject; i++ ) {
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

    mtranslate = Translate( 1.2, 0, 0 );


    float range = 0.0;
    if((x_max - x_min) > (y_max - y_min) && (x_max - x_min) > (z_max - z_min)){
        range = x_max-x_min;
    }
    else if((y_max - y_min) > (x_max - x_min) && (y_max - y_min) > (z_max - z_min)){
        range = y_max-y_min;
    }
    else if((z_max - z_min) > (x_max - x_min) && (z_max - z_min) > (y_max - y_min)){
        range = z_max-z_min;
    }

    mscale = Scale(vec3((1.0/range) * 3.5 , (1.0/range)/ 3 , (1.0/range)*3));

    multiply_matrix = mscale * mtranslate;

    for(int i=startObject ; i<endObject; i++) {
        points[i] = multiply_matrix * points[i];
    }

    for( int i = startObject+1; i < endObject; i++ ) {
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
}

void translate_scale_to_origin4( int startObject, int endObject ){

    float temp_x_min = points[startObject].x;
    float temp_y_min = points[startObject].y;
    float temp_z_min = points[startObject].z;
    float temp_x_max = points[startObject].x;
    float temp_y_max = points[startObject].y;
    float temp_z_max = points[startObject].z;

    for( int i = startObject+1; i < endObject; i++ ) {
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

    mtranslate = Translate(3, 6, 0);


    float range = 0.0;
    if((x_max - x_min) > (y_max - y_min) && (x_max - x_min) > (z_max - z_min)){
        range = x_max-x_min;
    }
    else if((y_max - y_min) > (x_max - x_min) && (y_max - y_min) > (z_max - z_min)){
        range = y_max-y_min;
    }
    else if((z_max - z_min) > (x_max - x_min) && (z_max - z_min) > (y_max - y_min)){
        range = z_max-z_min;
    }

    mscale = Scale(vec3((1.0/range)*3 , (1.0/range)/3 , (1.0/range)*3));

    multiply_matrix = mscale * mtranslate;

    for(int i=startObject ; i<endObject; i++) {
        points[i] = multiply_matrix * points[i];
    }

    trans--;
}

void translate_scale_to_origin5( int startObject, int endObject ){

    float temp_x_min = points[startObject].x;
    float temp_y_min = points[startObject].y;
    float temp_z_min = points[startObject].z;
    float temp_x_max = points[startObject].x;
    float temp_y_max = points[startObject].y;
    float temp_z_max = points[startObject].z;

    for( int i = startObject+1; i < endObject; i++ ) {
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

    mtranslate = Translate( 4, 0, 0 );


    float range = 0.0;
    if((x_max - x_min) > (y_max - y_min) && (x_max - x_min) > (z_max - z_min)){
        range = x_max-x_min;
    }
    else if((y_max - y_min) > (x_max - x_min) && (y_max - y_min) > (z_max - z_min)){
        range = y_max-y_min;
    }
    else if((z_max - z_min) > (x_max - x_min) && (z_max - z_min) > (y_max - y_min)){
        range = z_max-z_min;
    }

    mscale = Scale(vec3((1.0/range) , (1.0/range) , (1.0/range)));

    mrotate = RotateX ( 90 ) * RotateY ( 0 ) * RotateZ ( 0 );

    multiply_matrix = mscale * mtranslate * mrotate;

    for(int i=startObject ; i<endObject; i++) {
        points[i] = multiply_matrix * points[i];
    }

    trans--;
}
//----------------------------------------------------------------------------

void changeColor(int diffuseColorOption) {
    if(diffuseColorOption == 1) {
        First_DiffuseProduct = First_Light_Diffuse * colorObject1;
    }
    else if(diffuseColorOption == 2) {
        First_DiffuseProduct = First_Light_Diffuse * colorObject2;
    }
    else if(diffuseColorOption == 3) {
        First_DiffuseProduct = First_Light_Diffuse * colorObject3;
    }

    GLint DiffuseProduct1 = glGetUniformLocation(program, "DiffuseProduct1");
    glUniform4f(DiffuseProduct1 , First_DiffuseProduct.x , First_DiffuseProduct.y , First_DiffuseProduct.z , First_DiffuseProduct.w);
}

void moveObject3 ( int startObject, int endObject ){
    mrotate = RotateX ( 0 ) * RotateY ( move_object_3 ) * RotateZ ( 0 );

    for(int i=startObject ; i<endObject; i++) {
        points[i] = mrotate * points[i];
    }
}

void moveObject4 (  int startObject, int endObject ) {
    float temp_x_min = points[startObject].x;
    float temp_y_min = points[startObject].y;
    float temp_z_min = points[startObject].z;
    float temp_x_max = points[startObject].x;
    float temp_y_max = points[startObject].y;
    float temp_z_max = points[startObject].z;

    for( int i = startObject+1; i < endObject; i++ ) {
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

    mrotate = RotateX ( 0 ) * RotateY ( move_object_4 ) * RotateZ ( 0 );

    vec3 center_of_object = vec3((x_min+x_max)/2.0, (y_min+y_max)/2.0, (z_min+z_max)/2.0);

    multiply_matrix =  Translate( center_of_object.x , center_of_object.y, center_of_object.z ) * mrotate * Translate( -center_of_object.x, -center_of_object.y, -center_of_object.z );

    for(int i=startObject ; i<endObject; i++) {
        points[i] = multiply_matrix * points[i];
    }
}

void moveObject5 ( int startObject, int endObject ) {

    mtranslate = Translate(0, move_object_5, 0);

    for(int i=startObject ; i<endObject; i++) {
        points[i] = mtranslate * points[i];
    }
}

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    perspective_function();
        projection = Perspective(100.0 , 1.0 , 0.5 , 12);
    vec4 arr1[points.size()];
    std::copy(points.begin(), points.end(), arr1);
    vec4 arr2[colors.size()];
    std::copy(colors.begin(), colors.end(), arr2);

    buffer_function ( arr1, sizeof(arr1), arr2, sizeof(arr2) );
    GLint onclickColor = glGetUniformLocation(program, "onclickColor");
    glUniform1i(onclickColor , 0);
    changeColor(1);
    glDrawArrays( GL_TRIANGLES, 0, size1 * 3 );
    changeColor(2);
    glDrawArrays( GL_TRIANGLES, size1 * 3,  size2 * 3 );
    changeColor(3);
    glDrawArrays( GL_TRIANGLES, size1 * 3 + size2 * 3,  size3 * 3 );
    changeColor(3);
    glDrawArrays( GL_TRIANGLES, size1 * 3 + size2 * 3 + size3 * 3,  size4 * 3 );
    changeColor(2);
    glDrawArrays( GL_TRIANGLES, size1 * 3 + size2 * 3 + size3 * 3 + size4 * 3,  size5 * 3 );

    glutPostRedisplay();
    glutSwapBuffers();
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

    case 'f':
        if(constraint2 < 50 ) {
            if( constraint1 < 33 ) {
                move_object_5 = 0.02;
                constraint1+=1;
                moveObject5((size1*3 + size2*3 + size3*3 + size4*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
            }
        }
    break;

    case 'v':
        if (constraint2 > -50){
            if( constraint1 > 0 ) {
                move_object_5= -0.02;
                constraint1-=1;
                moveObject5((size1*3 + size2*3 + size3*3 + size4*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
            }
        }
    break;

    case 'g':
        if(constraint1!=33){
            if ( constraint2 < 50 ) {
                move_object_4 = 3;
                constraint2+=1;
                moveObject4((size1*3 + size2*3 + size3*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
            }
        }
        else {
            move_object_4 = 3;
            constraint2+=1;
            if(constraint2 == 61) {
                constraint2 = -59;
            }
            moveObject4((size1*3 + size2*3 + size3*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
        }
    break;

    case 'b':
        if(constraint1!=33){
            if ( constraint2 > -50 ) {
                move_object_4 = -3;
                constraint2-=1;
                moveObject4((size1*3 + size2*3 + size3*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
            }
        }
        else {
            move_object_4 = -3;
            constraint2-=1;
            if(constraint2 == -60) {
                constraint2 = 60;
            }
            moveObject4((size1*3 + size2*3 + size3*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
        }
    break;

    case 'h':
        move_object_3 = 3;
        moveObject3((size1*3 + size2*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
    break;

    case 'n':
        move_object_3 = -3;
        moveObject3((size1*3 + size2*3), (size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
    break;

    case 'r':

        vertices.clear();
        faces.clear();
        points.clear();
        colors.clear();
        normalized_values.clear();
        move_object_5 = 0;
        a=90;
        r=3.0;
        h=0.0;
        colorObject1 = vec4(1.0 , 0.0 , 0.0 , 1.0);
        colorObject2 = vec4(0.0 , 1.0 , 0.0 , 1.0);
        colorObject3 = vec4(0.0 , 0.0 , 1.0 , 1.0);
        read_file("base.smf");
        size1=faces.size();
        normalize_function();
        translate_scale_to_origin1(0, size1*3);


        read_file("cylinder1.smf");
        size2=faces.size();
        normalize_function();
        translate_scale_to_origin2(size1*3, (size1*3 + size2*3));


        read_file("rectangle1.smf");
        size3=faces.size();
        normalize_function();
        translate_scale_to_origin3((size1*3 + size2*3),(size1*3 + size2*3 + size3*3));

        read_file("rectangle2.smf");
        size4=faces.size();
        normalize_function();
        translate_scale_to_origin4((size1*3 + size2*3 + size3*3),(size1*3 + size2*3 + size3*3 + size4*3));

        read_file("cylinder2.smf");
        size5=faces.size();
        normalize_function();
        translate_scale_to_origin5((size1*3 + size2*3 + size3*3 + size4*3),(size1*3 + size2*3 + size3*3 + size4*3 + size5*3));
    break;
    }
    glutPostRedisplay();
}

void generateRandomColors (int x) {
    switch(x) {
        case 10:
            colorObject1 = vec4(rand()%100/100.0 , rand()%100/100.0 , rand()%100/100.0 , 1.0);
            break;
        case 20:
            colorObject2 = vec4(rand()%100/100.0 , rand()%100/100.0 , rand()%100/100.0 , 1.0);
            break;
        case 30:
            colorObject3 = vec4(rand()%100/100.0 , rand()%100/100.0 , rand()%100/100.0 , 1.0);
            break;
    }
}

void changeColor_onclick (int option) {
    vec4 onclickChange = vec4(0.0 , 0.0 , 0.0 , 1.0);
    if(option ==  1){
        onclickChange =  vec4(0.0, 10.0 / 255.0 , 0.0  , 1.0);
    }
    else if(option == 2){
        onclickChange = vec4(0.0, 20.0 / 255.0 , 0.0 , 1.0);
    }
    else if(option == 3){
        onclickChange = vec4(0.0, 30.0 / 255.0 , 0.0  , 1.0);
    }
    GLint sendColor = glGetUniformLocation(program, "sendColor");
    glUniform4f(sendColor , onclickChange.x , onclickChange.y , onclickChange.z , onclickChange.w);
}

//----------------------------------------------------------------------------
int
main( int argc, char **argv) {

    cout << "Keys: \na and z is used to increase and decrease the angle of camera respectively. \ns and x are used to increase or decrease the height of camera respectively. \nd and c are used to increase and decrease the radius of the cylinder of camera respectively. \nf and v can be used to move the last cylinder along the y-axis. \ng and b can be used to rotate the second object. \nh and n can be used to rotate the first joint \nr is used to reset all the values to default. \nq is used to quit.\n";

        glutInit( &argc, argv );
    #ifdef _APPLE_
        glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA );
    #else
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    #endif
        glutInitWindowSize( 500, 500 );

        window = glutCreateWindow( "Animation" );


    #ifndef _APPLE_
        GLenum err = glewInit();
        if (GLEW_OK != err)
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    #endif

    read_file("base.smf");
    size1=faces.size();
    normalize_function();
    translate_scale_to_origin1(0, size1*3);


    read_file("cylinder1.smf");
    size2=faces.size();
    normalize_function();
    translate_scale_to_origin2(size1*3, (size1*3 + size2*3));


    read_file("rectangle1.smf");
    size3=faces.size();
    normalize_function();
    translate_scale_to_origin3((size1*3 + size2*3),(size1*3 + size2*3 + size3*3));

    read_file("rectangle2.smf");
    size4=faces.size();
    normalize_function();
    translate_scale_to_origin4((size1*3 + size2*3 + size3*3),(size1*3 + size2*3 + size3*3 + size4*3));

    read_file("cylinder2.smf");
    size5=faces.size();
    normalize_function();
    translate_scale_to_origin5((size1*3 + size2*3 + size3*3 + size4*3),(size1*3 + size2*3 + size3*3 + size4*3 + size5*3));

    init();
    glutKeyboardFunc( keyboard );
    glutDisplayFunc( display );
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
