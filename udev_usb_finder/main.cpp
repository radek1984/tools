#include <iostream>
#include <string>
#include <vector>

#include <libudev.h>


void find(uint16_t vid, uint16_t pid, const char *dev_class, std::vector<std::string> &out_dev_ifce_nodes)
{
	struct udev *udev_ctx = udev_new();

	struct udev_enumerate *enumerate = udev_enumerate_new(udev_ctx);
	int res;
	res = udev_enumerate_add_match_subsystem(enumerate, dev_class);
	//std::cout <<"res enumerate add filter: "<< res <<std::endl;
	res = udev_enumerate_scan_devices(enumerate);
	//std::cout <<"res scan: "<< res <<std::endl<<std::endl;

	struct udev_list_entry *usb_dev_ifce_list = udev_enumerate_get_list_entry(enumerate);
	struct udev_list_entry *ifce_it = nullptr;
	
	udev_list_entry_foreach(ifce_it, usb_dev_ifce_list)
	{
		bool pid_ok = false;
		bool vid_ok = false;
		//ifce:
		struct udev_device *ifce_dev = udev_device_new_from_syspath(udev_ctx, udev_list_entry_get_name(ifce_it));

		 if (ifce_dev != nullptr)
		 {
			//the usb device:
			struct udev_device *device = udev_device_get_parent_with_subsystem_devtype(ifce_dev, "usb", "usb_device");
			if (device != nullptr)
			{
				const char *vendor_id = udev_device_get_sysattr_value(device, "idVendor");
				const char *product_id;

				if (vendor_id)
				{
					std::string vid_str(vendor_id);
					uint16_t vid_tmp = (uint16_t)std::stoi(vid_str, nullptr, 16);
					vid_ok = (vid_tmp == vid);

					product_id = udev_device_get_sysattr_value(device, "idProduct");
					if (product_id)
					{
						std::string pid_str(product_id);
						uint16_t pid_tmp = (uint16_t)std::stoi(pid_str, nullptr, 16);
						pid_ok = (pid_tmp == pid);
					}
				}
		
				if (pid_ok && vid_ok)
				{
					const char *ifce_dev_node = nullptr;

					if((ifce_dev_node = udev_device_get_devnode(ifce_dev)))
					{
						std::cout<<"ifce node: " << ifce_dev_node << std::endl;
						out_dev_ifce_nodes.push_back(std::string(ifce_dev_node));
					}

					if ((vendor_id = udev_device_get_sysattr_value(device, "idVendor")))
					{
						std::cout<<"device vendor: " << vendor_id << std::endl;
					}

					if ((product_id = udev_device_get_sysattr_value(device, "idProduct")))
					{
						std::cout<<"device pid: " << product_id << std::endl;
					}

					const char *usb_dev_node = udev_device_get_devnode(device);
					if(usb_dev_node)
					{
						std::cout<<"usb dev_node: " << usb_dev_node << std::endl;
					}

					const char *action = udev_device_get_action(device);
					if(action) {
						std::cout<<"dev_action: " << action << std::endl;
					}

					const char *subsystem = udev_device_get_subsystem(device);
					if (subsystem) {
						std::cout<<"device subsystem: " << subsystem << std::endl;
					}

					const char *dev_type = udev_device_get_devtype(device);
					if (dev_type) {
						std::cout<<"device type: " << dev_type << std::endl;
					}

					const char *dev_path = udev_device_get_devpath(device);
					if (dev_path) {
						std::cout<<"dev_path: " << dev_path << std::endl;
					}

					const char *sys_path = udev_device_get_syspath(device);
					if (sys_path) {
						std::cout<<"sys_path: " << sys_path << std::endl;
					}

				}

				udev_device_unref(device);
			}//if (device != nullptr)
			
		} //if (ifce_dev != nullptr)

	} //loop-end

	std::cout <<std::endl;
	udev_enumerate_unref(enumerate);
	udev_unref(udev_ctx);
}

int main()
{
	//choose
	const uint16_t vid = 0x09da;
	const uint16_t pid = 0x3597;
	const char *dev_class = "input";
	std::vector<std::string> out_dev_ifce_nodes;

	find(vid, pid, dev_class, out_dev_ifce_nodes);

	std::cout<<std::endl<<"Found:"<<std::endl;
	for (auto &n:out_dev_ifce_nodes)
		std::cout<<n<<std::endl;
	std::cout<<std::endl;

	return 0;
}
