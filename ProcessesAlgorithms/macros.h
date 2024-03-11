#define SYSC(value,command,message) if ((value = command) == -1){perror(message); exit(errno); }
#define SYSCN(value,command,message) if ((value = command) == NULL){perror(message); exit(errno); }
