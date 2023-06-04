#ifndef _FORMAT_H
#define	_FORMAT_H

#include <stdarg.h>
#include<stdlib.h>
#include <stdio.h>

#include <string>
using namespace std;
string format(const char *pFormat, ...);
string vFormat(const char *pFormat, va_list args);

#endif	/* _FORMAT_H */

