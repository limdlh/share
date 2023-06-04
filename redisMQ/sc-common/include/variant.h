#ifndef _MY_VARIANT_H
#define _MY_VARIANT_H

#include <stdint.h>
#include <time.h>


#include <string>
#include <map>
using namespace std;


#define Timestamp struct tm


typedef enum _VariantType {
	V_NULL=1 ,
	V_UNDEFINED ,
	V_BOOL ,
	V_INT8 ,
	V_INT16 ,
	V_INT32 ,
	V_INT64 ,
	V_UINT8 ,
	V_UINT16 ,
	V_UINT32 ,
	V_UINT64 ,
	V_DOUBLE ,
	_V_NUMERIC ,
	V_TIMESTAMP ,
	V_DATE ,
	V_TIME ,
	V_STRING ,
	V_TYPED_MAP ,
	V_MAP ,
	V_BYTEARRAY 

} VariantType;


struct VariantMap;

class Variant{
private:
	VariantType _type;

	union {
		bool b;
		int8_t i8;
		int16_t i16;
		int32_t i32;
		int64_t i64;
		uint8_t ui8;
		uint16_t ui16;
		uint32_t ui32;
		uint64_t ui64;
		double d;
		Timestamp *t;
		string *s;
		VariantMap *m;
	} _value;	
	char _pNumericKey[11];
public:
	Variant();
	Variant(const Variant &val);
	
	Variant(const bool &val);
	Variant(const int8_t &val);
	Variant(const int16_t &val);
	Variant(const int32_t &val);
	Variant(const int64_t &val);
	Variant(const uint8_t &val);
	Variant(const uint16_t &val);
	Variant(const uint32_t &val);
	Variant(const uint64_t &val);
	Variant(const double &val);
	
	Variant(const Timestamp &time);
	Variant(const uint16_t year, const uint8_t month, const uint8_t day);
	Variant(const uint8_t hour, const uint8_t min, const uint8_t sec, const uint16_t m);
	Variant(const uint16_t year, const uint8_t month, const uint8_t day,
			const uint8_t hour, const uint8_t min, const uint8_t sec, const uint16_t m);
	
	Variant(const char *pValue);
	Variant(const string &value);
	Variant(const uint8_t *pValue, uint32_t len);
	
	virtual ~Variant();
	void Reset(bool isUndefined = false);
	string ToString(string name = "", uint32_t indent = 0);
	
	Variant & operator=(const Variant &val);
	Variant & operator=(const bool &val);
	Variant & operator=(const int8_t &val);
	Variant & operator=(const int16_t &val);
	Variant & operator=(const int32_t &val);
	Variant & operator=(const int64_t &val);
	Variant & operator=(const uint8_t &val);
	Variant & operator=(const uint16_t &val);
	Variant & operator=(const uint32_t &val);
	Variant & operator=(const uint64_t &val);
	Variant & operator=(const double &val);	
	Variant & operator=(const Timestamp &val);
	Variant & operator=(const char *pVal);
	Variant & operator=(const string &val);
	
	operator VariantType();
	operator bool();
	operator int8_t();
	operator int16_t();
	operator int32_t();
	operator int64_t();
	operator uint8_t();
	operator uint16_t();
	operator uint32_t();
	operator uint64_t();
	operator double();
	operator Timestamp();
	operator string();

	Variant & operator[](const string &key);
	Variant & operator[](const char *key);
	Variant & operator[](const uint32_t &key);
	Variant & operator[](Variant &key);
	Variant & GetValue(string key, bool caseSensitive);
	
	bool operator==(const Variant &value) const;
	bool operator!=(const Variant &value) const;
	bool operator==(const char *pValue) const;
	bool operator!=(const char *pValue) const;
	bool operator==(const string &value) const;
	bool operator!=(const string &value) const;
	bool operator==(const VariantType value) const;
	bool operator!=(const VariantType value) const;
	
	time_t GetTimeT();
	string GetTypeName();
	void SetTypeName(string name);
	bool HasKey(const string &key, bool caseSensitive = true);
	bool HasIndex(const uint32_t index);
	bool HasKeyChain(VariantType end, bool caseSensitive, uint32_t depth, ...);
	void RemoveKey(const string &key, bool caseSensitive = true);
	void RemoveAt(const uint32_t index);
	void RemoveAllKeys();
	uint32_t MapSize();
	uint32_t MapDenseSize();
	void PushToArray(Variant value);

	map<string, Variant>::iterator begin();
	map<string, Variant>::iterator end();

	bool IsTimestamp(VariantType &type);
	bool IsNumeric();
	bool IsArray();
	void IsArray(bool isArray);
	bool IsByteArray();
	void IsByteArray(bool isByteArray);
	bool ConvertToTimestamp();
	void Compact();
	
	static Variant Now();
	
	static bool DeserializeFromJSON(string &raw, Variant &result, uint32_t &start);
	static bool DeserializeFromJSON(string raw,Variant &result);
	bool SerializeToJSON(string &result);
private:
	void InternalCopy(const Variant &val);
	void NormalizeTs();
	static void EscapeJSON(string &value);
	static void UnEscapeJSON(string &value);
	static bool ReadJSONWhiteSpace(string &raw, uint32_t &start);
	static bool ReadJSONDelimiter(string &raw, uint32_t &start, char &c);
	static bool ReadJSONString(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONNumber(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONObject(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONArray(string &raw, Variant &result, uint32_t &start);
	static bool ReadJSONBool(string &raw, Variant &result, uint32_t &start, string wanted);
	static bool ReadJSONNull(string &raw, Variant &result, uint32_t &start);


};


struct VariantMap {
	string typeName;
	map<string, Variant> children;
	bool isArray;

	VariantMap(VariantMap & variantMap) {
		typeName = variantMap.typeName;
		children = variantMap.children;
		isArray = variantMap.isArray;
	}

	VariantMap() {
		isArray = false;
	}
	
	
};


#endif
