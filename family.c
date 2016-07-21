// Program Header Information ////////////////////////////////////////
/**
* @file family.c
*
* @brief program for PA01
* 
* @details Simulation of a family tree by using process creation and 
*          deletion system calls
*
* @version 1.00
*          C.S. Student (11 February 2016)
*          Initial development of PA01
*
* @note None
*/
// Program Description/Support /////////////////////////////////////
/*
 This program simulate a family tree by using process creation and
 deletion system calls. It will output the family tree according to
 the input file.
*/
// Precompiler Directives //////////////////////////////////////////
//
/////  NONE
//
// Header Files ///////////////////////////////////////////////////
//
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/wait.h>
   #include <sys/types.h>
   #include <unistd.h>
//
// Global Constant Definitions ////////////////////////////////////
//
   const int ROW_MAX = 10;
   const int COL_MAX = 10;
   const int COL_PARENT1 = 1;
   const int COL_PARENT2 = 2;
   const int CHILD_START_POSITION = 4;
//
// Class Definitions //////////////////////////////////////////////
//
///// NONE
//
// Free Function Prototypes ///////////////////////////////////////
//
   void spawn( int currentRow, int numOfTab, char treeArray[][COL_MAX] );
//
// Main Function Implementation ///////////////////////////////////
//
   int main( int argc, char* argv[] )
      {
       int rowIndex, colIndex;  // indexes of row and column
       char cursor;  // cursor of the input file
       FILE *filePtr;   // pointer of the input file
       char familyTreeArray[ROW_MAX][COL_MAX];  // 2D array of family tree

       // initialize the family tree
       for( rowIndex = 0; rowIndex < ROW_MAX; rowIndex ++ )
           {
            for( colIndex = 0; colIndex < COL_MAX; colIndex ++ )
               {
                familyTreeArray[rowIndex][colIndex] = 0;
               }
           }

       // set the array indexes
       rowIndex = 0;
       colIndex = 1;

       // open file
       filePtr = fopen( argv[1], "r" );

       // raise exception if cannot access file
       if( filePtr == NULL )
          {
           printf("FILE NOT FOUND!\n");
           exit( 1 );
          }
       else
          {
           // loop through the file
           while( ( cursor = fgetc( filePtr ) ) != EOF )
              {
               // copy data to array of family tree omit space and nextline
               if( ( cursor != ' ' ) && ( cursor != '\n' ) )
                  {
                   familyTreeArray[rowIndex][colIndex] = cursor;
                   colIndex ++;
                  }
               // when get nextline, increase row index
               if( cursor == '\n' )
                  {
                   familyTreeArray[rowIndex][0] = colIndex - 1;
                   rowIndex ++;
                   colIndex = 1;
                  }         
              }
            // end of loop
          }

       // close file
       fclose( filePtr );

       // print the first parent1 & 2
       printf( "%c-%c\n", familyTreeArray[0][COL_PARENT1], 
              familyTreeArray[0][COL_PARENT2] );

       // spawn children
       spawn( 0, 1, familyTreeArray );

       return 0;
      }   // end of main
//
// Free Function Implementation ///////////////////////////////////
/**
* @brief Function spawns child process 
*
* @details Function fork child process, check if child process has
*          child process and spawn
*
* @pre int currentRow contains parent row number
*
* @pre int numOfTab contains number of tabs should be inserted
*
* @pre char treeArray[][COL_MAX] contains the 2D array of family tree
*
* @post spawn each child on the current row
*
* @post if the child has child process, spawn
*
* @post parent wait until child finishes process
*
* @return None
*
*/
   void spawn
        ( 
         int currentRow, // holds parent row number
         int numOfTab, // holds number of tabs
         char treeArray[][COL_MAX] // holds data of family tree
        )
      {
       pid_t pid;  // process id
       int status = 0;  // status of child process
       int childIndex = CHILD_START_POSITION;  // index of child
       int tabIndex = 0;  // index to type tab
       int rowIndex = 0;  // index to search in rows

       // loop from first child to the last
       while( childIndex <= treeArray[currentRow][0] )
          {
           // fork child
           pid = fork();

           // child process
           if( pid == 0 )
              {
               // loop to type tabs
               for( tabIndex = 0; tabIndex < numOfTab; tabIndex++ )
                  {
                   printf("\t");
                  }

               // set row index
               rowIndex = 1;
               // loop to search if married to another
               while( ( treeArray[rowIndex][0] != 0 ) && 
                      ( rowIndex < COL_MAX ) )
                  {
                   // if married, print, spawn child and exit
                   if( treeArray[rowIndex][COL_PARENT1] == 
                      treeArray[currentRow][childIndex] )
                      {
                       printf( "%c-%c\n",treeArray[rowIndex][COL_PARENT1], 
                               treeArray[rowIndex][COL_PARENT2] );
                       spawn( rowIndex, numOfTab + 1, treeArray );
                       exit( 0 );
                      }

                   // row increment
                   rowIndex ++; 
                  }

               // if not, print and exit
               printf( "%c\n", treeArray[currentRow][childIndex] );
               exit( 0 );
              }

           // parent process
           else if( pid > 0 )
              {
               waitpid( pid, &status, 0 );
              }

           // fork fail
           else
              {
               printf( "Fork returned error code\n" );
               exit( 1 );
              }
           // child index increment
           childIndex ++;
          }
       // end of loop
      }   // end of spawn
