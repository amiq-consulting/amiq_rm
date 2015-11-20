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
 * NAME:        amiq_rm_types.cpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef	AMIQ_RM_TYPES
#define	AMIQ_RM_TYPES	1

namespace amiq_rm {

typedef int unsigned amiq_rm_reg_data_t;
typedef int unsigned amiq_rm_reg_address_t;
typedef enum {
	READ = 0x0, WRITE = 0x1
} amiq_rm_direction_t;
typedef enum {
	OKAY = 0x0, ERROR = 0x1, HOLE = 0x2
} amiq_rm_status_t;

}

#endif
