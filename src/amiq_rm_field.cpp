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
 * NAME:        amiq_rm_field.cpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef	AMIQ_RM_FIELD
#define	AMIQ_RM_FIELD	1

#include <sstream>
#include <iostream>
#include "amiq_rm_field.hpp"

using namespace std;

namespace amiq_rm {

bool amiq_rm_field::is_writable() {
	return ((attrib == "RW") || (attrib == "WO") || (attrib == "WRC") || (attrib == "WRS"));
}

bool amiq_rm_field::is_readable() {
	return ((attrib == "RO") || (attrib == "RW") || (attrib == "RC") || (attrib == "RS") || (attrib == "WRC") || (attrib == "WRS") ||
			(attrib == "WC") || (attrib == "WS") || (attrib == "WSRC") || (attrib == "WCRS") || (attrib == "W1C"));
}

bool amiq_rm_field::is_error_on_write() {
	return false;
}

bool amiq_rm_field::is_error_on_read() {
	return (attrib == "WO");
}

bool amiq_rm_field::is_clear_on_read() {
	return ((attrib == "RC") || (attrib == "WRC") || (attrib == "WSRC"));
}

bool amiq_rm_field::is_clear_on_write() {
	return ((attrib == "WC") || (attrib == "WCRS"));
}

bool amiq_rm_field::is_set_on_read() {
	return ((attrib == "RS") || (attrib == "WRS") || (attrib == "WCRS"));
}

bool amiq_rm_field::is_set_on_write() {
	return ((attrib == "WS") || (attrib == "WSRC"));
}

string amiq_rm_field::to_string() {
	ostringstream convert;
	convert << name << " Lsb Position: " << lsb_position << " Size: " << size << " W: " << is_writable() << " R: " << is_readable() << " Reset: "
			<< reset_value;
	return convert.str();
}

}

#endif
