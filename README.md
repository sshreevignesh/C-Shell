An attempt to recreate the Linux Command Line using CA Command line terminal built using C

### Use the following commands for running the code ##
  ```
  make
  ./shell
  ```

### Files ##
  shell.c- Contains the main function and the main loop
  builtin.c- Contains the code for the builtin functions(cd,pwd,echo and exit)
  ls.c - Contains the code for ls function
  display.c - Contains the display function which displays the shell prompt
  operation.c - Takes the input from shell.c and calls the corresponding functions for the command given
  pinfo.c - Contains the code for the pinfo command
  bonus.c - Contains the code for the bonus part
