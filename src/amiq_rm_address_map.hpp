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
 * NAME:        amiq_rm_address_map.hpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef AMIQ_RM_ADDRESS_MAP_HEADER
#define AMIQ_RM_ADDRESS_MAP_HEADER 1

#include <vector>
#include <map>
#include "amiq_rm_reg_block.hpp"
#include "amiq_rm_reg.hpp"

namespace amiq_rm {

/** This class is used to model an address_map. An address map may contain registers and sub-maps,
 * both categories are mapped by offset. It provides mechanisms to perform recursive search of the registers
 * (by name, by offset). */
class amiq_rm_address_map {
public:
	/** Container which stores pointers to registers associated with an offset (the offset is used as key) */
	typedef std::map<amiq_rm_reg_address_t, amiq_rm_reg*> amiq_rm_reg_map_t;
	/** Container which stores pointers to address maps associated with an offset (the offset is used as key) */
	typedef std::map<amiq_rm_reg_address_t, amiq_rm_address_map*> amiq_rm_addressmap_map_t;

	/** The name of the address map. */
	std::string name;

	/** The map contains pointers to the registers added to the map by using the offset as the key.
	 * Functions like get_reg_by_name() relies on this map.
	 * To add a register the user must use add_reg(). */
	amiq_rm_reg_map_t regs;

	/** The map contains pointers to the sub-maps added by using the offset as the key.
	 * Functions which iterate recursively like get_reg_by_name() relies on this map.
	 * To add a sub-map the user must use add_map(). */
	amiq_rm_addressmap_map_t submaps;

	/** The vector holds the address maps which contain this map. New parents are added when amiq_rm_address_map::add_map() is called. */
	std::vector<amiq_rm_address_map*> parents;

	/** Pointer to the register block. This may be used by the user in case there is need for access to the registers or to other address_maps. */
	amiq_rm_reg_block *reg_block;

	/** Create new address map.
	 * @param my_name is set as name */
	amiq_rm_address_map(std::string my_name) {
		name = my_name;

		reg_block = NULL;
	}

	/** There are no pointers to delete. */
	virtual ~amiq_rm_address_map() {
	}

	/** The function implements the reset functionality for the address map.
	 * When this function is called, it will trigger reset() for the registers mapped in @b regs,
	 * also it will trigger reset() in each sub-map mapped in @b sub-maps. */
	void reset();

	/** The function must be called after all registers and all sub-maps have been added.
	 * It will recursively descend and call build() for mapped registers and sub-maps, connect parent pointers, etc.
	 * It is not necessary for the build() to be called if a parent map calls it's build(). */
	virtual void build();

	/**The function maps a register to the address_map. The pointer to the register is stored in a C++ map and uses the offset as key.
	 * @param my_reg represents a reference to the register that is going to be mapped
	 * @param my_offset represents the offset of the register within the address map */
	void add_reg(amiq_rm_reg &my_reg, amiq_rm_reg_address_t my_offset);

	/**The function maps a sub-map to the address_map. The pointer to the map is stored in a C++ map and uses the offset as key.
	 * @param my_map represents a reference to the address map that is going to be mapped
	 * @param my_offset represents the offset of the register within the address map */
	void add_map(amiq_rm_address_map &my_map, amiq_rm_reg_address_t my_offset);

	/**The function maps a sub-map to the address_map. The pointer to the map is stored in a C++ map and uses the offset as key.
	 * @param my_map represents a reference to the address map that is going to be mapped
	 * @param my_offset represents the offset of the register within the address map
	 * @param reg_block represents a reference to the reg_block that is going to contain the address_map,
	 * in case the user needs to access other registers or address_maps instantiated in the reg_block */
	void add_map(amiq_rm_address_map &my_map, amiq_rm_reg_address_t my_offset, amiq_rm_reg_block &reg_block);

	/** The function returns a pointer to the register specified by name. In case there are multiple registers with the same name,
	 * the returned register will be the first one found.
	 * @param my_name is the name of the register that is going to be searched for
	 * @returns a pointer to the register whose name was given as argument. In case there is no such a register, NULL is returned. */
	amiq_rm_reg* get_reg_by_name(std::string my_name);

	/** The function returns a pointer to the register specified by offset.
	 * The search will take place within directly mapped registers, but will also continue recursively through sub-maps.
	 * If the instance of the address_map in which this function is called is a top level map (associated with a physical interface),
	 * the offset will basically be the @b absolute @b address.
	 * @param my_offset is the offset at which a mapped register will be searched for
	 * @returns a pointer to the register whose offset was given as argument. In case there is no such a register, NULL is returned. */
	amiq_rm_reg* get_reg_by_offset(amiq_rm_reg_address_t my_offset);

	/** The function returns the offsets of the a register relative to the address map which calls this function.
	 * @param reg is a reference to the register which is searched for in the mapped registers and sub-maps
	 * If the the instance of the address map calling the function is not instantiated anywhere else (it is a physical map - associated with an interface)
	 * then the returned offsets are basically @b absolute @b addresses.
	 * @returns a vector which contains all the offsets at which the register is mapped.
	 * If the register is not mapped anywhere under the address map the returned vector will be 0 sized. */
	std::vector<amiq_rm_reg_address_t> get_reg_offsets(amiq_rm_reg &reg);

	/** @returns a string with debug purpose information. */
	std::string to_string();

};

class amiq_rm_physical_address_map: public amiq_rm_address_map {
public:

	/** Create new physical address map. Calls the constructor of amiq_rm_address_map
	 * @param name is passed to the address map constructor for setting the name of the address_map*/
	amiq_rm_physical_address_map(std::string name) :
			amiq_rm_address_map(name) {
	}

	/** The function reads the value from the register by specifying the address at which the register is instanced.
	 * The function calls amiq_rm_reg::read() function
	 * @param address is the address of the register on which the write operation is exercised
	 * @returns the value read from the register as well as the status of the read operation */
	std::pair<amiq_rm_reg_data_t, amiq_rm_status_t> read(amiq_rm_reg_address_t address);

	/** This function writes a data to a register by specifying the address. The function calls the amiq_rm_reg::write() function.
	 * @param address is the absolute address of a register on which the write operation is exercised
	 * @param write_data is the data that is going to be written to the register
	 * @returns the status of the write operation */
	amiq_rm_status_t write(amiq_rm_reg_address_t address, amiq_rm_reg_data_t write_data);

	/** This function gets the register's value by specifying the address at which the register is mapped.
	 * The function calls the amiq_rm_reg::get() function.
	 * @param address is the absolute address of a register on which the get operation is exercised
	 * @returns the value of the register */
	amiq_rm_reg_data_t get(amiq_rm_reg_address_t address);

	/** This function sets the register's value by specifying the address at which the register is mapped.
	 * The function calls the amiq_rm_reg::set() function.
	 * @param address is the absolute address of a register on which the set operation is exercised
	 * @param write_data is the value set to the register */
	void set(amiq_rm_reg_address_t address, amiq_rm_reg_data_t write_data);
};

}

#endif
