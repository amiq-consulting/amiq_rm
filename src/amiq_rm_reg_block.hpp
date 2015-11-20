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
 * NAME:        amiq_rm_reg_block.hpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef AMIQ_RM_REG_BLOCK_HEADER
#define AMIQ_RM_REG_BLOCK_HEADER 1

#include <iostream>

namespace amiq_rm {

/** This class is used to model a register block. The register block contains the instances of all registers.
 * It can also contain the instances of all address maps (or just the top level address maps). The register block
 * is the solution of having the registers visible in all address maps.
 * Each address map contains a pointer to a reg_block. If an hierarchical implementation is used then the pointer
 * should be connected and propagated down the hierarchy (to have visibility to the registers). */
class amiq_rm_reg_block {
public:
	/** The name of the address map. */
	std::string name;

	/** Create new reg_block.
	 * @param my_name is set as name */
	amiq_rm_reg_block(std::string my_name) {
		name = my_name;
	}
};

}

#endif
