#include<string>
using namespace std;
#include "format.h"

string format(const char *pFormat, ...) {
	char *pBuffer = NULL;
	va_list arguments;
	va_start(arguments, pFormat);
	if (vasprintf(&pBuffer, pFormat, arguments) == -1) {
		va_end(arguments);
		return "";
	}
	va_end(arguments);
	string result;
	if (pBuffer != NULL) {
		result = pBuffer;
		free(pBuffer);
	}
	return result;
}

