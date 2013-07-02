#include "badleveldescription.h"

const char *BadLevelDescription::what() const throw()
{
	char *what = (char*)malloc(sizeof(char) * 34);
	snprintf(what, 34, "Found unexepected character: '%c'", bad_);
	return what;
}
