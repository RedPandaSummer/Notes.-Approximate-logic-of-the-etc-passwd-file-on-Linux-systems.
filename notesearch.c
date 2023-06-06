#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FILENAME "/path/to/your/notes_file"

void fatal(char*);
void *ec_malloc(unsigned_int);
int print_notes(int, int, char*); //Function that prints notes.
int find_user_note(int, int);     //Function that searching for the user's note in file.
int search_note(char*, char*);    //Function that searching notes by keywords.


int main(int argc, char *argv[])
{
int userid, printing=1, fd;

char searchstring[100];

if(argc>1)				//If there is argument then it is string for searching
{strcpy(searchstring, argv[1]);}
else					//Else string is NULL
{searchstring[0]=0;}

userid=getuid();
fd=open(FILENAME, O_RDONLY); //We are opening the file for the read only.
if(fd == -1)
{fatal("in function main() while opening the file for reading");}

while(printing)
{printing=print_notes(fd, userid, searchstring);}

printf("-------[The end of the note data]----------\n");
close(fd);

return 0;
}



//Function to output notes for a specific uid, matching an optional search string;
//at the end of the file returns 0, if there are more notes, returns 1.
int print_notes(int fd, int uid, char *searchstring)
{
int note_length;

char byte=0, note_buffer[100];

note_length = find_user_note(fd, uid);

if(note_length == -1) //If it is the end of the file returns 0.
{return 0;}

read(fd, note_buffer, note_length); //Reading for data of the note.
note_buffer[note_length] = 0; //The ending of the string.

if(search_note(note_buffer, searchstring)) //If the searchstring detected
{printf(note_buffer);}                     //printing the note.
return 1;
}






//Function to search for the next note for the specified userID ;
//returns -1 when end of file is reached;
//otherwise returns the length of the detected note.
int find_user_note(int fd, int user_uid)
{
int note_uid = -1;
unsigned char byte;
int length;


while(note_uid != user_uid) //While the note for the user_uid was not found.
{
if(read(fd, &note_uid, 4) != 4) //Reading the data of uid.
{return -1;}                    //If 4 bytes were not read, return the end of the file.
if(read(fd, &byte, 1) != 1)     //Reading the newline character.
{return -1;}

byte=length=0;
while(byte != '\n') //Determine the number of bytes to the end of the string.
{
if (read(fd, &byte, 1) != 1) //Reading 1 byte.
{return -1;}                 //If the byte was not read, return the end of the file.
length++;
}
}

off_t offset = (length*-1);
lseek(fd, offset, SEEK_CUR); //shift the read position by length bytes
printf("[DEBUG] %d bytes long note was found for id %d\n", length, note_uid);

return length;
}




//Keyword search function.
//Returns 1 if there is a match and 0 if there is none.
int search_note(char *note, char *keyword)
{

int i, keyword_length, match=0;

keyword_length=strlen(keyword);

if(keyword_length == 0) //If there is no search string,
{return 1;}             //Always a match.

for(i=0; i<strlen(note); ++i) //Byte view note.
{
if(note[i] == keyword[match]) //If the byte matches the keyword.
{match++;}                    //Preparing to check the next byte.
else
{
if(note[i] == keyword[0]) //If the byte matches the first byte of the keyword.
{match=1;}                //Start counting match from 1.
else
{match=0;}                //Otherwise it is zero.
}
if(match == keyword_length)//In case of complete match
{return 1;}                //Returning code 1.

}

return 0; //Returning code 0.

}




void fatal(char *message)
{
char error_message[100];

strcpy(error_message, "[!!] Critical error ");
strncat(error_message, message, 83);
perror(error_message);
exit(-1);
}


void *ec_malloc(unsigned int size)
{
void *ptr;
ptr = malloc(size);
if(ptr == NULL)
{fatal("in the function ec_malloc() when allocating memory");}
return ptr;
}



