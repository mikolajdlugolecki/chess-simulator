#include "Program.h"

int main(void)
{
	Program* instance = new Program;
    instance->run();
    delete instance;
	return EXIT_SUCCESS;
}
