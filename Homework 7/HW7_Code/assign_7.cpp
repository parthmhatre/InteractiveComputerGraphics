#define GLEW_STATIC

#include "Angel.h"
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;
void buffer_function ( struct vec4 value[], int value_size, struct vec4 value_color[], int value_color_size );
void display(void);
void read_file();
void perspective_function();
void translate_scale_to_origin();
void normalize_function();
vec3 normalize_function(int x, int y, int z);
void quad( int a, int b, int c );

GLuint program;


static int menu_id;
static int window;
mat4 model_view, projection;
int parallel_perspective=1;
float xme, yme, zme, a=90, r=2.0, h=0.0, a1=90, r1=2.0, h1=0.0;
float xme1, yme1, zme1;
float x_min, y_min, z_min;
float x_max, y_max, z_max;

mat4 mtranslate, mscale;
mat4 multiply_matrix;

vector < vector<vec4> > input_points;



vec4 location_of_me;
std::vector<vec4> vertices;
std::vector<vec4> colors;
std::vector<vec4> faces;
std::vector<vec4> normalized_values;
std::vector<vec4> points;
std::string line;

vec3 temp;
vec4 temp1;
vec4 temp2;
vec4 temp_normal1;
vec4 temp_normal2;
vec4 temp_normal3;
vec4 t;
vec4 value;

vec4 First_Light_Ambient = vec4(0.2, 0.2, 0.2, 0.0);
vec4 First_Light_Diffuse = vec4(1.0, 1.0, 1.0, 0.0);
vec4 First_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);

vec4 Second_Light_Ambient = vec4(0.2, 0.2, 0.2, 0.0);
vec4 Second_Light_Diffuse = vec4(1.0, 1.0, 1.0, 0.0);
vec4 Second_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);

vec4 Material_Ambient = vec4(0.0, 0.2, 0.0, 0.0);
vec4 Material_Diffuse = vec4(0.0, 1.0, 0.0, 0.0);
vec4 Material_Specular = vec4(1.0, 1.0, 1.0, 0.0);
//vec4 lightposition1,LightPosition2;


vec4 First_AmbientProduct;
vec4 First_DiffuseProduct;
vec4 First_SpecularProduct;

vec4 Second_AmbientProduct;
vec4 Second_DiffuseProduct;
vec4 Second_SpecularProduct;

int gouraud_phong = 1;

vec4 LightPosition;
vec4 LightPosition1;
vec4 EyePosition;
float Shininess = 100;


double u = 10.0;
double v = 10.0;
double du=1.00/(u-1);
double dv=1.00/(v-1);

#define PI 3.14159265

//------------------------------------------------_rotate---------------------_rotate-----

void
init( void )
{
    program = InitShader("vshader21.glsl","fshader21.glsl" );
    // translate_scale_to_origin();
}

long factorial (int n) {
    long fact=1,i=1;
    while(i<=n){
        fact=fact*i;
        i++;
    }
    return fact;
}

double power (double x, int y){
    double pow=1.00;
    int i=1;
    while(i<=y){
        pow=pow*x;
        i++;
    }
    return pow;
}


double point ( int x_y_z, double u, double v ) {
    double bi=0.0,bj=0.0;
    int i=0,j=0;
    double q=0.0;
    while(i<=3){
        bi=(factorial(3)/(factorial(i)*factorial(3-i)))*power((1-u),(3-i))*power(u,i);
        j=0;
        while(j<=3){
            bj=(factorial(3)/(factorial(j)*factorial(3-j))*power((1-v),(3-j))*power(v,j));
            if(x_y_z == 1)
                q=q+((input_points[i][j].x)*bj*bi);
            else if(x_y_z == 2)
                q=q+((input_points[i][j].y)*bj*bi);
            else if(x_y_z == 3)
                q=q+((input_points[i][j].z)*bj*bi);
            j++;
        }
        i++;
    }
    return q;
}

double decimal(double x){
    double value=0.0;
    value = round(x*1000000.0)/1000000.0;
    return value;
}

void values_into_vertices_faces() {
    int m=0;
    int n=0;
    int l=0;
    double a=0;
    double b;

    while(a<=1.0){
        b=0;
        n=0;
        while(b<=1){
            vertices.push_back(vec4(decimal(point(1,a,b)), decimal(point(2,a,b)), decimal(point(3,a,b)), 1.0));
            if((b+dv)>1&&(b+dv)!=(1+dv)){
                b=1;
            }
            else {
                b+=dv;
            }
            n++;
        }
        if((a+du)>1.0 && (a+du)!=(1.0+du)){
            a=1;
        }
        else {
            a = a + du;
        }
        m++;
    }

    int count=0;
    int index[m][n];
    for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                index[i][j]=count;
                count++;
            }
        }
    for(int i=0;i<m-1;i++){
            for(int j=0;j<n-1;j++){
                faces.push_back(vec4(index[i][j], index[i+1][j], index[i][j+1], 1.0));
                faces.push_back(vec4(index[i][j+1], index[i+1][j], index[i+1][j+1], 1.0));
            }
        }
}

vec3 normalize_function(int x, int y, int z){
    vec3 final;
    vec4 temp1 = vertices[y]-vertices[x];
    vec4 temp2 = vertices[z]-vertices[x];
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


void start () {
    // cout << "You can right click on the screen to get a menu which has 9 options. You can choose if you want to view in Parallel or Perspective. The default choice is Perspective. We can change the color of the object and the color of the light by clicking on the other options. Keys: a and z is used to increase and decrease the angle of camera respectively. s and x are used to increase or decrease the height of camera respectively. d and c are used to increase and decrease the radius of the cylinder of camera respectively. Keys: f and v is used to increase and decrease the angle of the 2nd light respectively. g and b are used to increase or decrease the height of the 2nd light respectively. h and n are used to increase and decrease the radius of the cylinder of the 2nd light respectively. r is used to reset all the values to default. q is used to quit.";
    for (int i=0; i<faces.size(); i++){
        temp = normalize_function(faces[i].x, faces[i].y, faces[i].z);
        cout << temp << endl;
        quad((int)(faces[i].x),(int)(faces[i].y),(int)(faces[i].z));
    }
}

void menu(int num){
    if (num == 1 ) {
        Material_Ambient = vec4(0.0, 0.2, 0.0, 0.0);
        Material_Diffuse = vec4(0.0, 1.0, 0.0, 0.0);
        Material_Specular = vec4(1.0, 1.0, 1.0, 0.0);
    }
    if (num == 2 ) {
        Material_Ambient = vec4(0.2, 0.0, 0.0, 0.0);
        Material_Diffuse = vec4(1.0, 0.0, 0.0, 0.0);
        Material_Specular = vec4(1.0, 1.0, 1.0, 0.0);
    }
    if (num == 3 ) {
        Material_Ambient = vec4(0.0, 0.0, 0.2, 0.0);
        Material_Diffuse = vec4(0.0, 0.0, 1.0, 0.0);
        Material_Specular = vec4(1.0, 1.0, 1.0, 0.0);
    }
    if (num == 4 ) {
        First_Light_Ambient = vec4(0.2, 0.2, 0.2, 0.0);
        First_Light_Diffuse = vec4(1.0, 1.0, 1.0, 0.0);
        First_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);

        Second_Light_Ambient = vec4(0.2, 0.2, 0.2, 0.0);
        Second_Light_Diffuse = vec4(1.0, 1.0, 1.0, 0.0);
        Second_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);
    }
    if (num == 5 ) {
        First_Light_Ambient = vec4(0.2, 0.2, 0.0, 0.0);
        First_Light_Diffuse = vec4(1.0, 1.0, 0.0, 0.0);
        First_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);

        Second_Light_Ambient = vec4(0.2, 0.2, 0.0, 0.0);
        Second_Light_Diffuse = vec4(1.0, 1.0, 0.0, 0.0);
        Second_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);
    }
    if (num == 6 ) {
        gouraud_phong = 0;
    }
    if (num == 7 ) {
        gouraud_phong = 1;
    }
    if (num == 8 ) {
        parallel_perspective = 1;
    }
    if (num == 9 ) {
        parallel_perspective = 2;
    }

    glutPostRedisplay();
}

void createMenu(void){
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Material:Green", 1);
    glutAddMenuEntry("Material:Red", 2);
    glutAddMenuEntry("Material:Blue", 3);
    glutAddMenuEntry("First Light: White", 4);
    glutAddMenuEntry("Second Light: Yellow", 5);
    glutAddMenuEntry("Gouraud", 6);
    glutAddMenuEntry("Phong", 7);
    glutAddMenuEntry("Perspective", 8);
    glutAddMenuEntry("Parallel", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//----------------------------------------------------------------------------
void buffer_function (struct vec4 value[], int value_size, struct vec4 value_color[], int value_color_size ) {

    First_AmbientProduct = First_Light_Ambient * Material_Ambient;
    First_DiffuseProduct = First_Light_Diffuse * Material_Diffuse;
    First_SpecularProduct = First_Light_Specular * Material_Specular;

    Second_AmbientProduct = Second_Light_Ambient * Material_Ambient;
    Second_DiffuseProduct = Second_Light_Diffuse * Material_Diffuse;
    Second_SpecularProduct = Second_Light_Specular * Material_Specular;
    LightPosition = location_of_me;
    LightPosition1 = vec4( xme ,yme ,zme , 1.0 );

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
    GLint proj = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(proj , 1 , GL_TRUE , &projection[0][0]);

    GLint view = glGetUniformLocation(program, "model_view");
    glUniformMatrix4fv(view , 1 , GL_TRUE , &model_view[0][0]);

    GLint Ambient = glGetUniformLocation(program, "AmbientProduct1");
    glUniform4f(Ambient, First_AmbientProduct.x, First_AmbientProduct.y, First_AmbientProduct.z, First_AmbientProduct.w);

    GLint Diffuse = glGetUniformLocation(program, "DiffuseProduct1");
    glUniform4f(Diffuse, First_DiffuseProduct.x, First_DiffuseProduct.y, First_DiffuseProduct.z, First_DiffuseProduct.w);

    GLint Specular = glGetUniformLocation(program, "SpecularProduct1");
    glUniform4f(Specular, First_SpecularProduct.x, First_SpecularProduct.y, First_SpecularProduct.z, First_SpecularProduct.w);

    GLint Ambient1 = glGetUniformLocation(program, "AmbientProduct2");
    glUniform4f(Ambient1, Second_AmbientProduct.x, Second_AmbientProduct.y, Second_AmbientProduct.z, Second_AmbientProduct.w);

    GLint Diffuse1 = glGetUniformLocation(program, "DiffuseProduct2");
    glUniform4f(Diffuse1, Second_DiffuseProduct.x, Second_DiffuseProduct.y, Second_DiffuseProduct.z, Second_DiffuseProduct.w);

    GLint Specular1 = glGetUniformLocation(program, "SpecularProduct2");
    glUniform4f(Specular1, Second_SpecularProduct.x, Second_SpecularProduct.y, Second_SpecularProduct.z, Second_SpecularProduct.w);

    GLint Light1 = glGetUniformLocation(program, "LightPosition1");
    glUniform4f(Light1 , LightPosition.x, LightPosition.y, LightPosition.z, LightPosition.w);

    GLint Light2 = glGetUniformLocation(program, "LightPosition2");
    glUniform4f(Light2 , LightPosition1.x, LightPosition1.y, LightPosition1.z, LightPosition1.w);

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

    xme = cos(a1*M_PI/180.0) * r1;
    yme = h1;
    zme = sin(a1*M_PI/180.0) * r1;
    LightPosition1 = vec4( xme1 ,yme1 ,zme1 , 1.0 );
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

    for(int i=0 ; i<points.size(); i++) {
        points[i] = multiply_matrix * points[i];
        colors[i] = multiply_matrix * colors[i];
    }
}
//----------------------------------------------------------------------------

void display( void ) {

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

    case 'f':
        a1+=2;
        if(a1>360){
            a1=0;
        }
    break;

    case 'g':
        h1+=0.2;
    break;

    case 'h':
        r1+=1;
    break;

    case 'v':
        a1-=2;
        if(a1<-360){
            a1=0;
        }
    break;

    case 'b':
        h1-=0.2;
    break;

    case 'n':
        r1-=1;
        if( r1 <= 0.5){
            r1 = 0.5;
        }
    break;

    case 'r':
        a=90;
        r=2.0;
        h=0.0;
        a1=90;
        r1=2.0;
        h1=0.0;
        parallel_perspective=1;
        First_Light_Ambient = vec4(0.2, 0.2, 0.2, 0.0);
        First_Light_Diffuse = vec4(1.0, 1.0, 1.0, 0.0);
        First_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);

        Second_Light_Ambient = vec4(0.2, 0.2, 0.2, 0.0);
        Second_Light_Diffuse = vec4(1.0, 1.0, 1.0, 0.0);
        Second_Light_Specular = vec4(1.0, 1.0, 1.0, 0.0);

        Material_Ambient = vec4(0.0, 0.2, 0.0, 0.0);
        Material_Diffuse = vec4(0.0, 1.0, 0.0, 0.0);
        Material_Specular = vec4(1.0, 1.0, 1.0, 0.0);
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

    window = glutCreateWindow( "Bezier" );



    du=decimal(du);
    dv=decimal(dv);

    for(int i=0; i<4; i++)
    {
            std::vector <vec4> fogRow;
            for(int j=0; j<4; j++)
            {
                 fogRow.push_back( vec4( 0.0, 0.0, 0.0, 0.0 ) );
            }
            input_points.push_back(fogRow);
    }

    input_points[0][0] = vec4( 0.0, 0.0, 0.0, 0.0 );
    input_points[1][0] = vec4( 0.0, 2.0, 1.1, 0.0 );
    input_points[2][0] = vec4( 0.0, 4.0, -0.5, 0.0 );
    input_points[3][0] = vec4( 0.0, 6.0, 0.3, 0.0 );
    input_points[0][1] = vec4( 2.0, 0.0, 1.5, 0.0 );
    input_points[1][1] = vec4( 2.0, 2.0, 3.9, 0.0 );
    input_points[2][1] = vec4( 2.0, 4.0, 2.6, 0.0 );
    input_points[3][1] = vec4( 2.0, 6.0, -1.1, 0.0 );
    input_points[0][2] = vec4( 4.0, 0.0, 2.9, 0.0 );
    input_points[1][2] = vec4( 4.0, 2.0, 3.1, 0.0 );
    input_points[2][2] = vec4( 4.0, 4.0, 2.4, 0.0 );
    input_points[3][2] = vec4( 4.0, 6.0, 1.3, 0.0 );
    input_points[0][3] = vec4( 6.0, 0.0, 0.0, 0.0 );
    input_points[1][3] = vec4( 6.0, 2.0, 0.7, 0.0 );
    input_points[2][3] = vec4( 6.0, 4.0, 0.4, 0.0 );
    input_points[3][3] = vec4( 6.0, 6.0, -0.2, 0.0 );

    values_into_vertices_faces();
    

    start();

    #ifndef _APPLE_
        GLenum err = glewInit();
        if (GLEW_OK != err)
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    #endif

    init();

    for(int i=0;i<points.size();i++)
        // cout << colors[i] << endl;

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    createMenu();
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
