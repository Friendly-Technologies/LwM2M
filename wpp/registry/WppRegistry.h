/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPPREGISTRY_H_
#define WPPREGISTRY_H_

#include "Object.h"
#include "config.h"

#include "mandatory/security/Security.h"
#include "mandatory/server/Server.h"
#include "mandatory/device/Device.h"

namespace wpp {

// TODO: Add ability to check whether some object id is exist
// TODO: Add ability to get Lwm2mObject by ID
class WppRegistry {
private:
	WppRegistry() {}

public:
	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	/* ------------- Registry management ------------- */
	static WppRegistry& instance() {
		static WppRegistry registry;
		return registry;
	}

	/* ------------- Mandatory objects ------------- */
	Object<Security>& security() {
		static const ObjectInfo info = {
				"Server",					// Name
				OBJ_ID::SERVER,				// Object ID
				"urn:oma:lwm2m:oma:1:1.1",	// URN
				{1,1},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::MULTIPLE,		// Is single
				IS_MANDATORY::MANDATORY,	// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Security>::isCreated()) Object<Security>::create(info);
		return *Object<Security>::instance();
	}

	Object<Server>& server() {
		static const ObjectInfo info = {
				"Server",					// Name
				OBJ_ID::SERVER,				// Object ID
				"urn:oma:lwm2m:oma:1:1.1",	// URN
				{1,1},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::MULTIPLE,		// Is single
				IS_MANDATORY::MANDATORY,	// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Server>::isCreated()) Object<Server>::create(info);
		return *Object<Server>::instance();
	}

	Object<Device>& device() {
		static const ObjectInfo info = {
				"Server",					// Name
				OBJ_ID::SERVER,				// Object ID
				"urn:oma:lwm2m:oma:1:1.1",	// URN
				{1,1},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::MULTIPLE,		// Is single
				IS_MANDATORY::MANDATORY,	// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Device>::isCreated()) Object<Device>::create(info);
		return *Object<Device>::instance();
	}
};

} // namespace wpp

#endif // WPPREGISTRY_H_
