
#include "logging.h"
#include "format.h"
#include "linuxplatform.h"
string alowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
static map<int, SignalFnc> _signalHandlers;



string GetEnvVariable(const char *pEnvVarName) {
	char *pTemp = getenv(pEnvVarName);
	if (pTemp != NULL)
		return pTemp;
	return "";
}

void replace(string &target, string search, string replacement) {
	if (search == replacement)
		return;
	if (search == "")
		return;
	string::size_type i = string::npos;
	string::size_type lastPos = 0;
	while ((i = target.find(search, lastPos)) != string::npos) {
		target.replace(i, search.length(), replacement);
		lastPos = i + replacement.length();
	}
}

bool fileExists(string path) {
	struct stat fileInfo;
	if (stat(STR(path), &fileInfo) == 0) {
		return true;
	} else {
		return false;
	}
}
int fileSize(string path){
	struct stat buf;
	if (stat(path.c_str(), &buf) != 0) return -1;
	return buf.st_size;
}
string lowerCase(string value) {
	return changeCase(value, true);
}

string upperCase(string value) {
	return changeCase(value, false);
}

string changeCase(string &value, bool lowerCase) {
	string result = "";
	for (string::size_type i = 0; i < value.length(); i++) {
		if (lowerCase)
			result += tolower(value[i]);
		else
			result += toupper(value[i]);
	}
	return result;
}

string tagToString(uint64_t tag) {
	string result;
	for (uint32_t i = 0; i < 8; i++) {
		uint8_t v = (tag >> ((7 - i)*8)&0xff);
		if (v == 0)
			break;
		result += (char) v;
	}
	return result;
}

bool setMaxFdCount(uint32_t &current, uint32_t &max) {
	//1. reset stuff
	current = 0;
	max = 0;
	struct rlimit limits;
	memset(&limits, 0, sizeof (limits));

	//2. get the current value
	if (getrlimit(RLIMIT_NOFILE, &limits) != 0) {
		int err = errno;
		FATAL("getrlimit failed: (%d) %s", err, strerror(err));
		return false;
	}
	current = (uint32_t) limits.rlim_cur;
	max = (uint32_t) limits.rlim_max;

	//3. Set the current value to max value
	limits.rlim_cur = limits.rlim_max;
	if (setrlimit(RLIMIT_NOFILE, &limits) != 0) {
		int err = errno;
		FATAL("setrlimit failed: (%d) %s", err, strerror(err));
		return false;
	}

	//4. Try to get it back
	memset(&limits, 0, sizeof (limits));
	if (getrlimit(RLIMIT_NOFILE, &limits) != 0) {
		int err = errno;
		FATAL("getrlimit failed: (%d) %s", err, strerror(err));
		return false;
	}
	current = (uint32_t) limits.rlim_cur;
	max = (uint32_t) limits.rlim_max;


	return true;
}

bool enableCoreDumps() {
	struct rlimit limits;
	memset(&limits, 0, sizeof (limits));

	memset(&limits, 0, sizeof (limits));
	if (getrlimit(RLIMIT_CORE, &limits) != 0) {
		int err = errno;
		FATAL("getrlimit failed: (%d) %s", err, strerror(err));
		return false;
	}

	limits.rlim_cur = limits.rlim_max = RLIM_INFINITY;
	if (setrlimit(RLIMIT_CORE, &limits) != 0) {
		int err = errno;
		FATAL("setrlimit failed: (%d) %s", err, strerror(err));
		return false;
	}

	memset(&limits, 0, sizeof (limits));
	if (getrlimit(RLIMIT_CORE, &limits) != 0) {
		int err = errno;
		FATAL("getrlimit failed: (%d) %s", err, strerror(err));
		return false;
	}

	return limits.rlim_cur == RLIM_INFINITY;
}

bool setFdJoinMulticast(SOCKET sock, string bindIp, uint16_t bindPort, string ssmIp) {
	if (ssmIp == "") {
		struct ip_mreq group;
		group.imr_multiaddr.s_addr = inet_addr(STR(bindIp));
		group.imr_interface.s_addr = INADDR_ANY;
		if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				(char *) &group, sizeof (group)) < 0) {
			int err = errno;
			FATAL("Adding multicast failed. Error was: (%d) %s", err, strerror(err));
			return false;
		}
		return true;
	} else {
		struct group_source_req multicast;
		struct sockaddr_in *pGroup = (struct sockaddr_in*) &multicast.gsr_group;
		struct sockaddr_in *pSource = (struct sockaddr_in*) &multicast.gsr_source;

		memset(&multicast, 0, sizeof (multicast));

		//Setup the group we want to join
		pGroup->sin_family = AF_INET;
		pGroup->sin_addr.s_addr = inet_addr(STR(bindIp));
		pGroup->sin_port = EHTONS(bindPort);

		//setup the source we want to listen
		pSource->sin_family = AF_INET;
		pSource->sin_addr.s_addr = inet_addr(STR(ssmIp));
		if (pSource->sin_addr.s_addr == INADDR_NONE) {
			FATAL("Unable to SSM on address %s", STR(ssmIp));
			return false;
		}
		pSource->sin_port = 0;

		INFO("Try to SSM on ip %s", STR(ssmIp));

		if (setsockopt(sock, IPPROTO_IP, MCAST_JOIN_SOURCE_GROUP, &multicast,
				sizeof (multicast)) < 0) {
			int err = errno;
			FATAL("Adding multicast failed. Error was: (%d) %s", err,
					strerror(err));
			return false;
		}

		return true;
	}
}

bool setFdNonBlock(SOCKET fd) {
	int32_t arg;
	if ((arg = fcntl(fd, F_GETFL, NULL)) < 0) {
		int err = errno;
		FATAL("Unable to get fd flags: (%d) %s", err, strerror(err));
		return false;
	}
	arg |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, arg) < 0) {
		int err = errno;
		FATAL("Unable to set fd flags: (%d) %s", err, strerror(err));
		return false;
	}

	return true;
}

bool setFdNoSIGPIPE(SOCKET fd) {
	//This is not needed because we use MSG_NOSIGNAL when using
	//send/write functions
	return true;
}

bool setFdKeepAlive(SOCKET fd, bool isUdp) {
	if (isUdp)
		return true;
	int32_t one = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
			(const char*) & one, sizeof (one)) != 0) {
		FATAL("Unable to set SO_NOSIGPIPE");
		return false;
	}
	return true;
}

bool setFdNoNagle(SOCKET fd, bool isUdp) {
	if (isUdp)
		return true;
	int32_t one = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) & one, sizeof (one)) != 0) {
		return false;
	}
	return true;
}

bool setFdReuseAddress(SOCKET fd) {
	int32_t one = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) & one, sizeof (one)) != 0) {
		FATAL("Unable to reuse address");
		return false;
	}
#ifdef SO_REUSEPORT
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (char *) & one, sizeof (one)) != 0) {
		FATAL("Unable to reuse port");
		return false;
	}
#endif /* SO_REUSEPORT */
	return true;
}

bool setFdTTL(SOCKET fd, uint8_t ttl) {
	int temp = ttl;
	if (setsockopt(fd, IPPROTO_IP, IP_TTL, &temp, sizeof (temp)) != 0) {
		int err = errno;
		WARN("Unable to set IP_TTL: %"PRIu8"; error was (%d) %s", ttl, err, strerror(err));
	}
	return true;
}

bool setFdMulticastTTL(SOCKET fd, uint8_t ttl) {
	int temp = ttl;
	if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &temp, sizeof (temp)) != 0) {
		int err = errno;
		WARN("Unable to set IP_MULTICAST_TTL: %"PRIu8"; error was (%d) %s", ttl, err, strerror(err));
	}
	return true;
}

bool setFdTOS(SOCKET fd, uint8_t tos) {
	int temp = tos;
	if (setsockopt(fd, IPPROTO_IP, IP_TOS, &temp, sizeof (temp)) != 0) {
		int err = errno;
		WARN("Unable to set IP_TOS: %"PRIu8"; error was (%d) %s", tos, err, strerror(err));
	}
	return true;
}

int32_t __maxSndBufValUdp = 0;
int32_t __maxRcvBufValUdp = 0;
int32_t __maxSndBufValTcp = 0;
int32_t __maxRcvBufValTcp = 0;
SOCKET __maxSndBufSocket = -1;

bool DetermineMaxRcvSndBuff(int option, bool isUdp) {
	int32_t &maxVal = isUdp ?
			((option == SO_SNDBUF) ? __maxSndBufValUdp : __maxRcvBufValUdp)
			: ((option == SO_SNDBUF) ? __maxSndBufValTcp : __maxRcvBufValTcp);
	CLOSE_SOCKET(__maxSndBufSocket);
	__maxSndBufSocket = -1;
	__maxSndBufSocket = socket(AF_INET, isUdp ? SOCK_DGRAM : SOCK_STREAM, 0);
	if (__maxSndBufSocket < 0) {
		FATAL("Unable to create testing socket");
		return false;
	}

	int32_t known = 0;
	int32_t testing = 0x7fffffff;
	int32_t prevTesting = testing;
	//	FINEST("---- isUdp: %d; option: %s ----", isUdp, (option == SO_SNDBUF ? "SO_SNDBUF" : "SO_RCVBUF"));
	while (known != testing) {
		//		assert(known <= testing);
		//		assert(known <= prevTesting);
		//		assert(testing <= prevTesting);
		//		FINEST("%"PRId32" (%"PRId32") %"PRId32, known, testing, prevTesting);
		if (setsockopt(__maxSndBufSocket, SOL_SOCKET, option, (const char*) & testing,
				sizeof (testing)) == 0) {
			known = testing;
			testing = known + (prevTesting - known) / 2;
			//FINEST("---------");
		} else {
			prevTesting = testing;
			testing = known + (testing - known) / 2;
		}
	}
	CLOSE_SOCKET(__maxSndBufSocket);
	__maxSndBufSocket = -1;
	maxVal = known;
	//	FINEST("%s maxVal: %"PRId32, (option == SO_SNDBUF ? "SO_SNDBUF" : "SO_RCVBUF"), maxVal);
	return maxVal > 0;
}

bool setFdMaxSndRcvBuff(SOCKET fd, bool isUdp) {
	int32_t &maxSndBufVal = isUdp ? __maxSndBufValUdp : __maxSndBufValTcp;
	int32_t &maxRcvBufVal = isUdp ? __maxRcvBufValUdp : __maxRcvBufValTcp;
	if (maxSndBufVal == 0) {
		if (!DetermineMaxRcvSndBuff(SO_SNDBUF, isUdp)) {
			FATAL("Unable to determine maximum value for SO_SNDBUF");
			return false;
		}
	}

	if (maxRcvBufVal == 0) {
		if (!DetermineMaxRcvSndBuff(SO_RCVBUF, isUdp)) {
			FATAL("Unable to determine maximum value for SO_SNDBUF");
			return false;
		}
	}

	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*) & maxSndBufVal,
			sizeof (maxSndBufVal)) != 0) {
		FATAL("Unable to set SO_SNDBUF");
		return false;
	}

	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*) & maxRcvBufVal,
			sizeof (maxSndBufVal)) != 0) {
		FATAL("Unable to set SO_RCVBUF");
		return false;
	}
	return true;
}

bool setFdOptions(SOCKET fd, bool isUdp) {
	if (!isUdp) {
		if (!setFdNonBlock(fd)) {
			FATAL("Unable to set non block");
			return false;
		}
	}

	if (!setFdNoSIGPIPE(fd)) {
		FATAL("Unable to set no SIGPIPE");
		return false;
	}

	if (!setFdKeepAlive(fd, isUdp)) {
		FATAL("Unable to set keep alive");
		return false;
	}

	if (!setFdNoNagle(fd, isUdp)) {
		WARN("Unable to disable Nagle algorithm");
	}

	if (!setFdReuseAddress(fd)) {
		FATAL("Unable to enable reuse address");
		return false;
	}

	if (!setFdMaxSndRcvBuff(fd, isUdp)) {
		FATAL("Unable to set max SO_SNDBUF on UDP socket");
		return false;
	}

	return true;
}

bool deleteFile(string path) {
	if (remove(STR(path)) != 0) {
		FATAL("Unable to delete file `%s`", STR(path));
		return false;
	}
	return true;
}

bool deleteFolder(string path, bool force) {
	if (!force) {
		return deleteFile(path);
	} else {
		string command = format("rm -rf %s", STR(path));
		if (system(STR(command)) != 0) {
			FATAL("Unable to delete folder %s", STR(path));
			return false;
		}
		return true;
	}
}

bool createFolder(string path, bool recursive) {
	string command = format("mkdir %s %s",
			recursive ? "-p" : "",
			STR(path));
	if (system(STR(command)) != 0) {
		FATAL("Unable to create folder %s", STR(path));
		return false;
	}

	return true;
}

string getHostByName(string name) {
	struct hostent *pHostEnt = gethostbyname(STR(name));
	if (pHostEnt == NULL)
		return "";
	if (pHostEnt->h_length <= 0)
		return "";
	string result = format("%hhu.%hhu.%hhu.%hhu",
			(uint8_t) pHostEnt->h_addr_list[0][0],
			(uint8_t) pHostEnt->h_addr_list[0][1],
			(uint8_t) pHostEnt->h_addr_list[0][2],
			(uint8_t) pHostEnt->h_addr_list[0][3]);
	return result;
}

bool isNumeric(string value) {
	return value == format("%d", atoi(STR(value)));
}

void split(string str, string separator, vector<string> &result) {
	result.clear();
	string::size_type position = str.find(separator);
	string::size_type lastPosition = 0;
	uint32_t separatorLength = separator.length();

	while (position != str.npos) {
		ADD_VECTOR_END(result, str.substr(lastPosition, position - lastPosition));
		lastPosition = position + separatorLength;
		position = str.find(separator, lastPosition);
	}
	ADD_VECTOR_END(result, str.substr(lastPosition, string::npos));
}

uint64_t getTagMask(uint64_t tag) {
	uint64_t result = 0xffffffffffffffffLL;
	for (int8_t i = 56; i >= 0; i -= 8) {
		if (((tag >> i)&0xff) == 0)
			break;
		result = result >> 8;
	}
	return ~result;
}

string generateRandomString(uint32_t length) {
	string result = "";
	for (uint32_t i = 0; i < length; i++)
		result += alowedCharacters[rand() % alowedCharacters.length()];
	return result;
}

void lTrim(string &value) {
	string::size_type i = 0;
	for (i = 0; i < value.length(); i++) {
		if (value[i] != ' ' &&
				value[i] != '\t' &&
				value[i] != '\n' &&
				value[i] != '\r')
			break;
	}
	value = value.substr(i);
}

void rTrim(string &value) {
	int32_t i = 0;
	for (i = (int32_t) value.length() - 1; i >= 0; i--) {
		if (value[i] != ' ' &&
				value[i] != '\t' &&
				value[i] != '\n' &&
				value[i] != '\r')
			break;
	}
	value = value.substr(0, i + 1);
}

void trim(string &value) {
	lTrim(value);
	rTrim(value);
}

int8_t getCPUCount() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}

map<string, string> mapping(string str, string separator1, string separator2, bool trimStrings) {
	map<string, string> result;

	vector<string> pairs;
	split(str, separator1, pairs);

	FOR_VECTOR_ITERATOR(string, pairs, i) {
		if (VECTOR_VAL(i) != "") {
			if (VECTOR_VAL(i).find(separator2) != string::npos) {
				string key = VECTOR_VAL(i).substr(0, VECTOR_VAL(i).find(separator2));
				string value = VECTOR_VAL(i).substr(VECTOR_VAL(i).find(separator2) + 1);
				if (trimStrings) {
					trim(key);
					trim(value);
				}
				result[key] = value;
			} else {
				if (trimStrings) {
					trim(VECTOR_VAL(i));
				}
				result[VECTOR_VAL(i)] = "";
			}
		}
	}
	return result;
}

void splitFileName(string fileName, string &name, string & extension, char separator) {
	size_t dotPosition = fileName.find_last_of(separator);
	if (dotPosition == string::npos) {
		name = fileName;
		extension = "";
		return;
	}
	name = fileName.substr(0, dotPosition);
	extension = fileName.substr(dotPosition + 1);
}

double getFileModificationDate(string path) {
	struct stat s;
	if (stat(STR(path), &s) != 0) {
		FATAL("Unable to stat file %s", STR(path));
		return 0;
	}
	return (double) s.st_mtime;
}

string normalizePath(string base, string file) {
	char dummy1[PATH_MAX];
	char dummy2[PATH_MAX];
	char *pBase = realpath(STR(base), dummy1);
	char *pFile = realpath(STR(base + file), dummy2);

	if (pBase != NULL) {
		base = pBase;
	} else {
		base = "";
	}

	if (pFile != NULL) {
		file = pFile;
	} else {
		file = "";
	}

	if (file == "" || base == "") {
		return "";
	}

	if (file.find(base) != 0) {
		return "";
	} else {
		if (!fileExists(file)) {
			return "";
		} else {
			return file;
		}
	}
}

bool listFolder(string path, vector<string> &result, bool normalizeAllPaths,
		bool includeFolders, bool recursive) {
	if (path == "")
		path = ".";
	if (path[path.size() - 1] != PATH_SEPARATOR)
		path += PATH_SEPARATOR;

	DIR *pDir = NULL;
	pDir = opendir(STR(path));
	if (pDir == NULL) {
		int err = errno;
		FATAL("Unable to open folder: %s (%d) %s", STR(path), err, strerror(err));
		return false;
	}

	struct dirent *pDirent = NULL;
	while ((pDirent = readdir(pDir)) != NULL) {
		string entry = pDirent->d_name;
		if ((entry == ".")
				|| (entry == "..")) {
			continue;
		}
		if (normalizeAllPaths) {
			entry = normalizePath(path, entry);
		} else {
			entry = path + entry;
		}
		if (entry == "")
			continue;

		if (pDirent->d_type == DT_UNKNOWN) {
			struct stat temp;
			if (stat(STR(entry), &temp) != 0) {
				WARN("Unable to stat entry %s", STR(entry));
				continue;
			}
			pDirent->d_type = ((temp.st_mode & S_IFDIR) == S_IFDIR) ? DT_DIR : DT_REG;
		}

		switch (pDirent->d_type) {
			case DT_DIR:
			{
				if (includeFolders) {
					ADD_VECTOR_END(result, entry);
				}
				if (recursive) {
					if (!listFolder(entry, result, normalizeAllPaths, includeFolders, recursive)) {
						FATAL("Unable to list folder");
						closedir(pDir);
						return false;
					}
				}
				break;
			}
			case DT_REG:
			{
				ADD_VECTOR_END(result, entry);
				break;
			}
			default:
			{
				WARN("Invalid dir entry detected");
				break;
			}
		}
	}

	closedir(pDir);
	return true;
}
/*resultType 0 only file, 1 only dir,2 all*/
bool listFolder1(string path, vector<string> &result, bool normalizeAllPaths,
	int  resultType, bool recursive) {
	if (path == "")
		path = ".";
	if (path[path.size() - 1] != PATH_SEPARATOR)
		path += PATH_SEPARATOR;

	DIR *pDir = NULL;
	pDir = opendir(STR(path));
	if (pDir == NULL) {
		int err = errno;
		FATAL("Unable to open folder: %s (%d) %s", STR(path), err, strerror(err));
		return false;
	}

	struct dirent *pDirent = NULL;
	while ((pDirent = readdir(pDir)) != NULL) {
		string entry = pDirent->d_name;
		if ((entry == ".")
			|| (entry == "..")) {
			continue;
		}
		if (normalizeAllPaths) {
			entry = normalizePath(path, entry);
		}
		else {
			entry = path + entry;
		}
		if (entry == "")
			continue;

		if (pDirent->d_type == DT_UNKNOWN) {
			struct stat temp;
			if (stat(STR(entry), &temp) != 0) {
				WARN("Unable to stat entry %s", STR(entry));
				continue;
			}
			pDirent->d_type = ((temp.st_mode & S_IFDIR) == S_IFDIR) ? DT_DIR : DT_REG;
		}

		switch (pDirent->d_type) {
		case DT_DIR:
		{
					   if (resultType == 1 || resultType==2) {
						   ADD_VECTOR_END(result, entry);
					   }
					   if (recursive) {
						   if (!listFolder1(entry, result, normalizeAllPaths, resultType, recursive)) {
							   FATAL("Unable to list folder");
							   closedir(pDir);
							   return false;
						   }
					   }
					   break;
		}
		case DT_REG:
		{				
					   if (resultType == 0 || resultType == 2){
						   ADD_VECTOR_END(result, entry);
					   }
					   break;
		}
		default:
		{
				   WARN("Invalid dir entry detected");
				   break;
		}
		}
	}

	closedir(pDir);
	return true;
}

bool moveFile(string src, string dst) {
	if (rename(STR(src), STR(dst)) != 0) {
		FATAL("Unable to move file from `%s` to `%s`",
				STR(src), STR(dst));
		return false;
	}
	return true;
}

bool isAbsolutePath(string &path) {
	return (bool)((path.size() > 0) && (path[0] == PATH_SEPARATOR));
}

void signalHandler(int sig) {
	if (!MAP_HAS1(_signalHandlers, sig))
		return;
	_signalHandlers[sig]();
}

void installSignal(int sig, SignalFnc pSignalFnc) {
	_signalHandlers[sig] = pSignalFnc;
	struct sigaction action;
	action.sa_handler = signalHandler;
	action.sa_flags = 0;
	if (sigemptyset(&action.sa_mask) != 0) {
		ASSERT("Unable to install the quit signal");
		return;
	}
	if (sigaction(sig, &action, NULL) != 0) {
		ASSERT("Unable to install the quit signal");
		return;
	}
}

void installQuitSignal(SignalFnc pQuitSignalFnc) {
	installSignal(SIGTERM, pQuitSignalFnc);
}

void installConfRereadSignal(SignalFnc pConfRereadSignalFnc) {
	installSignal(SIGHUP, pConfRereadSignalFnc);
}

static time_t _gUTCOffset = -1;

void computeUTCOffset() {
	time_t now = time(NULL);
	struct tm *pTemp = localtime(&now);
	_gUTCOffset = pTemp->tm_gmtoff;
}

time_t getlocaltime() {
	if (_gUTCOffset == -1)
		computeUTCOffset();
	return getutctime() + _gUTCOffset;
}

time_t gettimeoffset() {
	if (_gUTCOffset == -1)
		computeUTCOffset();
	return _gUTCOffset;
}


int code_convert( const char *from_charset,const char *to_charset,  char *inbuf, size_t inlen,
		char *outbuf, size_t outlen) {
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
		return -1;
	iconv_close(cd);
	**pout = '\0';

	return 0;
}

int u2g(const char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	return code_convert("utf-8", "gb2312",(char*) inbuf, inlen, outbuf, outlen);
}

int g2u(const char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	return code_convert("gb2312", "utf-8",(char*) inbuf, inlen, outbuf, outlen);
}

string u2g(string src){
	int allocLen=src.length()*1.5;
	char * dest=new char [allocLen];
	string ret="";
	if(u2g(src.c_str(),src.length(),dest,allocLen)==0){
		ret=dest;
	}
	delete dest;
	return ret;
	
};

string g2u(string src){
	int allocLen=src.length()*1.5;
	char * dest=new char [allocLen];
	string ret="";
	if(g2u(src.c_str(),src.length(),dest,allocLen)==0){
		ret=dest;
	}
	delete[] dest;
	return ret;
};
static bool srand_flag = false;
string create_uuid(){
	unsigned char buff[16] = { 0 };
	int *tmp = (int*)buff;
	*tmp =(int) time(NULL);
	if (srand_flag == false){
		srand(*tmp);
		srand_flag = true;
	}
	tmp = (int*)(buff + 4);
	*tmp = rand();
	tmp = (int*)(buff + 8);
	*tmp = rand();
	tmp = (int*)(buff + 12);
	*tmp = rand();
	return format("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7], buff[8], buff[9], buff[10], buff[11], buff[12], buff[3], buff[14], buff[15]);
}
/*
%a 星期几的简写

%A 星期几的全称

%b 月份的简写

%B 月份的全称

%c 标准的日期的时间串

%C 年份的前两位数字

%d 十进制表示的每月的第几天

%D 月/天/年

%e 在两字符域中，十进制表示的每月的第几天

%F 年-月-日

%g 年份的后两位数字，使用基于周的年

%G 年份，使用基于周的年

%h 简写的月份名

%H 24小时制的小时

%I 12小时制的小时

%j 十进制表示的每年的第几天

%m 十进制表示的月份

%M 十时制表示的分钟数

%n 新行符

%p 本地的AM或PM的等价显示

%r 12小时的时间

%R 显示小时和分钟：hh:mm

%S 十进制的秒数

%t 水平制表符

%T 显示时分秒：hh:mm:ss

%u 每周的第几天，星期一为第一天 （值从1到7，星期一为1）

%U 第年的第几周，把星期日作为第一天（值从0到53）

%V 每年的第几周，使用基于周的年

%w 十进制表示的星期几（值从0到6，星期天为0）

%W 每年的第几周，把星期一做为第一天（值从0到53）

%x 标准的日期串

%X 标准的时间串

%y 不带世纪的十进制年份（值从0到99）

%Y 带世纪部分的十制年份

%z，%Z 时区名称，如果不能得到时区名称则返回空字符。

%% 百分号
*/
string from_unix_time(time_t time,const char* format){
	struct tm *localtm = localtime(&time);
	char tempBuff[64]={0};
	return string(tempBuff, strftime(tempBuff, 64, format, localtm));
}
//获取微秒
long get_utime(){
	 struct timeval tv;
   	 gettimeofday(&tv,NULL);
	 return tv.tv_sec*1000000 + tv.tv_usec;
}


