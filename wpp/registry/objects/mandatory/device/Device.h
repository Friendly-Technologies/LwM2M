/*
 * Device.h
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_DEVICE_H_
#define WPP_DEVICE_H_

#include "DeviceConfig.h"
#include "DeviceInfo.h"
#include "Instance.h"
#include "InstSubject.h"

namespace wpp {

class Device: public Instance, public InstSubject<Device> {
public:
	enum ID: ID_T {
		REBOOT = 4,
		ERROR_CODE = 11,
		SUPPORTED_BINDINGS = 16,
	};

public:
	Device(WppClient &client, const OBJ_LINK_T &id);

protected:
	/* ---------------Instance implementation part --------------- */
	/*
	 * Returns Resource object if it is exist
	 */
	Resource * getResource(ID_T id) override;
	/*
	 * Returns list with available resources
	 */
	std::vector<Resource *> getResourcesList() override;
	std::vector<Resource *> getResourcesList(const Operation& filter) override;
	/*
	 * Returns list with available instantiated resources
	 */
	std::vector<Resource *> getInstantiatedResourcesList() override;
	std::vector<Resource *> getInstantiatedResourcesList(const Operation& filter) override;
	/*
	 * Reset all resources values and internal state to default.
	 */
	void clear() override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Operation::TYPE type, const ResLink &resId) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(Operation::TYPE type, const ResLink &resId) override;

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Initialize resources with default values.
	 * Resource always must have at least one instance.
	 */
	void resourcesInit();
	
private:
    std::unordered_map<ID_T, Resource> _resources = {
    	//  KEY            				 VALUE
    	{REBOOT, 			  {REBOOT, 				Operation(Operation::EXECUTE),  IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE}},
		{ERROR_CODE, 		  {ERROR_CODE, 			Operation(Operation::READ), 	IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{SUPPORTED_BINDINGS,  {SUPPORTED_BINDINGS,  Operation(Operation::READ),     IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
	};
};

} /* namespace wpp */

#endif /* WPP_DEVICE_H_ */
