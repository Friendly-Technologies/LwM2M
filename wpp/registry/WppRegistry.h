/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPPREGISTRY_H_
#define WPPREGISTRY_H_

#include <mutex>

#include "Object.h"

#include "mandatory/security/Security.h"
#include "mandatory/server/Server.h"
#include "mandatory/device/Device.h"

#ifdef ACL_OBJ
#include "optional/acl/Acl.h"
#endif
#ifdef CONN_MONITORING_OBJ
#include "optional/conn_monitoring/ConnMonitoring.h"
#endif
#ifdef FIRMWARE_UPD_OBJ
#include "optional/firmware_upd/FirmwareUpd.h"
#endif

namespace wpp {

// TODO: Split mandatory and optional registers
// TODO: Add ability to check whether some object id is exist
// TODO: Add ability to get Lwm2mObject by ID
class WppRegistry {
public:
	/*
	 * OBJ_RESTORE_T represents callback that notifies user about the requirement of restoring the object state to default
	 *
	 * NOTE: Keep in mind that while std::function itself is always copy able,
	 * it might hold a callable object (like a lambda) that captures
	 * variables which may not be copy able. If you try to copy a
	 * std::function that holds a non-copyable callable, it will compile,
	 * but will throw a std::bad_function_call exception at runtime if
	 * you try to call the copied std::function.
	 */
	using OBJ_RESTORE_T = std::function<void(Lwm2mObject &)>;

private:
	WppRegistry(const OBJ_RESTORE_T &objRestoreFunc);

public:
	~WppRegistry() {}

	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	/* ------------- Registry management ------------- */
	/*
	 * Create registry instance
	 */
	static bool create(const OBJ_RESTORE_T &objRestoreFunc);
	static bool isCreated();

	/*
	 * At the same time, only one actor can have access to the registry,
	 * either in WppClient or wpp user, this is done in order to synchronize
	 * access to the registry and avoid possible simultaneous writing/reading/deleting
	 * ofInstanceI or Resource.
	 * 
	 * To gain access to the registry, the user must call takeOwnership() which
	 * returns a pointer to the registry, or NULL if the registry is occupied by
	 * another actor or uninitialized. After the registry is finished, the user
	 * must call giveOwnership() to allow WppClient to process the server's
	 * internal state and messages. Implementation of WppConnectionI, WppPlatformI,
	 * InstanceI or callback generated by them should not take ownership for registry
	 * instead they should use registry() method. This is due to the fact that
	 * these interfaces are not the initial registry usage points.
	 */
	static WppRegistry* takeOwnership();
	static void giveOwnership();
	static WppRegistry* registry();

	/* ------------- Registry functionality ------------- */
	void restoreObject(Lwm2mObject &object);

	/* ------------- Mandatory objects ------------- */
	Object<Security>& security() {
		static const ObjectInfo info = {
				"LWM2M Security",			// Name
				OBJ_ID::SECURITY,			// Object ID
				"urn:oma:lwm2m:oma:0:1.1",	// URN
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
		return *Object<Security>::object();
	}

	Object<Server>& server() {
		static const ObjectInfo info = {
				"LwM2M Server",				// Name
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
		return *Object<Server>::object();
	}

	Object<Device>& device() {
		// Device Object (ID:3) SHALL NOT be affected by any Delete operation.
		static const ObjectInfo info = {
				"Device",					// Name
				OBJ_ID::DEVICE,				// Object ID
				"urn:oma:lwm2m:oma:3:1.1",	// URN
				{1,1},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::SINGLE,			// Is single
				IS_MANDATORY::MANDATORY,	// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE),
		};

		if (!Object<Device>::isCreated()) Object<Device>::create(info);
		return *Object<Device>::object();
	}

	/* ------------- Optional objects ------------- */
	#ifdef ACL_OBJ
	Object<Acl>& acl() {
		static const ObjectInfo info = {
				"LwM2M Access Control",		// Name
				OBJ_ID::ACL,				// Object ID
				"urn:oma:lwm2m:oma:2",		// URN
				{1,0},						// Object version
				{1,0},						// Lwm2m version
				IS_SINGLE::MULTIPLE,		// Is single
				IS_MANDATORY::OPTIONAL,		// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Acl>::isCreated()) Object<Acl>::create(info);
		return *Object<Acl>::object();
	}
	#endif

	#ifdef CONN_MONITORING_OBJ
	Object<ConnMonitoring>& connMonitoring() {
		static const ObjectInfo info = {
				"Connectivity Monitoring",	// Name
				OBJ_ID::CONN_MONITORING,	// Object ID
				"urn:oma:lwm2m:oma:4:1.2 ",	// URN
				{1,2},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::SINGLE,			// Is single
				IS_MANDATORY::OPTIONAL,		// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<ConnMonitoring>::isCreated()) Object<ConnMonitoring>::create(info);
		return *Object<ConnMonitoring>::object();
	}
	#endif

	#ifdef FIRMWARE_UPD_OBJ
		Object<FirmwareUpd>& firmwareUpd() {
		static const ObjectInfo info = {
				"Firmware Update",			// Name
				OBJ_ID::ACFIRMWARE_UPDL,	// Object ID
				"urn:oma:lwm2m:oma:5",		// URN
				{1,0},						// Object version
				{1,0},						// Lwm2m version
				IS_SINGLE::SINGLE,			// Is single
				IS_MANDATORY::OPTIONAL,		// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<FirmwareUpd>::isCreated()) Object<FirmwareUpd>::create(info);
		return *Object<FirmwareUpd>::object();
	}
	#endif

private:
	static WppRegistry *_registry;
	static std::mutex _regGuard;

	OBJ_RESTORE_T _objRestoreFunc;
};

} // namespace wpp

#endif // WPPREGISTRY_H_
