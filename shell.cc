#include "shell.hh"
#include <fstream>
#include <unistd.h>
#include <syscall.h>
int main()
{
    std::string input;
	std::string prompt;
 	int fd = syscall(SYS_open, "prompt.txt", O_RDONLY, 0755);
	char byte[1];
	while(syscall(SYS_read, fd, byte, 1)) {
		prompt.append(byte);
    }
		
    while(true)
    {
        std::cout << prompt;                   // Print het prompt
        std::getline(std::cin, input); 
        if (input == "new_file") new_file();   // Kies de functie
        else if (input == "ls") list();        //   op basis van
        else if (input == "src") src();        //   de invoer
        else if (input == "find") find();
        else if (input == "seek") seek();
        else if (input == "exit") return 0;
        else if (input == "quit") return 0;
        else if (input == "error") return 1;

        if (std::cin.eof()) return 0;      // EOF is een exit
    }
}

void new_file() {
    std::string tmp = "";
    std::string tmp1 = "";
	std::cout << "Voer een bestandsnaam in en druk op enter: "; 
	std::getline(std::cin, tmp1);
	const char* bestandsnaam[] = {tmp1};
	int fd = syscall(SYS_creat, bestandsnaam[0], 0644);
	std::cout << "Voer nu de inhoud van text bestand in, sluit af met EOF: " << std::endl; 
    std::string text_file_str;
    while(std::getline(std::cin, tmp)) {
          if(tmp == "EOF") {
            break;
          }
          text_file_str += tmp + "\n";
    }

	const char* text_file[] = {text_file_str.c_str()};
    syscall(SYS_write, fd, text_file[0], text_file_str.size());
}

void list()
{   
    if (syscall(SYS_fork) == 0) {
        const char* flags[] = {"/bin/ls", "-l", "-a", NULL};
        int pd = syscall(SYS_execve, flags[0], flags, NULL);
    }
    else {
        syscall(SYS_wait4, 0, NULL, NULL);
    }
}

void find() // ToDo: Implementeer volgens specificatie.
{
    std::cout << "Voer in waarop je wilt filteren: " << std::endl;
    std::string input_find = "";
    std::getline(std::cin, input_find);
    int fd[2];
    syscall(SYS_pipe, &fd);
    if(syscall(SYS_fork) == 0) {
        syscall(SYS_close, fd[0]);
        syscall(SYS_dup2, fd[1], 1);
        const char* flags[] = {"/usr/bin/find", ".", NULL};
        int pd = syscall(SYS_execve, flags[0], flags, NULL);
    }
    else {
        syscall(SYS_wait4, 0, NULL, NULL);
    }
    if(syscall(SYS_fork) == 0) {
        syscall(SYS_close, fd[1]);
        syscall(SYS_dup2, fd[0], 0);
        const char* flags2[] = {"/bin/grep", input_find.c_str(), NULL};
        syscall(SYS_execve, flags2[0], flags2, NULL);
    }
    else {
        syscall(SYS_close, fd[1]);
        syscall(SYS_close, fd[0]);
        syscall(SYS_wait4, 0, NULL, NULL);
    }

}

void seek() // ToDo: Implementeer volgens specificatie.
{
    int fd = syscall(SYS_creat, "seek.txt", 0755);
    char byte[1] = {'x'};
    char byte1[1] = {'\0'};
    syscall(SYS_write, fd, byte, 1);
    syscall(SYS_lseek, fd, 5000000, 0);
    syscall(SYS_write, fd, byte, 1);  
    int fd1 = syscall(SYS_creat, "loop.txt", 0755);
    syscall(SYS_write, fd1, byte, 1);
    for(int i = 0; i <= 5000000; i++) {
        syscall(SYS_write, fd1, byte1, 1);
    }
    syscall(SYS_write, fd1, byte, 1);
}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{
    int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
    char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
    while(syscall(SYS_read, fd, byte, 1)) {                 // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
        std::cout << byte;                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
    }
}
