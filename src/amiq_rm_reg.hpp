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
 * NAME:        amiq_rm_reg.hpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef	AMIQ_RM_REG_HEADER
#define	AMIQ_RM_REG_HEADER	1

#include "amiq_rm_types.cpp"
#include "amiq_rm_field.hpp"
#include <vector>

namespace amiq_rm {

class amiq_rm_address_map;

/** This class is used to model a register. It contains a mechanism for field operations: adding fields, getting and setting the value
 * of a field. Basic register operations on a register are supported: reset, reading and writing, setting and getting the value of the register.
 * The difference between read/write and get/set is that the first pair takes into consideration the attributes of the fields (ex: a write to a
 * READ-ONLY register will not take place), while the second pair will bypass the attributes and perform the operation (ex: to change the value
 * of a status register, which is READ-ONLY, a set can be performed). There are hooks like pre_access() and post_access() in which side-effects
 * of the register can be implemented.
 * @n Before using the register (performing certain operations on it), the build() function must be called. This allows for some internal logic
 * and validation to take place. If the register is added to an address map and the map's build() is called, there is no need to call the build inside
 * from the register (is is called recursively from the address map).
 */
class amiq_rm_reg {
public:
	/** The name of the register. */
	std::string name;

	/** The value of the register. */
	amiq_rm_reg_data_t value;

	/** The vector contains the fields from the register. Functions like get_field_by_name() relies on this vector.
	 * To add a field the user must use add_field().*/
	std::vector<amiq_rm_field*> fields;

	/** The vector holds the address maps which contain the register. New parents are added when amiq_rm_address_map::add_reg() is called.*/
	std::vector<amiq_rm_address_map*> parent_maps;

	/** Create new register, the value of the register and the masks are set to 0.
	 * @param my_name is set as name. */
	amiq_rm_reg(std::string my_name) {
		name = my_name;

		value = 0;
		read_mask = 0;
		write_mask = 0;
		reset_value = 0;
	}

	/** Delete the fields vector. */
	virtual ~amiq_rm_reg() {
		for (int unsigned i = 0; i < fields.size(); i++)
			delete fields[i];
	}

	/** The function implements the reset functionality for the register. Value of the register is set to reset_value. */
	void reset();

	/**The function adds a field to the register - lsb_position is computed automatically
	 * fields are added from LSB to MSB -  there should be no gap between fields.
	 * @param my_field pointer to the field that is added to std::vector<amiq_rm_field*> fields */
	void add_field(amiq_rm_field *my_field);

	/** The function must be called after all fields have been added to the register
	 * it will compute all necessary masks based on the information which is stored in the fields.
	 * It is not necessary for the build() to be called if the @b address map::build() from one of the parent maps is called */
	void build();

	/** @param my_name is the name of the field on which the operation is addressed to
	 * @returns a pointer to the register whose name was given as argument. In case there is no such a register, NULL is returned. */
	amiq_rm_field* get_field_my_name(std::string my_name);

	/** @param field_name is the name of the field on which the operation is addressed to
	 * @returns the value of a field. */
	amiq_rm_reg_data_t get_field_value(std::string field_name);

	/** Changes the value of a field.
	 * @param field_name is the name of the field on which the operation is addressed to
	 * @param new_value is the value that is going to be set to the field */
	void set_field_value(std::string field_name, amiq_rm_reg_data_t new_value);

	/** @returns the write_mask. The value is taken from class member write_mask (computed by calling build()).*/
	amiq_rm_reg_data_t get_write_mask();

	/** @returns the read_mask. The value is taken from class member read_mask (computed by calling build()).*/
	amiq_rm_reg_data_t get_read_mask();

	/** @returns the reset. The value is taken from class member reset_value (computed by calling build()).*/
	amiq_rm_reg_data_t get_reset_value();

	/** @returns the size of the register in terms of bits by adding the size of each field. */
	int unsigned get_size();

	/** @returns the value of the register as well as the status of the read operation.
	 * For the read data the read_mask is applied. pre_access() and post_access() hooks are also called. */
	std::pair<amiq_rm_reg_data_t, amiq_rm_status_t> read();

	/** @param write_data is the data that is going to be written to the register.
	 * The value is written to the register my applying the write_mask. pre_access() and post_access() hooks are also called.
	 * @returns the status of the write operation */
	amiq_rm_status_t write(amiq_rm_reg_data_t write_data);

	/** @returns the value of the register - it does not apply masking, no pre/post access hooks are called. The value is taken from class member value. */
	amiq_rm_reg_data_t get();

	/** The function modifies the value of the register - it does not apply masking, no pre/post access hooks are called.
	 * @param write_data is the data that is going to be set as the register value */
	void set(amiq_rm_reg_data_t write_data);

	/** The function is used to determine the status of an operation to the register. The user can use this function as a hook and rewrite it,
	 * thus making possible an implementation of more exotic side-effects (before the actual read/write of the register).
	 * @param direction is the direction of the operation READ/WRITE
	 * @param access_data is the data which the register is going to be accessed with. In case of a READ operation access_data is NULL
	 * @returns the status of the access to the register. */
	virtual amiq_rm_status_t pre_access(amiq_rm_direction_t direction, amiq_rm_reg_data_t access_data);

	/** The function is used for side-effects that take place after the actual READ/WRITE operation. In this function are implemented mechanisms like W1C.
	 * The user can use this function as a hook and rewrite it, thus making possible an implementation of more exotic side-effects.
	 * @param direction is the direction of the operation READ/WRITE
	 * @param access_data is the data which the register is going to be accessed with. In case of a READ operation access_data is NULL*/
	virtual void post_access(amiq_rm_direction_t direction, amiq_rm_reg_data_t access_data);

	amiq_rm_reg_data_t get_access_data_for_field(std::string field_name, amiq_rm_reg_data_t access_data);

	/** The function returns the offsets of the registers. The offsets are calculated relative to the address map passed as argument.
	 * This function is a wrapper of the equivalent function @b amiq_rm_address_map::get_reg_offset().
	 * @param map pointer to the address map relative to which the calculation of the offset takes place.
	 * If the the map is not instantiated anywhere else (it is a physical map - associated with an interface) then the offset is basically @b absolute @b address.
	 * @returns a vector which contains all the offsets at which the register is mapped under the map argument.
	 * If the register is not mapped in the address map the returned vector will be 0 sized. */
	std::vector<amiq_rm_reg_address_t> get_offsets(amiq_rm_address_map &map);

	/** @returns a string with debug purpose information. */
	std::string to_string();

private:
	/** write_mask of the register. It is set when calling build() (which calls compute_write_mask()). To access write_mask, user can call get_write_mask(). */
	amiq_rm_reg_data_t write_mask;

	/** read_mask of the register. It is set when calling build() (which calls compute_read_mask()). To access read_mask, user can call get_read_mask(). */
	amiq_rm_reg_data_t read_mask;

	/** reset_mask of the register. It is set when calling build() (which calls compute_reset_mask()). To access reset_mask, user can call get_reset_mask(). */
	amiq_rm_reg_data_t reset_value;

	/** The function is used to create an extraction mask (with 1 from bit @b a to bit @b b) - useful when extracting field values.
	 * @param a is the lsb used for creating the mask
	 * @param b is the msb used for creating the mask
	 * @return a mask of amiq_rm_reg_data_t type which has 1 from bit @b a to bit @b b.*/
	amiq_rm_reg_data_t extract_mask(unsigned a, unsigned b);

	/** @returns the write_mask which is computed from the fields which were added with add_field().*/
	amiq_rm_reg_data_t compute_write_mask();

	/** @returns the read_mask which is computed from the fields which were added with add_field().*/
	amiq_rm_reg_data_t compute_read_mask();

	/** @returns the reset_mask which is computed from the fields which were added with add_field().*/
	amiq_rm_reg_data_t compute_reset_value();

	/** The function is called in build(). It verifies the sanity of the definition of the fields. */
	void validate_fields();
};

}

#endif
