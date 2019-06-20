#include "../include/SpellcorrectServer.h"

using namespace mm;

int main(void)
{
    SpellcorrectServer server("../conf");

    server.start();

	return 0;
}
