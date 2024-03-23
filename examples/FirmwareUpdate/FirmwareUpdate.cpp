#include "FirmwareUpdate.h"

FirmwareUpdateImpl::FirmwareUpdateImpl() {}

FirmwareUpdateImpl::~FirmwareUpdateImpl() {}

void FirmwareUpdateImpl::init(Object &obj) {
	obj.actSubscribe(this);
	FirmwareUpdate *fwInst = static_cast<FirmwareUpdate *>(obj.createInstance(0));

    fwInst->setFwUpdater(*this);
    fwInst->setFwAutoDownloader(_autoDownloader);
    #if RES_5_8
    fwInst->setFwExternalDownloader(_uriDownloader);
    #endif
}

void FirmwareUpdateImpl::objectRestore(Object &object) {
	cout << "FwUpdateImpl: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
    init(object);
}

void FirmwareUpdateImpl::startUpdating() {
    cout << "FwUpdateImpl: startUpdating" << endl;
}

bool FirmwareUpdateImpl::isUpdated() {
    cout << "FwUpdateImpl: isUpdated" << endl;
    return true;
}

FwUpdRes FirmwareUpdateImpl::lastUpdateResult() {
    cout << "FwUpdateImpl: lastUpdateResult" << endl;
    return FwUpdRes::R_FW_UPD_SUCCESS;
}

STRING_T read_metadata(uint8_t line_num) {
    ifstream is;
    string str = "default";
   
    cout << "FwUpdateImpl: read_metadata" << endl;

    is.open("test_http.fw", ios::binary);
    if (is.is_open()) {
        // if file can't be opened it means it's not exists. Will return "default"
        for (uint8_t i = 0; i < line_num; i++) {
            getline(is, str);
            // TODO: split by ":=" str
        }
    }
    is.close();

    return str;
}

#if RES_5_6
STRING_T FirmwareUpdateImpl::pkgName() {
    string res = read_metadata(2);
    cout << "FwUpdateImpl: pkgName: " << res << endl;
    return res == "default" ? "current_fw" : res;
}
#endif

#if RES_5_7
STRING_T FirmwareUpdateImpl::pkgVersion() {
    string res = read_metadata(3);
    cout << "FwUpdateImpl: pkgVersion: " << res << endl;
    return res == "default" ? "1.0.0" : res;
}
#endif