#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include "bt.h"
typedef signed long TT_Fixed;
typedef signed short TT_FWord;
typedef unsigned short TT_UFWord;
typedef signed short TT_Short;
typedef unsigned short TT_UShort;
typedef signed long TT_Long;
typedef ULONG TT_ULONG;
typedef ULONG TT_Offset;
typedef unsigned long long LONGDATETIME;
typedef signed short F2Dot14;


class char_class {
	int small;
	std::vector<char> known_values;
	char value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(char);
	char operator () () { return value; }
	char_class(int small, std::vector<char> known_values = {}) : small(small), known_values(known_values) {}

	char generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(char), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(char), 0, known_values);
		}
		return value;
	}

	char generate(std::vector<char> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(char), 0, possible_values);
		return value;
	}
};



class char_array_class {
	char_class& element;
	std::vector<std::string> known_values;
	std::unordered_map<int, std::vector<char>> element_known_values;
	std::string value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::string& operator () () { return value; }
	char operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return value[index];
	}
	char_array_class(char_class& element, std::unordered_map<int, std::vector<char>> element_known_values = {})
		: element(element), element_known_values(element_known_values) {}
	char_array_class(char_class& element, std::vector<std::string> known_values)
		: element(element), known_values(known_values) {}

	std::string generate(unsigned size, std::vector<std::string> possible_values = {}) {
		check_array_length(size);
		_startof = FTell();
		value = "";
		if (possible_values.size()) {
			value = file_acc.file_string(possible_values);
			assert(value.length() == size);
			_sizeof = size;
			return value;
		}
		if (known_values.size()) {
			value = file_acc.file_string(known_values);
			assert(value.length() == size);
			_sizeof = size;
			return value;
		}
		if (!element_known_values.size()) {
			if (size == 0)
				 return "";
			value = file_acc.file_string(size);
			_sizeof = size;
			return value;
		}
		for (unsigned i = 0; i < size; ++i) {
			auto known = element_known_values.find(i);
			if (known == element_known_values.end()) {
				value.push_back(element.generate());
				_sizeof += element._sizeof;
			} else {
				value.push_back(file_acc.file_integer(sizeof(char), 0, known->second));
				_sizeof += sizeof(char);
			}
		}
		return value;
	}
};



class uint16_class {
	int small;
	std::vector<uint16> known_values;
	uint16 value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(uint16);
	uint16 operator () () { return value; }
	uint16_class(int small, std::vector<uint16> known_values = {}) : small(small), known_values(known_values) {}

	uint16 generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(uint16), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(uint16), 0, known_values);
		}
		return value;
	}

	uint16 generate(std::vector<uint16> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(uint16), 0, possible_values);
		return value;
	}
};



class uint32_class {
	int small;
	std::vector<uint32> known_values;
	uint32 value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(uint32);
	uint32 operator () () { return value; }
	uint32_class(int small, std::vector<uint32> known_values = {}) : small(small), known_values(known_values) {}

	uint32 generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(uint32), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(uint32), 0, known_values);
		}
		return value;
	}

	uint32 generate(std::vector<uint32> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(uint32), 0, possible_values);
		return value;
	}
};



class uint32_array_class {
	uint32_class& element;
	std::unordered_map<int, std::vector<uint32>> element_known_values;
	std::vector<uint32> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<uint32>& operator () () { return value; }
	uint32 operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return value[index];
	}
	uint32_array_class(uint32_class& element, std::unordered_map<int, std::vector<uint32>> element_known_values = {})
		: element(element), element_known_values(element_known_values) {}

	std::vector<uint32> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			auto known = element_known_values.find(i);
			if (known == element_known_values.end()) {
				value.push_back(element.generate());
				_sizeof += element._sizeof;
			} else {
				value.push_back(file_acc.file_integer(sizeof(uint32), 0, known->second));
				_sizeof += sizeof(uint32);
			}
		}
		return value;
	}
};



class TTCHeader {
	std::vector<TTCHeader*>& instances;

	std::string tag_var;
	uint16 majorVersion_var;
	uint16 minorVersion_var;
	uint32 numFonts_var;
	std::vector<uint32> offsetTable_var;
	std::string dsigTag_var;
	uint32 dsigLength_var;
	uint32 dsigOffset_var;

public:
	bool tag_exists = false;
	bool majorVersion_exists = false;
	bool minorVersion_exists = false;
	bool numFonts_exists = false;
	bool offsetTable_exists = false;
	bool dsigTag_exists = false;
	bool dsigLength_exists = false;
	bool dsigOffset_exists = false;

	std::string& tag() {
		assert_cond(tag_exists, "struct field tag does not exist");
		return tag_var;
	}
	uint16& majorVersion() {
		assert_cond(majorVersion_exists, "struct field majorVersion does not exist");
		return majorVersion_var;
	}
	uint16& minorVersion() {
		assert_cond(minorVersion_exists, "struct field minorVersion does not exist");
		return minorVersion_var;
	}
	uint32& numFonts() {
		assert_cond(numFonts_exists, "struct field numFonts does not exist");
		return numFonts_var;
	}
	std::vector<uint32>& offsetTable() {
		assert_cond(offsetTable_exists, "struct field offsetTable does not exist");
		return offsetTable_var;
	}
	std::string& dsigTag() {
		assert_cond(dsigTag_exists, "struct field dsigTag does not exist");
		return dsigTag_var;
	}
	uint32& dsigLength() {
		assert_cond(dsigLength_exists, "struct field dsigLength does not exist");
		return dsigLength_var;
	}
	uint32& dsigOffset() {
		assert_cond(dsigOffset_exists, "struct field dsigOffset does not exist");
		return dsigOffset_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	TTCHeader& operator () () { return *instances.back(); }
	TTCHeader& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	TTCHeader(std::vector<TTCHeader*>& instances) : instances(instances) { instances.push_back(this); }
	~TTCHeader() {
		if (generated == 2)
			return;
		while (instances.size()) {
			TTCHeader* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	TTCHeader* generate();
};

int TTCHeader::_parent_id = 0;
int TTCHeader::_index_start = 0;



class TT_Fixed_class {
	int small;
	std::vector<TT_Fixed> known_values;
	TT_Fixed value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(TT_Fixed);
	TT_Fixed operator () () { return value; }
	TT_Fixed_class(int small, std::vector<TT_Fixed> known_values = {}) : small(small), known_values(known_values) {}

	TT_Fixed generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(TT_Fixed), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(TT_Fixed), 0, known_values);
		}
		return value;
	}

	TT_Fixed generate(std::vector<TT_Fixed> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(TT_Fixed), 0, possible_values);
		return value;
	}
};



class USHORT_class {
	int small;
	std::vector<USHORT> known_values;
	USHORT value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(USHORT);
	USHORT operator () () { return value; }
	USHORT_class(int small, std::vector<USHORT> known_values = {}) : small(small), known_values(known_values) {}

	USHORT generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(USHORT), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(USHORT), 0, known_values);
		}
		return value;
	}

	USHORT generate(std::vector<USHORT> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(USHORT), 0, possible_values);
		return value;
	}
};



class tOffsetTable {
	std::vector<tOffsetTable*>& instances;

	TT_Fixed SFNT_Ver_var;
	USHORT numTables_var;
	USHORT searchRange_var;
	USHORT entrySelector_var;
	USHORT rangeShift_var;

public:
	bool SFNT_Ver_exists = false;
	bool numTables_exists = false;
	bool searchRange_exists = false;
	bool entrySelector_exists = false;
	bool rangeShift_exists = false;

	TT_Fixed& SFNT_Ver() {
		assert_cond(SFNT_Ver_exists, "struct field SFNT_Ver does not exist");
		return SFNT_Ver_var;
	}
	USHORT& numTables() {
		assert_cond(numTables_exists, "struct field numTables does not exist");
		return numTables_var;
	}
	USHORT& searchRange() {
		assert_cond(searchRange_exists, "struct field searchRange does not exist");
		return searchRange_var;
	}
	USHORT& entrySelector() {
		assert_cond(entrySelector_exists, "struct field entrySelector does not exist");
		return entrySelector_var;
	}
	USHORT& rangeShift() {
		assert_cond(rangeShift_exists, "struct field rangeShift does not exist");
		return rangeShift_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tOffsetTable& operator () () { return *instances.back(); }
	tOffsetTable& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tOffsetTable(std::vector<tOffsetTable*>& instances) : instances(instances) { instances.push_back(this); }
	~tOffsetTable() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tOffsetTable* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tOffsetTable* generate();
};

int tOffsetTable::_parent_id = 0;
int tOffsetTable::_index_start = 0;



class string_class {
	std::vector<std::string> known_values;
	std::string value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::string operator () () { return value; }
	string_class(std::vector<std::string> known_values = {}) : known_values(known_values) {}

	std::string generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_string();
		} else {
			value = file_acc.file_string(known_values);
		}
		_sizeof = value.length() + 1;
		return value;
	}
};



class ULONG_class {
	int small;
	std::vector<ULONG> known_values;
	ULONG value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(ULONG);
	ULONG operator () () { return value; }
	ULONG_class(int small, std::vector<ULONG> known_values = {}) : small(small), known_values(known_values) {}

	ULONG generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(ULONG), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(ULONG), 0, known_values);
		}
		return value;
	}

	ULONG generate(std::vector<ULONG> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(ULONG), 0, possible_values);
		return value;
	}
};



class tTable_Tag_union {
	std::vector<tTable_Tag_union*>& instances;

// union {
	std::string asChar_var;
	ULONG asLong_var;
// };

public:
	bool asChar_exists = false;
	bool asLong_exists = false;

	std::string& asChar() {
		assert_cond(asChar_exists, "struct field asChar does not exist");
		return asChar_var;
	}
	ULONG& asLong() {
		assert_cond(asLong_exists, "struct field asLong does not exist");
		return asLong_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tTable_Tag_union& operator () () { return *instances.back(); }
	tTable_Tag_union& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tTable_Tag_union(std::vector<tTable_Tag_union*>& instances) : instances(instances) { instances.push_back(this); }
	~tTable_Tag_union() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tTable_Tag_union* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tTable_Tag_union* generate();
};

int tTable_Tag_union::_parent_id = 0;
int tTable_Tag_union::_index_start = 0;



class tTable {
	std::vector<tTable*>& instances;

	tTable_Tag_union* Tag_var;
	ULONG checkSum_var;
	ULONG offset_var;
	ULONG length_var;

public:
	bool Tag_exists = false;
	bool checkSum_exists = false;
	bool offset_exists = false;
	bool length_exists = false;

	tTable_Tag_union& Tag() {
		assert_cond(Tag_exists, "struct field Tag does not exist");
		return *Tag_var;
	}
	ULONG& checkSum() {
		assert_cond(checkSum_exists, "struct field checkSum does not exist");
		return checkSum_var;
	}
	ULONG& offset() {
		assert_cond(offset_exists, "struct field offset does not exist");
		return offset_var;
	}
	ULONG& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tTable& operator () () { return *instances.back(); }
	tTable& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tTable(std::vector<tTable*>& instances) : instances(instances) { instances.push_back(this); }
	~tTable() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tTable* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tTable* generate();
};

int tTable::_parent_id = 0;
int tTable::_index_start = 0;



class BYTE_class {
	int small;
	std::vector<BYTE> known_values;
	BYTE value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(BYTE);
	BYTE operator () () { return value; }
	BYTE_class(int small, std::vector<BYTE> known_values = {}) : small(small), known_values(known_values) {}

	BYTE generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(BYTE), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(BYTE), 0, known_values);
		}
		return value;
	}

	BYTE generate(std::vector<BYTE> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(BYTE), 0, possible_values);
		return value;
	}
};



class BYTE_array_class {
	BYTE_class& element;
	std::unordered_map<int, std::vector<BYTE>> element_known_values;
	std::vector<BYTE> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<BYTE>& operator () () { return value; }
	BYTE operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return value[index];
	}
	BYTE_array_class(BYTE_class& element, std::unordered_map<int, std::vector<BYTE>> element_known_values = {})
		: element(element), element_known_values(element_known_values) {}

	std::vector<BYTE> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			auto known = element_known_values.find(i);
			if (known == element_known_values.end()) {
				value.push_back(element.generate());
				_sizeof += element._sizeof;
			} else {
				value.push_back(file_acc.file_integer(sizeof(BYTE), 0, known->second));
				_sizeof += sizeof(BYTE);
			}
		}
		return value;
	}
};



class tcmap_format0 {
	std::vector<tcmap_format0*>& instances;

	USHORT format_var;
	USHORT length_var;
	USHORT language_var;
	std::vector<BYTE> glyphIdArray_var;

public:
	bool format_exists = false;
	bool length_exists = false;
	bool language_exists = false;
	bool glyphIdArray_exists = false;

	USHORT& format() {
		assert_cond(format_exists, "struct field format does not exist");
		return format_var;
	}
	USHORT& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	USHORT& language() {
		assert_cond(language_exists, "struct field language does not exist");
		return language_var;
	}
	std::vector<BYTE>& glyphIdArray() {
		assert_cond(glyphIdArray_exists, "struct field glyphIdArray does not exist");
		return glyphIdArray_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format0& operator () () { return *instances.back(); }
	tcmap_format0& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format0(std::vector<tcmap_format0*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format0() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format0* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format0* generate(quad& cmap_subtable);
};

int tcmap_format0::_parent_id = 0;
int tcmap_format0::_index_start = 0;



class USHORT_array_class {
	USHORT_class& element;
	std::unordered_map<int, std::vector<USHORT>> element_known_values;
	std::vector<USHORT> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<USHORT>& operator () () { return value; }
	USHORT operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return value[index];
	}
	USHORT_array_class(USHORT_class& element, std::unordered_map<int, std::vector<USHORT>> element_known_values = {})
		: element(element), element_known_values(element_known_values) {}

	std::vector<USHORT> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			auto known = element_known_values.find(i);
			if (known == element_known_values.end()) {
				value.push_back(element.generate());
				_sizeof += element._sizeof;
			} else {
				value.push_back(file_acc.file_integer(sizeof(USHORT), 0, known->second));
				_sizeof += sizeof(USHORT);
			}
		}
		return value;
	}
};



class int16_class {
	int small;
	std::vector<int16> known_values;
	int16 value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(int16);
	int16 operator () () { return value; }
	int16_class(int small, std::vector<int16> known_values = {}) : small(small), known_values(known_values) {}

	int16 generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(int16), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(int16), 0, known_values);
		}
		return value;
	}

	int16 generate(std::vector<int16> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(int16), 0, possible_values);
		return value;
	}
};



class SubHeader {
	std::vector<SubHeader*>& instances;

	uint16 firstCode_var;
	uint16 entryCount_var;
	int16 idDelta_var;
	uint16 idRangeOffset_var;
	std::vector<USHORT> glyphIdArray_var;

public:
	bool firstCode_exists = false;
	bool entryCount_exists = false;
	bool idDelta_exists = false;
	bool idRangeOffset_exists = false;
	bool glyphIdArray_exists = false;

	uint16& firstCode() {
		assert_cond(firstCode_exists, "struct field firstCode does not exist");
		return firstCode_var;
	}
	uint16& entryCount() {
		assert_cond(entryCount_exists, "struct field entryCount does not exist");
		return entryCount_var;
	}
	int16& idDelta() {
		assert_cond(idDelta_exists, "struct field idDelta does not exist");
		return idDelta_var;
	}
	uint16& idRangeOffset() {
		assert_cond(idRangeOffset_exists, "struct field idRangeOffset does not exist");
		return idRangeOffset_var;
	}
	std::vector<USHORT>& glyphIdArray() {
		assert_cond(glyphIdArray_exists, "struct field glyphIdArray does not exist");
		return glyphIdArray_var;
	}

	/* locals */
	quad curPos;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	SubHeader& operator () () { return *instances.back(); }
	SubHeader& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	SubHeader(std::vector<SubHeader*>& instances) : instances(instances) { instances.push_back(this); }
	~SubHeader() {
		if (generated == 2)
			return;
		while (instances.size()) {
			SubHeader* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	SubHeader* generate();
};

int SubHeader::_parent_id = 0;
int SubHeader::_index_start = 0;



class SubHeader_array_class {
	SubHeader& element;
	std::vector<SubHeader*> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<SubHeader*>& operator () () { return value; }
	SubHeader operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return *value[index];
	}
	SubHeader_array_class(SubHeader& element) : element(element) {}

	std::vector<SubHeader*> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			value.push_back(element.generate());
			_sizeof += element._sizeof;
		}
		return value;
	}
};



class tcmap_format2 {
	std::vector<tcmap_format2*>& instances;

	USHORT format_var;
	USHORT length_var;
	USHORT language_var;
	std::vector<USHORT> subHeaderKeys_var;
	std::vector<SubHeader*> subHeaders_var;

public:
	bool format_exists = false;
	bool length_exists = false;
	bool language_exists = false;
	bool subHeaderKeys_exists = false;
	bool subHeaders_exists = false;

	USHORT& format() {
		assert_cond(format_exists, "struct field format does not exist");
		return format_var;
	}
	USHORT& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	USHORT& language() {
		assert_cond(language_exists, "struct field language does not exist");
		return language_var;
	}
	std::vector<USHORT>& subHeaderKeys() {
		assert_cond(subHeaderKeys_exists, "struct field subHeaderKeys does not exist");
		return subHeaderKeys_var;
	}
	std::vector<SubHeader*>& subHeaders() {
		assert_cond(subHeaders_exists, "struct field subHeaders does not exist");
		return subHeaders_var;
	}

	/* locals */
	int maxidx;
	int i;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format2& operator () () { return *instances.back(); }
	tcmap_format2& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format2(std::vector<tcmap_format2*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format2() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format2* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format2* generate(quad& cmap_subtable);
};

int tcmap_format2::_parent_id = 0;
int tcmap_format2::_index_start = 0;



class SHORT_class {
	int small;
	std::vector<SHORT> known_values;
	SHORT value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(SHORT);
	SHORT operator () () { return value; }
	SHORT_class(int small, std::vector<SHORT> known_values = {}) : small(small), known_values(known_values) {}

	SHORT generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(SHORT), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(SHORT), 0, known_values);
		}
		return value;
	}

	SHORT generate(std::vector<SHORT> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(SHORT), 0, possible_values);
		return value;
	}
};



class SHORT_array_class {
	SHORT_class& element;
	std::unordered_map<int, std::vector<SHORT>> element_known_values;
	std::vector<SHORT> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<SHORT>& operator () () { return value; }
	SHORT operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return value[index];
	}
	SHORT_array_class(SHORT_class& element, std::unordered_map<int, std::vector<SHORT>> element_known_values = {})
		: element(element), element_known_values(element_known_values) {}

	std::vector<SHORT> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			auto known = element_known_values.find(i);
			if (known == element_known_values.end()) {
				value.push_back(element.generate());
				_sizeof += element._sizeof;
			} else {
				value.push_back(file_acc.file_integer(sizeof(SHORT), 0, known->second));
				_sizeof += sizeof(SHORT);
			}
		}
		return value;
	}
};



class tcmap_format4 {
	std::vector<tcmap_format4*>& instances;

	USHORT format_var;
	USHORT length_var;
	USHORT language_var;
	USHORT segCountX2_var;
	USHORT searchRange_var;
	USHORT entrySelector_var;
	USHORT rangeShift_var;
	std::vector<USHORT> endCount_var;
	USHORT reservedPad_var;
	std::vector<USHORT> startCount_var;
	std::vector<SHORT> idDelta_var;
	std::vector<USHORT> idRangeOffset_var;
	std::vector<USHORT> glyphIdArray_var;

public:
	bool format_exists = false;
	bool length_exists = false;
	bool language_exists = false;
	bool segCountX2_exists = false;
	bool searchRange_exists = false;
	bool entrySelector_exists = false;
	bool rangeShift_exists = false;
	bool endCount_exists = false;
	bool reservedPad_exists = false;
	bool startCount_exists = false;
	bool idDelta_exists = false;
	bool idRangeOffset_exists = false;
	bool glyphIdArray_exists = false;

	USHORT& format() {
		assert_cond(format_exists, "struct field format does not exist");
		return format_var;
	}
	USHORT& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	USHORT& language() {
		assert_cond(language_exists, "struct field language does not exist");
		return language_var;
	}
	USHORT& segCountX2() {
		assert_cond(segCountX2_exists, "struct field segCountX2 does not exist");
		return segCountX2_var;
	}
	USHORT& searchRange() {
		assert_cond(searchRange_exists, "struct field searchRange does not exist");
		return searchRange_var;
	}
	USHORT& entrySelector() {
		assert_cond(entrySelector_exists, "struct field entrySelector does not exist");
		return entrySelector_var;
	}
	USHORT& rangeShift() {
		assert_cond(rangeShift_exists, "struct field rangeShift does not exist");
		return rangeShift_var;
	}
	std::vector<USHORT>& endCount() {
		assert_cond(endCount_exists, "struct field endCount does not exist");
		return endCount_var;
	}
	USHORT& reservedPad() {
		assert_cond(reservedPad_exists, "struct field reservedPad does not exist");
		return reservedPad_var;
	}
	std::vector<USHORT>& startCount() {
		assert_cond(startCount_exists, "struct field startCount does not exist");
		return startCount_var;
	}
	std::vector<SHORT>& idDelta() {
		assert_cond(idDelta_exists, "struct field idDelta does not exist");
		return idDelta_var;
	}
	std::vector<USHORT>& idRangeOffset() {
		assert_cond(idRangeOffset_exists, "struct field idRangeOffset does not exist");
		return idRangeOffset_var;
	}
	std::vector<USHORT>& glyphIdArray() {
		assert_cond(glyphIdArray_exists, "struct field glyphIdArray does not exist");
		return glyphIdArray_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format4& operator () () { return *instances.back(); }
	tcmap_format4& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format4(std::vector<tcmap_format4*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format4() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format4* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format4* generate(quad& cmap_subtable);
};

int tcmap_format4::_parent_id = 0;
int tcmap_format4::_index_start = 0;



class tcmap_format6 {
	std::vector<tcmap_format6*>& instances;

	USHORT format_var;
	USHORT length_var;
	USHORT language_var;
	USHORT firstCode_var;
	USHORT entryCount_var;
	std::vector<USHORT> glyphIdArray_var;

public:
	bool format_exists = false;
	bool length_exists = false;
	bool language_exists = false;
	bool firstCode_exists = false;
	bool entryCount_exists = false;
	bool glyphIdArray_exists = false;

	USHORT& format() {
		assert_cond(format_exists, "struct field format does not exist");
		return format_var;
	}
	USHORT& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	USHORT& language() {
		assert_cond(language_exists, "struct field language does not exist");
		return language_var;
	}
	USHORT& firstCode() {
		assert_cond(firstCode_exists, "struct field firstCode does not exist");
		return firstCode_var;
	}
	USHORT& entryCount() {
		assert_cond(entryCount_exists, "struct field entryCount does not exist");
		return entryCount_var;
	}
	std::vector<USHORT>& glyphIdArray() {
		assert_cond(glyphIdArray_exists, "struct field glyphIdArray does not exist");
		return glyphIdArray_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format6& operator () () { return *instances.back(); }
	tcmap_format6& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format6(std::vector<tcmap_format6*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format6() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format6* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format6* generate(quad& cmap_subtable);
};

int tcmap_format6::_parent_id = 0;
int tcmap_format6::_index_start = 0;



class tcmap_format8_groupings_struct {
	std::vector<tcmap_format8_groupings_struct*>& instances;

	ULONG startCharCode_var;
	ULONG endCharCode_var;
	ULONG startGlyphID_var;

public:
	bool startCharCode_exists = false;
	bool endCharCode_exists = false;
	bool startGlyphID_exists = false;

	ULONG& startCharCode() {
		assert_cond(startCharCode_exists, "struct field startCharCode does not exist");
		return startCharCode_var;
	}
	ULONG& endCharCode() {
		assert_cond(endCharCode_exists, "struct field endCharCode does not exist");
		return endCharCode_var;
	}
	ULONG& startGlyphID() {
		assert_cond(startGlyphID_exists, "struct field startGlyphID does not exist");
		return startGlyphID_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format8_groupings_struct& operator () () { return *instances.back(); }
	tcmap_format8_groupings_struct& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format8_groupings_struct(std::vector<tcmap_format8_groupings_struct*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format8_groupings_struct() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format8_groupings_struct* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format8_groupings_struct* generate();
};

int tcmap_format8_groupings_struct::_parent_id = 0;
int tcmap_format8_groupings_struct::_index_start = 0;



class tcmap_format8_groupings_struct_array_class {
	tcmap_format8_groupings_struct& element;
	std::vector<tcmap_format8_groupings_struct*> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<tcmap_format8_groupings_struct*>& operator () () { return value; }
	tcmap_format8_groupings_struct operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return *value[index];
	}
	tcmap_format8_groupings_struct_array_class(tcmap_format8_groupings_struct& element) : element(element) {}

	std::vector<tcmap_format8_groupings_struct*> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			value.push_back(element.generate());
			_sizeof += element._sizeof;
		}
		return value;
	}
};



class tcmap_format8 {
	std::vector<tcmap_format8*>& instances;

	USHORT format_var;
	USHORT reserved_var;
	ULONG length_var;
	ULONG language_var;
	std::vector<BYTE> is32_var;
	ULONG nGroups_var;
	std::vector<tcmap_format8_groupings_struct*> groupings_var;

public:
	bool format_exists = false;
	bool reserved_exists = false;
	bool length_exists = false;
	bool language_exists = false;
	bool is32_exists = false;
	bool nGroups_exists = false;
	bool groupings_exists = false;

	USHORT& format() {
		assert_cond(format_exists, "struct field format does not exist");
		return format_var;
	}
	USHORT& reserved() {
		assert_cond(reserved_exists, "struct field reserved does not exist");
		return reserved_var;
	}
	ULONG& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	ULONG& language() {
		assert_cond(language_exists, "struct field language does not exist");
		return language_var;
	}
	std::vector<BYTE>& is32() {
		assert_cond(is32_exists, "struct field is32 does not exist");
		return is32_var;
	}
	ULONG& nGroups() {
		assert_cond(nGroups_exists, "struct field nGroups does not exist");
		return nGroups_var;
	}
	std::vector<tcmap_format8_groupings_struct*>& groupings() {
		assert_cond(groupings_exists, "struct field groupings does not exist");
		return groupings_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format8& operator () () { return *instances.back(); }
	tcmap_format8& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format8(std::vector<tcmap_format8*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format8() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format8* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format8* generate(quad& cmap_subtable);
};

int tcmap_format8::_parent_id = 0;
int tcmap_format8::_index_start = 0;



class tcmap_format12_groupings_struct {
	std::vector<tcmap_format12_groupings_struct*>& instances;

	ULONG startCharCode_var;
	ULONG endCharCode_var;
	ULONG startGlyphID_var;

public:
	bool startCharCode_exists = false;
	bool endCharCode_exists = false;
	bool startGlyphID_exists = false;

	ULONG& startCharCode() {
		assert_cond(startCharCode_exists, "struct field startCharCode does not exist");
		return startCharCode_var;
	}
	ULONG& endCharCode() {
		assert_cond(endCharCode_exists, "struct field endCharCode does not exist");
		return endCharCode_var;
	}
	ULONG& startGlyphID() {
		assert_cond(startGlyphID_exists, "struct field startGlyphID does not exist");
		return startGlyphID_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format12_groupings_struct& operator () () { return *instances.back(); }
	tcmap_format12_groupings_struct& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format12_groupings_struct(std::vector<tcmap_format12_groupings_struct*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format12_groupings_struct() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format12_groupings_struct* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format12_groupings_struct* generate();
};

int tcmap_format12_groupings_struct::_parent_id = 0;
int tcmap_format12_groupings_struct::_index_start = 0;



class tcmap_format12_groupings_struct_array_class {
	tcmap_format12_groupings_struct& element;
	std::vector<tcmap_format12_groupings_struct*> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<tcmap_format12_groupings_struct*>& operator () () { return value; }
	tcmap_format12_groupings_struct operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return *value[index];
	}
	tcmap_format12_groupings_struct_array_class(tcmap_format12_groupings_struct& element) : element(element) {}

	std::vector<tcmap_format12_groupings_struct*> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			value.push_back(element.generate());
			_sizeof += element._sizeof;
		}
		return value;
	}
};



class tcmap_format12 {
	std::vector<tcmap_format12*>& instances;

	USHORT format_var;
	USHORT reserved_var;
	ULONG length_var;
	ULONG language_var;
	ULONG nGroups_var;
	std::vector<tcmap_format12_groupings_struct*> groupings_var;

public:
	bool format_exists = false;
	bool reserved_exists = false;
	bool length_exists = false;
	bool language_exists = false;
	bool nGroups_exists = false;
	bool groupings_exists = false;

	USHORT& format() {
		assert_cond(format_exists, "struct field format does not exist");
		return format_var;
	}
	USHORT& reserved() {
		assert_cond(reserved_exists, "struct field reserved does not exist");
		return reserved_var;
	}
	ULONG& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	ULONG& language() {
		assert_cond(language_exists, "struct field language does not exist");
		return language_var;
	}
	ULONG& nGroups() {
		assert_cond(nGroups_exists, "struct field nGroups does not exist");
		return nGroups_var;
	}
	std::vector<tcmap_format12_groupings_struct*>& groupings() {
		assert_cond(groupings_exists, "struct field groupings does not exist");
		return groupings_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap_format12& operator () () { return *instances.back(); }
	tcmap_format12& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap_format12(std::vector<tcmap_format12*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap_format12() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap_format12* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap_format12* generate();
};

int tcmap_format12::_parent_id = 0;
int tcmap_format12::_index_start = 0;



class tEncodingRecord {
	std::vector<tEncodingRecord*>& instances;

	USHORT platformID_var;
	USHORT encodingID_var;
	ULONG offset_var;
	tcmap_format0* format0_var;
	tcmap_format2* format2_var;
	tcmap_format4* format4_var;
	tcmap_format6* format6_var;
	tcmap_format8* format8_var;
	tcmap_format12* format12_var;

public:
	bool platformID_exists = false;
	bool encodingID_exists = false;
	bool offset_exists = false;
	bool format0_exists = false;
	bool format2_exists = false;
	bool format4_exists = false;
	bool format6_exists = false;
	bool format8_exists = false;
	bool format12_exists = false;

	USHORT& platformID() {
		assert_cond(platformID_exists, "struct field platformID does not exist");
		return platformID_var;
	}
	USHORT& encodingID() {
		assert_cond(encodingID_exists, "struct field encodingID does not exist");
		return encodingID_var;
	}
	ULONG& offset() {
		assert_cond(offset_exists, "struct field offset does not exist");
		return offset_var;
	}
	tcmap_format0& format0() {
		assert_cond(format0_exists, "struct field format0 does not exist");
		return *format0_var;
	}
	tcmap_format2& format2() {
		assert_cond(format2_exists, "struct field format2 does not exist");
		return *format2_var;
	}
	tcmap_format4& format4() {
		assert_cond(format4_exists, "struct field format4 does not exist");
		return *format4_var;
	}
	tcmap_format6& format6() {
		assert_cond(format6_exists, "struct field format6 does not exist");
		return *format6_var;
	}
	tcmap_format8& format8() {
		assert_cond(format8_exists, "struct field format8 does not exist");
		return *format8_var;
	}
	tcmap_format12& format12() {
		assert_cond(format12_exists, "struct field format12 does not exist");
		return *format12_var;
	}

	/* locals */
	quad cmap_subtable;
	quad cmap_record;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tEncodingRecord& operator () () { return *instances.back(); }
	tEncodingRecord& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tEncodingRecord(std::vector<tEncodingRecord*>& instances) : instances(instances) { instances.push_back(this); }
	~tEncodingRecord() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tEncodingRecord* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tEncodingRecord* generate(quad& cmap_table, quad& next_cmap_record);
};

int tEncodingRecord::_parent_id = 0;
int tEncodingRecord::_index_start = 0;



class tEncodingRecord_array_class {
	tEncodingRecord& element;
	std::vector<tEncodingRecord*> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<tEncodingRecord*>& operator () () { return value; }
	tEncodingRecord operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return *value[index];
	}
	tEncodingRecord_array_class(tEncodingRecord& element) : element(element) {}

	std::vector<tEncodingRecord*> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			value.push_back(element.generate());
			_sizeof += element._sizeof;
		}
		return value;
	}
};



class tcmap {
	std::vector<tcmap*>& instances;

	USHORT version_var;
	USHORT numTables_var;
	std::vector<tEncodingRecord*> EncodingRecord_var;

public:
	bool version_exists = false;
	bool numTables_exists = false;
	bool EncodingRecord_exists = false;

	USHORT& version() {
		assert_cond(version_exists, "struct field version does not exist");
		return version_var;
	}
	USHORT& numTables() {
		assert_cond(numTables_exists, "struct field numTables does not exist");
		return numTables_var;
	}
	std::vector<tEncodingRecord*>& EncodingRecord() {
		assert_cond(EncodingRecord_exists, "struct field EncodingRecord does not exist");
		return EncodingRecord_var;
	}

	/* locals */
	quad cmap_table;
	quad next_cmap_record;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tcmap& operator () () { return *instances.back(); }
	tcmap& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tcmap(std::vector<tcmap*>& instances) : instances(instances) { instances.push_back(this); }
	~tcmap() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tcmap* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tcmap* generate();
};

int tcmap::_parent_id = 0;
int tcmap::_index_start = 0;



class LONGDATETIME_class {
	int small;
	std::vector<LONGDATETIME> known_values;
	LONGDATETIME value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(LONGDATETIME);
	LONGDATETIME operator () () { return value; }
	LONGDATETIME_class(int small, std::vector<LONGDATETIME> known_values = {}) : small(small), known_values(known_values) {}

	LONGDATETIME generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(LONGDATETIME), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(LONGDATETIME), 0, known_values);
		}
		return value;
	}

	LONGDATETIME generate(std::vector<LONGDATETIME> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(LONGDATETIME), 0, possible_values);
		return value;
	}
};



class thead {
	std::vector<thead*>& instances;

	TT_Fixed version_var;
	TT_Fixed fontRevision_var;
	ULONG checkSumAdjustment_var;
	ULONG magicNumber_var;
	USHORT flags_var;
	USHORT unitsPerEm_var;
	LONGDATETIME created_var;
	LONGDATETIME modified_var;
	SHORT xMin_var;
	SHORT yMin_var;
	SHORT xMax_var;
	SHORT yMax_var;
	USHORT macStyle_var;
	USHORT lowestRecPPEM_var;
	SHORT fontDirectionHint_var;
	SHORT indexToLocFormat_var;
	SHORT glyphDataFormat_var;

public:
	bool version_exists = false;
	bool fontRevision_exists = false;
	bool checkSumAdjustment_exists = false;
	bool magicNumber_exists = false;
	bool flags_exists = false;
	bool unitsPerEm_exists = false;
	bool created_exists = false;
	bool modified_exists = false;
	bool xMin_exists = false;
	bool yMin_exists = false;
	bool xMax_exists = false;
	bool yMax_exists = false;
	bool macStyle_exists = false;
	bool lowestRecPPEM_exists = false;
	bool fontDirectionHint_exists = false;
	bool indexToLocFormat_exists = false;
	bool glyphDataFormat_exists = false;

	TT_Fixed& version() {
		assert_cond(version_exists, "struct field version does not exist");
		return version_var;
	}
	TT_Fixed& fontRevision() {
		assert_cond(fontRevision_exists, "struct field fontRevision does not exist");
		return fontRevision_var;
	}
	ULONG& checkSumAdjustment() {
		assert_cond(checkSumAdjustment_exists, "struct field checkSumAdjustment does not exist");
		return checkSumAdjustment_var;
	}
	ULONG& magicNumber() {
		assert_cond(magicNumber_exists, "struct field magicNumber does not exist");
		return magicNumber_var;
	}
	USHORT& flags() {
		assert_cond(flags_exists, "struct field flags does not exist");
		return flags_var;
	}
	USHORT& unitsPerEm() {
		assert_cond(unitsPerEm_exists, "struct field unitsPerEm does not exist");
		return unitsPerEm_var;
	}
	LONGDATETIME& created() {
		assert_cond(created_exists, "struct field created does not exist");
		return created_var;
	}
	LONGDATETIME& modified() {
		assert_cond(modified_exists, "struct field modified does not exist");
		return modified_var;
	}
	SHORT& xMin() {
		assert_cond(xMin_exists, "struct field xMin does not exist");
		return xMin_var;
	}
	SHORT& yMin() {
		assert_cond(yMin_exists, "struct field yMin does not exist");
		return yMin_var;
	}
	SHORT& xMax() {
		assert_cond(xMax_exists, "struct field xMax does not exist");
		return xMax_var;
	}
	SHORT& yMax() {
		assert_cond(yMax_exists, "struct field yMax does not exist");
		return yMax_var;
	}
	USHORT& macStyle() {
		assert_cond(macStyle_exists, "struct field macStyle does not exist");
		return macStyle_var;
	}
	USHORT& lowestRecPPEM() {
		assert_cond(lowestRecPPEM_exists, "struct field lowestRecPPEM does not exist");
		return lowestRecPPEM_var;
	}
	SHORT& fontDirectionHint() {
		assert_cond(fontDirectionHint_exists, "struct field fontDirectionHint does not exist");
		return fontDirectionHint_var;
	}
	SHORT& indexToLocFormat() {
		assert_cond(indexToLocFormat_exists, "struct field indexToLocFormat does not exist");
		return indexToLocFormat_var;
	}
	SHORT& glyphDataFormat() {
		assert_cond(glyphDataFormat_exists, "struct field glyphDataFormat does not exist");
		return glyphDataFormat_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	thead& operator () () { return *instances.back(); }
	thead& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	thead(std::vector<thead*>& instances) : instances(instances) { instances.push_back(this); }
	~thead() {
		if (generated == 2)
			return;
		while (instances.size()) {
			thead* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	thead* generate();
};

int thead::_parent_id = 0;
int thead::_index_start = 0;



class TT_FWord_class {
	int small;
	std::vector<TT_FWord> known_values;
	TT_FWord value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(TT_FWord);
	TT_FWord operator () () { return value; }
	TT_FWord_class(int small, std::vector<TT_FWord> known_values = {}) : small(small), known_values(known_values) {}

	TT_FWord generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(TT_FWord), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(TT_FWord), 0, known_values);
		}
		return value;
	}

	TT_FWord generate(std::vector<TT_FWord> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(TT_FWord), 0, possible_values);
		return value;
	}
};



class TT_UFWord_class {
	int small;
	std::vector<TT_UFWord> known_values;
	TT_UFWord value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(TT_UFWord);
	TT_UFWord operator () () { return value; }
	TT_UFWord_class(int small, std::vector<TT_UFWord> known_values = {}) : small(small), known_values(known_values) {}

	TT_UFWord generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(TT_UFWord), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(TT_UFWord), 0, known_values);
		}
		return value;
	}

	TT_UFWord generate(std::vector<TT_UFWord> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(TT_UFWord), 0, possible_values);
		return value;
	}
};



class thhea {
	std::vector<thhea*>& instances;

	TT_Fixed version_var;
	TT_FWord Ascender_var;
	TT_FWord Descender_var;
	TT_FWord LineGap_var;
	TT_UFWord advanceWidthMax_var;
	TT_FWord minLeftSideBearing_var;
	TT_FWord minRightSideBearing_var;
	TT_FWord xMaxExtent_var;
	SHORT caretSlopeRise_var;
	SHORT caretSlopeRun_var;
	SHORT caretOffset_var;
	SHORT reserved_var;
	SHORT metricDataFormat_var;
	USHORT numberOfHMetrics_var;

public:
	bool version_exists = false;
	bool Ascender_exists = false;
	bool Descender_exists = false;
	bool LineGap_exists = false;
	bool advanceWidthMax_exists = false;
	bool minLeftSideBearing_exists = false;
	bool minRightSideBearing_exists = false;
	bool xMaxExtent_exists = false;
	bool caretSlopeRise_exists = false;
	bool caretSlopeRun_exists = false;
	bool caretOffset_exists = false;
	bool reserved_exists = false;
	bool metricDataFormat_exists = false;
	bool numberOfHMetrics_exists = false;

	TT_Fixed& version() {
		assert_cond(version_exists, "struct field version does not exist");
		return version_var;
	}
	TT_FWord& Ascender() {
		assert_cond(Ascender_exists, "struct field Ascender does not exist");
		return Ascender_var;
	}
	TT_FWord& Descender() {
		assert_cond(Descender_exists, "struct field Descender does not exist");
		return Descender_var;
	}
	TT_FWord& LineGap() {
		assert_cond(LineGap_exists, "struct field LineGap does not exist");
		return LineGap_var;
	}
	TT_UFWord& advanceWidthMax() {
		assert_cond(advanceWidthMax_exists, "struct field advanceWidthMax does not exist");
		return advanceWidthMax_var;
	}
	TT_FWord& minLeftSideBearing() {
		assert_cond(minLeftSideBearing_exists, "struct field minLeftSideBearing does not exist");
		return minLeftSideBearing_var;
	}
	TT_FWord& minRightSideBearing() {
		assert_cond(minRightSideBearing_exists, "struct field minRightSideBearing does not exist");
		return minRightSideBearing_var;
	}
	TT_FWord& xMaxExtent() {
		assert_cond(xMaxExtent_exists, "struct field xMaxExtent does not exist");
		return xMaxExtent_var;
	}
	SHORT& caretSlopeRise() {
		assert_cond(caretSlopeRise_exists, "struct field caretSlopeRise does not exist");
		return caretSlopeRise_var;
	}
	SHORT& caretSlopeRun() {
		assert_cond(caretSlopeRun_exists, "struct field caretSlopeRun does not exist");
		return caretSlopeRun_var;
	}
	SHORT& caretOffset() {
		assert_cond(caretOffset_exists, "struct field caretOffset does not exist");
		return caretOffset_var;
	}
	SHORT& reserved() {
		assert_cond(reserved_exists, "struct field reserved does not exist");
		return reserved_var;
	}
	SHORT& metricDataFormat() {
		assert_cond(metricDataFormat_exists, "struct field metricDataFormat does not exist");
		return metricDataFormat_var;
	}
	USHORT& numberOfHMetrics() {
		assert_cond(numberOfHMetrics_exists, "struct field numberOfHMetrics does not exist");
		return numberOfHMetrics_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	thhea& operator () () { return *instances.back(); }
	thhea& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	thhea(std::vector<thhea*>& instances) : instances(instances) { instances.push_back(this); }
	~thhea() {
		if (generated == 2)
			return;
		while (instances.size()) {
			thhea* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	thhea* generate();
};

int thhea::_parent_id = 0;
int thhea::_index_start = 0;



class tlongHorMetric {
	std::vector<tlongHorMetric*>& instances;

	USHORT advanceWidth_var;
	SHORT lsb_var;

public:
	bool advanceWidth_exists = false;
	bool lsb_exists = false;

	USHORT& advanceWidth() {
		assert_cond(advanceWidth_exists, "struct field advanceWidth does not exist");
		return advanceWidth_var;
	}
	SHORT& lsb() {
		assert_cond(lsb_exists, "struct field lsb does not exist");
		return lsb_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tlongHorMetric& operator () () { return *instances.back(); }
	tlongHorMetric& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tlongHorMetric(std::vector<tlongHorMetric*>& instances) : instances(instances) { instances.push_back(this); }
	~tlongHorMetric() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tlongHorMetric* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tlongHorMetric* generate();
};

int tlongHorMetric::_parent_id = 0;
int tlongHorMetric::_index_start = 0;



class tlongHorMetric_array_class {
	tlongHorMetric& element;
	std::vector<tlongHorMetric*> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<tlongHorMetric*>& operator () () { return value; }
	tlongHorMetric operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return *value[index];
	}
	tlongHorMetric_array_class(tlongHorMetric& element) : element(element) {}

	std::vector<tlongHorMetric*> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			value.push_back(element.generate());
			_sizeof += element._sizeof;
		}
		return value;
	}
};



class thmtx {
	std::vector<thmtx*>& instances;

	std::vector<tlongHorMetric*> hMetrics_var;
	std::vector<SHORT> leftSideBearing_var;

public:
	bool hMetrics_exists = false;
	bool leftSideBearing_exists = false;

	std::vector<tlongHorMetric*>& hMetrics() {
		assert_cond(hMetrics_exists, "struct field hMetrics does not exist");
		return hMetrics_var;
	}
	std::vector<SHORT>& leftSideBearing() {
		assert_cond(leftSideBearing_exists, "struct field leftSideBearing does not exist");
		return leftSideBearing_var;
	}

	/* locals */
	ULONG numberOfHMetrics;
	ULONG numLeftSideBearing;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	thmtx& operator () () { return *instances.back(); }
	thmtx& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	thmtx(std::vector<thmtx*>& instances) : instances(instances) { instances.push_back(this); }
	~thmtx() {
		if (generated == 2)
			return;
		while (instances.size()) {
			thmtx* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	thmtx* generate();
};

int thmtx::_parent_id = 0;
int thmtx::_index_start = 0;



class tmaxp {
	std::vector<tmaxp*>& instances;

	TT_Fixed version_var;
	USHORT numGlyphs_var;
	USHORT maxPoints_var;
	USHORT maxContours_var;
	USHORT maxCompositePoints_var;
	USHORT maxCompositeContours_var;
	USHORT maxZones_var;
	USHORT maxTwilightPoints_var;
	USHORT maxStorage_var;
	USHORT maxFunctionDefs_var;
	USHORT maxInstructionDefs_var;
	USHORT maxStackElements_var;
	USHORT maxSizeOfInstructions_var;
	USHORT maxComponentElements_var;
	USHORT maxComponentDepth_var;

public:
	bool version_exists = false;
	bool numGlyphs_exists = false;
	bool maxPoints_exists = false;
	bool maxContours_exists = false;
	bool maxCompositePoints_exists = false;
	bool maxCompositeContours_exists = false;
	bool maxZones_exists = false;
	bool maxTwilightPoints_exists = false;
	bool maxStorage_exists = false;
	bool maxFunctionDefs_exists = false;
	bool maxInstructionDefs_exists = false;
	bool maxStackElements_exists = false;
	bool maxSizeOfInstructions_exists = false;
	bool maxComponentElements_exists = false;
	bool maxComponentDepth_exists = false;

	TT_Fixed& version() {
		assert_cond(version_exists, "struct field version does not exist");
		return version_var;
	}
	USHORT& numGlyphs() {
		assert_cond(numGlyphs_exists, "struct field numGlyphs does not exist");
		return numGlyphs_var;
	}
	USHORT& maxPoints() {
		assert_cond(maxPoints_exists, "struct field maxPoints does not exist");
		return maxPoints_var;
	}
	USHORT& maxContours() {
		assert_cond(maxContours_exists, "struct field maxContours does not exist");
		return maxContours_var;
	}
	USHORT& maxCompositePoints() {
		assert_cond(maxCompositePoints_exists, "struct field maxCompositePoints does not exist");
		return maxCompositePoints_var;
	}
	USHORT& maxCompositeContours() {
		assert_cond(maxCompositeContours_exists, "struct field maxCompositeContours does not exist");
		return maxCompositeContours_var;
	}
	USHORT& maxZones() {
		assert_cond(maxZones_exists, "struct field maxZones does not exist");
		return maxZones_var;
	}
	USHORT& maxTwilightPoints() {
		assert_cond(maxTwilightPoints_exists, "struct field maxTwilightPoints does not exist");
		return maxTwilightPoints_var;
	}
	USHORT& maxStorage() {
		assert_cond(maxStorage_exists, "struct field maxStorage does not exist");
		return maxStorage_var;
	}
	USHORT& maxFunctionDefs() {
		assert_cond(maxFunctionDefs_exists, "struct field maxFunctionDefs does not exist");
		return maxFunctionDefs_var;
	}
	USHORT& maxInstructionDefs() {
		assert_cond(maxInstructionDefs_exists, "struct field maxInstructionDefs does not exist");
		return maxInstructionDefs_var;
	}
	USHORT& maxStackElements() {
		assert_cond(maxStackElements_exists, "struct field maxStackElements does not exist");
		return maxStackElements_var;
	}
	USHORT& maxSizeOfInstructions() {
		assert_cond(maxSizeOfInstructions_exists, "struct field maxSizeOfInstructions does not exist");
		return maxSizeOfInstructions_var;
	}
	USHORT& maxComponentElements() {
		assert_cond(maxComponentElements_exists, "struct field maxComponentElements does not exist");
		return maxComponentElements_var;
	}
	USHORT& maxComponentDepth() {
		assert_cond(maxComponentDepth_exists, "struct field maxComponentDepth does not exist");
		return maxComponentDepth_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tmaxp& operator () () { return *instances.back(); }
	tmaxp& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tmaxp(std::vector<tmaxp*>& instances) : instances(instances) { instances.push_back(this); }
	~tmaxp() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tmaxp* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tmaxp* generate();
};

int tmaxp::_parent_id = 0;
int tmaxp::_index_start = 0;



class tNameRecord {
	std::vector<tNameRecord*>& instances;

	USHORT platformID_var;
	USHORT encodingID_var;
	USHORT languageID_var;
	USHORT nameID_var;
	USHORT length_var;
	USHORT offset_var;
	std::string name_var;

public:
	bool platformID_exists = false;
	bool encodingID_exists = false;
	bool languageID_exists = false;
	bool nameID_exists = false;
	bool length_exists = false;
	bool offset_exists = false;
	bool name_exists = false;

	USHORT& platformID() {
		assert_cond(platformID_exists, "struct field platformID does not exist");
		return platformID_var;
	}
	USHORT& encodingID() {
		assert_cond(encodingID_exists, "struct field encodingID does not exist");
		return encodingID_var;
	}
	USHORT& languageID() {
		assert_cond(languageID_exists, "struct field languageID does not exist");
		return languageID_var;
	}
	USHORT& nameID() {
		assert_cond(nameID_exists, "struct field nameID does not exist");
		return nameID_var;
	}
	USHORT& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	USHORT& offset() {
		assert_cond(offset_exists, "struct field offset does not exist");
		return offset_var;
	}
	std::string& name() {
		assert_cond(name_exists, "struct field name does not exist");
		return name_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tNameRecord& operator () () { return *instances.back(); }
	tNameRecord& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tNameRecord(std::vector<tNameRecord*>& instances) : instances(instances) { instances.push_back(this); }
	~tNameRecord() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tNameRecord* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tNameRecord* generate(quad& name_table, quad& NextNameRecord);
};

int tNameRecord::_parent_id = 0;
int tNameRecord::_index_start = 0;



class tNameRecord_array_class {
	tNameRecord& element;
	std::vector<tNameRecord*> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<tNameRecord*>& operator () () { return value; }
	tNameRecord operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return *value[index];
	}
	tNameRecord_array_class(tNameRecord& element) : element(element) {}

	std::vector<tNameRecord*> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			value.push_back(element.generate());
			_sizeof += element._sizeof;
		}
		return value;
	}
};



class tname {
	std::vector<tname*>& instances;

	USHORT format_var;
	USHORT count_var;
	USHORT stringOffset_var;
	std::vector<tNameRecord*> NameRecord_var;

public:
	bool format_exists = false;
	bool count_exists = false;
	bool stringOffset_exists = false;
	bool NameRecord_exists = false;

	USHORT& format() {
		assert_cond(format_exists, "struct field format does not exist");
		return format_var;
	}
	USHORT& count() {
		assert_cond(count_exists, "struct field count does not exist");
		return count_var;
	}
	USHORT& stringOffset() {
		assert_cond(stringOffset_exists, "struct field stringOffset does not exist");
		return stringOffset_var;
	}
	std::vector<tNameRecord*>& NameRecord() {
		assert_cond(NameRecord_exists, "struct field NameRecord does not exist");
		return NameRecord_var;
	}

	/* locals */
	quad name_table;
	quad NextNameRecord;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tname& operator () () { return *instances.back(); }
	tname& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tname(std::vector<tname*>& instances) : instances(instances) { instances.push_back(this); }
	~tname() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tname* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tname* generate();
};

int tname::_parent_id = 0;
int tname::_index_start = 0;



class UBYTE_class {
	int small;
	std::vector<UBYTE> known_values;
	UBYTE value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(UBYTE);
	UBYTE operator () () { return value; }
	UBYTE_class(int small, std::vector<UBYTE> known_values = {}) : small(small), known_values(known_values) {}

	UBYTE generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(UBYTE), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(UBYTE), 0, known_values);
		}
		return value;
	}

	UBYTE generate(std::vector<UBYTE> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(UBYTE), 0, possible_values);
		return value;
	}
};



class tpanose {
	std::vector<tpanose*>& instances;

	UBYTE bFamilyType_var;
	UBYTE bSerifStyle_var;
	UBYTE bWeight_var;
	UBYTE bProportion_var;
	UBYTE bContrast_var;
	UBYTE bStrokeVariation_var;
	UBYTE bArmStyle_var;
	UBYTE bLetterform_var;
	UBYTE bMidline_var;
	UBYTE bXHeight_var;

public:
	bool bFamilyType_exists = false;
	bool bSerifStyle_exists = false;
	bool bWeight_exists = false;
	bool bProportion_exists = false;
	bool bContrast_exists = false;
	bool bStrokeVariation_exists = false;
	bool bArmStyle_exists = false;
	bool bLetterform_exists = false;
	bool bMidline_exists = false;
	bool bXHeight_exists = false;

	UBYTE& bFamilyType() {
		assert_cond(bFamilyType_exists, "struct field bFamilyType does not exist");
		return bFamilyType_var;
	}
	UBYTE& bSerifStyle() {
		assert_cond(bSerifStyle_exists, "struct field bSerifStyle does not exist");
		return bSerifStyle_var;
	}
	UBYTE& bWeight() {
		assert_cond(bWeight_exists, "struct field bWeight does not exist");
		return bWeight_var;
	}
	UBYTE& bProportion() {
		assert_cond(bProportion_exists, "struct field bProportion does not exist");
		return bProportion_var;
	}
	UBYTE& bContrast() {
		assert_cond(bContrast_exists, "struct field bContrast does not exist");
		return bContrast_var;
	}
	UBYTE& bStrokeVariation() {
		assert_cond(bStrokeVariation_exists, "struct field bStrokeVariation does not exist");
		return bStrokeVariation_var;
	}
	UBYTE& bArmStyle() {
		assert_cond(bArmStyle_exists, "struct field bArmStyle does not exist");
		return bArmStyle_var;
	}
	UBYTE& bLetterform() {
		assert_cond(bLetterform_exists, "struct field bLetterform does not exist");
		return bLetterform_var;
	}
	UBYTE& bMidline() {
		assert_cond(bMidline_exists, "struct field bMidline does not exist");
		return bMidline_var;
	}
	UBYTE& bXHeight() {
		assert_cond(bXHeight_exists, "struct field bXHeight does not exist");
		return bXHeight_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tpanose& operator () () { return *instances.back(); }
	tpanose& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tpanose(std::vector<tpanose*>& instances) : instances(instances) { instances.push_back(this); }
	~tpanose() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tpanose* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tpanose* generate();
};

int tpanose::_parent_id = 0;
int tpanose::_index_start = 0;



class CHAR_class {
	int small;
	std::vector<CHAR> known_values;
	CHAR value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = sizeof(CHAR);
	CHAR operator () () { return value; }
	CHAR_class(int small, std::vector<CHAR> known_values = {}) : small(small), known_values(known_values) {}

	CHAR generate() {
		_startof = FTell();
		if (known_values.empty()) {
			value = file_acc.file_integer(sizeof(CHAR), 0, small);
		} else {
			value = file_acc.file_integer(sizeof(CHAR), 0, known_values);
		}
		return value;
	}

	CHAR generate(std::vector<CHAR> possible_values) {
		_startof = FTell();
		value = file_acc.file_integer(sizeof(CHAR), 0, possible_values);
		return value;
	}
};



class CHAR_array_class {
	CHAR_class& element;
	std::vector<std::string> known_values;
	std::unordered_map<int, std::vector<CHAR>> element_known_values;
	std::string value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::string& operator () () { return value; }
	CHAR operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return value[index];
	}
	CHAR_array_class(CHAR_class& element, std::unordered_map<int, std::vector<CHAR>> element_known_values = {})
		: element(element), element_known_values(element_known_values) {}
	CHAR_array_class(CHAR_class& element, std::vector<std::string> known_values)
		: element(element), known_values(known_values) {}

	std::string generate(unsigned size, std::vector<std::string> possible_values = {}) {
		check_array_length(size);
		_startof = FTell();
		value = "";
		if (possible_values.size()) {
			value = file_acc.file_string(possible_values);
			assert(value.length() == size);
			_sizeof = size;
			return value;
		}
		if (known_values.size()) {
			value = file_acc.file_string(known_values);
			assert(value.length() == size);
			_sizeof = size;
			return value;
		}
		if (!element_known_values.size()) {
			if (size == 0)
				 return "";
			value = file_acc.file_string(size);
			_sizeof = size;
			return value;
		}
		for (unsigned i = 0; i < size; ++i) {
			auto known = element_known_values.find(i);
			if (known == element_known_values.end()) {
				value.push_back(element.generate());
				_sizeof += element._sizeof;
			} else {
				value.push_back(file_acc.file_integer(sizeof(CHAR), 0, known->second));
				_sizeof += sizeof(CHAR);
			}
		}
		return value;
	}
};



class tOS_2 {
	std::vector<tOS_2*>& instances;

	USHORT version_var;
	SHORT xAvgCharWidth_var;
	USHORT usWeightClass_var;
	USHORT usWidthClass_var;
	USHORT fsType_var;
	SHORT ySubscriptXSize_var;
	SHORT ySubscriptYSize_var;
	SHORT ySubscriptXOffset_var;
	SHORT ySubscriptYOffset_var;
	SHORT ySuperscriptXSize_var;
	SHORT ySuperscriptYSize_var;
	SHORT ySuperscriptXOffset_var;
	SHORT ySuperscriptYOffset_var;
	SHORT yStrikeoutSize_var;
	SHORT yStrikeoutPosition_var;
	SHORT sFamilyClass_var;
	tpanose* panose_var;
	ULONG ulUnicodeRange1_var;
	ULONG ulUnicodeRange2_var;
	ULONG ulUnicodeRange3_var;
	ULONG ulUnicodeRange4_var;
	std::string achVendID_var;
	USHORT fsSelection_var;
	USHORT usFirstCharIndex_var;
	USHORT usLastCharIndex_var;
	SHORT sTypoAscender_var;
	SHORT sTypoDescender_var;
	SHORT sTypoLineGap_var;
	USHORT usWinAscent_var;
	USHORT usWinDescent_var;
	ULONG ulCodePageRange1_var;
	ULONG ulCodePageRange2_var;
	SHORT sxHeight_var;
	SHORT sCapHeight_var;
	USHORT usDefaultChar_var;
	USHORT usBreakChar_var;
	USHORT usMaxContext_var;

public:
	bool version_exists = false;
	bool xAvgCharWidth_exists = false;
	bool usWeightClass_exists = false;
	bool usWidthClass_exists = false;
	bool fsType_exists = false;
	bool ySubscriptXSize_exists = false;
	bool ySubscriptYSize_exists = false;
	bool ySubscriptXOffset_exists = false;
	bool ySubscriptYOffset_exists = false;
	bool ySuperscriptXSize_exists = false;
	bool ySuperscriptYSize_exists = false;
	bool ySuperscriptXOffset_exists = false;
	bool ySuperscriptYOffset_exists = false;
	bool yStrikeoutSize_exists = false;
	bool yStrikeoutPosition_exists = false;
	bool sFamilyClass_exists = false;
	bool panose_exists = false;
	bool ulUnicodeRange1_exists = false;
	bool ulUnicodeRange2_exists = false;
	bool ulUnicodeRange3_exists = false;
	bool ulUnicodeRange4_exists = false;
	bool achVendID_exists = false;
	bool fsSelection_exists = false;
	bool usFirstCharIndex_exists = false;
	bool usLastCharIndex_exists = false;
	bool sTypoAscender_exists = false;
	bool sTypoDescender_exists = false;
	bool sTypoLineGap_exists = false;
	bool usWinAscent_exists = false;
	bool usWinDescent_exists = false;
	bool ulCodePageRange1_exists = false;
	bool ulCodePageRange2_exists = false;
	bool sxHeight_exists = false;
	bool sCapHeight_exists = false;
	bool usDefaultChar_exists = false;
	bool usBreakChar_exists = false;
	bool usMaxContext_exists = false;

	USHORT& version() {
		assert_cond(version_exists, "struct field version does not exist");
		return version_var;
	}
	SHORT& xAvgCharWidth() {
		assert_cond(xAvgCharWidth_exists, "struct field xAvgCharWidth does not exist");
		return xAvgCharWidth_var;
	}
	USHORT& usWeightClass() {
		assert_cond(usWeightClass_exists, "struct field usWeightClass does not exist");
		return usWeightClass_var;
	}
	USHORT& usWidthClass() {
		assert_cond(usWidthClass_exists, "struct field usWidthClass does not exist");
		return usWidthClass_var;
	}
	USHORT& fsType() {
		assert_cond(fsType_exists, "struct field fsType does not exist");
		return fsType_var;
	}
	SHORT& ySubscriptXSize() {
		assert_cond(ySubscriptXSize_exists, "struct field ySubscriptXSize does not exist");
		return ySubscriptXSize_var;
	}
	SHORT& ySubscriptYSize() {
		assert_cond(ySubscriptYSize_exists, "struct field ySubscriptYSize does not exist");
		return ySubscriptYSize_var;
	}
	SHORT& ySubscriptXOffset() {
		assert_cond(ySubscriptXOffset_exists, "struct field ySubscriptXOffset does not exist");
		return ySubscriptXOffset_var;
	}
	SHORT& ySubscriptYOffset() {
		assert_cond(ySubscriptYOffset_exists, "struct field ySubscriptYOffset does not exist");
		return ySubscriptYOffset_var;
	}
	SHORT& ySuperscriptXSize() {
		assert_cond(ySuperscriptXSize_exists, "struct field ySuperscriptXSize does not exist");
		return ySuperscriptXSize_var;
	}
	SHORT& ySuperscriptYSize() {
		assert_cond(ySuperscriptYSize_exists, "struct field ySuperscriptYSize does not exist");
		return ySuperscriptYSize_var;
	}
	SHORT& ySuperscriptXOffset() {
		assert_cond(ySuperscriptXOffset_exists, "struct field ySuperscriptXOffset does not exist");
		return ySuperscriptXOffset_var;
	}
	SHORT& ySuperscriptYOffset() {
		assert_cond(ySuperscriptYOffset_exists, "struct field ySuperscriptYOffset does not exist");
		return ySuperscriptYOffset_var;
	}
	SHORT& yStrikeoutSize() {
		assert_cond(yStrikeoutSize_exists, "struct field yStrikeoutSize does not exist");
		return yStrikeoutSize_var;
	}
	SHORT& yStrikeoutPosition() {
		assert_cond(yStrikeoutPosition_exists, "struct field yStrikeoutPosition does not exist");
		return yStrikeoutPosition_var;
	}
	SHORT& sFamilyClass() {
		assert_cond(sFamilyClass_exists, "struct field sFamilyClass does not exist");
		return sFamilyClass_var;
	}
	tpanose& panose() {
		assert_cond(panose_exists, "struct field panose does not exist");
		return *panose_var;
	}
	ULONG& ulUnicodeRange1() {
		assert_cond(ulUnicodeRange1_exists, "struct field ulUnicodeRange1 does not exist");
		return ulUnicodeRange1_var;
	}
	ULONG& ulUnicodeRange2() {
		assert_cond(ulUnicodeRange2_exists, "struct field ulUnicodeRange2 does not exist");
		return ulUnicodeRange2_var;
	}
	ULONG& ulUnicodeRange3() {
		assert_cond(ulUnicodeRange3_exists, "struct field ulUnicodeRange3 does not exist");
		return ulUnicodeRange3_var;
	}
	ULONG& ulUnicodeRange4() {
		assert_cond(ulUnicodeRange4_exists, "struct field ulUnicodeRange4 does not exist");
		return ulUnicodeRange4_var;
	}
	std::string& achVendID() {
		assert_cond(achVendID_exists, "struct field achVendID does not exist");
		return achVendID_var;
	}
	USHORT& fsSelection() {
		assert_cond(fsSelection_exists, "struct field fsSelection does not exist");
		return fsSelection_var;
	}
	USHORT& usFirstCharIndex() {
		assert_cond(usFirstCharIndex_exists, "struct field usFirstCharIndex does not exist");
		return usFirstCharIndex_var;
	}
	USHORT& usLastCharIndex() {
		assert_cond(usLastCharIndex_exists, "struct field usLastCharIndex does not exist");
		return usLastCharIndex_var;
	}
	SHORT& sTypoAscender() {
		assert_cond(sTypoAscender_exists, "struct field sTypoAscender does not exist");
		return sTypoAscender_var;
	}
	SHORT& sTypoDescender() {
		assert_cond(sTypoDescender_exists, "struct field sTypoDescender does not exist");
		return sTypoDescender_var;
	}
	SHORT& sTypoLineGap() {
		assert_cond(sTypoLineGap_exists, "struct field sTypoLineGap does not exist");
		return sTypoLineGap_var;
	}
	USHORT& usWinAscent() {
		assert_cond(usWinAscent_exists, "struct field usWinAscent does not exist");
		return usWinAscent_var;
	}
	USHORT& usWinDescent() {
		assert_cond(usWinDescent_exists, "struct field usWinDescent does not exist");
		return usWinDescent_var;
	}
	ULONG& ulCodePageRange1() {
		assert_cond(ulCodePageRange1_exists, "struct field ulCodePageRange1 does not exist");
		return ulCodePageRange1_var;
	}
	ULONG& ulCodePageRange2() {
		assert_cond(ulCodePageRange2_exists, "struct field ulCodePageRange2 does not exist");
		return ulCodePageRange2_var;
	}
	SHORT& sxHeight() {
		assert_cond(sxHeight_exists, "struct field sxHeight does not exist");
		return sxHeight_var;
	}
	SHORT& sCapHeight() {
		assert_cond(sCapHeight_exists, "struct field sCapHeight does not exist");
		return sCapHeight_var;
	}
	USHORT& usDefaultChar() {
		assert_cond(usDefaultChar_exists, "struct field usDefaultChar does not exist");
		return usDefaultChar_var;
	}
	USHORT& usBreakChar() {
		assert_cond(usBreakChar_exists, "struct field usBreakChar does not exist");
		return usBreakChar_var;
	}
	USHORT& usMaxContext() {
		assert_cond(usMaxContext_exists, "struct field usMaxContext does not exist");
		return usMaxContext_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tOS_2& operator () () { return *instances.back(); }
	tOS_2& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tOS_2(std::vector<tOS_2*>& instances) : instances(instances) { instances.push_back(this); }
	~tOS_2() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tOS_2* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tOS_2* generate();
};

int tOS_2::_parent_id = 0;
int tOS_2::_index_start = 0;



class tpostName {
	std::vector<tpostName*>& instances;

	UBYTE length_var;
	std::string text_var;

public:
	bool length_exists = false;
	bool text_exists = false;

	UBYTE& length() {
		assert_cond(length_exists, "struct field length does not exist");
		return length_var;
	}
	std::string& text() {
		assert_cond(text_exists, "struct field text does not exist");
		return text_var;
	}

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tpostName& operator () () { return *instances.back(); }
	tpostName& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tpostName(std::vector<tpostName*>& instances) : instances(instances) { instances.push_back(this); }
	~tpostName() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tpostName* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tpostName* generate();
};

int tpostName::_parent_id = 0;
int tpostName::_index_start = 0;



class tpost {
	std::vector<tpost*>& instances;

	TT_Fixed version_var;
	TT_Fixed italicAngle_var;
	TT_FWord underlinePosition_var;
	TT_FWord underlineThickness_var;
	ULONG isFixedPitch_var;
	ULONG minMemType42_var;
	ULONG maxMemType42_var;
	ULONG minMemType1_var;
	ULONG maxMemType1_var;
	USHORT numberOfGlyphs_var;
	std::vector<USHORT> glyphNameIndex_var;
	tpostName* name_var;
	std::vector<USHORT> offset_var;

public:
	bool version_exists = false;
	bool italicAngle_exists = false;
	bool underlinePosition_exists = false;
	bool underlineThickness_exists = false;
	bool isFixedPitch_exists = false;
	bool minMemType42_exists = false;
	bool maxMemType42_exists = false;
	bool minMemType1_exists = false;
	bool maxMemType1_exists = false;
	bool numberOfGlyphs_exists = false;
	bool glyphNameIndex_exists = false;
	bool name_exists = false;
	bool offset_exists = false;

	TT_Fixed& version() {
		assert_cond(version_exists, "struct field version does not exist");
		return version_var;
	}
	TT_Fixed& italicAngle() {
		assert_cond(italicAngle_exists, "struct field italicAngle does not exist");
		return italicAngle_var;
	}
	TT_FWord& underlinePosition() {
		assert_cond(underlinePosition_exists, "struct field underlinePosition does not exist");
		return underlinePosition_var;
	}
	TT_FWord& underlineThickness() {
		assert_cond(underlineThickness_exists, "struct field underlineThickness does not exist");
		return underlineThickness_var;
	}
	ULONG& isFixedPitch() {
		assert_cond(isFixedPitch_exists, "struct field isFixedPitch does not exist");
		return isFixedPitch_var;
	}
	ULONG& minMemType42() {
		assert_cond(minMemType42_exists, "struct field minMemType42 does not exist");
		return minMemType42_var;
	}
	ULONG& maxMemType42() {
		assert_cond(maxMemType42_exists, "struct field maxMemType42 does not exist");
		return maxMemType42_var;
	}
	ULONG& minMemType1() {
		assert_cond(minMemType1_exists, "struct field minMemType1 does not exist");
		return minMemType1_var;
	}
	ULONG& maxMemType1() {
		assert_cond(maxMemType1_exists, "struct field maxMemType1 does not exist");
		return maxMemType1_var;
	}
	USHORT& numberOfGlyphs() {
		assert_cond(numberOfGlyphs_exists, "struct field numberOfGlyphs does not exist");
		return numberOfGlyphs_var;
	}
	std::vector<USHORT>& glyphNameIndex() {
		assert_cond(glyphNameIndex_exists, "struct field glyphNameIndex does not exist");
		return glyphNameIndex_var;
	}
	tpostName& name() {
		assert_cond(name_exists, "struct field name does not exist");
		return *name_var;
	}
	std::vector<USHORT>& offset() {
		assert_cond(offset_exists, "struct field offset does not exist");
		return offset_var;
	}

	/* locals */
	quad post;
	ushort numGlyphs;
	ushort numberNewGlyphs;
	quad end_name;
	quad next_name;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tpost& operator () () { return *instances.back(); }
	tpost& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tpost(std::vector<tpost*>& instances) : instances(instances) { instances.push_back(this); }
	~tpost() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tpost* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tpost* generate();
};

int tpost::_parent_id = 0;
int tpost::_index_start = 0;



class ULONG_array_class {
	ULONG_class& element;
	std::unordered_map<int, std::vector<ULONG>> element_known_values;
	std::vector<ULONG> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<ULONG>& operator () () { return value; }
	ULONG operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return value[index];
	}
	ULONG_array_class(ULONG_class& element, std::unordered_map<int, std::vector<ULONG>> element_known_values = {})
		: element(element), element_known_values(element_known_values) {}

	std::vector<ULONG> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			auto known = element_known_values.find(i);
			if (known == element_known_values.end()) {
				value.push_back(element.generate());
				_sizeof += element._sizeof;
			} else {
				value.push_back(file_acc.file_integer(sizeof(ULONG), 0, known->second));
				_sizeof += sizeof(ULONG);
			}
		}
		return value;
	}
};



class tloca {
	std::vector<tloca*>& instances;


public:


	/* locals */
	ULONG n;
	short format;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	tloca& operator () () { return *instances.back(); }
	tloca& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	tloca(std::vector<tloca*>& instances) : instances(instances) { instances.push_back(this); }
	~tloca() {
		if (generated == 2)
			return;
		while (instances.size()) {
			tloca* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	tloca* generate();
};

int tloca::_parent_id = 0;
int tloca::_index_start = 0;



class ttfFile {
	std::vector<ttfFile*>& instances;

	tOffsetTable* OffsetTable_var;
	std::string tempstr_var;
	tTable* Table_var;
	tcmap* cmap_var;
	thead* head_var;
	thhea* hhea_var;
	thmtx* hmtx_var;
	tmaxp* maxp_var;
	tname* name_var;
	tOS_2* OS_2_var;
	tpost* post_var;
	tloca* loca_var;

public:
	bool OffsetTable_exists = false;
	bool tempstr_exists = false;
	bool Table_exists = false;
	bool cmap_exists = false;
	bool head_exists = false;
	bool hhea_exists = false;
	bool hmtx_exists = false;
	bool maxp_exists = false;
	bool name_exists = false;
	bool OS_2_exists = false;
	bool post_exists = false;
	bool loca_exists = false;

	tOffsetTable& OffsetTable() {
		assert_cond(OffsetTable_exists, "struct field OffsetTable does not exist");
		return *OffsetTable_var;
	}
	std::string& tempstr() {
		assert_cond(tempstr_exists, "struct field tempstr does not exist");
		return tempstr_var;
	}
	tTable& Table() {
		assert_cond(Table_exists, "struct field Table does not exist");
		return *Table_var;
	}
	tcmap& cmap() {
		assert_cond(cmap_exists, "struct field cmap does not exist");
		return *cmap_var;
	}
	thead& head() {
		assert_cond(head_exists, "struct field head does not exist");
		return *head_var;
	}
	thhea& hhea() {
		assert_cond(hhea_exists, "struct field hhea does not exist");
		return *hhea_var;
	}
	thmtx& hmtx() {
		assert_cond(hmtx_exists, "struct field hmtx does not exist");
		return *hmtx_var;
	}
	tmaxp& maxp() {
		assert_cond(maxp_exists, "struct field maxp does not exist");
		return *maxp_var;
	}
	tname& name() {
		assert_cond(name_exists, "struct field name does not exist");
		return *name_var;
	}
	tOS_2& OS_2() {
		assert_cond(OS_2_exists, "struct field OS_2 does not exist");
		return *OS_2_var;
	}
	tpost& post() {
		assert_cond(post_exists, "struct field post does not exist");
		return *post_var;
	}
	tloca& loca() {
		assert_cond(loca_exists, "struct field loca does not exist");
		return *loca_var;
	}

	/* locals */
	std::vector<int> possible_sfntver;
	std::vector<uint16> possible_table_number;
	int name_number;
	std::vector<std::string> required_tables;
	std::vector<int> table_found;
	int tablenum;
	int64 tempNext;
	int targetTableidex;
	int yk1;

	unsigned char generated = 0;
	static int _parent_id;
	static int _index_start;
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	ttfFile& operator () () { return *instances.back(); }
	ttfFile& operator [] (int index) {
		assert_cond((unsigned)(_index_start + index) < instances.size(), "instance index out of bounds");
		return *instances[_index_start + index];
	}
	std::size_t array_size() {
		return instances.size() - _index_start;
	}
	ttfFile(std::vector<ttfFile*>& instances) : instances(instances) { instances.push_back(this); }
	~ttfFile() {
		if (generated == 2)
			return;
		while (instances.size()) {
			ttfFile* instance = instances.back();
			instances.pop_back();
			if (instance->generated == 2)
				delete instance;
		}
	}
	ttfFile* generate();
};

int ttfFile::_parent_id = 0;
int ttfFile::_index_start = 0;



class ttfFile_array_class {
	ttfFile& element;
	std::vector<ttfFile*> value;
public:
	int64 _startof = 0;
	std::size_t _sizeof = 0;
	std::vector<ttfFile*>& operator () () { return value; }
	ttfFile operator [] (int index) {
		assert_cond((unsigned)index < value.size(), "array index out of bounds");
		return *value[index];
	}
	ttfFile_array_class(ttfFile& element) : element(element) {}

	std::vector<ttfFile*> generate(unsigned size) {
		check_array_length(size);
		_startof = FTell();
		value = {};
		for (unsigned i = 0; i < size; ++i) {
			value.push_back(element.generate());
			_sizeof += element._sizeof;
		}
		return value;
	}
};

std::vector<byte> ReadByteInitValues;
std::vector<ubyte> ReadUByteInitValues;
std::vector<short> ReadShortInitValues;
std::vector<ushort> ReadUShortInitValues;
std::vector<int> ReadIntInitValues = { 0x74746366, 0x10000 };
std::vector<uint> ReadUIntInitValues;
std::vector<int64> ReadQuadInitValues;
std::vector<uint64> ReadUQuadInitValues;
std::vector<int64> ReadInt64InitValues;
std::vector<uint64> ReadUInt64InitValues;
std::vector<hfloat> ReadHFloatInitValues;
std::vector<float> ReadFloatInitValues;
std::vector<double> ReadDoubleInitValues;
std::vector<std::string> ReadBytesInitValues;


std::vector<TTCHeader*> TTCHeader_ttc_instances;
std::vector<tOffsetTable*> tOffsetTable_OffsetTable_instances;
std::vector<tTable_Tag_union*> tTable_Tag_union_Tag_instances;
std::vector<tTable*> tTable_Table_instances;
std::vector<tcmap_format0*> tcmap_format0_format0_instances;
std::vector<SubHeader*> SubHeader_subHeaders_element_instances;
std::vector<tcmap_format2*> tcmap_format2_format2_instances;
std::vector<tcmap_format4*> tcmap_format4_format4_instances;
std::vector<tcmap_format6*> tcmap_format6_format6_instances;
std::vector<tcmap_format8_groupings_struct*> tcmap_format8_groupings_struct_groupings_element_instances;
std::vector<tcmap_format8*> tcmap_format8_format8_instances;
std::vector<tcmap_format12_groupings_struct*> tcmap_format12_groupings_struct_groupings__element_instances;
std::vector<tcmap_format12*> tcmap_format12_format12_instances;
std::vector<tEncodingRecord*> tEncodingRecord_EncodingRecord_element_instances;
std::vector<tcmap*> tcmap_cmap_instances;
std::vector<thead*> thead_head_instances;
std::vector<thhea*> thhea_hhea_instances;
std::vector<tlongHorMetric*> tlongHorMetric_hMetrics_element_instances;
std::vector<thmtx*> thmtx_hmtx_instances;
std::vector<tmaxp*> tmaxp_maxp_instances;
std::vector<tNameRecord*> tNameRecord_NameRecord_element_instances;
std::vector<tname*> tname_name__instances;
std::vector<tpanose*> tpanose_panose_instances;
std::vector<tOS_2*> tOS_2_OS_2_instances;
std::vector<tpostName*> tpostName_name___instances;
std::vector<tpost*> tpost_post_instances;
std::vector<tloca*> tloca_loca_instances;
std::vector<ttfFile*> ttfFile_ttf_element_instances;


std::unordered_map<std::string, std::string> variable_types = { { "tag", "char_array_class" }, { "majorVersion", "uint16_class" }, { "minorVersion", "uint16_class" }, { "numFonts", "uint32_class" }, { "offsetTable", "uint32_array_class" }, { "dsigTag", "char_array_class" }, { "dsigLength", "uint32_class" }, { "dsigOffset", "uint32_class" }, { "ttc", "TTCHeader" }, { "SFNT_Ver", "TT_Fixed_class" }, { "numTables", "USHORT_class" }, { "searchRange", "USHORT_class" }, { "entrySelector", "USHORT_class" }, { "rangeShift", "USHORT_class" }, { "OffsetTable", "tOffsetTable" }, { "tempstr", "string_class" }, { "asChar", "char_array_class" }, { "asLong", "ULONG_class" }, { "Tag", "tTable_Tag_union" }, { "checkSum", "ULONG_class" }, { "offset", "ULONG_class" }, { "length", "ULONG_class" }, { "Table", "tTable" }, { "version", "USHORT_class" }, { "platformID", "USHORT_class" }, { "encodingID", "USHORT_class" }, { "format", "USHORT_class" }, { "length_", "USHORT_class" }, { "language", "USHORT_class" }, { "glyphIdArray", "BYTE_array_class" }, { "format0", "tcmap_format0" }, { "subHeaderKeys", "USHORT_array_class" }, { "firstCode", "uint16_class" }, { "entryCount", "uint16_class" }, { "idDelta", "int16_class" }, { "idRangeOffset", "uint16_class" }, { "glyphIdArray_", "USHORT_array_class" }, { "subHeaders", "SubHeader_array_class" }, { "format2", "tcmap_format2" }, { "segCountX2", "USHORT_class" }, { "endCount", "USHORT_array_class" }, { "reservedPad", "USHORT_class" }, { "startCount", "USHORT_array_class" }, { "idDelta_", "SHORT_array_class" }, { "idRangeOffset_", "USHORT_array_class" }, { "format4", "tcmap_format4" }, { "firstCode_", "USHORT_class" }, { "entryCount_", "USHORT_class" }, { "format6", "tcmap_format6" }, { "reserved", "USHORT_class" }, { "language_", "ULONG_class" }, { "is32", "BYTE_array_class" }, { "nGroups", "ULONG_class" }, { "startCharCode", "ULONG_class" }, { "endCharCode", "ULONG_class" }, { "startGlyphID", "ULONG_class" }, { "groupings", "tcmap_format8_groupings_struct_array_class" }, { "format8", "tcmap_format8" }, { "groupings_", "tcmap_format12_groupings_struct_array_class" }, { "format12", "tcmap_format12" }, { "EncodingRecord", "tEncodingRecord_array_class" }, { "cmap", "tcmap" }, { "version_", "TT_Fixed_class" }, { "fontRevision", "TT_Fixed_class" }, { "checkSumAdjustment", "ULONG_class" }, { "magicNumber", "ULONG_class" }, { "flags", "USHORT_class" }, { "unitsPerEm", "USHORT_class" }, { "created", "LONGDATETIME_class" }, { "modified", "LONGDATETIME_class" }, { "xMin", "SHORT_class" }, { "yMin", "SHORT_class" }, { "xMax", "SHORT_class" }, { "yMax", "SHORT_class" }, { "macStyle", "USHORT_class" }, { "lowestRecPPEM", "USHORT_class" }, { "fontDirectionHint", "SHORT_class" }, { "indexToLocFormat", "SHORT_class" }, { "glyphDataFormat", "SHORT_class" }, { "head", "thead" }, { "Ascender", "TT_FWord_class" }, { "Descender", "TT_FWord_class" }, { "LineGap", "TT_FWord_class" }, { "advanceWidthMax", "TT_UFWord_class" }, { "minLeftSideBearing", "TT_FWord_class" }, { "minRightSideBearing", "TT_FWord_class" }, { "xMaxExtent", "TT_FWord_class" }, { "caretSlopeRise", "SHORT_class" }, { "caretSlopeRun", "SHORT_class" }, { "caretOffset", "SHORT_class" }, { "reserved_", "SHORT_class" }, { "metricDataFormat", "SHORT_class" }, { "numberOfHMetrics", "USHORT_class" }, { "hhea", "thhea" }, { "advanceWidth", "USHORT_class" }, { "lsb", "SHORT_class" }, { "hMetrics", "tlongHorMetric_array_class" }, { "leftSideBearing", "SHORT_array_class" }, { "hmtx", "thmtx" }, { "numGlyphs", "USHORT_class" }, { "maxPoints", "USHORT_class" }, { "maxContours", "USHORT_class" }, { "maxCompositePoints", "USHORT_class" }, { "maxCompositeContours", "USHORT_class" }, { "maxZones", "USHORT_class" }, { "maxTwilightPoints", "USHORT_class" }, { "maxStorage", "USHORT_class" }, { "maxFunctionDefs", "USHORT_class" }, { "maxInstructionDefs", "USHORT_class" }, { "maxStackElements", "USHORT_class" }, { "maxSizeOfInstructions", "USHORT_class" }, { "maxComponentElements", "USHORT_class" }, { "maxComponentDepth", "USHORT_class" }, { "maxp", "tmaxp" }, { "count", "USHORT_class" }, { "stringOffset", "USHORT_class" }, { "languageID", "USHORT_class" }, { "nameID", "USHORT_class" }, { "offset_", "USHORT_class" }, { "name", "char_array_class" }, { "NameRecord", "tNameRecord_array_class" }, { "name_", "tname" }, { "xAvgCharWidth", "SHORT_class" }, { "usWeightClass", "USHORT_class" }, { "usWidthClass", "USHORT_class" }, { "fsType", "USHORT_class" }, { "ySubscriptXSize", "SHORT_class" }, { "ySubscriptYSize", "SHORT_class" }, { "ySubscriptXOffset", "SHORT_class" }, { "ySubscriptYOffset", "SHORT_class" }, { "ySuperscriptXSize", "SHORT_class" }, { "ySuperscriptYSize", "SHORT_class" }, { "ySuperscriptXOffset", "SHORT_class" }, { "ySuperscriptYOffset", "SHORT_class" }, { "yStrikeoutSize", "SHORT_class" }, { "yStrikeoutPosition", "SHORT_class" }, { "sFamilyClass", "SHORT_class" }, { "bFamilyType", "UBYTE_class" }, { "bSerifStyle", "UBYTE_class" }, { "bWeight", "UBYTE_class" }, { "bProportion", "UBYTE_class" }, { "bContrast", "UBYTE_class" }, { "bStrokeVariation", "UBYTE_class" }, { "bArmStyle", "UBYTE_class" }, { "bLetterform", "UBYTE_class" }, { "bMidline", "UBYTE_class" }, { "bXHeight", "UBYTE_class" }, { "panose", "tpanose" }, { "ulUnicodeRange1", "ULONG_class" }, { "ulUnicodeRange2", "ULONG_class" }, { "ulUnicodeRange3", "ULONG_class" }, { "ulUnicodeRange4", "ULONG_class" }, { "achVendID", "CHAR_array_class" }, { "fsSelection", "USHORT_class" }, { "usFirstCharIndex", "USHORT_class" }, { "usLastCharIndex", "USHORT_class" }, { "sTypoAscender", "SHORT_class" }, { "sTypoDescender", "SHORT_class" }, { "sTypoLineGap", "SHORT_class" }, { "usWinAscent", "USHORT_class" }, { "usWinDescent", "USHORT_class" }, { "ulCodePageRange1", "ULONG_class" }, { "ulCodePageRange2", "ULONG_class" }, { "sxHeight", "SHORT_class" }, { "sCapHeight", "SHORT_class" }, { "usDefaultChar", "USHORT_class" }, { "usBreakChar", "USHORT_class" }, { "usMaxContext", "USHORT_class" }, { "OS_2", "tOS_2" }, { "italicAngle", "TT_Fixed_class" }, { "underlinePosition", "TT_FWord_class" }, { "underlineThickness", "TT_FWord_class" }, { "isFixedPitch", "ULONG_class" }, { "minMemType42", "ULONG_class" }, { "maxMemType42", "ULONG_class" }, { "minMemType1", "ULONG_class" }, { "maxMemType1", "ULONG_class" }, { "numberOfGlyphs", "USHORT_class" }, { "glyphNameIndex", "USHORT_array_class" }, { "length__", "UBYTE_class" }, { "text", "CHAR_array_class" }, { "name__", "tpostName" }, { "offset__", "USHORT_array_class" }, { "post", "tpost" }, { "offsets", "USHORT_array_class" }, { "offsets_", "ULONG_array_class" }, { "loca", "tloca" }, { "ttf", "ttfFile_array_class" } };

std::vector<std::vector<int>> integer_ranges = { { 1, 16 } };

class globals_class {
public:
	int _struct_id = 0;
	int _struct_id_counter = 0;
	/*local*/ int maxGlyfRead;
	/*local*/ USHORT ARG_1_AND_2_ARE_WORDS;
	/*local*/ USHORT ARGS_ARE_XY_VALUES;
	/*local*/ USHORT ROUND_XY_TO_GRID;
	/*local*/ USHORT WE_HAVE_A_SCALE;
	/*local*/ USHORT RESERVED;
	/*local*/ USHORT MORE_COMPONENTS;
	/*local*/ USHORT WE_HAVE_AN_X_AND_Y_SCALE;
	/*local*/ USHORT WE_HAVE_A_TWO_BY_TWO;
	/*local*/ USHORT WE_HAVE_INSTRUCTIONS;
	/*local*/ USHORT USE_MY_METRICS;
	/*local*/ USHORT OVERLAP_COMPOUND;
	/*local*/ USHORT SCALED_COMPONENT_OFFSET;
	/*local*/ USHORT UNSCALED_COMPONENT_OFFSET;
	/*local*/ UBYTE ON_CURVE;
	/*local*/ UBYTE X_BYTE;
	/*local*/ UBYTE Y_BYTE;
	/*local*/ UBYTE REPEAT;
	/*local*/ UBYTE X_TYPE;
	/*local*/ UBYTE Y_TYPE;
	/*local*/ UBYTE RES_1;
	/*local*/ UBYTE RES_2;
	/*local*/ ULONG curTblOffset;
	/*local*/ ULONG curTblLength;
	/*local*/ int64 nextTTFRec;
	/*local*/ uint32 ttfId;
	/*local*/ uint32 numFonts_2;
	/*local*/ std::vector<int> possible_values_1;
	char_class tag_element;
	char_array_class tag;
	uint16_class majorVersion;
	uint16_class minorVersion;
	uint32_class numFonts;
	uint32_class offsetTable_element;
	uint32_array_class offsetTable;
	char_class dsigTag_element;
	char_array_class dsigTag;
	uint32_class dsigLength;
	uint32_class dsigOffset;
	TTCHeader ttc;
	TT_Fixed_class SFNT_Ver;
	USHORT_class numTables;
	USHORT_class searchRange;
	USHORT_class entrySelector;
	USHORT_class rangeShift;
	tOffsetTable OffsetTable;
	string_class tempstr;
	char_class asChar_element;
	char_array_class asChar;
	ULONG_class asLong;
	tTable_Tag_union Tag;
	ULONG_class checkSum;
	ULONG_class offset;
	ULONG_class length;
	tTable Table;
	USHORT_class version;
	USHORT_class platformID;
	USHORT_class encodingID;
	USHORT_class format;
	USHORT_class length_;
	USHORT_class language;
	BYTE_class glyphIdArray_element;
	BYTE_array_class glyphIdArray;
	tcmap_format0 format0;
	USHORT_class subHeaderKeys_element;
	USHORT_array_class subHeaderKeys;
	uint16_class firstCode;
	uint16_class entryCount;
	int16_class idDelta;
	uint16_class idRangeOffset;
	USHORT_class glyphIdArray__element;
	USHORT_array_class glyphIdArray_;
	SubHeader subHeaders_element;
	SubHeader_array_class subHeaders;
	tcmap_format2 format2;
	USHORT_class segCountX2;
	USHORT_class endCount_element;
	USHORT_array_class endCount;
	USHORT_class reservedPad;
	USHORT_class startCount_element;
	USHORT_array_class startCount;
	SHORT_class idDelta__element;
	SHORT_array_class idDelta_;
	USHORT_class idRangeOffset__element;
	USHORT_array_class idRangeOffset_;
	tcmap_format4 format4;
	USHORT_class firstCode_;
	USHORT_class entryCount_;
	tcmap_format6 format6;
	USHORT_class reserved;
	ULONG_class language_;
	BYTE_class is32_element;
	BYTE_array_class is32;
	ULONG_class nGroups;
	ULONG_class startCharCode;
	ULONG_class endCharCode;
	ULONG_class startGlyphID;
	tcmap_format8_groupings_struct groupings_element;
	tcmap_format8_groupings_struct_array_class groupings;
	tcmap_format8 format8;
	tcmap_format12_groupings_struct groupings__element;
	tcmap_format12_groupings_struct_array_class groupings_;
	tcmap_format12 format12;
	tEncodingRecord EncodingRecord_element;
	tEncodingRecord_array_class EncodingRecord;
	tcmap cmap;
	TT_Fixed_class version_;
	TT_Fixed_class fontRevision;
	ULONG_class checkSumAdjustment;
	ULONG_class magicNumber;
	USHORT_class flags;
	USHORT_class unitsPerEm;
	LONGDATETIME_class created;
	LONGDATETIME_class modified;
	SHORT_class xMin;
	SHORT_class yMin;
	SHORT_class xMax;
	SHORT_class yMax;
	USHORT_class macStyle;
	USHORT_class lowestRecPPEM;
	SHORT_class fontDirectionHint;
	SHORT_class indexToLocFormat;
	SHORT_class glyphDataFormat;
	thead head;
	TT_FWord_class Ascender;
	TT_FWord_class Descender;
	TT_FWord_class LineGap;
	TT_UFWord_class advanceWidthMax;
	TT_FWord_class minLeftSideBearing;
	TT_FWord_class minRightSideBearing;
	TT_FWord_class xMaxExtent;
	SHORT_class caretSlopeRise;
	SHORT_class caretSlopeRun;
	SHORT_class caretOffset;
	SHORT_class reserved_;
	SHORT_class metricDataFormat;
	USHORT_class numberOfHMetrics;
	thhea hhea;
	USHORT_class advanceWidth;
	SHORT_class lsb;
	tlongHorMetric hMetrics_element;
	tlongHorMetric_array_class hMetrics;
	SHORT_class leftSideBearing_element;
	SHORT_array_class leftSideBearing;
	thmtx hmtx;
	USHORT_class numGlyphs;
	USHORT_class maxPoints;
	USHORT_class maxContours;
	USHORT_class maxCompositePoints;
	USHORT_class maxCompositeContours;
	USHORT_class maxZones;
	USHORT_class maxTwilightPoints;
	USHORT_class maxStorage;
	USHORT_class maxFunctionDefs;
	USHORT_class maxInstructionDefs;
	USHORT_class maxStackElements;
	USHORT_class maxSizeOfInstructions;
	USHORT_class maxComponentElements;
	USHORT_class maxComponentDepth;
	tmaxp maxp;
	USHORT_class count;
	USHORT_class stringOffset;
	USHORT_class languageID;
	USHORT_class nameID;
	USHORT_class offset_;
	char_class name_element;
	char_array_class name;
	tNameRecord NameRecord_element;
	tNameRecord_array_class NameRecord;
	tname name_;
	SHORT_class xAvgCharWidth;
	USHORT_class usWeightClass;
	USHORT_class usWidthClass;
	USHORT_class fsType;
	SHORT_class ySubscriptXSize;
	SHORT_class ySubscriptYSize;
	SHORT_class ySubscriptXOffset;
	SHORT_class ySubscriptYOffset;
	SHORT_class ySuperscriptXSize;
	SHORT_class ySuperscriptYSize;
	SHORT_class ySuperscriptXOffset;
	SHORT_class ySuperscriptYOffset;
	SHORT_class yStrikeoutSize;
	SHORT_class yStrikeoutPosition;
	SHORT_class sFamilyClass;
	UBYTE_class bFamilyType;
	UBYTE_class bSerifStyle;
	UBYTE_class bWeight;
	UBYTE_class bProportion;
	UBYTE_class bContrast;
	UBYTE_class bStrokeVariation;
	UBYTE_class bArmStyle;
	UBYTE_class bLetterform;
	UBYTE_class bMidline;
	UBYTE_class bXHeight;
	tpanose panose;
	ULONG_class ulUnicodeRange1;
	ULONG_class ulUnicodeRange2;
	ULONG_class ulUnicodeRange3;
	ULONG_class ulUnicodeRange4;
	CHAR_class achVendID_element;
	CHAR_array_class achVendID;
	USHORT_class fsSelection;
	USHORT_class usFirstCharIndex;
	USHORT_class usLastCharIndex;
	SHORT_class sTypoAscender;
	SHORT_class sTypoDescender;
	SHORT_class sTypoLineGap;
	USHORT_class usWinAscent;
	USHORT_class usWinDescent;
	ULONG_class ulCodePageRange1;
	ULONG_class ulCodePageRange2;
	SHORT_class sxHeight;
	SHORT_class sCapHeight;
	USHORT_class usDefaultChar;
	USHORT_class usBreakChar;
	USHORT_class usMaxContext;
	tOS_2 OS_2;
	TT_Fixed_class italicAngle;
	TT_FWord_class underlinePosition;
	TT_FWord_class underlineThickness;
	ULONG_class isFixedPitch;
	ULONG_class minMemType42;
	ULONG_class maxMemType42;
	ULONG_class minMemType1;
	ULONG_class maxMemType1;
	USHORT_class numberOfGlyphs;
	USHORT_class glyphNameIndex_element;
	USHORT_array_class glyphNameIndex;
	UBYTE_class length__;
	CHAR_class text_element;
	CHAR_array_class text;
	tpostName name__;
	USHORT_class offset___element;
	USHORT_array_class offset__;
	tpost post;
	USHORT_class offsets_element;
	USHORT_array_class offsets;
	ULONG_class offsets__element;
	ULONG_array_class offsets_;
	tloca loca;
	ttfFile ttf_element;
	ttfFile_array_class ttf;


	globals_class() :
		tag_element(false),
		tag(tag_element),
		majorVersion(1, { 2 }),
		minorVersion(1),
		numFonts(1),
		offsetTable_element(false),
		offsetTable(offsetTable_element),
		dsigTag_element(false),
		dsigTag(dsigTag_element),
		dsigLength(1),
		dsigOffset(1),
		ttc(TTCHeader_ttc_instances),
		SFNT_Ver(1),
		numTables(1),
		searchRange(1),
		entrySelector(1),
		rangeShift(1),
		OffsetTable(tOffsetTable_OffsetTable_instances),
		asChar_element(false),
		asChar(asChar_element),
		asLong(1),
		Tag(tTable_Tag_union_Tag_instances),
		checkSum(1),
		offset(1),
		length(1),
		Table(tTable_Table_instances),
		version(1, { 0x00005000, 0x00020000, 0x00025000 }),
		platformID(1),
		encodingID(1),
		format(1),
		length_(1),
		language(1),
		glyphIdArray_element(false),
		glyphIdArray(glyphIdArray_element),
		format0(tcmap_format0_format0_instances),
		subHeaderKeys_element(false),
		subHeaderKeys(subHeaderKeys_element),
		firstCode(1),
		entryCount(1),
		idDelta(1),
		idRangeOffset(1),
		glyphIdArray__element(false),
		glyphIdArray_(glyphIdArray__element),
		subHeaders_element(SubHeader_subHeaders_element_instances),
		subHeaders(subHeaders_element),
		format2(tcmap_format2_format2_instances),
		segCountX2(1),
		endCount_element(false),
		endCount(endCount_element),
		reservedPad(1),
		startCount_element(false),
		startCount(startCount_element),
		idDelta__element(false),
		idDelta_(idDelta__element),
		idRangeOffset__element(false),
		idRangeOffset_(idRangeOffset__element),
		format4(tcmap_format4_format4_instances),
		firstCode_(1),
		entryCount_(1),
		format6(tcmap_format6_format6_instances),
		reserved(1),
		language_(1),
		is32_element(false),
		is32(is32_element),
		nGroups(1),
		startCharCode(1),
		endCharCode(1),
		startGlyphID(1),
		groupings_element(tcmap_format8_groupings_struct_groupings_element_instances),
		groupings(groupings_element),
		format8(tcmap_format8_format8_instances),
		groupings__element(tcmap_format12_groupings_struct_groupings__element_instances),
		groupings_(groupings__element),
		format12(tcmap_format12_format12_instances),
		EncodingRecord_element(tEncodingRecord_EncodingRecord_element_instances),
		EncodingRecord(EncodingRecord_element),
		cmap(tcmap_cmap_instances),
		version_(1),
		fontRevision(1),
		checkSumAdjustment(1),
		magicNumber(1),
		flags(1),
		unitsPerEm(1),
		created(1),
		modified(1),
		xMin(1),
		yMin(1),
		xMax(1),
		yMax(1),
		macStyle(1),
		lowestRecPPEM(1),
		fontDirectionHint(1),
		indexToLocFormat(1),
		glyphDataFormat(1),
		head(thead_head_instances),
		Ascender(1),
		Descender(1),
		LineGap(1),
		advanceWidthMax(1),
		minLeftSideBearing(1),
		minRightSideBearing(1),
		xMaxExtent(1),
		caretSlopeRise(1),
		caretSlopeRun(1),
		caretOffset(1),
		reserved_(1),
		metricDataFormat(1),
		numberOfHMetrics(1),
		hhea(thhea_hhea_instances),
		advanceWidth(1),
		lsb(1),
		hMetrics_element(tlongHorMetric_hMetrics_element_instances),
		hMetrics(hMetrics_element),
		leftSideBearing_element(false),
		leftSideBearing(leftSideBearing_element),
		hmtx(thmtx_hmtx_instances),
		numGlyphs(1),
		maxPoints(1),
		maxContours(1),
		maxCompositePoints(1),
		maxCompositeContours(1),
		maxZones(1),
		maxTwilightPoints(1),
		maxStorage(1),
		maxFunctionDefs(1),
		maxInstructionDefs(1),
		maxStackElements(1),
		maxSizeOfInstructions(1),
		maxComponentElements(1),
		maxComponentDepth(1),
		maxp(tmaxp_maxp_instances),
		count(1),
		stringOffset(1),
		languageID(1),
		nameID(1),
		offset_(1),
		name_element(false),
		name(name_element),
		NameRecord_element(tNameRecord_NameRecord_element_instances),
		NameRecord(NameRecord_element),
		name_(tname_name__instances),
		xAvgCharWidth(1),
		usWeightClass(1),
		usWidthClass(1),
		fsType(1),
		ySubscriptXSize(1),
		ySubscriptYSize(1),
		ySubscriptXOffset(1),
		ySubscriptYOffset(1),
		ySuperscriptXSize(1),
		ySuperscriptYSize(1),
		ySuperscriptXOffset(1),
		ySuperscriptYOffset(1),
		yStrikeoutSize(1),
		yStrikeoutPosition(1),
		sFamilyClass(1),
		bFamilyType(1),
		bSerifStyle(1),
		bWeight(1),
		bProportion(1),
		bContrast(1),
		bStrokeVariation(1),
		bArmStyle(1),
		bLetterform(1),
		bMidline(1),
		bXHeight(1),
		panose(tpanose_panose_instances),
		ulUnicodeRange1(1),
		ulUnicodeRange2(1),
		ulUnicodeRange3(1),
		ulUnicodeRange4(1),
		achVendID_element(false),
		achVendID(achVendID_element),
		fsSelection(1),
		usFirstCharIndex(1),
		usLastCharIndex(1),
		sTypoAscender(1),
		sTypoDescender(1),
		sTypoLineGap(1),
		usWinAscent(1),
		usWinDescent(1),
		ulCodePageRange1(1),
		ulCodePageRange2(1),
		sxHeight(1),
		sCapHeight(1),
		usDefaultChar(1),
		usBreakChar(1),
		usMaxContext(1),
		OS_2(tOS_2_OS_2_instances),
		italicAngle(1),
		underlinePosition(1),
		underlineThickness(1),
		isFixedPitch(1),
		minMemType42(1),
		maxMemType42(1),
		minMemType1(1),
		maxMemType1(1),
		numberOfGlyphs(1),
		glyphNameIndex_element(false),
		glyphNameIndex(glyphNameIndex_element),
		length__(1),
		text_element(false),
		text(text_element),
		name__(tpostName_name___instances),
		offset___element(false),
		offset__(offset___element),
		post(tpost_post_instances),
		offsets_element(false),
		offsets(offsets_element),
		offsets__element(false),
		offsets_(offsets__element),
		loca(tloca_loca_instances),
		ttf_element(ttfFile_ttf_element_instances),
		ttf(ttf_element)
	{}
};

globals_class* g;


TTCHeader* TTCHeader::generate() {
	if (generated == 1) {
		TTCHeader* new_instance = new TTCHeader(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(tag, ::g->tag.generate(4));
	GENERATE_VAR(majorVersion, ::g->majorVersion.generate());
	GENERATE_VAR(minorVersion, ::g->minorVersion.generate());
	GENERATE_VAR(numFonts, ::g->numFonts.generate());
	GENERATE_VAR(offsetTable, ::g->offsetTable.generate(numFonts()));
	if ((majorVersion() == 2)) {
		GENERATE_VAR(dsigTag, ::g->dsigTag.generate(4));
		GENERATE_VAR(dsigLength, ::g->dsigLength.generate());
		GENERATE_VAR(dsigOffset, ::g->dsigOffset.generate());
	};

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tOffsetTable* tOffsetTable::generate() {
	if (generated == 1) {
		tOffsetTable* new_instance = new tOffsetTable(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(SFNT_Ver, ::g->SFNT_Ver.generate());
	GENERATE_VAR(numTables, ::g->numTables.generate());
	GENERATE_VAR(searchRange, ::g->searchRange.generate());
	GENERATE_VAR(entrySelector, ::g->entrySelector.generate());
	GENERATE_VAR(rangeShift, ::g->rangeShift.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tTable_Tag_union* tTable_Tag_union::generate() {
	if (generated == 1) {
		tTable_Tag_union* new_instance = new tTable_Tag_union(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(asChar, ::g->asChar.generate(4));
	GENERATE_EXISTS(asLong, ::g->asLong.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tTable* tTable::generate() {
	if (generated == 1) {
		tTable* new_instance = new tTable(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(Tag, ::g->Tag.generate());
	GENERATE_VAR(checkSum, ::g->checkSum.generate());
	GENERATE_VAR(offset, ::g->offset.generate());
	GENERATE_VAR(length, ::g->length.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format0* tcmap_format0::generate(quad& cmap_subtable) {
	if (generated == 1) {
		tcmap_format0* new_instance = new tcmap_format0(instances);
		new_instance->generated = 2;
		return new_instance->generate(cmap_subtable);
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	cmap_subtable = FTell();
	GENERATE_VAR(format, ::g->format.generate());
	GENERATE_VAR(length, ::g->length_.generate());
	GENERATE_VAR(language, ::g->language.generate());
	GENERATE_VAR(glyphIdArray, ::g->glyphIdArray.generate(256));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


SubHeader* SubHeader::generate() {
	if (generated == 1) {
		SubHeader* new_instance = new SubHeader(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(firstCode, ::g->firstCode.generate());
	GENERATE_VAR(entryCount, ::g->entryCount.generate());
	GENERATE_VAR(idDelta, ::g->idDelta.generate());
	GENERATE_VAR(idRangeOffset, ::g->idRangeOffset.generate());
	curPos = FTell();
	FSeek(((curPos + idRangeOffset()) - 2));
	GENERATE_VAR(glyphIdArray, ::g->glyphIdArray_.generate(entryCount()));
	FSeek(curPos);

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format2* tcmap_format2::generate(quad& cmap_subtable) {
	if (generated == 1) {
		tcmap_format2* new_instance = new tcmap_format2(instances);
		new_instance->generated = 2;
		return new_instance->generate(cmap_subtable);
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	cmap_subtable = FTell();
	GENERATE_VAR(format, ::g->format.generate());
	GENERATE_VAR(length, ::g->length_.generate());
	GENERATE_VAR(language, ::g->language.generate());
	GENERATE_VAR(subHeaderKeys, ::g->subHeaderKeys.generate(256));
	maxidx = 0;
	i = 0;
	for (i = 0; (i < 256); ++i) {
		if ((subHeaderKeys()[i] > maxidx)) {
	maxidx = subHeaderKeys()[i];
	};
	};
	maxidx = (maxidx / 8);
	GENERATE_VAR(subHeaders, ::g->subHeaders.generate(maxidx));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format4* tcmap_format4::generate(quad& cmap_subtable) {
	if (generated == 1) {
		tcmap_format4* new_instance = new tcmap_format4(instances);
		new_instance->generated = 2;
		return new_instance->generate(cmap_subtable);
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	cmap_subtable = FTell();
	GENERATE_VAR(format, ::g->format.generate());
	GENERATE_VAR(length, ::g->length_.generate());
	GENERATE_VAR(language, ::g->language.generate());
	GENERATE_VAR(segCountX2, ::g->segCountX2.generate());
	GENERATE_VAR(searchRange, ::g->searchRange.generate());
	GENERATE_VAR(entrySelector, ::g->entrySelector.generate());
	GENERATE_VAR(rangeShift, ::g->rangeShift.generate());
	GENERATE_VAR(endCount, ::g->endCount.generate((segCountX2() / 2)));
	GENERATE_VAR(reservedPad, ::g->reservedPad.generate());
	GENERATE_VAR(startCount, ::g->startCount.generate((segCountX2() / 2)));
	GENERATE_VAR(idDelta, ::g->idDelta_.generate((segCountX2() / 2)));
	GENERATE_VAR(idRangeOffset, ::g->idRangeOffset_.generate((segCountX2() / 2)));
	GENERATE_VAR(glyphIdArray, ::g->glyphIdArray_.generate(((length() - (FTell() - cmap_subtable)) / 2)));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format6* tcmap_format6::generate(quad& cmap_subtable) {
	if (generated == 1) {
		tcmap_format6* new_instance = new tcmap_format6(instances);
		new_instance->generated = 2;
		return new_instance->generate(cmap_subtable);
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	cmap_subtable = FTell();
	GENERATE_VAR(format, ::g->format.generate());
	GENERATE_VAR(length, ::g->length_.generate());
	GENERATE_VAR(language, ::g->language.generate());
	GENERATE_VAR(firstCode, ::g->firstCode_.generate());
	GENERATE_VAR(entryCount, ::g->entryCount_.generate());
	GENERATE_VAR(glyphIdArray, ::g->glyphIdArray_.generate(entryCount()));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format8_groupings_struct* tcmap_format8_groupings_struct::generate() {
	if (generated == 1) {
		tcmap_format8_groupings_struct* new_instance = new tcmap_format8_groupings_struct(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(startCharCode, ::g->startCharCode.generate());
	GENERATE_VAR(endCharCode, ::g->endCharCode.generate());
	GENERATE_VAR(startGlyphID, ::g->startGlyphID.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format8* tcmap_format8::generate(quad& cmap_subtable) {
	if (generated == 1) {
		tcmap_format8* new_instance = new tcmap_format8(instances);
		new_instance->generated = 2;
		return new_instance->generate(cmap_subtable);
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	cmap_subtable = FTell();
	GENERATE_VAR(format, ::g->format.generate());
	GENERATE_VAR(reserved, ::g->reserved.generate());
	GENERATE_VAR(length, ::g->length.generate());
	GENERATE_VAR(language, ::g->language_.generate());
	GENERATE_VAR(is32, ::g->is32.generate(8192));
	GENERATE_VAR(nGroups, ::g->nGroups.generate());
	GENERATE_VAR(groupings, ::g->groupings.generate(nGroups()));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format12_groupings_struct* tcmap_format12_groupings_struct::generate() {
	if (generated == 1) {
		tcmap_format12_groupings_struct* new_instance = new tcmap_format12_groupings_struct(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(startCharCode, ::g->startCharCode.generate());
	GENERATE_VAR(endCharCode, ::g->endCharCode.generate());
	GENERATE_VAR(startGlyphID, ::g->startGlyphID.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap_format12* tcmap_format12::generate() {
	if (generated == 1) {
		tcmap_format12* new_instance = new tcmap_format12(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(format, ::g->format.generate());
	GENERATE_VAR(reserved, ::g->reserved.generate());
	GENERATE_VAR(length, ::g->length.generate());
	GENERATE_VAR(language, ::g->language_.generate());
	GENERATE_VAR(nGroups, ::g->nGroups.generate());
	GENERATE_VAR(groupings, ::g->groupings_.generate(nGroups()));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tEncodingRecord* tEncodingRecord::generate(quad& cmap_table, quad& next_cmap_record) {
	if (generated == 1) {
		tEncodingRecord* new_instance = new tEncodingRecord(instances);
		new_instance->generated = 2;
		return new_instance->generate(cmap_table, next_cmap_record);
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	cmap_record = FTell();
	GENERATE_VAR(platformID, ::g->platformID.generate());
	GENERATE_VAR(encodingID, ::g->encodingID.generate());
	GENERATE_VAR(offset, ::g->offset.generate());
	next_cmap_record = FTell();
	FSeek((cmap_table + offset()));
	switch (ReadUShort(FTell())) {
	case 0:
		GENERATE_VAR(format0, ::g->format0.generate(cmap_subtable));
		break;
	case 2:
		GENERATE_VAR(format2, ::g->format2.generate(cmap_subtable));
		break;
	case 4:
		GENERATE_VAR(format4, ::g->format4.generate(cmap_subtable));
		break;
	case 6:
		GENERATE_VAR(format6, ::g->format6.generate(cmap_subtable));
		break;
	case 8:
		GENERATE_VAR(format8, ::g->format8.generate(cmap_subtable));
		break;
	case 12:
		GENERATE_VAR(format12, ::g->format12.generate());
		break;
	};
	FSeek(next_cmap_record);

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tcmap* tcmap::generate() {
	if (generated == 1) {
		tcmap* new_instance = new tcmap(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	cmap_table = FTell();
	GENERATE_VAR(version, ::g->version.generate());
	GENERATE_VAR(numTables, ::g->numTables.generate());
	GENERATE_VAR(EncodingRecord, ::g->EncodingRecord.generate(numTables()));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


thead* thead::generate() {
	if (generated == 1) {
		thead* new_instance = new thead(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	::g->ttfId++;
	GENERATE_VAR(version, ::g->version_.generate());
	GENERATE_VAR(fontRevision, ::g->fontRevision.generate());
	GENERATE_VAR(checkSumAdjustment, ::g->checkSumAdjustment.generate());
	GENERATE_VAR(magicNumber, ::g->magicNumber.generate());
	GENERATE_VAR(flags, ::g->flags.generate());
	GENERATE_VAR(unitsPerEm, ::g->unitsPerEm.generate());
	GENERATE_VAR(created, ::g->created.generate());
	GENERATE_VAR(modified, ::g->modified.generate());
	GENERATE_VAR(xMin, ::g->xMin.generate());
	GENERATE_VAR(yMin, ::g->yMin.generate());
	GENERATE_VAR(xMax, ::g->xMax.generate());
	GENERATE_VAR(yMax, ::g->yMax.generate());
	GENERATE_VAR(macStyle, ::g->macStyle.generate());
	GENERATE_VAR(lowestRecPPEM, ::g->lowestRecPPEM.generate());
	GENERATE_VAR(fontDirectionHint, ::g->fontDirectionHint.generate());
	GENERATE_VAR(indexToLocFormat, ::g->indexToLocFormat.generate());
	GENERATE_VAR(glyphDataFormat, ::g->glyphDataFormat.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


thhea* thhea::generate() {
	if (generated == 1) {
		thhea* new_instance = new thhea(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(version, ::g->version_.generate());
	GENERATE_VAR(Ascender, ::g->Ascender.generate());
	GENERATE_VAR(Descender, ::g->Descender.generate());
	GENERATE_VAR(LineGap, ::g->LineGap.generate());
	GENERATE_VAR(advanceWidthMax, ::g->advanceWidthMax.generate());
	GENERATE_VAR(minLeftSideBearing, ::g->minLeftSideBearing.generate());
	GENERATE_VAR(minRightSideBearing, ::g->minRightSideBearing.generate());
	GENERATE_VAR(xMaxExtent, ::g->xMaxExtent.generate());
	GENERATE_VAR(caretSlopeRise, ::g->caretSlopeRise.generate());
	GENERATE_VAR(caretSlopeRun, ::g->caretSlopeRun.generate());
	GENERATE_VAR(caretOffset, ::g->caretOffset.generate());
	GENERATE_VAR(reserved, ::g->reserved_.generate());
	GENERATE_VAR(reserved, ::g->reserved_.generate());
	GENERATE_VAR(reserved, ::g->reserved_.generate());
	GENERATE_VAR(reserved, ::g->reserved_.generate());
	GENERATE_VAR(metricDataFormat, ::g->metricDataFormat.generate());
	GENERATE_VAR(numberOfHMetrics, ::g->numberOfHMetrics.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tlongHorMetric* tlongHorMetric::generate() {
	if (generated == 1) {
		tlongHorMetric* new_instance = new tlongHorMetric(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(advanceWidth, ::g->advanceWidth.generate());
	GENERATE_VAR(lsb, ::g->lsb.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


thmtx* thmtx::generate() {
	if (generated == 1) {
		thmtx* new_instance = new thmtx(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	numberOfHMetrics = ::g->ttf()[(::g->ttfId - 1)].hhea().numberOfHMetrics();
	GENERATE_VAR(hMetrics, ::g->hMetrics.generate(numberOfHMetrics));
	numLeftSideBearing = ((::g->curTblLength - (FTell() - ::g->curTblOffset)) / 2);
	if (numLeftSideBearing) {
		GENERATE_VAR(leftSideBearing, ::g->leftSideBearing.generate(numLeftSideBearing));
	};

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tmaxp* tmaxp::generate() {
	if (generated == 1) {
		tmaxp* new_instance = new tmaxp(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(version, ::g->version_.generate());
	if ((version() == 0x00005000)) {
		GENERATE_VAR(numGlyphs, ::g->numGlyphs.generate());
	} else {
		GENERATE_VAR(numGlyphs, ::g->numGlyphs.generate());
		GENERATE_VAR(maxPoints, ::g->maxPoints.generate());
		GENERATE_VAR(maxContours, ::g->maxContours.generate());
		GENERATE_VAR(maxCompositePoints, ::g->maxCompositePoints.generate());
		GENERATE_VAR(maxCompositeContours, ::g->maxCompositeContours.generate());
		GENERATE_VAR(maxZones, ::g->maxZones.generate());
		GENERATE_VAR(maxTwilightPoints, ::g->maxTwilightPoints.generate());
		GENERATE_VAR(maxStorage, ::g->maxStorage.generate());
		GENERATE_VAR(maxFunctionDefs, ::g->maxFunctionDefs.generate());
		GENERATE_VAR(maxInstructionDefs, ::g->maxInstructionDefs.generate());
		GENERATE_VAR(maxStackElements, ::g->maxStackElements.generate());
		GENERATE_VAR(maxSizeOfInstructions, ::g->maxSizeOfInstructions.generate());
		GENERATE_VAR(maxComponentElements, ::g->maxComponentElements.generate());
		GENERATE_VAR(maxComponentDepth, ::g->maxComponentDepth.generate());
	};

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tNameRecord* tNameRecord::generate(quad& name_table, quad& NextNameRecord) {
	if (generated == 1) {
		tNameRecord* new_instance = new tNameRecord(instances);
		new_instance->generated = 2;
		return new_instance->generate(name_table, NextNameRecord);
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(platformID, ::g->platformID.generate());
	GENERATE_VAR(encodingID, ::g->encodingID.generate());
	GENERATE_VAR(languageID, ::g->languageID.generate());
	GENERATE_VAR(nameID, ::g->nameID.generate());
	GENERATE_VAR(length, ::g->length_.generate());
	GENERATE_VAR(offset, ::g->offset_.generate());
	NextNameRecord = FTell();
	FSeek(((name_table + ::g->stringOffset()) + offset()));
	GENERATE_VAR(name, ::g->name.generate(length()));
	FSeek(NextNameRecord);

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tname* tname::generate() {
	if (generated == 1) {
		tname* new_instance = new tname(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	name_table = FTell();
	GENERATE_VAR(format, ::g->format.generate());
	GENERATE_VAR(count, ::g->count.generate());
	GENERATE_VAR(stringOffset, ::g->stringOffset.generate());
	GENERATE_VAR(NameRecord, ::g->NameRecord.generate(count()));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tpanose* tpanose::generate() {
	if (generated == 1) {
		tpanose* new_instance = new tpanose(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(bFamilyType, ::g->bFamilyType.generate());
	GENERATE_VAR(bSerifStyle, ::g->bSerifStyle.generate());
	GENERATE_VAR(bWeight, ::g->bWeight.generate());
	GENERATE_VAR(bProportion, ::g->bProportion.generate());
	GENERATE_VAR(bContrast, ::g->bContrast.generate());
	GENERATE_VAR(bStrokeVariation, ::g->bStrokeVariation.generate());
	GENERATE_VAR(bArmStyle, ::g->bArmStyle.generate());
	GENERATE_VAR(bLetterform, ::g->bLetterform.generate());
	GENERATE_VAR(bMidline, ::g->bMidline.generate());
	GENERATE_VAR(bXHeight, ::g->bXHeight.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tOS_2* tOS_2::generate() {
	if (generated == 1) {
		tOS_2* new_instance = new tOS_2(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(version, ::g->version.generate());
	GENERATE_VAR(xAvgCharWidth, ::g->xAvgCharWidth.generate());
	GENERATE_VAR(usWeightClass, ::g->usWeightClass.generate());
	GENERATE_VAR(usWidthClass, ::g->usWidthClass.generate());
	GENERATE_VAR(fsType, ::g->fsType.generate());
	GENERATE_VAR(ySubscriptXSize, ::g->ySubscriptXSize.generate());
	GENERATE_VAR(ySubscriptYSize, ::g->ySubscriptYSize.generate());
	GENERATE_VAR(ySubscriptXOffset, ::g->ySubscriptXOffset.generate());
	GENERATE_VAR(ySubscriptYOffset, ::g->ySubscriptYOffset.generate());
	GENERATE_VAR(ySuperscriptXSize, ::g->ySuperscriptXSize.generate());
	GENERATE_VAR(ySuperscriptYSize, ::g->ySuperscriptYSize.generate());
	GENERATE_VAR(ySuperscriptXOffset, ::g->ySuperscriptXOffset.generate());
	GENERATE_VAR(ySuperscriptYOffset, ::g->ySuperscriptYOffset.generate());
	GENERATE_VAR(yStrikeoutSize, ::g->yStrikeoutSize.generate());
	GENERATE_VAR(yStrikeoutPosition, ::g->yStrikeoutPosition.generate());
	GENERATE_VAR(sFamilyClass, ::g->sFamilyClass.generate());
	GENERATE_VAR(panose, ::g->panose.generate());
	GENERATE_VAR(ulUnicodeRange1, ::g->ulUnicodeRange1.generate());
	GENERATE_VAR(ulUnicodeRange2, ::g->ulUnicodeRange2.generate());
	GENERATE_VAR(ulUnicodeRange3, ::g->ulUnicodeRange3.generate());
	GENERATE_VAR(ulUnicodeRange4, ::g->ulUnicodeRange4.generate());
	GENERATE_VAR(achVendID, ::g->achVendID.generate(4));
	GENERATE_VAR(fsSelection, ::g->fsSelection.generate());
	GENERATE_VAR(usFirstCharIndex, ::g->usFirstCharIndex.generate());
	GENERATE_VAR(usLastCharIndex, ::g->usLastCharIndex.generate());
	GENERATE_VAR(sTypoAscender, ::g->sTypoAscender.generate());
	GENERATE_VAR(sTypoDescender, ::g->sTypoDescender.generate());
	GENERATE_VAR(sTypoLineGap, ::g->sTypoLineGap.generate());
	GENERATE_VAR(usWinAscent, ::g->usWinAscent.generate());
	GENERATE_VAR(usWinDescent, ::g->usWinDescent.generate());
	GENERATE_VAR(ulCodePageRange1, ::g->ulCodePageRange1.generate());
	GENERATE_VAR(ulCodePageRange2, ::g->ulCodePageRange2.generate());
	GENERATE_VAR(sxHeight, ::g->sxHeight.generate());
	GENERATE_VAR(sCapHeight, ::g->sCapHeight.generate());
	GENERATE_VAR(usDefaultChar, ::g->usDefaultChar.generate());
	GENERATE_VAR(usBreakChar, ::g->usBreakChar.generate());
	GENERATE_VAR(usMaxContext, ::g->usMaxContext.generate());

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tpostName* tpostName::generate() {
	if (generated == 1) {
		tpostName* new_instance = new tpostName(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	GENERATE_VAR(length, ::g->length__.generate());
	GENERATE_VAR(text, ::g->text.generate(length()));

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tpost* tpost::generate() {
	if (generated == 1) {
		tpost* new_instance = new tpost(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	post = FTell();
	GENERATE_VAR(version, ::g->version_.generate());
	GENERATE_VAR(italicAngle, ::g->italicAngle.generate());
	GENERATE_VAR(underlinePosition, ::g->underlinePosition.generate());
	GENERATE_VAR(underlineThickness, ::g->underlineThickness.generate());
	GENERATE_VAR(isFixedPitch, ::g->isFixedPitch.generate());
	GENERATE_VAR(minMemType42, ::g->minMemType42.generate());
	GENERATE_VAR(maxMemType42, ::g->maxMemType42.generate());
	GENERATE_VAR(minMemType1, ::g->minMemType1.generate());
	GENERATE_VAR(maxMemType1, ::g->maxMemType1.generate());
	if ((version() == 0x00020000)) {
		GENERATE_VAR(numberOfGlyphs, ::g->numberOfGlyphs.generate());
		numGlyphs = numberOfGlyphs();
		GENERATE_VAR(glyphNameIndex, ::g->glyphNameIndex.generate(numGlyphs));
		numberNewGlyphs = numberOfGlyphs();
		end_name = (post + ::g->curTblLength);
		next_name = FTell();
		while ((next_name < end_name)) {
			GENERATE_VAR(name, ::g->name__.generate());
			next_name = FTell();
		};
	};
	if ((version() == 0x00025000)) {
		GENERATE_VAR(numberOfGlyphs, ::g->numberOfGlyphs.generate());
		numGlyphs = numberOfGlyphs();
		GENERATE_VAR(offset, ::g->offset__.generate(numGlyphs));
	};

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


tloca* tloca::generate() {
	if (generated == 1) {
		tloca* new_instance = new tloca(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	n = (::g->ttf()[(::g->ttfId - 1)].maxp().numGlyphs() + 1);
	format = ::g->ttf()[(::g->ttfId - 1)].head().indexToLocFormat();
	if ((format == 0)) {
		GENERATE(offsets, ::g->offsets.generate(n));
	} else {
		GENERATE(offsets, ::g->offsets_.generate(n));
	};

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}


ttfFile* ttfFile::generate() {
	if (generated == 1) {
		ttfFile* new_instance = new ttfFile(instances);
		new_instance->generated = 2;
		return new_instance->generate();
	}
	if (!generated)
		generated = 1;
	_startof = FTell();
	if (_parent_id != ::g->_struct_id && !global_indexing_of_arrays) {
		_index_start = instances.size() - 1;
	}
	_parent_id = ::g->_struct_id;
	::g->_struct_id = ++::g->_struct_id_counter;

	possible_sfntver = { 0x10000 };
	possible_table_number = { 9 };
	for (name_number = 10; (name_number <= 21); name_number++) {
			possible_table_number.insert(possible_table_number.end(), { name_number });
	;
	};
	if (((ReadInt(FTell(), possible_sfntver) == 0x10000) && (ReadUShort((FTell() + 4), possible_table_number) >= 9))) {
		GENERATE_VAR(OffsetTable, ::g->OffsetTable.generate());
		required_tables = { "cmap", "head", "hhea", "hmtx", "maxp", "name", "OS/2", "post", "loca" };
		table_found = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		tablenum = 0;
		targetTableidex = 0;
		while ((tablenum < 9)) {
			tempNext = FTell();
			GENERATE_VAR(tempstr, ::g->tempstr.generate());
			FSeek(tempNext);
			for (yk1 = 0; (yk1 < 9); yk1 += 1) {
					if (((tempstr() == required_tables[yk1]) && (table_found[yk1] == 0))) {
					table_found[yk1] = 1;
					tablenum = (tablenum + 1);
					GENERATE_VAR(Table, ::g->Table.generate());
					targetTableidex = yk1;
					tempNext = FTell();
					FSeek(Table().offset());
					break;
				};
			;
			};
			if ((targetTableidex == 0)) {
				GENERATE_VAR(cmap, ::g->cmap.generate());
			} else {
			if ((targetTableidex == 1)) {
				GENERATE_VAR(head, ::g->head.generate());
			} else {
			if ((targetTableidex == 2)) {
				GENERATE_VAR(hhea, ::g->hhea.generate());
			} else {
			if ((targetTableidex == 3)) {
				GENERATE_VAR(hmtx, ::g->hmtx.generate());
			} else {
			if ((targetTableidex == 4)) {
				GENERATE_VAR(maxp, ::g->maxp.generate());
			} else {
			if ((targetTableidex == 5)) {
				GENERATE_VAR(name, ::g->name_.generate());
			} else {
			if ((targetTableidex == 6)) {
				GENERATE_VAR(OS_2, ::g->OS_2.generate());
			} else {
			if ((targetTableidex == 7)) {
				GENERATE_VAR(post, ::g->post.generate());
			} else {
			if ((targetTableidex == 8)) {
				GENERATE_VAR(loca, ::g->loca.generate());
			};
			};
			};
			};
			};
			};
			};
			};
			};
			FSeek(tempNext);
		};
	};

	::g->_struct_id = _parent_id;
	_sizeof = FTell() - _startof;
	return this;
}



void generate_file() {
	::g = new globals_class();

	BigEndian();
	::g->maxGlyfRead = 9;
	::g->ARG_1_AND_2_ARE_WORDS = (1 << 0);
	::g->ARGS_ARE_XY_VALUES = (1 << 1);
	::g->ROUND_XY_TO_GRID = (1 << 2);
	::g->WE_HAVE_A_SCALE = (1 << 3);
	::g->RESERVED = (1 << 4);
	::g->MORE_COMPONENTS = (1 << 5);
	::g->WE_HAVE_AN_X_AND_Y_SCALE = (1 << 6);
	::g->WE_HAVE_A_TWO_BY_TWO = (1 << 7);
	::g->WE_HAVE_INSTRUCTIONS = (1 << 8);
	::g->USE_MY_METRICS = (1 << 9);
	::g->OVERLAP_COMPOUND = (1 << 10);
	::g->SCALED_COMPONENT_OFFSET = (1 << 11);
	::g->UNSCALED_COMPONENT_OFFSET = (1 << 12);
	::g->ON_CURVE = (1 << 0);
	::g->X_BYTE = (1 << 1);
	::g->Y_BYTE = (1 << 2);
	::g->REPEAT = (1 << 3);
	::g->X_TYPE = (1 << 4);
	::g->Y_TYPE = (1 << 5);
	::g->RES_1 = (1 << 6);
	::g->RES_2 = (1 << 7);
	::g->nextTTFRec = 0;
	::g->ttfId = 0;
	::g->numFonts_2 = 1;
	::g->possible_values_1 = { 0x74746366 };
	if ((ReadInt(FTell(), ::g->possible_values_1) == 0x74746366)) {
		GENERATE(ttc, ::g->ttc.generate());
		::g->numFonts_2 = ::g->ttc().numFonts();
	};
	GENERATE(ttf, ::g->ttf.generate(::g->numFonts()));

	file_acc.finish();
	delete_globals();
}

void delete_globals() { delete ::g; }

