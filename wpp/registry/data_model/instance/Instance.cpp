/*
 * Instnace.cpp
 *
 *  Created on: 17 Jul 2023
 *      Author: valentin
 */

#include "Instance.h"

namespace wpp {

bool Instance::resourceToLwm2mData(Resource &resource, ID_T instanceId, lwm2m_data_t &data) {
	std::lock_guard<std::mutex> guard(resource.getGuard()); // TODO: it is critical part, and looks like we have conflict here

	if (!resource.isInstanceExist(instanceId)) return false;

	switch(resource.getDataType()) {
	case Resource::DATA_TYPE::BOOL: {
		// TODO: lwm2m_data_encode_bool(std::get<BOOL_T>(resource.getInstances()[instanceId]), data_ptr);
		break;
	}
	case Resource::DATA_TYPE::TIME:
	case Resource::DATA_TYPE::INT: {
			// TODO: lwm2m_data_encode_int(std::get<INT_T>(resource.getInstances()[instanceId]), data_ptr);
		break;
	}
	case Resource::DATA_TYPE::UINT: {
			// TODO: lwm2m_data_encode_uint(std::get<UINT_T>(resource.getInstances()[instanceId]), data_ptr);
		break;
	}
	case Resource::DATA_TYPE::FLOAT: {
			// TODO: lwm2m_data_encode_float(std::get<FLOAT_T>(resource.getInstances()[instanceId], data_ptr);
		break;
	}
	case Resource::DATA_TYPE::OBJ_LINK: {
			OBJ_LINK_T &value = std::get<OBJ_LINK_T>(resource.getInstances()[instanceId]);
			// TODO: lwm2m_data_encode_objlink(value.objectId, value.objectInstanceId, data_ptr);
		break;
	}
	case Resource::DATA_TYPE::OPAQUE: {
			OPAQUE_T &value = std::get<OPAQUE_T>(resource.getInstances()[instanceId]);
			// TODO: lwm2m_data_encode_objlink(value.data(), value.size(), data_ptr);
		break;
	}
	case Resource::DATA_TYPE::STRING: {
			// TODO: lwm2m_data_encode_string(std::get<STRING_T>(resource.getInstances()[instanceId]).c_str(), data_ptr);
		break;
	}
	case Resource::DATA_TYPE::CORE_LINK: {
			// TODO: lwm2m_data_encode_corelink(std::get<CORE_LINK_T>(resource.getInstances()[instanceId]).c_str(), data_ptr);
		break;
	}
	default: return false;
	}

	return true;
}

bool Instance::lwm2mDataToResource(const lwm2m_data_t &data, Resource &resource, ID_T instanceId) {
//	 TODO: Here must be lock for resource mutex but then we will can not call Resource::set()
//	std::lock_guard<std::mutex> guard(resource.getGuard()); // TODO: it is critical part, and looks like we have conflict here

	switch (resource.getDataType()) {
	case Resource::DATA_TYPE::BOOL: {
		BOOL_T value;
		if (/*TODO: !lwm2m_data_decode_bool(&data, &value) || */!resource.set(value, instanceId)) return false;
		break;
	}
	case Resource::DATA_TYPE::TIME:
	case Resource::DATA_TYPE::INT: {
		INT_T value;
		if (/*TODO: !lwm2m_data_decode_int(&data, &value) || */!resource.set(value, instanceId)) return false;
		break;
	}
	case Resource::DATA_TYPE::UINT: {
		UINT_T value;
		if (/*TODO: !lwm2m_data_decode_uint(&data, &value) || */!resource.set(value, instanceId)) return false;
		break;
	}
	case Resource::DATA_TYPE::FLOAT: {
		FLOAT_T value;
		if (/*TODO: !lwm2m_data_decode_float(&data, &value) || */!resource.set(value, instanceId)) return false;
		break;
	}
	case Resource::DATA_TYPE::OBJ_LINK: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		// TODO: Convertation algorithm is not clear
		if (data.type != LWM2M_TYPE_OBJECT_LINK) return false;
		if (resource.set(OBJ_LINK_T{ID_T_MAX_VAL, ID_T_MAX_VAL}, instanceId)) return false;
		break;
	}
	case Resource::DATA_TYPE::OPAQUE: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OPAQUE) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (resource.set(OPAQUE_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	case Resource::DATA_TYPE::STRING: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (resource.set(STRING_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	case Resource::DATA_TYPE::CORE_LINK: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING && data.type != LWM2M_TYPE_CORE_LINK) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (resource.set(CORE_LINK_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	default: return false;
	}

	return true;
}

uint8_t Instance::resourceRead(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP) {
	// Requested each resource
	if (!*numDataP) {
		std::vector<Resource *> readResources = getInstantiatedResourcesList(Operation(Operation::READ));

		// TODO: *dataArrayP = lwm2m_data_new(readResources.size());
		if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
		*numDataP = readResources.size();

		lwm2m_data_t *tmp_data_p = *dataArrayP;
		for (auto &resource : readResources) (tmp_data_p++)->id = resource->getID();
	}

	for (int i = 0; i < *numDataP; i++) {
		lwm2m_data_t *data = (*dataArrayP) + i;

		Resource *resource = getResource(data->id);
		if (resource == NULL) return COAP_404_NOT_FOUND;
		// TODO: Resource must be locked here

		// Check the server operation permission for resource
		if (!resource->getOperation().isRead()) return COAP_405_METHOD_NOT_ALLOWED;
		//  Note that availability is not mandatory for optional resources
		if (resource->isEmpty()) {
			if (resource->isOptional()) continue;
			else return COAP_404_NOT_FOUND;
		}

		// if has been received data for multiple resource with not allocated memory
		// then we ourselves allocate memory for instances
		if (resource->isMultiple() && data->type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
	//		 TODO: lwm2m_data_t *subData = lwm2m_data_new(resource.instanceCnt());
	//		lwm2m_data_t *dataCnt = subData;
	//		for (const auto& pair : resource->getInstances()) *(dataCnt++).id = pair.first;
	//		lwm2m_data_encode_instances(subData, resource->instanceCnt(), *data);
		}

		// TODO: It is bad approach but it helps to reduce code size
		size_t count = 1;
		lwm2m_data_t *data_ptr = data;
		if (data->type == LWM2M_TYPE_MULTIPLE_RESOURCE) {
			count = data->value.asChildren.count;
			data_ptr = data->value.asChildren.array;
		}

		// If resource is single then this loop execute only once
		for (size_t i = 0; i < count; i++) {
			// TODO: It is bad approach but it helps to reduce code size
			ID_T instanceId = resource->isSingle()? SINGLE_INSTANCE_ID : data_ptr[i].id;
			//  Note that availability is not mandatory for optional resources
			if (!resourceToLwm2mData(*resource, instanceId, *data)) {
				if (resource->isOptional()) continue;
				else return COAP_404_NOT_FOUND;
			}
			// If execution get to this place then operation completed with
			// success and we can notify Instance implementation about it
			serverOperationNotifier(Operation::READ, resource->getID(), instanceId);
		}
	}

	return COAP_205_CONTENT;
}

uint8_t Instance::resourceWrite(ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_write_type_t writeType) {
	// Protect access to instance list

	// TODO: In some cases, according to the implementation of the wakaama,
	// the resources marked as R can be written by the server during the
	// instance creation operation (Ex: ACL object resource 0). I did not
	// find the necessary description in the documentation, so this question
	// needs to be investigated in detail.
	//
	// DOC: Only in the Bootstrap Interface, the "Bootstrap-Write" MAY target just an Object ID,
	// which will allow a BootstrapServer in using a TLV, SenML CBOR or SenML JSON formatted
	// payload, to populate a LwM2M Client in a single message containing several Instances
	// of the same Object.
	if (writeType == LWM2M_WRITE_REPLACE_INSTANCE) {
		std::vector<Resource *> resources = getInstantiatedResourcesList(Operation(Operation::WRITE));
		for (auto resource : resources) resource->clear();
	}

	for (int i = 0; i < numData; i++) {
		Resource *resource = getResource(dataArray[i].id);
		if (!resource) return COAP_404_NOT_FOUND;
		//	 TODO: Here must be lock for resource but then we will can not call Resource::set()

		// Check the server operation permission for resource
		if (!resource->getOperation().isWrite()) return COAP_405_METHOD_NOT_ALLOWED;
		if ((dataArray[i].type == LWM2M_TYPE_MULTIPLE_RESOURCE && resource->isSingle()) ||
			(dataArray[i].type != LWM2M_TYPE_MULTIPLE_RESOURCE && resource->isMultiple())) return COAP_405_METHOD_NOT_ALLOWED;

		// Clear resource data if we need to replace it
		if (writeType == LWM2M_WRITE_REPLACE_RESOURCES || writeType == LWM2M_WRITE_REPLACE_INSTANCE) {
			resource->clear();
			// Notify Instance implementation about operation
			serverOperationNotifier(Operation::DELETE, resource->getID());
		}

		// TODO: It is bad approach but it helps to reduce code size
		size_t count = 1;
		const lwm2m_data_t *data_ptr = dataArray + i;
		if (dataArray[i].type == LWM2M_TYPE_MULTIPLE_RESOURCE) {
			count = dataArray[i].value.asChildren.count;
			data_ptr = dataArray[i].value.asChildren.array;
		}

		// If resource is single then this loop execute only once
		for (size_t i = 0; i < count; i++) {
			// TODO: It is bad approach but it helps to reduce code size
			ID_T instanceId = resource->isSingle()? SINGLE_INSTANCE_ID : data_ptr[i].id;
			//  Note that availability is not mandatory for optional resources
			if (!lwm2mDataToResource(dataArray[i], *resource, instanceId)) {
				if (resource->isOptional()) continue;
				else return COAP_404_NOT_FOUND;
			}
			// If execution get to this place then operation completed with
			// success and we can notify Instance implementation about it
			serverOperationNotifier(Operation::WRITE, resource->getID(), instanceId);
		}
	}

	return COAP_204_CHANGED;
}

uint8_t Instance::resourceExecute(ID_T instanceId, ID_T resourceId, uint8_t * buffer, int length) {
	Resource *resource = getResource(resourceId);
	if (!resource) return COAP_404_NOT_FOUND;
	// Check the server operation permission for resource
	if (!resource->getOperation().isExecute()) return COAP_405_METHOD_NOT_ALLOWED;

	EXECUTE_T execute;
	if (!resource->get(execute) || execute) {
		//  Note that availability is not mandatory for optional resources
		if (resource->isOptional()) return COAP_204_CHANGED;
		else return COAP_405_METHOD_NOT_ALLOWED;
	}

	execute(resourceId, OPAQUE_T(buffer, buffer + length));

	// If execution get to this place then operation completed with
	// success and we can notify Instance implementation about it
	serverOperationNotifier(Operation::EXECUTE, resource->getID());

	return COAP_204_CHANGED;
}

uint8_t Instance::resourceDiscover(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP) {
	// Requested each resource
	if (!*numDataP) {
		std::vector<Resource *> resources = getInstantiatedResourcesList();

		// TODO: *dataArrayP = lwm2m_data_new(readResources.size());
		if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
		*numDataP = resources.size();

		lwm2m_data_t *tmp_data_p = *dataArrayP;
		for (auto &resource : resources) {
			(tmp_data_p++)->id = resource->getID();
		}
	}

	for (int i = 0; i < *numDataP; i++) {
		lwm2m_data_t *data = (*dataArrayP) + i;
		Resource *resource = getResource(data->id);
		if (resource == NULL) return COAP_404_NOT_FOUND;

		std::lock_guard<std::mutex> guard(resource->getGuard()); // TODO: it is critical part, and looks like we have conflict here

		//  Note that availability is not mandatory for optional resources
		if (resource->isEmpty() && resource->isMandatory()) return COAP_404_NOT_FOUND;
		// if has been received data for multiple resource with not allocated memory
		// then we ourselves allocate memory for instances
		if (resource->isMultiple() && data->type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
//			 TODO: lwm2m_data_t *subData = lwm2m_data_new(resource->instanceCnt());
//			lwm2m_data_t *dataCnt = subData;
//			for (const auto& pair : resource->getInstances()) {
//				*(dataCnt++).id = pair.first;
				// If execution get to this place then operation completed with
				// success and we can notify Instance implementation about it
//				serverOperationNotifier(Operation::DISCOVER, resource->getID(), pair.first);
//			}
//			lwm2m_data_encode_instances(subData, resource->instanceCnt(), data);
		}
	}
	return COAP_205_CONTENT;
}

} // namespcae wpp

