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



class ttfFile {
	std::vector<ttfFile*>& instances;

	tOffsetTable* OffsetTable_var;
	tTable* Table_var;

public:
	bool OffsetTable_exists = false;
	bool Table_exists = false;

	tOffsetTable& OffsetTable() {
		assert_cond(OffsetTable_exists, "struct field OffsetTable does not exist");
		return *OffsetTable_var;
	}
	tTable& Table() {
		assert_cond(Table_exists, "struct field Table does not exist");
		return *Table_var;
	}

	/* locals */
	std::vector<std::string> required_tables;
	std::vector<int> table_found;
	std::vector<ULONG> table_offsets;
	int tablenum;
	int targetTableidex;
	std::string current_tag;

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
std::vector<int> ReadIntInitValues;
std::vector<uint> ReadUIntInitValues;
std::vector<int64> ReadQuadInitValues;
std::vector<uint64> ReadUQuadInitValues;
std::vector<int64> ReadInt64InitValues;
std::vector<uint64> ReadUInt64InitValues;
std::vector<hfloat> ReadHFloatInitValues;
std::vector<float> ReadFloatInitValues;
std::vector<double> ReadDoubleInitValues;
std::vector<std::string> ReadBytesInitValues;


std::vector<tOffsetTable*> tOffsetTable_OffsetTable_instances;
std::vector<tTable_Tag_union*> tTable_Tag_union_Tag_instances;
std::vector<tTable*> tTable_Table_instances;
std::vector<ttfFile*> ttfFile_ttf_element_instances;


std::unordered_map<std::string, std::string> variable_types = { { "SFNT_Ver", "TT_Fixed_class" }, { "numTables", "USHORT_class" }, { "searchRange", "USHORT_class" }, { "entrySelector", "USHORT_class" }, { "rangeShift", "USHORT_class" }, { "OffsetTable", "tOffsetTable" }, { "asChar", "char_array_class" }, { "asLong", "ULONG_class" }, { "Tag", "tTable_Tag_union" }, { "checkSum", "ULONG_class" }, { "offset", "ULONG_class" }, { "length", "ULONG_class" }, { "Table", "tTable" }, { "ttf", "ttfFile_array_class" } };

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
	/*local*/ USHORT numtable_offset;
	/*local*/ uint32 ttfId;
	/*local*/ uint32 numFonts;
	TT_Fixed_class SFNT_Ver;
	USHORT_class numTables;
	USHORT_class searchRange;
	USHORT_class entrySelector;
	USHORT_class rangeShift;
	tOffsetTable OffsetTable;
	char_class asChar_element;
	char_array_class asChar;
	ULONG_class asLong;
	tTable_Tag_union Tag;
	ULONG_class checkSum;
	ULONG_class offset;
	ULONG_class length;
	tTable Table;
	ttfFile ttf_element;
	ttfFile_array_class ttf;


	globals_class() :
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
		ttf_element(ttfFile_ttf_element_instances),
		ttf(ttf_element)
	{}
};

globals_class* g;


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

	GENERATE_VAR(SFNT_Ver, ::g->SFNT_Ver.generate({ 0x00010000 }));
	::g->numtable_offset = FTell();
	GENERATE_VAR(numTables, ::g->numTables.generate({ 5, 16, 21 }));
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

	GENERATE_VAR(OffsetTable, ::g->OffsetTable.generate());
	required_tables = { "cmap", "head", "hhea", "hmtx", "maxp", "name", "OS/2", "post", "loca" };
	table_found = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	table_offsets = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	tablenum = 0;
	targetTableidex = -1;
	current_tag.resize(4);
	while ((tablenum < 9)) {
		while (ReadBytes(current_tag, FTell(), 4, required_tables)) {
			switch (STR2INT(current_tag)) {
			case STR2INT("cmap"):
				if ((table_found[0] == 1)) {
					break;
				};
				table_found[0] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[0] = Table().offset();
				break;
			case STR2INT("head"):
				if ((table_found[1] == 1)) {
					break;
				};
				table_found[1] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[1] = Table().offset();
				break;
			case STR2INT("hhea"):
				if ((table_found[2] == 1)) {
					break;
				};
				table_found[2] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[2] = Table().offset();
				break;
			case STR2INT("hmtx"):
				if ((table_found[3] == 1)) {
					break;
				};
				table_found[3] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[3] = Table().offset();
				break;
			case STR2INT("maxp"):
				if ((table_found[4] == 1)) {
					break;
				};
				table_found[4] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[4] = Table().offset();
				break;
			case STR2INT("name"):
				if ((table_found[5] == 1)) {
					break;
				};
				table_found[5] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[5] = Table().offset();
				break;
			case STR2INT("OS/2"):
				if ((table_found[6] == 1)) {
					break;
				};
				table_found[6] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[6] = Table().offset();
				break;
			case STR2INT("post"):
				if ((table_found[7] == 1)) {
					break;
				};
				table_found[7] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[7] = Table().offset();
				break;
			case STR2INT("loca"):
				if ((table_found[8] == 1)) {
					break;
				};
				table_found[8] = 1;
				tablenum = (tablenum + 1);
				GENERATE_VAR(Table, ::g->Table.generate());
				table_offsets[8] = Table().offset();
				break;
			};
		};
	};
	FSeek(::g->nextTTFRec);

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
	::g->numFonts = 1;
	GENERATE(ttf, ::g->ttf.generate(::g->numFonts));

	file_acc.finish();
	delete_globals();
}

void delete_globals() { delete ::g; }

