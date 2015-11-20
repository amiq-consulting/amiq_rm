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
 * NAME:        amiq_rm_reg.cpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef	AMIQ_RM_REG
#define	AMIQ_RM_REG	1

#include <assert.h>
#include "amiq_rm_reg.hpp"
#include "amiq_rm_address_map.hpp"

using namespace std;

namespace amiq_rm {

void amiq_rm_reg::reset() {
	value = reset_value;
}

void amiq_rm_reg::add_field(amiq_rm_field *my_field) {
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			assert(fields[i]->name != my_field->name);
			if (i >= 1)
				assert(fields[i - 1]->lsb_position + fields[i - 1]->size == fields[i]->lsb_position);
		}
		my_field->lsb_position = fields[fields.size() - 1]->lsb_position + fields[fields.size() - 1]->size;
	}

	fields.push_back(my_field);
}

amiq_rm_field* amiq_rm_reg::get_field_my_name(string my_name) {
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			if (fields[i]->name == my_name) {
				return fields[i];
			}
		}
	}
	return NULL;
}

amiq_rm_reg_data_t amiq_rm_reg::get_field_value(string field_name) {
	return get_access_data_for_field(field_name, value);
}

void amiq_rm_reg::set_field_value(string field_name, amiq_rm_reg_data_t new_value) {
	amiq_rm_field *field = get_field_my_name(field_name);

	if (field != NULL) {
		amiq_rm_reg_data_t field_mask = extract_mask(field->lsb_position, field->lsb_position + field->size - 1);
		amiq_rm_reg_data_t value_to_write = (new_value << field->lsb_position);
		value_to_write = value_to_write & field_mask;

		//clear previous field value and replace with new value;
		value = value & (!field_mask);
		value = value ^ value_to_write;
	} else {
		assert(0);
	}
}

amiq_rm_reg_data_t amiq_rm_reg::extract_mask(unsigned a, unsigned b) {
	assert(a <= b);
	amiq_rm_reg_data_t mask = 0;
	for (unsigned i = a; i <= b; i++)
		mask |= 1 << i;

	return mask;
}

amiq_rm_reg_data_t amiq_rm_reg::compute_write_mask() {
	amiq_rm_reg_data_t my_mask = 0;
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			if (fields[i]->is_writable()) {
				my_mask = my_mask | extract_mask(fields[i]->lsb_position, fields[i]->lsb_position + fields[i]->size - 1);
			}
		}
	}
	return my_mask;
}

amiq_rm_reg_data_t amiq_rm_reg::compute_read_mask() {
	amiq_rm_reg_data_t my_mask = 0;
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			if (fields[i]->is_readable()) {
				my_mask = my_mask | extract_mask(fields[i]->lsb_position, fields[i]->lsb_position + fields[i]->size - 1);
			}
		}
	}
	return my_mask;
}

amiq_rm_reg_data_t amiq_rm_reg::compute_reset_value() {
	amiq_rm_reg_data_t my_reset_value = 0;
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			amiq_rm_reg_data_t masked_reset_field_value = fields[i]->reset_value & extract_mask(0, fields[i]->size - 1);
			my_reset_value = my_reset_value | (masked_reset_field_value << fields[i]->lsb_position);
		}
	}
	return my_reset_value;
}

void amiq_rm_reg::validate_fields() {
	int unsigned total_size = 0;
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			total_size += fields[i]->size;
		}
	}
	//total size of the fields must not exceed the size of register data type
	assert((total_size / 8) < sizeof(amiq_rm_reg_data_t));

	//TODO maybe add a check for overlapping fields
}

void amiq_rm_reg::build() {
	validate_fields();
	write_mask = compute_write_mask();
	read_mask = compute_read_mask();
	reset_value = compute_reset_value();
}

amiq_rm_reg_data_t amiq_rm_reg::get_write_mask() {
	return write_mask;
}

amiq_rm_reg_data_t amiq_rm_reg::get_read_mask() {
	return read_mask;
}

amiq_rm_reg_data_t amiq_rm_reg::get_reset_value() {
	return reset_value;
}

int unsigned amiq_rm_reg::get_size() {
	int unsigned total_size = 0;
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			total_size += fields[i]->size;
		}
	}
	return total_size;
}

pair<amiq_rm_reg_data_t, amiq_rm_status_t> amiq_rm_reg::read() {
	pair<amiq_rm_reg_data_t, amiq_rm_status_t> data_with_status;
	data_with_status.second = pre_access(READ, NULL);
	if (data_with_status.second == OKAY) {
		data_with_status.first = value & read_mask;
		post_access(READ, NULL);
	}
	return data_with_status;
}

amiq_rm_reg_data_t amiq_rm_reg::get() {
	return value;
}

void amiq_rm_reg::set(amiq_rm_reg_data_t write_data) {
	value = write_data;
}

amiq_rm_status_t amiq_rm_reg::write(amiq_rm_reg_data_t write_data) {
	amiq_rm_status_t status = pre_access(WRITE, write_data);
	if (status == OKAY) {
		value = write_data & write_mask;
		post_access(WRITE, write_data);
	}
	return status;
}

amiq_rm_status_t amiq_rm_reg::pre_access(amiq_rm_direction_t direction, amiq_rm_reg_data_t access_data) {
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			if (((direction == READ) && (fields[i]->is_error_on_read())) || ((direction == WRITE) && (fields[i]->is_error_on_write()))) {
				return ERROR;
			}
		}
	}
	return OKAY;
}

amiq_rm_reg_data_t amiq_rm_reg::get_access_data_for_field(string field_name, amiq_rm_reg_data_t access_data) {
	amiq_rm_field *field = get_field_my_name(field_name);
	if (field != NULL) {
		amiq_rm_reg_data_t extracted_value = access_data & extract_mask(field->lsb_position, field->lsb_position + field->size - 1);
		return (extracted_value >> field->lsb_position);
	} else {
		assert(0);
		return NULL;
	}
}

void amiq_rm_reg::post_access(amiq_rm_direction_t direction, amiq_rm_reg_data_t access_data) {
	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			//implement W1C mechanism
			if ((direction == WRITE) && (fields[i]->atrib == "W1C")) {
				set_field_value(fields[i]->name, get_field_value(fields[i]->name) ^ (!get_access_data_for_field(fields[i]->name, access_data)));
			}

			//clear field
			if (((direction == READ) && (fields[i]->is_clear_on_read())) || ((direction == WRITE) && (fields[i]->is_clear_on_write()))) {
				set_field_value(fields[i]->name, 0);
			}
		}
	}
}

vector<amiq_rm_reg_address_t> amiq_rm_reg::get_offsets(amiq_rm_address_map &map) {
	return map.get_reg_offsets(*this);
}

string amiq_rm_reg::to_string() {
	ostringstream convert;
	convert << name << " Value: " << hex << value << " Reset Value: " << hex << reset_value;
	convert << " Read_mask: " << hex << read_mask << " Write_mask: " << hex << write_mask << "\n";

	if (fields.size() > 0) {
		for (int unsigned i = 0; i <= fields.size() - 1; i++) {
			convert << fields[i]->to_string() << "\n";
		}
	}

	return convert.str();
}

}

#endif
