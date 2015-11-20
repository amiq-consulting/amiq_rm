/* ****************************************************************************
 * (C) Copyright 2014 AMIQ Consulting
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * NAME:        amiq_rm_field.hpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef AMIQ_RM_FIELD_HEADER
#define AMIQ_RM_FIELD_HEADER 1

#include "amiq_rm_types.cpp"
#include <sstream>

namespace amiq_rm {

/** This class is used to model a field (which can be contained in a register). The field is defined through a name,
 * reset_value, size and attribute. The behavior of the attribute is defined through virtual functions like:
 * is_readable(), is_writable(), etc. To add additional field attributes, the virtual functions mentioned earlier
 * should be overwritten accordingly. */
class amiq_rm_field {
public:

	/** The name of the field. */
	std::string name;

	/** The least significant bit position of the field. */
	int unsigned lsb_position;

	/** The size of the field in terms of bits. */
	int unsigned size;

	/** The value the field has after reset. */
	amiq_rm_reg_data_t reset_value;

	/** The attribute of the field represents the behavior of the field at access. The implemented attributes:
	 * @n "RO"    - W: no effect, R: no effect
	 * @n "RW"    - W: as-is, R: no effect
	 * @n "RC"    - W: no effect, R: clears all bits
	 * @n "WO"    - W: as-is, R: error
	 * @n "RS"    - W: no effect, R: sets all bits
	 * @n "WRC"   - W: as-is, R: clears all bits
	 * @n "WRS"   - W: as-is, R: sets all bits
	 * @n "WC"    - W: clears all bits, R: no effect
	 * @n "WS"    - W: sets all bits, R: no effect
	 * @n "WSRC"  - W: sets all bits, R: clears all bits
	 * @n "WCRS"  - W: clears all bits, R: sets all bits
	 * @n "W1C"   - W: 1/0 clears/no effect on matching bit, R: no effect
	 * @n There is a mechanism which relies on functions like is_writable(), is_readable(), etc. through which
	 * the attribute's behavior is extracted and used by the register at access. */
	std::string atrib;

	/** Create new field.
	 * @param my_name is set as the name of the field
	 * @param my_reset_value is set as reset value
	 * @param my_size is set as size
	 * @param my_attrib is set as attribute
	 * The lsb_position is set when adding a field to a register */
	amiq_rm_field(std::string my_name, amiq_rm_reg_data_t my_reset_value, int unsigned my_size, std::string my_attrib) {
		name = my_name;
		reset_value = my_reset_value;
		size = my_size;
		atrib = my_attrib;

		lsb_position = 0;
	}

	/** There are no pointers to delete. */
	virtual ~amiq_rm_field() {
	}

	/** @return The function returns true is the field's attribute permits writes. */
	virtual bool is_writable();

	/** @return The function returns true is the field's attribute permits reads. */
	virtual bool is_readable();

	/** @return The function returns true is the field's attribute implies an error while performing a write. */
	virtual bool is_error_on_write();

	/** @return The function returns true is the field's attribute implies an error while performing a read. */
	virtual bool is_error_on_read();

	/** @return The function returns true is the field's attribute implies the value to be set to 0 while performing a read. */
	virtual bool is_clear_on_read();

	/** @return The function returns true is the field's attribute implies the value to be set to 0 while performing a write. */
	virtual bool is_clear_on_write();

	/** @return The function returns true is the field's attribute implies the value to be set to 1 while performing a read. */
	virtual bool is_set_on_read();

	/** @return The function returns true is the field's attribute implies the value to be set to 1 while performing a write. */
	virtual bool is_set_on_write();

	/** @returns a string with debug purpose information. */
	std::string to_string();

};

}

#endif
