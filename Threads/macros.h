#define SYSC(value,command,message) if ((value = command) == -1){perror(message); exit(EXIT_FAILURE); }
#define SYSCN(value,command,message) if ((value = command) == NULL){perror(message); exit(EXIT_FAILURE); }
#define SYST(command) if (commmand == -1){perror("Thread Error"); exit(EXIT_FAILURE); }
