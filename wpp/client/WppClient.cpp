/*
 * WppManager.cpp
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#include "WppClient.h"

//TODO: #include "liblwm2m.h"

namespace wpp {

WppClient *WppClient::_client = NULL;
std::mutex WppClient::_clientGuard;

WppClient::WppClient(const ClientInfo &info, WppConnectionI &connection): _connection(connection) {
	lwm2mContextOpen();
	lwm2mConfigure(info.endpointName, info.msisdn, info.altPath);
}

WppClient::~WppClient() {
	lwm2mContextClose();
}

/* ------------- WppClient management ------------- */
bool WppClient::create(const ClientInfo &info, WppConnectionI &connection) {
	_client = new WppClient(info, connection);
	return true;
}

bool WppClient::isCreated() {
	return _client != NULL;
}

WppClient* WppClient::takeOwnership() {
    if (!_clientGuard.try_lock()) return NULL;
    return _client;
}

void WppClient::giveOwnership() {
    _clientGuard.unlock();
}


/* ------------- WppClient components ------------- */
WppConnectionI & WppClient::connection() {
	return _connection;
}

WppRegistry & WppClient::registry() {
	return _registry;
}

/* ------------- Wakaama core state processing ------------- */
lwm2m_client_state_t WppClient::getState() {
	return _lwm2m_context->state;
}

void WppClient::loop(time_t &sleepTime) {
	// Handle wakaama core state
	int result = 0;// TODO: lwm2m_step(_lwm2m_context, availableTime);
	if (!result && getState() == STATE_BOOTSTRAPPING) {
		registry().restoreObject(registry().security());
		registry().restoreObject(registry().server());
	}

	// Handle packets retreived from server
	if (connection().getPacketQueueSize()) connection().handlePacketsInQueue(getContext());
}

bool WppClient::updateServerRegistration(INT_T serverId, bool withObjects) {
	return true;// TODO: !lwm2m_update_registration(_lwm2m_context, serverId, withObjects);
}

bool WppClient::updateServerRegistration(bool withObjects) {
	return true;// TODO: !lwm2m_update_registration(_lwm2m_context, 0, withObjects);
}

void WppClient::deregister() {
	// TODO: lwm2m_deregister(_lwm2m_context);
}


/* ------------- Wakaama core object managing ------------- */
bool WppClient::registerObject(Lwm2mObject &object) {
	return true;// TODO: !lwm2m_add_object(_lwm2m_context, &object.getLwm2mObject());
}

bool WppClient::deregisterObject(Lwm2mObject &object) {
	return true;// TODO: !lwm2m_remove_object(_lwm2m_context, &object.getLwm2mObject().objID);
}

bool WppClient::isObjectRegistered(Lwm2mObject &object) {
	lwm2m_object_t * lwm2m_object = NULL; // TODO: (lwm2m_object_t *)LWM2M_LIST_FIND(_lwm2m_context->objectList, object.getLwm2mObject().objID);
	return lwm2m_object != NULL;
}


/* ------------- Wakaama core observer notify ------------- */
void WppClient::notifyValueChanged(const DataID &data) {
	lwm2m_uri_t uri = {data.instance.objectId, data.instance.objectInstanceId, data.resource.resourceId, data.resource.resourceInstanceId};
	// TODO: lwm2m_resource_value_changed(_lwm2m_context, &uri);
}


/* ------------- Wakaama client initialisation ------------- */

WppClient * WppClient::client() {
	return _client;
}

bool WppClient::lwm2mContextOpen() {
	_lwm2m_context = new lwm2m_context_t; // TODO: lwm2m_init(this);
	return _lwm2m_context != NULL;
}

void WppClient::lwm2mContextClose() {
	// TODO: lwm2m_close(_lwm2m_context);
	_lwm2m_context = NULL;
}

lwm2m_context_t * WppClient::getContext() {
	return _lwm2m_context;
}

bool WppClient::lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath) {
//TODO:	lwm2m_object_t lwm2m_major_objects[] = {&registry().security().getLwm2mObject(),
//										  	    &registry().server().getLwm2mObject(),
//										        &registry().device().getLwm2mObject()};
//	uint16_t objectsCnt = sizeof(lwm2m_major_objects)/sizeof(lwm2m_object_t);
//	char *msisdn_c = msisdn.empty()? NULL : msisdn.c_str();
//	char *altPath_c = altPath.empty()? NULL : altPath.c_str();
//	return !lwm2m_configure(_lwm2m_context, endpointName.c_str(), msisdn_c, altPath_c, objectsCnt, lwm2m_major_objects);
	return true;
}

} /* namespace wpp */
