#include "apue.h"

int main(int argc, char* argv[])
{
	int n;
	if(argc<2){
		err_sys("Не введено значение буффера");
	}
        if(atoi(argv[1])==0){
		err_sys("введено некорректное значение буффера");
	}
	int bufferSize=atoi(argv[1]);
	char buf[bufferSize];

	while ((n = read(STDIN_FILENO, buf, bufferSize)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n)
			err_sys("ошибка записи");

	if (n < 0)
		err_sys("ошибка чтения");

	exit(0);
}
