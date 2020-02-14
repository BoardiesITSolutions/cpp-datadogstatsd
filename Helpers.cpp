#include "Helpers.h"

int Helpers::rand()
{
	srand(time(0));
	return ::rand() % Helpers::rand_mx();
}

int Helpers::rand_mx()
{
	return INT_MAX;
}
