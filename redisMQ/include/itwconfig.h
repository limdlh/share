#ifndef ITWCONFIG_H
#define ITWCONFIG_H

#include <string>
using namespace std;

const int CFG_HASHTABLE_LEN = 32;
//const char *DEFAULT_CONFIG_FILENAME= "itworks.conf";
void getRootPath(char* buff, int buflen);

class ITWCONFIG {
	typedef struct CFG_ENTRY {
		char key[64];
		char value[64];
		CFG_ENTRY* next;
	} CFG_ENTRY;

	typedef struct CFG_HASH_ENTRY {
		CFG_ENTRY *link;
	} CFG_HASH_ENTRY;

	CFG_HASH_ENTRY cfgHash[CFG_HASHTABLE_LEN];

private:
	void init_cfg(const char *cfgFilename);
	void uninit_cfg();
	int cfgHashKey(const char *key);
	void StrTrim(char *str, const char *ss);

public:
	string getValueByKey(const char* key);
	ITWCONFIG();
	ITWCONFIG(char* cfgFilename);
	ITWCONFIG(string cfgFilename);
	~ITWCONFIG();
};
#endif
