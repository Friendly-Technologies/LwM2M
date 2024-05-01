/*
 * FirmwareUpdate
 * Generated on: 2024-04-03 15:26:42
 * Created by: SinaiR&D
 */

#include "o_5_firmware_update/FirmwareUpdate.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#include <cstring>
#include "WppPlatform.h"

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
	_pkgUpdater = NULL;
	_internalDownloader = NULL;
	#if RES_5_8
	_externalDownloader = NULL;
	_externalDownloaderTaskId = WPP_ERR_TASK_ID;
	#endif
	_internalDownloaderTaskId = WPP_ERR_TASK_ID;
	_updaterTaskId = WPP_ERR_TASK_ID;
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

FirmwareUpdate::~FirmwareUpdate() {
	/* --------------- Code_cpp block 3 start --------------- */
	#if RES_5_8
	WppTaskQueue::requestToRemoveTask(_externalDownloaderTaskId);
	#endif
	WppTaskQueue::requestToRemoveTask(_internalDownloaderTaskId);
	WppTaskQueue::requestToRemoveTask(_updaterTaskId);
	/* --------------- Code_cpp block 3 end --------------- */
}

void FirmwareUpdate::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 6 start --------------- */
	WPP_LOGD(TAG, "Server operation -> type: %d, resId: %d, resInstId: %d", type, resLink.resId, resLink.resInstId);
	switch (type) {
	case ItemOp::WRITE: {
		if (resLink.resId == PACKAGE_0 && _internalDownloader) internalDownloaderHandler();
		#if RES_5_8
		if (resLink.resId == PACKAGE_URI_1 && _externalDownloader) externalDownloaderHandler(securityInst);
		#endif
		break;
	}
	default: break;
	}
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resLink, type);

	/* --------------- Code_cpp block 7 start --------------- */
	/* --------------- Code_cpp block 7 end --------------- */
}

void FirmwareUpdate::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 8 start --------------- */
	WPP_LOGD(TAG, "User operation -> type: %d, resId: %d, resInstId: %d", type, resLink.resId, resLink.resInstId);
	/* --------------- Code_cpp block 8 end --------------- */
}

/* --------------- Code_cpp block 9 start --------------- */
/* --------------- Code_cpp block 9 end --------------- */

void FirmwareUpdate::resourcesCreate() {
	std::vector<Resource> resources = {
		{PACKAGE_0,                          ItemOp(ItemOp::WRITE),             IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },  
		{PACKAGE_URI_1,                      ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING },  
		{UPDATE_2,                           ItemOp(ItemOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE }, 
		{STATE_3,                            ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		{UPDATE_RESULT_5,                    ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		#if RES_5_6                                                                                                                                                                
		{PKGNAME_6,                          ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_7                                                                                                                                                                
		{PKGVERSION_7,                       ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_8                                                                                                                                                                
		{FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, ItemOp(ItemOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                     
		{FIRMWARE_UPDATE_DELIVERY_METHOD_9,  ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		#if RES_5_10                                                                                                                                                               
		{CANCEL_10,                          ItemOp(ItemOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                                     
		#if RES_5_11                                                                                                                                                               
		{SEVERITY_11,                        ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                     
		#if RES_5_12                                                                                                                                                               
		{LAST_STATE_CHANGE_TIME_12,          ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },    
		#endif                                                                                                                                                                     
		#if RES_5_13                                                                                                                                                               
		{MAXIMUM_DEFER_PERIOD_13,            ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },    
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
	resource(STATE_3)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { 
		if (S_IDLE > value || value >= STATE_MAX) return false;
		return true;
	});
	resource(UPDATE_RESULT_5)->set(INT_T(R_INITIAL));
	resource(UPDATE_RESULT_5)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { 
		if (R_INITIAL > value || value >= UPD_RES_MAX) return false;
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
	/* --------------- Code_cpp block 10 end --------------- */
}

/* --------------- Code_cpp block 11 start --------------- */
bool FirmwareUpdate::setFwUpdater(FwUpdater &updater) {
	resetStateMachine();
	clearArtifacts();

	_pkgUpdater = &updater;
	// Set the update method
	resource(UPDATE_2)->set((EXECUTE_T)[this](Instance& inst, ID_T resId, const OPAQUE_T& data) { return pkgUpdaterHandler(); });
	// Set last update result
	resource(UPDATE_RESULT_5)->set(INT_T(updater.lastUpdateResult()));
	notifyServerResChanged({UPDATE_RESULT_5,});
	// Set the package name and version
	#if RES_5_6
	resource(PKGNAME_6)->set(updater.pkgName());
	notifyServerResChanged({PKGNAME_6,});
	#endif
	#if RES_5_7
	resource(PKGVERSION_7)->set(updater.pkgVersion());
	notifyServerResChanged({PKGVERSION_7,});
	#endif

	return true;
}

#if RES_5_8
std::vector<FwUpdProtocol> FirmwareUpdate::supportedProtocols() {
	std::vector<FwUpdProtocol> supportedProtocols;
	for (auto id : resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->getInstIds()) {
		INT_T protocol;
		resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->get(protocol, id);
		supportedProtocols.push_back(FwUpdProtocol(protocol));
	}

	return supportedProtocols;
}

bool FirmwareUpdate::setFwExternalDownloader(FwExternalDl &downloader) {
	resetStateMachine();
	clearArtifacts();

	_externalDownloader = &downloader;

	std::vector<FwUpdProtocol> dlSupportedProtocols = _externalDownloader->supportedProtocols();
	if (dlSupportedProtocols.empty()) {
		_externalDownloader = NULL;
		return false;
	}

	// Setup delivery type
	if (_internalDownloader) resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(BOTH));
	else resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PULL));
	notifyServerResChanged({FIRMWARE_UPDATE_DELIVERY_METHOD_9,});

	// Setup supported protocols
	ID_T instId = 0;
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->clear();
	for (auto prot : dlSupportedProtocols) {
		resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->set(INT_T(prot), instId);
		instId++;
	}
	notifyServerResChanged({FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8,});

	return true;
}
#endif

bool FirmwareUpdate::setFwInternalDownloader(FwInternalDl &downloader) {
	// TODO: Update the implementation of this method after creating an
	// interface for downloading firmware via uri using the wpp library.
	// Currently, FwInternalDl only supports loading through the PACKAGE_0 resource.
	resetStateMachine();
	clearArtifacts();

	_internalDownloader = &downloader;
	
	// Setup delivery type
	#if RES_5_8
	if (_externalDownloader) resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(BOTH));
	else resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PUSH));
	#else
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PUSH));
	#endif
	notifyServerResChanged({FIRMWARE_UPDATE_DELIVERY_METHOD_9,});

	return true;
}

bool FirmwareUpdate::pkgUpdaterHandler() {
	INT_T state;
	resource(STATE_3)->get(state);
	if (state != S_DOWNLOADED) return false;

	_pkgUpdater->startUpdating();
	changeState(S_UPDATING);

	_updaterTaskId = WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, [this](WppClient &client, void *ctx) -> bool {
		if (!_pkgUpdater->isUpdated()) return false;

		FwUpdRes res = _pkgUpdater->lastUpdateResult();
		changeState(S_IDLE);
		changeUpdRes(res);

		if (res == R_FW_UPD_SUCCESS) {
			#if RES_5_6
			resource(PKGNAME_6)->set(_pkgUpdater->pkgName());
			notifyServerResChanged({PKGNAME_6,});
			#endif
			#if RES_5_7
			resource(PKGVERSION_7)->set(_pkgUpdater->pkgVersion());
			notifyServerResChanged({PKGVERSION_7,});
			#endif
			#if RES_3_3
			client.registry().device().instance()->set(Device::FIRMWARE_VERSION_3, _pkgUpdater->pkgVersion());
			#endif
		}

		return true;
	});

	return true;
}

#if RES_5_8
void FirmwareUpdate::externalDownloaderHandler(Instance *securityInst) {
	if (!securityInst) {
		WPP_LOGE(TAG, "Security object instance is not set");
		return;
	}

	STRING_T pkgUri;
	resetStateMachine();
	resource(PACKAGE_URI_1)->get(pkgUri);
	if (pkgUri.empty()) {
		clearArtifacts();
		WPP_LOGD(TAG, "Server reset state machine through PACKAGE_URI_1");
		return;
	}

	_externalDownloader->startDownloading(pkgUri, *static_cast<Lwm2mSecurity*>(securityInst));
	changeState(S_DOWNLOADING);

	_externalDownloaderTaskId = WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, [this](WppClient &client, void *ctx) -> bool {
		if (!_externalDownloader->isDownloaded()) return false;

		FwUpdRes res = _externalDownloader->downloadResult();
		if (res != R_INITIAL) changeState(S_IDLE);
		else changeState(S_DOWNLOADED);

		changeUpdRes(res);

		return true;
	});
}
#endif

void FirmwareUpdate::internalDownloaderHandler() {
	// TODO: Update the implementation of this method after creating an
	// interface for downloading firmware via uri using the wpp library.
	// Currently, FwInternalDl only supports loading through the PACKAGE_0 resource.
	OPAQUE_T *pkg;
	resetStateMachine();
	resource(PACKAGE_0)->ptr(&pkg);	
	if (pkg->empty()) {
		clearArtifacts();
		WPP_LOGD(TAG, "Server reset state machine through PACKAGE_0");
		return;
	} 
	
	_internalDownloader->downloadIsStarted();
	changeState(S_DOWNLOADING);

	_internalDownloaderTaskId = WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, [this](WppClient &client, void *ctx) -> bool {
		OPAQUE_T *pkg;
		resource(PACKAGE_0)->ptr(&pkg);	

		_internalDownloader->saveDownloadedBlock(*pkg);
		_internalDownloader->downloadIsCompleted();
		if (_internalDownloader->downloadResult() != R_INITIAL) changeState(S_IDLE);
		else changeState(S_DOWNLOADED);

		changeUpdRes(_internalDownloader->downloadResult());

		return true;
	});
}

void FirmwareUpdate::changeUpdRes(FwUpdRes res) {
	resource(UPDATE_RESULT_5)->set(INT_T(res));
	notifyServerResChanged({UPDATE_RESULT_5,});
}

void FirmwareUpdate::changeState(FwUpdState state) {
	resource(STATE_3)->set(INT_T(state));
	notifyServerResChanged({STATE_3,});
}

void FirmwareUpdate::resetStateMachine() {
	#if RES_5_8
	WppTaskQueue::requestToRemoveTask(_externalDownloaderTaskId);
	_externalDownloaderTaskId = WPP_ERR_TASK_ID;
	#endif
	WppTaskQueue::requestToRemoveTask(_internalDownloaderTaskId);
	_internalDownloaderTaskId = WPP_ERR_TASK_ID;
	WppTaskQueue::requestToRemoveTask(_updaterTaskId);
	_updaterTaskId = WPP_ERR_TASK_ID;
	
	if (_internalDownloader) _internalDownloader->reset();
	#if RES_5_8
	if (_externalDownloader) _externalDownloader->reset();
	#endif
	if (_pkgUpdater) _pkgUpdater->reset();

	changeState(S_IDLE);
	changeUpdRes(R_INITIAL);
}

void FirmwareUpdate::clearArtifacts() {
	resource(PACKAGE_0)->set(OPAQUE_T());
	notifyServerResChanged({PACKAGE_0,});
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	notifyServerResChanged({PACKAGE_URI_1,});
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

FwUpdProtocol FirmwareUpdate::schemeToProtId(STRING_T scheme) {
	if (!std::strcmp(scheme.c_str(), COAP_SCHEME)) return COAP;
	else if (!std::strcmp(scheme.c_str(), COAPS_SCHEME)) return COAPS;
	else if (!std::strcmp(scheme.c_str(), HTTP_SCHEME)) return HTTP;
	else if (!std::strcmp(scheme.c_str(), HTTPS_SCHEME)) return HTTPS;
	else if (!std::strcmp(scheme.c_str(), COAP_TCP_SCHEME)) return COAP_TCP;
	else if (!std::strcmp(scheme.c_str(), COAP_TLS_SCHEME)) return COAP_TLS;
	else return FW_UPD_PROTOCOL_MAX;
}
#endif

bool FirmwareUpdate::isDeliveryTypeSupported(FwUpdDelivery type) {
	INT_T deliveryType = FW_UPD_DELIVERY_MAX;
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->get(deliveryType);
	if (deliveryType == type || deliveryType == BOTH) return true;
	return false;
}
/* --------------- Code_cpp block 11 end --------------- */

} /* namespace wpp */