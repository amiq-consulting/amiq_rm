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
 * NAME:        test_usecase.cpp
 * PROJECT:     amiq_rm
 *******************************************************************************/

#ifndef TEST_USECASE
#define TEST_USECASE

#include "amiq_rm.h"
#include <iostream>

using namespace std;
using namespace amiq_rm;

//{{{ register definitions
class local_id: public amiq_rm_reg {
public:

	local_id(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("local_id", 0x00, 8, "RW"));
	}
};

class software_reset: public amiq_rm_reg {
public:

	software_reset(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("sw_reset", 0x00, 8, "W1C"));
	}
};

class input_enable: public amiq_rm_reg {
public:

	input_enable(string my_name) :
			amiq_rm_reg(my_name) {
		for (int unsigned i = 0; i < 4; i++) {
			ostringstream name;
			name << "input_en_" << i;
			add_field(new amiq_rm_field(name.str(), 0x1, 1, "RW"));
		}

		add_field(new amiq_rm_field("reserved", 0x0, 4, "RO"));
	}
};

class output_enable: public amiq_rm_reg {
public:

	output_enable(string my_name) :
			amiq_rm_reg(my_name) {
		for (int unsigned i = 0; i < 4; i++) {
			ostringstream name;
			name << "output_en_" << i;
			add_field(new amiq_rm_field(name.str(), 0x1, 1, "RW"));
		}
		add_field(new amiq_rm_field("reserved", 0x0, 4, "RO"));
	}
};

class prio_destination: public amiq_rm_reg {
public:

	prio_destination(string my_name) :
			amiq_rm_reg(my_name) {
		for (int unsigned i = 0; i < 4; i++) {
			ostringstream name;
			name << "prio_" << i;
			add_field(new amiq_rm_field(name.str(), i, 2, "RW"));
		}
	}
};

class dropped_ch: public amiq_rm_reg {
public:

	dropped_ch(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("dropped_ch", 0x0, 8, "RC"));
	}
};

class ld_baud: public amiq_rm_reg {
public:

	ld_baud(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("ld_baud", 0x0, 8, "W1C"));
	}
};

class baud_0: public amiq_rm_reg {
public:

	baud_0(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("baud_0", 0x00, 8, "RW"));
	}
};

class baud_1: public amiq_rm_reg {
public:

	baud_1(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("baud_1", 0x10, 8, "RW"));
	}
};

class baud_2: public amiq_rm_reg {
public:

	baud_2(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("baud_2", 0x0, 8, "RW"));
	}
};

class baud_3: public amiq_rm_reg {
public:

	baud_3(string my_name) :
			amiq_rm_reg(my_name) {
		add_field(new amiq_rm_field("baud_3", 0x1, 4, "RW"));
		add_field(new amiq_rm_field("reserved", 0x0, 4, "RO"));
	}
};

//}}}
class amiq_rtr_reg_block: public amiq_rm_reg_block {
public:
	local_id local_id_h;
	software_reset software_reset_h;
	input_enable input_enable_h;
	output_enable output_enable_h;
	prio_destination prio_destination_h;
	vector<dropped_ch*> dropped_ch_h;
	ld_baud ld_baud_h;
	baud_0 baud_0_h;
	baud_1 baud_1_h;
	baud_2 baud_2_h;
	baud_3 baud_3_h;

	amiq_rm_physical_address_map uart_map;
	amiq_rm_physical_address_map ahb_map;

	void add_to_map(amiq_rm_address_map &map, amiq_rm_reg_address_t map_offset) {
		map.add_reg(local_id_h, 0x00);
		map.add_reg(software_reset_h, 0x04);
		map.add_reg(input_enable_h, 0x08);
		map.add_reg(output_enable_h, 0x0C);
		map.add_reg(prio_destination_h, 0x10);
		for (int i = 0; i < 4; i++)
			map.add_reg(*dropped_ch_h[i], 0x32 + dropped_ch_h[i]->get_size());
		map.add_reg(ld_baud_h, 0xFB);
		map.add_reg(baud_0_h, 0xFC);
		map.add_reg(baud_1_h, 0xFD);
		map.add_reg(baud_2_h, 0xFE);
		map.add_reg(baud_3_h, 0xFF);

	}

	amiq_rtr_reg_block() :
			amiq_rm_reg_block("my_reg_block"), local_id_h("local_id"), software_reset_h("software_reset"), input_enable_h("input_enable"), output_enable_h(
					"output_enable"), prio_destination_h("prio_destination"), ld_baud_h("ld_baud"), baud_0_h("baud_0"), baud_1_h("baud_1"), baud_2_h(
					"baud_2"), baud_3_h("baud_3"), uart_map("uart_map"), ahb_map("ahb_map") {

		for (int i = 0; i < 4; i++) {
			ostringstream name;
			name << "dropped_ch_" << i;
			dropped_ch_h.push_back(new dropped_ch(name.str()));
		}

		add_to_map(ahb_map, 0x00);
		add_to_map(uart_map, 0x20);

		ahb_map.build();
		uart_map.build();
	}

	~amiq_rtr_reg_block() {
		for (int unsigned i = 0; i < dropped_ch_h.size(); i++)
			delete dropped_ch_h[i];
	}

};

class reg_test {
public:
	amiq_rtr_reg_block reg_block;

	void do_read_reg(amiq_rm_reg &reg, amiq_rm_physical_address_map &map) {
		pair<amiq_rm_reg_data_t, amiq_rm_status_t> data_with_status;
		amiq_rm_reg_address_t address = map.get_reg_offsets(reg)[0];

		data_with_status = map.read(address);

		cout << "Read value (from map " << map.name << ") from address " << hex << address << " (reg " << reg.name << ") - data: " << hex
				<< data_with_status.first << " status:" << data_with_status.second << endl;
	}

	void do_write_reg(amiq_rm_reg &reg, amiq_rm_reg_data_t write_data, amiq_rm_physical_address_map &map) {
		amiq_rm_reg_address_t address;
		amiq_rm_status_t status;

		address = map.get_reg_offsets(reg)[0];
		status = map.write(address, write_data);

		cout << "Write (from map " << map.name << ") to address " << hex << address << " (reg " << reg.name << ") - data: " << hex << write_data
				<< " status:" << status << endl;
	}

	void read_values_on_uart() {
		do_read_reg(reg_block.local_id_h, reg_block.uart_map);
		do_read_reg(reg_block.input_enable_h, reg_block.uart_map);
		do_read_reg(reg_block.output_enable_h, reg_block.uart_map);
		do_read_reg(reg_block.prio_destination_h, reg_block.uart_map);
		do_read_reg(reg_block.baud_0_h, reg_block.uart_map);
		do_read_reg(reg_block.baud_1_h, reg_block.uart_map);
		do_read_reg(reg_block.baud_2_h, reg_block.uart_map);
		do_read_reg(reg_block.baud_3_h, reg_block.uart_map);
	}

	void configure_on_ahb() {

		pair<amiq_rm_reg_data_t, amiq_rm_status_t> data_with_status;

		//do software reset
		do_write_reg(reg_block.software_reset_h, 0x01, reg_block.ahb_map);
		//check to see if software reset was ok
		do_read_reg(reg_block.software_reset_h, reg_block.ahb_map);

		//set local_id = 0xCF
		do_write_reg(reg_block.local_id_h, 0xCF, reg_block.ahb_map);

		//set baud_registers
		do_write_reg(reg_block.baud_0_h, 0xA0, reg_block.ahb_map);
		do_write_reg(reg_block.baud_1_h, 0xD2, reg_block.ahb_map);
		do_write_reg(reg_block.baud_2_h, 0xFF, reg_block.ahb_map);
		do_write_reg(reg_block.baud_3_h, 0x2, reg_block.ahb_map);
	}

	void test() {

		reg_block.uart_map.reset();
		reg_block.ahb_map.reset();

		read_values_on_uart();

		configure_on_ahb();

		//check that configuration was done correctly
		read_values_on_uart();

	}

};


int main(int args, char * argv[]) {
	cout << "Start running test ..." << endl;
	reg_test test;
	test.test();
	return 0;
}


#endif
