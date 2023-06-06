# Notes.-Approximate-logic-of-the-etc-passwd-file-on-Linux-systems.
Two files with code in C language for writing notes and searching for notes. 
When searching for notes, displays only those written by the user with the current ID.
Compile the notetaker.c file in the following way:
# (dont forget to change the path to the file in the text of code(/path/to/your/notes_file) to your path to your file):
gcc -o notetaker notetaker.c
sudo chown root:root ./notetaker.c
sudo chmod u+s ./notetaker
Then add some notes to the file:
./notetaker "This is some random note"
Change the user and add the some notes:
sudo su anotheruser
./notetaker "This is note of user anotheruser"
Compile the notesearch.c file in the following way:
gcc -o notesearch notesearch.c
sudo chown root:root ./notesearch
sudo chmod u+s ./notesearch
Test the notesearch program with different users.
./notesearch
All user notes have identifiers attached to them. 
It would seem that since the suid bits are set for the notetaker and notesearch programs, 
it means that they have full access to read from the data file and write data to the file, 
but the notesearch program logic does not allow the user to view other people's notes.
Similarly, /etc/passwd on Linux systems stores information about all users (such as passwords), 
but programs like chsh and passwd allow any user to change their password or shell.
