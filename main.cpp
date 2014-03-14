#include "initShaders.h"
#include <cstdlib>
#include <cstring>
using namespace std;

//walls that move at you
struct wall{
public:
int l;
int c;
float x;
bool active;
void randomize();
};

//unique random number 1 through 4
int remain = 4;              //remaining in set
int set[4]={0,1,2,3};        //set
int uniqueRand(bool reset){
int pick;//randomly pick index
int ret;//return value
if(reset){//reset values in set
remain = 4;
set[0]=0;
set[1]=1;
set[2]=2;
set[3]=3;
return 5;
}
pick = rand()%remain;//random
ret=set[pick];//sets return value to the set element stored at random index
for(int i = pick; i < (remain-1); i++){// "Deactivates" last index; moves indices forward
set[i]=set[i+1];
}
remain--;
set[remain]=-1;
return ret;
};

//initialize wall parameters before placed in game
void wall::randomize(){
l = (uniqueRand(false) % 4) - 1; //random level unique from other walls in grouping
c = rand() % 4; // random color
};

wall walls[6]; //array of walls; only six are used; 3 in each group removed and replaced in game



GLuint vaoID,vboID[2],eboID;
GLuint prog;

GLfloat pit,yaw,scalar=1;
glm::vec3 cubeTran;

int jump = 0;       //is player jumping?
int fall = 0;       //is player falling
int level = -1;     //which floor is player on?
int col = 0;        //which color is player?
int count = 0;      //counts frames for "guides"
int frameCount = 0; //total frame count/score

GLfloat size=1.0f;
GLfloat normal=1.0f/sqrt(3.0f);

GLfloat vertexarray[]={
//player cube
-6.4,-9.0f,0.11f,
-6.4,-11.0f,0.11f,
-7.6,-11.0f,0.11f,
-7.6,-9.0f,0.11f,
//white platform
-15.0f,-12.0f,0.0f,
-15.0f,-11.0f,0.0f,
15.0f,-11.0f,0.0f,
15.0f,-12.0f,0.0f,
//blue platform
-15.0f,-7.0f,0.0f,
-15.0f,-6.0f,0.0f,
15.0f,-6.0f,0.0f,
15.0f,-7.0f,0.0f,
//green platform
-15.0f,-2.0f,0.0f,
-15.0f,-1.0f,0.0f,
15.0f,-1.0f,0.0f,
15.0f,-2.0f,0.0f,
//red platform
-15.0f,3.0f,0.0f,
-15.0f,4.0f,0.0f,
15.0f,4.0f,0.0f,
15.0f,3.0f,0.0f,
//blue guide 1
-2.5f,-6.75f,0.1f,
-2.5f,-6.25f,0.1f,
-2.0f,-6.25f,0.1f,
-2.0f,-6.75f,0.1f,
//blue guide 2
10.0f,-6.75f,0.1f,
10.0f,-6.25f,0.1f,
10.5f,-6.25f,0.1f,
10.5f,-6.75f,0.1f,
//green guide 1
-2.5f,-1.75f,0.1f,
-2.5f,-1.25f,0.1f,
-2.0f,-1.25f,0.1f,
-2.0f,-1.75f,0.1f,
//green guide 2
10.0f,-1.75f,0.1f,
10.0f,-1.25f,0.1f,
10.5f,-1.25f,0.1f,
10.5f,-1.75f,0.1f,
//red guide 1
-2.5f,3.75f,0.1f,
-2.5f,3.25f,0.1f,
-2.0f,3.25f,0.1f,
-2.0f,3.75f,0.1f,
//red guide 2
10.0f,3.75f,0.1f,
10.0f,3.25f,0.1f,
10.5f,3.25f,0.1f,
10.5f,3.75f,0.1f,
//w0
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
//w1
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
//w2
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
//w3
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
//w4
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
//w5
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f};

GLfloat colorarray[]={
//cube
0.0f,0.0f,1.0f,1.0f,
0.0f,0.0f,1.0f,1.0f,
0.0f,0.0f,1.0f,1.0f,
0.0f,0.0f,1.0f,1.0f,
//w
1.0f,1.0f,1.0f,1.0f,
1.0f,1.0f,1.0f,1.0f,
1.0f,1.0f,1.0f,1.0f,
1.0f,1.0f,1.0f,1.0f,
//b
0.0f,0.0f,1.0f,1.0f,
0.0f,0.0f,1.0f,1.0f,
0.0f,0.0f,1.0f,1.0f,
0.0f,0.0f,1.0f,1.0f,
//g
0.0f,1.0f,0.0f,1.0f,
0.0f,1.0f,0.0f,1.0f,
0.0f,1.0f,0.0f,1.0f,
0.0f,1.0f,0.0f,1.0f,
//r
1.0f,0.0f,0.0f,1.0f,
1.0f,0.0f,0.0f,1.0f,
1.0f,0.0f,0.0f,1.0f,
1.0f,0.0f,0.0f,1.0f,
//b1
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//b2
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//g1
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//g2
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//r1
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//r2
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//w0
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//w1
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//w2
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//w3
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//w4
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
//w5
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f,
0.0f,0.0f,0.0f,1.0f};

GLubyte elems[]={
0,1,2,
0,2,3,//p
4,5,6,
4,6,7,//w
8,9,10,
8,10,11,//b
12,13,14,
12,14,15,//g
16,17,18,
16,18,19,//r
20,21,22,
20,22,23,//b1
24,25,26,
24,26,27,//b2
28,29,30,
28,30,31,//g1
32,33,34,
32,34,35,//g2
36,37,38,
36,38,39,//r1
40,41,42,
40,42,43,//r2
44,45,46,
44,46,47,//w0
48,49,50,
48,50,51,//w1
52,53,54,
52,54,55,//w2
56,57,58,
56,58,59,//w3
60,61,62,
60,62,63,//w4
64,65,66,
64,66,67};//w5

//places walls into game
void wallSet(wall in, int index){
//x vertices
vertexarray[132+(index*12)]=in.x+.4f;
vertexarray[135+(index*12)]=in.x+.4f;
vertexarray[138+(index*12)]=in.x-.6f;
vertexarray[141+(index*12)]=in.x-.6f;
//y vertices
vertexarray[133+(index*12)]=float((in.l*5)-6);
vertexarray[136+(index*12)]=float((in.l*5)-2);
vertexarray[139+(index*12)]=float((in.l*5)-2);
vertexarray[142+(index*12)]=float((in.l*5)-6);
//colors
if(in.c == 0){
colorarray[176+(index*16)]=0.0f;
colorarray[177+(index*16)]=0.0f;
colorarray[178+(index*16)]=1.0f;
colorarray[180+(index*16)]=0.0f;
colorarray[181+(index*16)]=0.0f;
colorarray[182+(index*16)]=1.0f;
colorarray[184+(index*16)]=0.0f;
colorarray[185+(index*16)]=0.0f;
colorarray[186+(index*16)]=1.0f;
colorarray[188+(index*16)]=0.0f;
colorarray[189+(index*16)]=0.0f;
colorarray[190+(index*16)]=1.0f;
}
if(in.c == 1){
colorarray[176+(index*16)]=0.0f;
colorarray[177+(index*16)]=1.0f;
colorarray[178+(index*16)]=0.0f;
colorarray[180+(index*16)]=0.0f;
colorarray[181+(index*16)]=1.0f;
colorarray[182+(index*16)]=0.0f;
colorarray[184+(index*16)]=0.0f;
colorarray[185+(index*16)]=1.0f;
colorarray[186+(index*16)]=0.0f;
colorarray[188+(index*16)]=0.0f;
colorarray[189+(index*16)]=1.0f;
colorarray[190+(index*16)]=0.0f;
}
if(in.c == 2){
colorarray[176+(index*16)]=1.0f;
colorarray[177+(index*16)]=0.0f;
colorarray[178+(index*16)]=0.0f;
colorarray[180+(index*16)]=1.0f;
colorarray[181+(index*16)]=0.0f;
colorarray[182+(index*16)]=0.0f;
colorarray[184+(index*16)]=1.0f;
colorarray[185+(index*16)]=0.0f;
colorarray[186+(index*16)]=0.0f;
colorarray[188+(index*16)]=1.0f;
colorarray[189+(index*16)]=0.0f;
colorarray[190+(index*16)]=0.0f;
}
if(in.c == 3){
colorarray[176+(index*16)]=1.0f;
colorarray[177+(index*16)]=1.0f;
colorarray[178+(index*16)]=1.0f;
colorarray[180+(index*16)]=1.0f;
colorarray[181+(index*16)]=1.0f;
colorarray[182+(index*16)]=1.0f;
colorarray[184+(index*16)]=1.0f;
colorarray[185+(index*16)]=1.0f;
colorarray[186+(index*16)]=1.0f;
colorarray[188+(index*16)]=1.0f;
colorarray[189+(index*16)]=1.0f;
colorarray[190+(index*16)]=1.0f;
}

};

void init(){

glEnable(GL_DEPTH_TEST);
glViewport(0, 0, 1200, 600);	
}

void display(SDL_Window* screen){
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

//detect walls in group 2
if((frameCount+31)%60==0){
if((frameCount+31)%120==0){
for(int i = 0; i < 3; i++){
if(walls[i].active && walls[i].l == level && walls[i].c != col){
cout << "You Lost. Your Score: " << frameCount << endl;
exit(0);
}
}
}
else{//detect walls in group 1
for(int i = 0; i < 3; i++){
if(walls[i+3].active && walls[i+3].l == level && walls[i+3].c != col){
cout << "You Lost. Your Score: " << frameCount << endl;
exit(0);
}
}
}
}

if(frameCount == 10800){//win condition
cout << "You Won! Your Score: " << frameCount << endl;
exit(0);
}

//if player gets "stuck" move them to where they should be
if(jump==0 && fall==0 && vertexarray[1] != -9.0f && vertexarray[1] != -4.0f && vertexarray[1] != 0.0f && vertexarray[1] != 5.0f){
vertexarray[1]=vertexarray[10]=(level*5)-4;
vertexarray[4]=vertexarray[7]=(level*5)-6;
}

//.5walls/sec
walls[0].active = 1;

//1 wall/sec
if(frameCount > 1499){
walls[3].active = 1;
}
//2walls/sec
if(frameCount > 2999){
walls[1].active = walls[4].active = 1;
}
//3walls/sec
if(frameCount > 6499){
walls[2].active = walls[5].active = 1;
}

//sets up walls in group 2
if((frameCount > 100 && frameCount%120==0)){
for(int i = 0; i < 3; i++){
walls[i].x+=24.0f;
if(walls[i].active){
walls[i].randomize();
wallSet(walls[i], i);
}
}
}

uniqueRand(true);//reset unique generator

//set up walls in group 1

if((frameCount > 100 && (frameCount+60)%120==0)){
for(int i = 0; i < 3; i++){
walls[i+3].x+=24.0f;
if(walls[i+3].active){
walls[i+3].randomize();
wallSet(walls[i+3], i+3);
}
}
}

uniqueRand(true);//reset

//move guide boxes from left of screen to right
count++;
if(count == 63){
vertexarray[60]+=25.0f;
vertexarray[63]+=25.0f;
vertexarray[66]+=25.0f;
vertexarray[69]+=25.0f;
vertexarray[84]+=25.0f;
vertexarray[87]+=25.0f;
vertexarray[90]+=25.0f;
vertexarray[93]+=25.0f;
vertexarray[108]+=25.0f;
vertexarray[111]+=25.0f;
vertexarray[114]+=25.0f;
vertexarray[117]+=25.0f;
}
if(count == 125){
count=0;
vertexarray[72]+=25.0f;
vertexarray[75]+=25.0f;
vertexarray[78]+=25.0f;
vertexarray[81]+=25.0f;
vertexarray[96]+=25.0f;
vertexarray[99]+=25.0f;
vertexarray[102]+=25.0f;
vertexarray[105]+=25.0f;
vertexarray[120]+=25.0f;
vertexarray[123]+=25.0f;
vertexarray[126]+=25.0f;
vertexarray[129]+=25.0f;
}

//move player to right
vertexarray[12]+=.2f;
vertexarray[15]+=.2f;
vertexarray[18]+=.2f;
vertexarray[21]+=.2f;
vertexarray[24]+=.2f;
vertexarray[27]+=.2f;
vertexarray[30]+=.2f;
vertexarray[33]+=.2f;
vertexarray[36]+=.2f;
vertexarray[39]+=.2f;
vertexarray[42]+=.2f;
vertexarray[45]+=.2f;
vertexarray[48]+=.2f;
vertexarray[51]+=.2f;
vertexarray[54]+=.2f;
vertexarray[57]+=.2f;

cubeTran.x-=.2;//move camera right

//jump action
switch(jump){
case 5:
vertexarray[1]+=2.0f;
vertexarray[4]+=2.0f;
vertexarray[7]+=2.0f;
vertexarray[10]+=2.0f;
jump--;
break;
case 4:
vertexarray[1]+=1.25f;
vertexarray[4]+=1.25f;
vertexarray[7]+=1.25f;
vertexarray[10]+=1.25f;
jump--;
break;
case 3:
vertexarray[1]+=.75f;
vertexarray[4]+=.75f;
vertexarray[7]+=.75f;
vertexarray[10]+=.75f;
jump--;
break;
case 2:
vertexarray[1]+=.6f;
vertexarray[4]+=.6f;
vertexarray[7]+=.6f;
vertexarray[10]+=.6f;
jump--;
break;
case 1:
vertexarray[1]+=.4f;
vertexarray[4]+=.4f;
vertexarray[7]+=.4f;
vertexarray[10]+=.4f;
level++;
jump--;
break;
}

//fall action
switch(fall){
case 5:
vertexarray[1]-=.4f;
vertexarray[4]-=.4f;
vertexarray[7]-=.4f;
vertexarray[10]-=.4f;
fall--;
break;
case 4:
vertexarray[1]-=.6f;
vertexarray[4]-=.6f;
vertexarray[7]-=.6f;
vertexarray[10]-=.6f;
fall--;
break;
case 3:
vertexarray[1]-=.75f;
vertexarray[4]-=.75f;
vertexarray[7]-=.75f;
vertexarray[10]-=.75f;
fall--;
break;
case 2:
vertexarray[1]-=1.25f;
vertexarray[4]-=1.25f;
vertexarray[7]-=1.25f;
vertexarray[10]-=1.25f;
fall--;
break;
case 1:
vertexarray[1]-=2.0f;
vertexarray[4]-=2.00f;
vertexarray[7]-=2.00f;
vertexarray[10]-=2.0f;
fall=0;
break;
}

//fall if on same color platform
if(level==col){
level--;
fall=5;
}

glGenVertexArrays(1,&vaoID);
glBindVertexArray(vaoID);

glGenBuffers(3, vboID);
glBindBuffer(GL_ARRAY_BUFFER,vboID[0]);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray),vertexarray,GL_STATIC_DRAW);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
glBufferData(GL_ARRAY_BUFFER,sizeof(colorarray),colorarray,GL_STATIC_DRAW);
glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
glGenBuffers(1,&eboID);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);

ShaderInfo s[]={
   { GL_VERTEX_SHADER , "vertexshader.glsl"},
   { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
   { GL_NONE , NULL}
   };

   prog=initShaders(s);
  
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
//


glm::mat4 trans;

trans = glm::translate(trans,cubeTran);

GLint tmpLocation = glGetUniformLocation(prog,"modelMatrix");
glUniformMatrix4fv(tmpLocation,1,GL_FALSE,&trans[0][0]);

glDrawElements(GL_TRIANGLES,102,GL_UNSIGNED_BYTE,NULL);
frameCount++;

glFlush();
SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){

//keyboard events
SDL_Event event;
while (SDL_PollEvent(&event)){
switch (event.type){
case SDL_QUIT:exit(0);break;
case SDL_KEYDOWN:
switch(event.key.keysym.sym){
case SDLK_ESCAPE:exit(0);
case SDLK_w:break;
case SDLK_s:
colorarray[1]=colorarray[5]=colorarray[9]=colorarray[13]=1.0f;
colorarray[0]=colorarray[2]=
colorarray[4]=colorarray[6]=
colorarray[8]=colorarray[10]=
colorarray[12]=colorarray[14]=0.0f;
col=1;
break;
case SDLK_a:
colorarray[0]=colorarray[4]=colorarray[8]=colorarray[12]=1.0f;
colorarray[1]=colorarray[2]=
colorarray[5]=colorarray[6]=
colorarray[9]=colorarray[10]=
colorarray[13]=colorarray[14]=0.0f;
col=2;
break;
case SDLK_d:
colorarray[2]=colorarray[6]=colorarray[10]=colorarray[14]=1.0f;
colorarray[0]=colorarray[1]=
colorarray[4]=colorarray[5]=
colorarray[8]=colorarray[9]=
colorarray[12]=colorarray[13]=0.0f;
col=0;
break;
case SDLK_e:scalar+=.2f;break;
case SDLK_q:scalar-=.2f;break;
case SDLK_i:pit-=0.05;break;
case SDLK_k:pit+=0.05;break;
case SDLK_j:yaw-=0.05;break;
case SDLK_l:yaw+=0.05;break;
case SDLK_SPACE:
if(jump == 0 && fall == 0 && level != 2){
jump=5;}
break;
}
}
}
}


int main(int argc, char **argv){
glutInit(&argc, argv);
time_t randSeed;
time(&randSeed);
srand(randSeed);

for(int i = 0; i < 3; i++){
walls[i].x = 12.0f;
walls[i+3].x = 24.0f;
}

SDL_Window *window;
//initialize SDL
if(SDL_Init(SDL_INIT_VIDEO)<0){
fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
     SDL_Quit();
     exit(1);
}

window = SDL_CreateWindow("Lighting",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1000,500,SDL_WINDOW_OPENGL);

if(window==NULL){
fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
}

SDL_GLContext glcontext=SDL_GL_CreateContext(window);

//initialize glew
   glewExperimental=GL_TRUE;
   if(glewInit()){
     fprintf(stderr, "Unable to initalize GLEW");
     exit(EXIT_FAILURE);
   }
  
init();
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
while(true){
input(window);
display(window);
}

SDL_GL_DeleteContext(glcontext);
   SDL_DestroyWindow(window);
   SDL_Quit();
 
   return 0;
}
