#include<GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <stdlib.h>
#include<stdio.h>
#include <time.h>

#define black 0
#define blue 1
#define red 2
#define Braxos 3
#define Psalidi 4
#define Xarti 5
#define rows 12
#define columns 15
#define step 40
#define width 480
#define height 660
#define IMAGE_ROWS 40
#define IMAGE_COLS 40


//===============GLOBAL VARIABLES=============================

static int enable=0;//enables filling the window with cubes
int mouse_enable=0;
int pick=0;
int cordinates[2];
int sq1Pos[2];
int sq2Pos[2];
int num_of_moves;
int moves=0;
int TrioBeginCellCol[2];
int TrioBeginCellRow[2];
int score=0;
int countPlayerMoves;
int tutorialEnable;
//=============================================================

//============================Prototypes================================

void createAndFillArray(); bool adjacent(); void display(); bool endGame(); void findCellPosition(int x,int y);
void findTrioCol(); void findTrioRow(); void initGL(); void KeyBoard(unsigned int key,int x,int y);
void mouse(int button,int state,int x,int y); void print(); void printArray(int row,int column);
void renderBitmapString(int x,int y,char * string); void squareClicked(int x,int y); void swapCells();
void takeUserInput(); void destroyTrioCol(); void destroyTrioRow(); void destroyAdj_by_8(); void destroy_dist_2(int cellValue);
void destroy_dist_3(int cellValue); void dropQubes(); void play(); void displayStarMessage(); void readFile();
void DrawImage(int x,int y);void destroyAdj_by_8_cols(); void destroy_dist_2_cols(int cellvalue);
void destroy_dist_3_cols(int cellvalue);
//========================================================================================


//=============================ARRAY FUNCTIONS=============================================
typedef struct SQUARE{
    int value;
    int posRow;//Y(height)
    int posColumn;//X(width)

}Square;

Square array[12][15];

void createAndFillArray(){
    srand(time(0));
   int i,j;
   for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
        array[i][j].value=rand()%5+1;
    }
   }
}


void print(){
   int i,j;
   for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
        printf("(%d,%d):%d\n",i,j,array[i][j].value);
    }
   }
}

bool findCol(){//checks each column for 3 equal consecutive squares
    int i,j;
    for(i=0;i<rows;i++){//columns
        for(j=0;j<columns;j++){//rows
            if(j>12){
                continue;
            }
            else if(array[i][j].value==array[i][j+1].value && array[i][j].value==array[i][j+2].value){
                return true;

            }
        }
    }
    return false;


}


void findTrioCol(){//checks each column for 3 equal consecutive squares
    int i,j;
    for(i=0;i<rows;i++){//columns
        for(j=0;j<columns;j++){//rows
            if(j>12){
                continue;
            }
            else if(array[i][j].value==array[i][j+1].value && array[i][j].value==array[i][j+2].value){
                TrioBeginCellCol[0]=i;//columns
                TrioBeginCellCol[1]=j;//rows

                if(array[i][j].value==1 || array[i][j].value==2){//if trio is only red or blue square
                    destroyTrioCol();



                }
                else{
                    destroyAdj_by_8_cols();

                }

            }
        }
    }


}

bool findRow(){//checks each row for 3 equal consecutive squares
    int i,j;
    for(j=0;j<columns;j++){
        for(i=0;i<rows;i++){
            if(i>9){
                continue;
            }
            else if(array[i][j].value==array[i+1][j].value && array[i][j].value==array[i+2][j].value){
                    return true;
            }
        }
    }
    return false;

}


void findTrioRow(){//checks each row for 3 equal consecutive squares
    int i,j;
    for(j=0;j<columns;j++){
        for(i=0;i<rows;i++){
            if(i>9){
                continue;
            }
            else if(array[i][j].value==array[i+1][j].value && array[i][j].value==array[i+2][j].value){

                TrioBeginCellRow[0]=i;//stands for columns
                TrioBeginCellRow[1]=j;//stands for rows

                if(array[i][j].value==1 || array[i][j].value==2){//if trio is only red or blue square
                    destroyTrioRow();



                }
                else{
                    destroyAdj_by_8();


                }
            }
    }

}

}

void printArray(int row,int column){
    printf("%d\n",array[row][column].value);

}

void swapCells(){
    int temp;
    moves+=1;
    countPlayerMoves-=1;
    temp=array[sq1Pos[1]][sq1Pos[0]].value;
    array[sq1Pos[1]][sq1Pos[0]].value=array[sq2Pos[1]][sq2Pos[0]].value;//because array is array[column][row] and not [row][column]
    array[sq2Pos[1]][sq2Pos[0]].value=temp;
    while(findCol()==true || findRow()==true){
        findTrioCol();
        findTrioRow();
        dropQubes();
        findCol();
        findRow();
    }
    //glutPostRedisplay();


}

void destroyTrioCol(){
    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]+1].value=0;
    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]+2].value=0;
    score+=10;
}
void destroyTrioRow(){
    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
    array[TrioBeginCellRow[0]+1][TrioBeginCellRow[1]].value=0;
    array[TrioBeginCellRow[0]+2][TrioBeginCellRow[1]].value=0;
    score+=10;
}

void destroyAdj_by_8(){//destroys both the trio and all the squares that the value of trio can eat
    int i,j;
    int cellValue=array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value;
    score+=10;
    for(j=TrioBeginCellRow[1]-1;j<TrioBeginCellRow[1]+2;j++){
        for(i=TrioBeginCellRow[0]-1;i<TrioBeginCellRow[0]+4;i++){

            if(i>11 || j<0 || i<0 || j>14){//so for not getting out of bounds in the array
                continue;
            }
            //Braxos(rock)
            if(array[i][j].value==4 && cellValue==3){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                    destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==3 && cellValue==3){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==2 && cellValue==3){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==1 && cellValue==3){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==0 && cellValue==3){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            //paper(xarti)
            else if(array[i][j].value==3 && cellValue==5){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==5 && cellValue==5){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==2 && cellValue==5){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==1 && cellValue==5){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==0 && cellValue==5){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            //scissor(psalidi)
            else if(array[i][j].value==5 && cellValue==4){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==4 && cellValue==4){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==2 && cellValue==4){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==1 && cellValue==4){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                     destroy_dist_3(cellValue);

            }
            else if(array[i][j].value==0 && cellValue==4){
                    score+=2;
                    array[i][j].value=0;
                    array[TrioBeginCellRow[0]][TrioBeginCellRow[1]].value=0;
                    destroy_dist_2(cellValue);
                    destroy_dist_3(cellValue);


            }
            else{
                score-=1;
            }

        }
    }

}


void destroyAdj_by_8_cols(){
    int i,j;
    int cellValue=array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value;
    score+=10;
    for(i=TrioBeginCellCol[0]-1;i<TrioBeginCellCol[0]+2;i++){
            //printf("j:%d\t",j);
        for(j=TrioBeginCellCol[1]-1;j<TrioBeginCellCol[1]+4;j++){
             //printf("i:%d\n",i);
            //printArray(i,j);
            //printArray(TrioBeginCellCol[0],TrioBeginCellCol[1]);

            if(i>11 || j<0 || i<0 || j>14){//so for not getting out of bounds in the array
                continue;
            }
            //Braxos(rock)
            if(array[i][j].value==4 && cellValue==3){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);
            }
            else if(array[i][j].value==3 && cellValue==3){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==2 && cellValue==3){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==1 && cellValue==3){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==0 && cellValue==3){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            //paper(xarti)
            else if(array[i][j].value==3 && cellValue==5){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==5 && cellValue==5){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==2 && cellValue==5){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==1 && cellValue==5){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==0 && cellValue==5){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            //scissor(psalidi)
            else if(array[i][j].value==5 && cellValue==4){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==4 && cellValue==4){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==2 && cellValue==4){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==1 && cellValue==4){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else if(array[i][j].value==0 && cellValue==4){
                    array[i][j].value=0;
                    array[TrioBeginCellCol[0]][TrioBeginCellCol[1]].value=0;
                    destroy_dist_2_cols(cellValue);
                    destroy_dist_3_cols(cellValue);

            }
            else{
                score-=1;
            }

        }
    }



}

void destroy_dist_2_cols(int cellValue){
    int i,j;//column,row
    for(i=TrioBeginCellCol[0]-2;i<TrioBeginCellCol[0]+3;i++){

        for(j=TrioBeginCellCol[1]-2;j<TrioBeginCellCol[1]+5;j++){
                if(i<0 ||j<0 || i>11 || j>14){//out of bound case
                    continue;
                }

                //rock eats scissor
                if(array[i][j].value==4 && cellValue==3){
                    array[i][j].value=0;
                    score+=3;
                }
                //scissor eats paper
               else if(array[i][j].value==5 && cellValue==4){
                    array[i][j].value=0;
                    score+=3;
                }
               //paper eats rock
               else if(array[i][j].value==3 && cellValue==5){
                    array[i][j].value=0;
                    score+=3;
                }
        }



    }


}

void destroy_dist_3_cols(int cellValue){
/*destroy squares in distance 2 that can be eaten by trio's value
(e.g if trio value is rock destroy all scissors in distance=2(meaning adjacency distance) from the trio)
*/
    int i,j;//column,row
   for(i=TrioBeginCellCol[0]-3;i<TrioBeginCellCol[0]+4;i++){

        for(j=TrioBeginCellCol[1]-3;j<TrioBeginCellCol[1]+6;j++){
                if(i<0 ||j<0 || i>11 || j>14){//out of bound case
                    continue;
                }

                //rock eats scissor
                if(array[i][j].value==4 && cellValue==3){
                    array[i][j].value=0;
                    score+=3;
                }
                //scissor eats paper
               else if(array[i][j].value==5 && cellValue==4){
                    array[i][j].value=0;
                    score+=3;
                }
               //paper eats rock
               else if(array[i][j].value==3 && cellValue==5){
                    array[i][j].value=0;
                    score+=3;
                }
        }



    }

}

void destroy_dist_2(int cellValue){

/*destroy squares in distance 2 that can be eaten by trio's value
(e.g if trio value is rock destroy all scissors in distance=2(meaning adjacency distance) from the trio)
*/
    int i,j;//column,row
    for(j=TrioBeginCellRow[1]-2;j<TrioBeginCellRow[1]+3;j++){

        for(i=TrioBeginCellRow[0]-2;i<TrioBeginCellRow[0]+5;i++){
                if(i<0 ||j<0 || i>11 || j>14){//out of bound case
                    continue;
                }

                //rock eats scissor
                if(array[i][j].value==4 && cellValue==3){
                    score+=3;
                    array[i][j].value=0;
                }
                //scissor eats paper
               else if(array[i][j].value==5 && cellValue==4){
                    score+=3;
                    array[i][j].value=0;
                }
               //paper eats rock
               else if(array[i][j].value==3 && cellValue==5){
                   score+=3;
                    array[i][j].value=0;
                }
        }



    }

}

void destroy_dist_3(int cellValue){
/*destroy squares in distance 2 that can be eaten by trio's value
(e.g if trio value is rock destroy all scissors in distance=2(meaning adjacency distance) from the trio)
*/
    int i,j;//column,row
    for(j=TrioBeginCellRow[1]-3;j<TrioBeginCellRow[1]+4;j++){

        for(i=TrioBeginCellRow[0]-3;i<TrioBeginCellRow[0]+6;i++){
                if(i<0 ||j<0 || i>11 || j>14){//out of bound case
                    continue;
                }

                //rock eats scissor
                if(array[i][j].value==4 && cellValue==3){
                    score+=3;
                    array[i][j].value=0;
                }
                //scissor eats paper
               else if(array[i][j].value==5 && cellValue==4){
                    score+=3;
                    array[i][j].value=0;
                }
               //paper eats rock
               else if(array[i][j].value==3 && cellValue==5){
                   score+=3;
                    array[i][j].value=0;
                }
        }




    }

}

void dropQubes(){
    int i,j;
    int temp;
    int clone_j;
    for(i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            clone_j=j;

            if(array[i][clone_j].value==0){
                    if(clone_j==0 && array[i][clone_j].value==0){
                        array[i][clone_j].value=rand()%5+1;
                    }
                while(clone_j!=0){
                    temp=array[i][clone_j].value;
                    array[i][clone_j].value=array[i][clone_j-1].value;
                    array[i][clone_j-1].value=temp;
                    clone_j=clone_j-1;
                    if(clone_j==0 && array[i][clone_j].value==0){
                        array[i][clone_j].value=rand()%5+1;
                    }
                }
            }

        }
    }
}

void play(){//destroys trios fill voids and searches again for any matched trio
    findTrioCol();
    findTrioRow();
    while(findCol()==true || findRow()==true){
        findTrioCol();
        findTrioRow();
        dropQubes();
        findCol();
        findRow();
    }
    score=0;

}


//======================================================================================================

bool endGame(){
    if(moves==num_of_moves){
        //printf("Press ESC to exit or 'b' to restart\n");
        mouse_enable=1;
        return true;
    }
    else return false;
}


//===========================DISPLAY TO SCREEN==========================================================
void renderBitmapString(int x,int y,char *string){//used for displaying text to screen
    char *c;
    glRasterPos2i(x,y);
    for(c=string; *c!='\0';c++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
    }
}

void displayStartMessage(){//it draws the menu
    glColor3f(0.8f,0.0f,0.0f);
        glBegin(GL_LINES);
            glVertex2i(240,640);
            glVertex2i(0,320);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(0,320);
            glVertex2i(240,0);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(240,0);
            glVertex2i(480,320);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(480,320);
            glVertex2i(240,640);
        glEnd();
        //second rombos

        glColor3f(0.0f,0.0f,0.8f);
        glBegin(GL_LINES);
            glVertex2i(240,600);
            glVertex2i(40,320);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(40,320);
            glVertex2i(240,40);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(240,40);
            glVertex2i(440,320);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(440,320);
            glVertex2i(240,600);
        glEnd();

        //third
        glColor3f(0.8f,0.0f,0.0f);
        glBegin(GL_LINES);
            glVertex2i(240,560);
            glVertex2i(80,320);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(80,320);
            glVertex2i(240,80);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(240,80);
            glVertex2i(400,320);
        glEnd();
        glBegin(GL_LINES);
            glVertex2i(400,320);
            glVertex2i(240,560);
        glEnd();

        //text
        glColor3f(0.0f,0.0f,1.0f);
        char beginGame[50]="Welcome to BraXaPsa II";
        renderBitmapString(140,315,beginGame);
        char beginGame2[50]="Press 'b' to begin";
        renderBitmapString(180,280,beginGame2);
        char beginGame3[50]="Press 't' for tutorial";
        renderBitmapString(170,240,beginGame3);

}

void drawQubes(int r,int c){
    glBegin(GL_QUADS);
        glVertex2i(r,c-step);
        glVertex2i(r,c);
        glVertex2i(r+step,c);
        glVertex2i(r+step,c-step);
    glEnd();
}

void drawOutline(int r,int c){
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
        //left
        glVertex2i(r,c-step);
        glVertex2i(r,c);
        //up
        glVertex2i(r,c);
        glVertex2i(r+step,c);
        //right
        glVertex2i(r+step,c);
        glVertex2i(r+step,c-step);
        //bottom
        glVertex2i(r+step,c-step);
        glVertex2i(r,c-step);
    glEnd();

}

void drawStats(){
    glColor3f(0.0f,0.0f,1.0f);
        char str_Endgame[40]="press ESC to exit or b to restart";
        char screenScore[20]="SCORE:";
        char StringScore[100];
        sprintf(StringScore,"%d",score);
        glColor3f(0.0f,0.0f,1.0f);
            renderBitmapString(3,40,screenScore);
            renderBitmapString(80,40,StringScore);

        if(endGame()){
            //game over message
            glColor3f(1.0f,0.0f,0.0f);
            renderBitmapString(200,10,str_Endgame);
            //score message

        }

}

void drawMoves(){
    char screenMoves[20]="Moves:";
    char StringCountMoves[50];
    sprintf(StringCountMoves,"%d",countPlayerMoves);
    renderBitmapString(70,7,StringCountMoves);
    renderBitmapString(3,7,screenMoves);

}

void drawTutorial(){
    if(tutorialEnable==1){
        int x,y;
        x=3;
        y=550;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        char msg1[30]="There are five types of qubes";
        char msg2[30]="1) rock(grey qube)";
        char msg3[30]="2) scissor(purple qube)";
        char msg4[30]="3) paper(white qube)";
        char msg5[30]="4) blue";
        char msg6[30]="5) red";
        char msg7[100]="Rock eats paper,paper eats rock,scissor eats paper";
        char msg8[100]=",blue and red are being eaten by all the previews";
        char msg9[100]="If you make three qubes of same type become neighbors";
        char msg95[100]="the will be destroyed";
        char msg10[100]="Try to make a high score...GOOD LUCK";

        char msg11[100]="Press 'b' to PLAY ";
        glColor3f(0.0f,0.0f,1.0f);
        renderBitmapString(x,y,msg1);
        renderBitmapString(x,y-30,msg2);
        renderBitmapString(x,y-60,msg3);
        renderBitmapString(x,y-90,msg4);
        renderBitmapString(x,y-120,msg5);
        renderBitmapString(x,y-150,msg6);
        renderBitmapString(x,y-180,msg7);
        renderBitmapString(x,y-210,msg8);
        renderBitmapString(x,y-240,msg9);
        renderBitmapString(x,y-270,msg95);
        renderBitmapString(x,y-300,msg10);

        glColor3f(1.0f,0.0f,0.0f);
        renderBitmapString(80,y-350,msg11);


    }

}

void display(){

        glClear(GL_COLOR_BUFFER_BIT);
        int i;//used to iterate rows of array
        int j;//used to iterate columns of array
        int r;//used for iterating through width
        int c;//used for iterating through height

    displayStartMessage();
    drawTutorial();

    if(enable==1){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawStats();
        drawMoves();

        for(i=0,r=0;i<rows;i++,r+=40){
                for(j=0,c=height;j<columns;j++,c=c-40){
                    if(array[i][j].value==1){//blue quad
                        glColor3f(0.0f,0.0f,1.0f);//blue color
                            drawQubes(r,c);
                            //create ouline of cube
                            drawOutline(r,c);
                    }
                    else if(array[i][j].value==2){//red quad
                         glColor3f(1.0f,0.0f,0.0f);//red color
                            drawQubes(r,c);
                            //create ouline of cube
                            drawOutline(r,c);
                    }
                    else if(array[i][j].value==3){//Braxos
                         glColor3f(0.2f,0.2f,0.2f);//grey color
                            drawQubes(r,c);
                            //create ouline of cube
                            drawOutline(r,c);
                    }
                    else if(array[i][j].value==4){//Psalidi
                        glColor3f(1.0f,0.0f,1.0f);//purple color
                            drawQubes(r,c);
                            //create ouline of cube
                            drawOutline(r,c);
                    }
                   else if(array[i][j].value==5){//Xarti
                        glColor3f(1.0f,1.0f,1.0f);//white color
                            drawQubes(r,c);
                            //create ouline of cube
                            drawOutline(r,c);
                    }
                    else if(array[i][j].value==0){//destroyed
                        glColor3f(0.0f,0.0f,0.0f);//black color
                            drawQubes(r,c);
                            //create ouline of cube
                            drawOutline(r,c);
                    }


                }
        }
    }

    glFlush();

}



//======================================================================================================



//find position in the matrix of the square that has been clicked
void findCellPosition(int x,int y){
    /* make float operation and then multiply with integers */
    /* x=columns and y=rows */
        float help_row,help_column;
        int row,column;

        help_row=(y/590.0);//used to calculate float data
        row=help_row*15;//keeps the integer part

        help_column=(x/480.0);
        column=help_column*12;
        cordinates[0]=row;
        cordinates[1]=column;
}

void squareClicked(int x,int y){
    findCellPosition(x,y);
    pick+=1;
    array[cordinates[0]][cordinates[1]].posRow=cordinates[0];
    array[cordinates[0]][cordinates[1]].posColumn=cordinates[1];
}

bool adjacent(){
    int x1,y1,x2,y2;
    x1=sq1Pos[0];
    x2=sq2Pos[0];
    y1=sq1Pos[1];
    y2=sq2Pos[1];
    if(x1==x2 && y2==y1+1){//west
        return true;
    }
    else if(x1==x2 && y2==y1-1){//east
        return true;
    }
    else if(x2==(x1+1) && y1==y2){//north
        return true;
    }
    else if(x2==(x1-1) && y1==y2){//south
        return true;
    }
    else{
        return false;
    }
}

void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f,0.0f,0.0f,1.0f); // Black and opaque
    //set the clipping area
    gluOrtho2D(0,width,0,height);
    tutorialEnable=0;

}


//=============================Keyboard========================================
void KeyBoard(unsigned char key, int x, int y) {
    if(key==27){
        exit(0);
    }
    else if(key =='b'){
        enable=1;
        tutorialEnable=0;
        score=0;
        mouse_enable=0;
        moves=0;
        countPlayerMoves=num_of_moves;
        createAndFillArray();
        glutPostRedisplay();
        play();
    }
    else if(key=='t'){
        tutorialEnable=1;
        glutPostRedisplay();
    }

}

void takeUserInput(){
    printf("Give number of moves\n");
    scanf("%d",&num_of_moves);
    countPlayerMoves=num_of_moves;
}

//==================================================================================



void mouse(int button,int state,int x,int y){
    int i;
    int MouseX,MouseY;
    if(mouse_enable==1){
        glutSetCursor(GLUT_CURSOR_NONE);
    }

    else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && y<=590){
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        MouseX=x;
        MouseY=y;
            squareClicked(MouseX,MouseY);

            if(pick==1){
                for(i=0;i<2;i++){
                    sq1Pos[i]=cordinates[i];
                }
            }
            else if(pick==2){
                for(i=0;i<2;i++){
                    sq2Pos[i]=cordinates[i];
                }
                pick=0;
                if(adjacent()){
                    swapCells();
                }

            }
    }

}



int main(int argc, char** argv) {
   takeUserInput();
   glutInit(&argc, argv);// Initialize GLUT
   glutInitWindowSize(12*40,15*40+60);
   glutInitWindowPosition(50,50);
   glutCreateWindow("BraXaPsa II");
   glutDisplayFunc(display);
   glutKeyboardFunc(KeyBoard);
   glutMouseFunc(mouse);
   initGL();
   glutMainLoop();

}
