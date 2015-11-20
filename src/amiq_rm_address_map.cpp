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
 * NAME:        amiq_rm_address_map.cpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef	AMIQ_RM_ADDRESS_MAP
#define	AMIQ_RM_ADDRESS_MAP	1

#include <assert.h>
#include <iostream>
#include "amiq_rm_reg.hpp"
#include "amiq_rm_address_map.hpp"

using namespace std;

namespace amiq_rm {

void amiq_rm_address_map::reset() {
	for (amiq_rm_reg_map_t::iterator it = regs.begin(); it != regs.end(); it++) {
		it->second->reset();
	}

	for (amiq_rm_addressmap_map_t::iterator it = submaps.begin(); it != submaps.end(); it++) {
		it->second->reset();
	}
}

void amiq_rm_address_map::build() {
	for (amiq_rm_reg_map_t::iterator it = regs.begin(); it != regs.end(); it++) {
		it->second->build();
	}

	for (amiq_rm_addressmap_map_t::iterator it = submaps.begin(); it != submaps.end(); it++) {
		it->second->build();
	}
}

void amiq_rm_address_map::add_reg(amiq_rm_reg &my_reg, amiq_rm_reg_address_t my_address) {
	regs[my_address] = &my_reg;
	my_reg.parent_maps.push_back(this);
}

void amiq_rm_address_map::add_map(amiq_rm_address_map &my_map, amiq_rm_reg_address_t my_address) {
	submaps[my_address] = &my_map;
	my_map.parents.push_back(this);
}

void amiq_rm_address_map::add_map(amiq_rm_address_map &my_map, amiq_rm_reg_address_t my_address, amiq_rm_reg_block &reg_block) {
	add_map(my_map, my_address);
	my_map.reg_block = &reg_block;
}

//TODO add a get_regs_by_name that will return a list
amiq_rm_reg* amiq_rm_address_map::get_reg_by_name(string my_name) {
	for (amiq_rm_reg_map_t::iterator it = regs.begin(); it != regs.end(); it++) {
		if (it->second->name == my_name) {
			return it->second;
		}
	}

	for (amiq_rm_addressmap_map_t::iterator it = submaps.begin(); it != submaps.end(); it++) {
		amiq_rm_reg *my_reg;
		my_reg = it->second->get_reg_by_name(my_name);
		if (my_reg != NULL) {
			return my_reg;
		}
	}
	return NULL;
}

amiq_rm_reg* amiq_rm_address_map::get_reg_by_offset(amiq_rm_reg_address_t my_address) {
	amiq_rm_reg_map_t::iterator it = regs.find(my_address);
	if (it != regs.end()) {
		return it->second;
	} else {
		amiq_rm_reg *my_reg;
		for (amiq_rm_addressmap_map_t::iterator it = submaps.begin(); it != submaps.end(); it++) {
			//there is a change that in the sub-map there is a register with absolute_address = my_address
			//if the offset is lower or equal to my_address
			if (it->first <= my_address) {
				my_reg = it->second->get_reg_by_offset(my_address - it->first);
				if (my_reg != NULL) {
					return my_reg;
				}
			}
		}
		return NULL;
	}
}

vector<amiq_rm_reg_address_t> amiq_rm_address_map::get_reg_offsets(amiq_rm_reg &reg) {
	vector<amiq_rm_reg_address_t> my_offsets;
	for (amiq_rm_reg_map_t::iterator it = regs.begin(); it != regs.end(); it++) {
		if (it->second == &reg) {
			my_offsets.push_back(it->first);
		}
	}
	for (amiq_rm_addressmap_map_t::iterator it = submaps.begin(); it != submaps.end(); it++) {
		vector<amiq_rm_reg_address_t> submaps_results;
		submaps_results = it->second->get_reg_offsets(reg);
		for (unsigned int i = 0; i < submaps_results.size(); i++)
			my_offsets.push_back(it->first + submaps_results[i]);
	}
	return my_offsets;
}

pair<amiq_rm_reg_data_t, amiq_rm_status_t> amiq_rm_physical_address_map::read(amiq_rm_reg_address_t address) {
	amiq_rm_reg *my_reg = get_reg_by_offset(address);
	pair<amiq_rm_reg_data_t, amiq_rm_status_t> data_with_status;

	if (my_reg == NULL) {
		data_with_status.first = 0;
		data_with_status.second = HOLE;
	} else {
		data_with_status = my_reg->read();
	}
	return data_with_status;
}

amiq_rm_status_t amiq_rm_physical_address_map::write(amiq_rm_reg_address_t address, amiq_rm_reg_data_t write_data) {
	amiq_rm_reg *my_reg = get_reg_by_offset(address);
	amiq_rm_status_t status;

	status = (my_reg == NULL) ? HOLE : (my_reg->write(write_data));
	return status;
}

amiq_rm_reg_data_t amiq_rm_physical_address_map::get(amiq_rm_reg_address_t address) {
	amiq_rm_reg *my_reg = get_reg_by_offset(address);

	assert(my_reg != NULL);
	return my_reg->get();
}

void amiq_rm_physical_address_map::set(amiq_rm_reg_address_t address, amiq_rm_reg_data_t write_data) {
	amiq_rm_reg *my_reg = get_reg_by_offset(address);

	assert(my_reg != NULL);
	my_reg->set(write_data);
}

string amiq_rm_address_map::to_string() {
	ostringstream convert;
	convert << "Address map: " << name << endl;
	for (amiq_rm_reg_map_t::iterator it = regs.begin(); it != regs.end(); it++) {
		convert << "Address: " << hex << it->first << "  " << it->second->to_string() << endl;
	}
	return convert.str();
}

}

#endif
