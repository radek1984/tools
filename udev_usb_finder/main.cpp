#include <iostream>
#include <string>
#include <libudev.h>


void find(uint16_t vid, uint16_t pid)
{
	std::cout << std::endl;
	struct udev *udev_ctx = udev_new();

	struct udev_enumerate *enumerate = udev_enumerate_new(udev_ctx);
	int res;
	res = udev_enumerate_add_match_subsystem(enumerate, "usb");
	std::cout <<"res enumerate add filter: "<< res <<std::endl;
	res = udev_enumerate_scan_devices(enumerate);
	std::cout <<"res scan: "<< res <<std::endl;

	struct udev_list_entry *dev_list = udev_enumerate_get_list_entry(enumerate);
	struct udev_list_entry *dev_it = nullptr;
	
	udev_list_entry_foreach(dev_it, dev_list) {

		struct udev_device *device = udev_device_new_from_syspath(udev_ctx, udev_list_entry_get_name(dev_it));

		 if (device != nullptr) {
			const char *vendor_id = udev_device_get_sysattr_value(device, "idVendor");
			const char *product_id;

			if (vendor_id)
			{
				std::string vid_str(vendor_id);
				uint16_t vid_tmp = (uint16_t)std::stoi(vid_str, nullptr, 16);

				if(vid_tmp == vid)
				{
					product_id = udev_device_get_sysattr_value(device, "idProduct");
					if (product_id)
					{
						std::string pid_str(product_id);
						uint16_t pid_tmp = (uint16_t)std::stoi(pid_str, nullptr, 16);
						if (pid_tmp != pid)
							continue;
					} else continue;
				} else continue;

			} else continue;

			if (vendor_id = udev_device_get_sysattr_value(device, "idVendor")) {
				std::cout<<"device vendor: " << vendor_id << std::endl;
			} else {
				std::cout<<"device vendor: none" << std::endl;
			}

			if (product_id = udev_device_get_sysattr_value(device, "idProduct")) {
				std::cout<<"device pid: " << product_id << std::endl;
			} else {
				std::cout<<"device pid: none" << std::endl;
			}

			if(const char *dev_node = udev_device_get_devnode(device)){
				std::cout<<"dev_node: " << dev_node << std::endl;
			} else {
				std::cout<<"dev_node: none" << std::endl;
			}

			if(const char *action = udev_device_get_action(device)) {
				std::cout<<"dev_action: " << action << std::endl;
			} else {
				std::cout<<"dev_action: none" << std::endl;
			}

			if (const char *subsystem = udev_device_get_subsystem(device)) {
				std::cout<<"device subsystem: " << subsystem << std::endl;
			} else {
				std::cout<<"device subsystem: none" << std::endl;
			}

			if (const char *dev_type = udev_device_get_devtype(device)) {
				std::cout<<"device type: " << dev_type << std::endl;
			} else {
				std::cout<<"device type: none" << std::endl;
			}

			if (const char *dev_path = udev_device_get_devpath(device)) {
				std::cout<<"dev_path: " << dev_path << std::endl;
			} else {
				std::cout<<"dev_path: none" << std::endl;
			}

			if (const char *sys_path = udev_device_get_syspath(device)) {
				std::cout<<"sys_path: " << sys_path << std::endl;
			} else {
				std::cout<<"sys_path: none" << std::endl;
			}

			std::cout <<std::endl;

			udev_device_unref(device);
		}
		
	}
	
	std::cout <<std::endl;
	
	udev_enumerate_unref(enumerate);
	
	udev_unref(udev_ctx);
}

int main()
{
	//choose
	const uint16_t vid = 0x09da;
	const uint16_t pid = 0x3597;
	find(vid, pid);
	return 0;
}
