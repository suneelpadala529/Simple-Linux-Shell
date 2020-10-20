# Simple-Linux-Shell in C++
Implemeted the linux terminal with below commands using builtin System calls fork(), exec() 
1) cd
2) pushd : push the current directory to stack
3) popd : pop the top directory in stack
4) dirs: print directories in the stack
5) The path variable holds a list of possible paths in which to search for executables
   i) path (without arguments) displays all entries in the list separated by colons e.g., /bin:/usr/bin.
   ii) path + /some/dir appends the given pathname to the path list.
   iii) path - /some/dir removes the given pathname from the path list.
