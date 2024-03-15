/*
 * FirmwareUpdate
 * Generated on: 2023-11-08 13:50:54
 * Created by: SinaiR&D
 */

#include "o_5_firmware_update_v11/FirmwareUpdate.h"

#include "Resource.h"
#include "ResOp.h"
#include "types.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#include <cstring>
#include "WppPlatform.h"

#if RES_5_13
#define DEFER_NOT_ALLWED 0
#endif

#define SCHEME_DIVIDER 	"://"
#define COAP_SCHEME 	"coap"
#define COAPS_SCHEME 	"coaps"
#define HTTP_SCHEME 	"http"
#define HTTPS_SCHEME 	"https"
#define COAP_TCP_SCHEME "coap+tcp"
#define COAP_TLS_SCHEME "coaps+tcp"
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "FirmwareUpdate"

namespace wpp {

FirmwareUpdate::FirmwareUpdate(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	#if RES_5_13
	_deferUpdateTaskId = WPP_ERR_TASK_ID;
	#endif
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

FirmwareUpdate::~FirmwareUpdate() {
	/* --------------- Code_cpp block 3 start --------------- */
	#if RES_5_13
	stopDeferUpdateGuard();
	#endif
	/* --------------- Code_cpp block 3 end --------------- */
}

void FirmwareUpdate::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	void stopDeferUpdateGuard();
	/* --------------- Code_cpp block 4 end --------------- */

	_resources.clear();
	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void FirmwareUpdate::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 6 start --------------- */
	WPP_LOGD_ARG(TAG, "Server operation -> type: %d, resId: %d, resInstId: %d", type, resId.resId, resId.resInstId);
	switch (type) {
	case ResOp::WRITE_UPD:
	case ResOp::WRITE_REPLACE_RES: {
		switch (resId.resId) {
		case PACKAGE_0: {
			OPAQUE_T *pkg;
			resource(PACKAGE_0)->ptr(&pkg);
			if (pkg->empty()) {
				WPP_LOGD_ARG(TAG, "Server reset state machine through PACKAGE_0", resId.resId, resId.resInstId);
				resetStateMachine();
				eventNotify(*this, E_RESET);
			} else {
				changeUpdRes(R_INITIAL);
				changeState(S_DOWNLOADING);
				eventNotify(*this, E_PKG_DOWNLOADING);
				changeState(S_DOWNLOADED);
				eventNotify(*this, E_DOWNLOADED);
			}
			break;
		}
		case PACKAGE_URI_1: {
			STRING_T pkgUri;
			resource(PACKAGE_URI_1)->get(pkgUri);
			if (pkgUri.empty()) {
				WPP_LOGD_ARG(TAG, "Server reset state machine through PACKAGE_URI_1", resId.resId, resId.resInstId);
				resetStateMachine();
				eventNotify(*this, E_RESET);
			} else {
				changeUpdRes(R_INITIAL);
				changeState(S_DOWNLOADING);
				eventNotify(*this, E_URI_DOWNLOADIN);
			}
			break;
		}
		default: break;
		}
	}
	case ResOp::EXECUTE: {
		switch (resId.resId) {
		case UPDATE_2: {
			INT_T state;
			resource(STATE_3)->get(state);
			if (state == S_DOWNLOADED) {
				#if RES_5_13
				stopDeferUpdateGuard();
				#endif
				changeUpdRes(R_INITIAL);
				changeState(S_UPDATING);
			}
			break;
		}
		#if RES_5_10
		case CANCEL_10: 
			resetStateMachine();
			changeUpdRes(R_FW_UPD_CANCELLED);
			break;
		#endif
		default: break;
		}
		break;
	}
	default: break;
	}
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resId, type);

	/* --------------- Code_cpp block 7 start --------------- */
	/* --------------- Code_cpp block 7 end --------------- */
}

void FirmwareUpdate::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 8 start --------------- */
	WPP_LOGD_ARG(TAG, "User operation -> type: %d, resId: %d, resInstId: %d", type, resId.resId, resId.resInstId);
	switch (type) {
	case ResOp::WRITE_UPD: {
		switch (resId.resId) {
		case STATE_3: {
			INT_T state;
			resource(STATE_3)->get(state);
			switch (state) {
			case S_IDLE:
				resource(PACKAGE_0)->set(OPAQUE_T());
				notifyServerResChanged({PACKAGE_0,});
				resource(PACKAGE_URI_1)->set(STRING_T(""));
				notifyServerResChanged({PACKAGE_URI_1,});
				changeUpdRes(R_INITIAL);
				break;
			case S_DOWNLOADED:
				eventNotify(*this, E_DOWNLOADED);
				break;
			#if RES_5_13
			case S_UPDATING: {
				INT_T res;
				resource(UPDATE_RESULT_5)->get(res);
				if (res == R_FW_UPD_DEFERRED) {
					resource(UPDATE_RESULT_5)->set(INT_T(R_INITIAL));
					stopDeferUpdateGuard();
				}
			}
			#endif
			default: break;
			}
			break;
		}
		case UPDATE_RESULT_5: {
			INT_T res;
			resource(UPDATE_RESULT_5)->get(res);
			switch (res) {
			case R_FW_UPD_SUCCESS:
				changeState(S_IDLE);
				break;
			#if RES_5_13
			case R_FW_UPD_DEFERRED: {
				INT_T state;
				resource(STATE_3)->get(state);
				if (state == S_UPDATING) {
					changeState(S_DOWNLOADED);
					startDeferUpdateGuard();
				}
				else resource(UPDATE_RESULT_5)->set(INT_T(R_INITIAL));
				break;
			}
			#endif
			case R_NOT_ENOUGH_FLASH:
			case R_OUT_OF_RAM:
			case R_CONN_LOST:
			case R_INTEGRITY_CHECK_FAIL:
			case R_FW_UPD_FAIL:
			case R_UNSUPPORTED_PROTOCOL:
			case R_FW_UPD_CANCELLED: {
				resource(PACKAGE_0)->set(OPAQUE_T());
				notifyServerResChanged({PACKAGE_0,});
				resource(PACKAGE_URI_1)->set(STRING_T(""));
				notifyServerResChanged({PACKAGE_URI_1,});
				changeState(S_IDLE);
			}
			default: break;
			}
			break;
		}
		default: break;
		}
		break;
	}
	default: break;
	}
	/* --------------- Code_cpp block 8 end --------------- */
}

/* --------------- Code_cpp block 9 start --------------- */
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
void FirmwareUpdate::serverBlockOperationNotifier(ResOp::TYPE type, const ResLink &resId, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {
	if (type != ResOp::BLOCK_WRITE || resId.resId != PACKAGE_0) return;

	if (!blockNum) {
		changeUpdRes(R_INITIAL);
		changeState(S_DOWNLOADING);
		eventNotify(*this, E_PKG_DOWNLOADING);
	} else if (isLastBlock) {
		changeState(S_DOWNLOADED);
		eventNotify(*this, E_DOWNLOADED);
	}

	blockOperationNotify(*this, resId, type, buff, blockNum, isLastBlock);
}
#endif
/* --------------- Code_cpp block 9 end --------------- */

void FirmwareUpdate::resourcesCreate() {
	std::vector<Resource> resources = {
		{PACKAGE_0,                          ResOp(ResOp::WRITE),             IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },  
		{PACKAGE_URI_1,                      ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING },  
		{UPDATE_2,                           ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE }, 
		{STATE_3,                            ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		{UPDATE_RESULT_5,                    ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		#if RES_5_6                                                                                                                                                                
		{PKGNAME_6,                          ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_7                                                                                                                                                                
		{PKGVERSION_7,                       ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_8                                                                                                                                                                
		{FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, ResOp(ResOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                     
		{FIRMWARE_UPDATE_DELIVERY_METHOD_9,  ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		#if RES_5_10                                                                                                                                                               
		{CANCEL_10,                          ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                                     
		#if RES_5_11                                                                                                                                                               
		{SEVERITY_11,                        ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                     
		#if RES_5_12                                                                                                                                                               
		{LAST_STATE_CHANGE_TIME_12,          ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },    
		#endif                                                                                                                                                                     
		#if RES_5_13                                                                                                                                                               
		{MAXIMUM_DEFER_PERIOD_13,            ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },    
		#endif                                                                                                                                                                     
	};
	_resources = std::move(resources);
}

void FirmwareUpdate::resourcesInit() {
	/* --------------- Code_cpp block 10 start --------------- */
	resource(PACKAGE_0)->set(OPAQUE_T());
	resource(PACKAGE_0)->setDataVerifier((VERIFY_OPAQUE_T)[this](const OPAQUE_T& value) { 
		if (value.empty() || isDeliveryTypeSupported(PUSH)) return true;
		return false;
	});
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	resource(PACKAGE_URI_1)->setDataVerifier((VERIFY_STRING_T)[this](const STRING_T& value) { return isUriValid(value); });
	resource(UPDATE_2)->set((EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	resource(STATE_3)->set(INT_T(S_IDLE));
	resource(STATE_3)->setDataVerifier((VERIFY_INT_T)[this](const INT_T& value) { 
		if (!isNewStateValid(State(value))) return false;
		#if RES_5_12
		resource(LAST_STATE_CHANGE_TIME_12)->set((TIME_T)WppPlatform::getTime());
		notifyServerResChanged({LAST_STATE_CHANGE_TIME_12,});
		#endif
		return true;
	});
	resource(UPDATE_RESULT_5)->set(INT_T(R_INITIAL));
	resource(UPDATE_RESULT_5)->setDataVerifier((VERIFY_INT_T)[this](const INT_T& value) { 
		if (R_INITIAL > value && value >= UPD_RES_MAX) return false;
		#if RES_5_13
		if (value == R_FW_UPD_DEFERRED) {
			UINT_T deferPeriod;
			resource(MAXIMUM_DEFER_PERIOD_13)->get(deferPeriod);
			if (deferPeriod == DEFER_NOT_ALLWED) return false;
		}
		#else
		(void)this;
		#endif
		return true; 
	});
	#if RES_5_6
	resource(PKGNAME_6)->set(STRING_T(""));
	#endif
	#if RES_5_7
	resource(PKGVERSION_7)->set(STRING_T(""));
	#endif
	#if RES_5_8
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->set(INT_T(COAP));
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return COAP <= value && value < FW_UPD_PROTOCOL_MAX; });
	#endif
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PUSH));
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return PULL <= value && value < FW_UPD_DELIVERY_MAX; });
	#if RES_5_10
	resource(CANCEL_10)->set((EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif
	#if RES_5_11
	resource(SEVERITY_11)->set(INT_T(MANDATORY));
	resource(SEVERITY_11)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return CRITICAL <= value && value < SEVERITY_MAX; });
	#endif
	#if RES_5_12
	resource(LAST_STATE_CHANGE_TIME_12)->set(TIME_T(0));
	#endif
	#if RES_5_13
	resource(MAXIMUM_DEFER_PERIOD_13)->set(UINT_T(DEFER_NOT_ALLWED));
	#endif
	/* --------------- Code_cpp block 10 end --------------- */
}

/* --------------- Code_cpp block 11 start --------------- */
void FirmwareUpdate::changeUpdRes(UpdRes res) {
	resource(UPDATE_RESULT_5)->set(INT_T(res));
	notifyServerResChanged({UPDATE_RESULT_5,});
}

void FirmwareUpdate::changeState(State state) {
	resource(STATE_3)->set(INT_T(state));
	notifyServerResChanged({STATE_3,});
}

void FirmwareUpdate::resetStateMachine() {
	resource(PACKAGE_0)->set(OPAQUE_T());
	notifyServerResChanged({PACKAGE_0,});
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	notifyServerResChanged({PACKAGE_URI_1,});
	changeState(S_IDLE);
	changeUpdRes(R_INITIAL);
}

bool FirmwareUpdate::isUriValid(STRING_T uri) {
	if (uri.empty()) return true;
	if (!isDeliveryTypeSupported(PULL)) return false;

	STRING_T scheme = extractSchemeFromUri(uri);
	if (!isSchemeValid(scheme)) {
		changeUpdRes(R_INVALID_URI);
		return false;
	}

	#if RES_5_8
	if (!isSchemeSupported(scheme)) {
		changeUpdRes(R_UNSUPPORTED_PROTOCOL);
		return false;
	}
	#endif

	return true;
}

STRING_T FirmwareUpdate::extractSchemeFromUri(STRING_T uri) {
	size_t startsAt = uri.find(SCHEME_DIVIDER);
	if (startsAt == std::string::npos) return STRING_T("");
	return uri.substr(0, startsAt);
}

bool FirmwareUpdate::isSchemeValid(STRING_T scheme) {
	if (scheme.empty()) return false;

	const char *validSchemes[] = {COAP_SCHEME, COAPS_SCHEME, HTTP_SCHEME, HTTPS_SCHEME, COAP_TCP_SCHEME, COAP_TLS_SCHEME};
	for (auto s : validSchemes) {
		if (!std::strcmp(scheme.c_str(), s)) return true;
	}
	return false;
}

#if RES_5_8
bool FirmwareUpdate::isSchemeSupported(STRING_T scheme) {
	FwUpdProtocol requiredProt = schemeToProtId(scheme);
	for (auto id : resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->getInstIds()) {
		INT_T suppProt;
		resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->get(suppProt, id);
		if (requiredProt == suppProt) return true;
	}
	return false;
}

FirmwareUpdate::FwUpdProtocol FirmwareUpdate::schemeToProtId(STRING_T scheme) {
	if (!std::strcmp(scheme.c_str(), COAP_SCHEME)) return COAP;
	else if (!std::strcmp(scheme.c_str(), COAPS_SCHEME)) return COAPS;
	else if (!std::strcmp(scheme.c_str(), HTTP_SCHEME)) return HTTP;
	else if (!std::strcmp(scheme.c_str(), HTTPS_SCHEME)) return HTTPS;
	else if (!std::strcmp(scheme.c_str(), COAP_TCP_SCHEME)) return COAP_TCP;
	else if (!std::strcmp(scheme.c_str(), COAP_TLS_SCHEME)) return COAP_TLS;
	else return FW_UPD_PROTOCOL_MAX;
}
#endif

bool FirmwareUpdate::isNewStateValid(State newState) {
	INT_T currState;
	resource(STATE_3)->get(currState);
	if (currState == newState) return true;

	switch (currState) {
		case S_IDLE:
			if (newState == S_DOWNLOADING) return true;
			break;
		case S_DOWNLOADING:
			if (newState == S_IDLE || newState == S_DOWNLOADED) return true;
			break;
		case S_DOWNLOADED:
			if (newState == S_IDLE || newState == S_UPDATING) return true;
			break;
		case S_UPDATING:
			if (newState == S_IDLE || newState == S_DOWNLOADED) return true;
			break;
		default: break;
	}
	return false;
}

bool FirmwareUpdate::isDeliveryTypeSupported(FwUpdDelivery type) {
	INT_T deliveryType = FW_UPD_DELIVERY_MAX;
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->get(deliveryType);
	if (deliveryType == type || deliveryType == BOTH) return true;
	return false;
}

#if RES_5_13
void FirmwareUpdate::startDeferUpdateGuard() {
	if (_deferUpdateTaskId != WPP_ERR_TASK_ID) return;

	UINT_T maxDefer = 0;
	resource(MAXIMUM_DEFER_PERIOD_13)->get(maxDefer);
	if (!maxDefer) return;

	_deferUpdateTaskId = WppTaskQueue::addTask(maxDefer, [this](WppClient &client, WppTaskQueue::ctx_t ctx) -> bool {
		INT_T res, state;
		resource(UPDATE_RESULT_5)->get(res);
		resource(STATE_3)->get(state);
		if (state != S_DOWNLOADED && res != R_FW_UPD_DEFERRED) return true;

		changeUpdRes(R_INITIAL);
		changeState(S_UPDATING);

		EXECUTE_T execute;
		resource(UPDATE_2)->get(execute);
		if (!resource(UPDATE_2)->get(execute) || !execute) return true;

		execute(*this, UPDATE_2, OPAQUE_T());

		return true;
	});
}

void FirmwareUpdate::stopDeferUpdateGuard() {
	if (_deferUpdateTaskId != WPP_ERR_TASK_ID) {
		WppTaskQueue::requestToRemoveTask(_deferUpdateTaskId);
		_deferUpdateTaskId = WPP_ERR_TASK_ID;
	}
}
#endif
/* --------------- Code_cpp block 11 end --------------- */

} /* namespace wpp */