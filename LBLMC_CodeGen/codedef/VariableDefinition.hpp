/*

Copyright (C) 2019 Matthew Milton

This file is part of the LB-LMC Solver C++ Code Generation Library.

LB-LMC Solver C++ Code Generation Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Code Generation Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Code Generation Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef LBLMC_VARIABLE_DEFINITION_HPP
#define LBLMC_VARIABLE_DEFINITION_HPP

#include <string>

namespace lblmc
{

/**
	\brief enumeration of common data types
**/
enum class Types : int
{
	TYPE_UNDEFINED = -1,///< default; undefined type
	TYPE_VOID = 0,		///< void (none) type
	TYPE_BOOL,			///< boolean type
	TYPE_UCHAR,			///< unsigned char type
	TYPE_CHAR,			///< signed char type
	TYPE_UINT,			///< unsigned int type
	TYPE_INT,			///< signed int type
	TYPE_ULONG,			///< unsigned long int type
	TYPE_LONG,			///< signed long int type
	TYPE_FLOAT,			///< single precision floating point type
	TYPE_DOUBLE,		///< double precision floating point type
	TYPE_REAL,			///< arbitrary type for real (decimal) numbers; can either be floating or fixed point
	TYPE_STRING,		///< character string type
	TYPE_CLASS,			///< C++ class/structure type
	TYPE_FUNCTION,		///< C++ function type
	TYPE_ENUM			///< C++ enumeration class type
};

/**
	\brief enumeration of variable storage specifiers
**/
enum class StorageSpecifiers : int
{
	STORAGE_DEFAULT = -1,	///< default storage
	STORAGE_CONSTANT,		///< constant (read-only; const) storage
	STORAGE_VOLATILE,		///< volatile storage
	STORAGE_STATIC,			///< static (persistent) storage
	STORAGE_LITERAL,		///< literal (const static) storage
	STORAGE_REGISTER,		///< register (on CPU register) storage
	STORAGE_EXTERNAL		///< external (defined elsewhere) storage
};

/**
	\brief enumeration of C++ reference types
**/
enum class ReferenceTypes : int
{
	REFERENCE_NONE = -1,	///< default; not a reference
	REFERENCE_POINTER,		///< address (type *) pointer
	REFERENCE_DOUBLE_POINTER, ///< pointer to address pointer (type **)
	REFERENCE_REFERENCE,	///< alias (type &) reference
	REFERENCE_RVALUE_REFERENCE	///< literal/temp rvalue (type &&) reference (C++11 and up)
};

/**
	\brief stores definition of variables
**/
class VariableDefinition
{
private:

	Types datatype;					///< data type of defined variable
	std::string type_name;	///< type name of defined variable if type is class, function, or enum
	StorageSpecifiers storage_spec;	///< storage specifier of defined variable
	referenceTypes ref_type;	///< reference type of the defined variable
	std::string name;				///< name or label of defined variable
	std::string value;	///< default or initialized value of defined variable, defined as a string

public:

	VariableDefinition() :
		datatype(TYPE_UNDEFINED),
		type_name("")
		storage_spec(STORAGE_DEFAULT),
		ref_type(REFERENCE_NONE),
		name("UNNAMED"),
		value("")
	{}

	VariableDefinition(
		Types datatype,
		std::string type_name,
		StorageSpecifiers storage_spec,
		ReferenceTypes ref_type,
		std::string name,
		std::string value
	) :
		datatype(datatype),
		type_name(type_name)
		storage_spec(storage_spec),
		ref_type(ref_type),
		name(name),
		value(value)
	{}

	VariableDefinition(const VariableDefinition& base) :
		datatype(base.datatype),
		type_name(base.type_name)
		storage_spec(base.storage_spec),
		ref_type(base.ref_type),
		name(base.name),
		value(base.value)
	{}

	inline void setType(Types datatype) { this->datatype = datatype; }
	inline void setTypeName(std::string type_name) { this->type_name = type_name; }
	inline void setStorageSpecifier(StorageSpecifiers storage_spec) { this->storage_spec = storage_spec; }
	inline void setReferenceType(ReferenceTypes ref_type) { this->ref_type = ref_type; }
	inline void setName(std::string name) { this->name = name; }
	inline void setValue(std::string value) { this->value = value; }

	inline Types getType() const { return datatype; }
	inline std::string getTypeName() const { return type_name; }
	inline StorageSpecifiers getStorageSpecifier() const { return storage_spec; }
	inline ReferenceType getReferenceType() const { return ref_type; }
	inline std::string getName() const { return name; }
	inline std::string getValue() const { return value; }

};

} //namespace lblmc

#endif // LBLMC_VARIABLE_DEFINITION_HPP
