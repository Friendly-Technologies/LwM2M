/*
 * Device.h
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "Instance.h"
#include "ObjectInfo.h"

namespace wpp {

class Device: public Instance {
public:
	Device(OBJ_ID objID, ID_T instanceID): Instance(objID, instanceID) {
	}
protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Returns Resource object if it is exist
	 */
	Resource * getResource(ID_T id) override;
	/*
	 * Returns return list with available resources
	 */
	std::vector<Resource *> getResourcesList() override;
	std::vector<Resource *> getResourcesList(const Operation& filter) override;
	/*
	 * Returns return list with available instantiated resources
	 */
	std::vector<Resource *> getInstantiatedResourcesList() override;
	std::vector<Resource *> getInstantiatedResourcesList(const Operation& filter) override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Operation::TYPE type, ID_T resourceId, ID_T resourceInstanceId = 0) override;
};

} /* namespace wpp */

#endif /* DEVICE_H_ */