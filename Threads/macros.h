#define SYSC(value,command,message) if ((value = command) == -1){perror(message); exit(errno); }
#define SYSCN(value,command,message) if ((value = command) == NULL){perror(message); exit(errno); }
#define SYST(command) if (commmand == -1){perror("Thread Error"); exit(errno); }
