/*
 * Lwm2mSecurity
 * Generated on: 2023-11-02 16:43:32
 * Created by: SinaiR&D
 */

#include "m_0_lwm2m_security_v11/Lwm2mSecurity.h"

#include "Resource.h"
#include "ResOp.h"
#include "types.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#define SERVER_URI_MAX_SIZE	255
#if RES_0_7
#define SMS_BIND_KEY_PARAMS_SIZE	6
#endif
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Lwm2mSecurity"

namespace wpp {

Lwm2mSecurity::Lwm2mSecurity(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Lwm2mSecurity::~Lwm2mSecurity() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

void Lwm2mSecurity::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	_resources.clear();
	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void Lwm2mSecurity::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 6 start --------------- */
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resId, type);

	/* --------------- Code_cpp block 7 start --------------- */
	/* --------------- Code_cpp block 7 end --------------- */
}

void Lwm2mSecurity::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 8 start --------------- */
	/* --------------- Code_cpp block 8 end --------------- */
}

void Lwm2mSecurity::resourcesCreate() {
	std::vector<Resource> resources = {
		{LWM2M_SERVER_URI_0,                  ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING },   
		{BOOTSTRAP_SERVER_1,                  ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::BOOL },     
		{SECURITY_MODE_2,                     ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },      
		{PUBLIC_KEY_OR_IDENTITY_3,            ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },   
		{SERVER_PUBLIC_KEY_4,                 ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },   
		{SECRET_KEY_5,                        ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },   
		#if RES_0_6                                                                                                                                                                  
		{SMS_SECURITY_MODE_6,                 ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                       
		#if RES_0_7                                                                                                                                                                  
		{SMS_BINDING_KEY_PARAMETERS_7,        ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::OPAQUE },   
		#endif                                                                                                                                                                       
		#if RES_0_8                                                                                                                                                                  
		{SMS_BINDING_SECRET_KEY_S__8,         ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::OPAQUE },   
		#endif                                                                                                                                                                       
		#if RES_0_9                                                                                                                                                                  
		{LWM2M_SERVER_SMS_NUMBER_9,           ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                       
		#if RES_0_10                                                                                                                                                                 
		{SHORT_SERVER_ID_10,                  ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                       
		#if RES_0_11                                                                                                                                                                 
		{CLIENT_HOLD_OFF_TIME_11,             ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                       
		#if RES_0_12                                                                                                                                                                 
		{BOOTSTRAP_SERVER_ACCOUNT_TIMEOUT_12, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                       
		#if RES_0_13                                                                                                                                                                 
		{MATCHING_TYPE_13,                    ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                       
		#if RES_0_14                                                                                                                                                                 
		{SNI_14,                              ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                       
		#if RES_0_15                                                                                                                                                                 
		{CERTIFICATE_USAGE_15,                ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                       
		#if RES_0_16                                                                                                                                                                 
		{DTLS_TLS_CIPHERSUITE_16,             ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                       
		#if RES_0_17                                                                                                                                                                 
		{OSCORE_SECURITY_MODE_17,             ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::OBJ_LINK }, 
		#endif                                                                                                                                                                       
	};
	_resources = std::move(resources);
}

void Lwm2mSecurity::resourcesInit() {
	/* --------------- Code_cpp block 9 start --------------- */
	resource(LWM2M_SERVER_URI_0)->set(STRING_T(""));
	resource(LWM2M_SERVER_URI_0)->setDataVerifier((VERIFY_STRING_T)[](const STRING_T& value) { return value.size() < SERVER_URI_MAX_SIZE; });

	resource(BOOTSTRAP_SERVER_1)->set(false);

	resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_NONE));
	resource(SECURITY_MODE_2)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return LWM2M_SECURITY_MODE_PRE_SHARED_KEY <= value && value <= LWM2M_SECURITY_MODE_NONE; });

	resource(PUBLIC_KEY_OR_IDENTITY_3)->set(OPAQUE_T());

	resource(SERVER_PUBLIC_KEY_4)->set(OPAQUE_T());
	
	resource(SECRET_KEY_5)->set(OPAQUE_T());

	#if RES_0_6
	resource(SMS_SECURITY_MODE_6)->set(INT_T(SMS_SEC_MODE_MAX));
	resource(SMS_SECURITY_MODE_6)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return SMS_SEC_MODE_MIN <= value && value <= SMS_SEC_MODE_MAX; });
	#endif
	
	#if RES_0_7
	resource(SMS_BINDING_KEY_PARAMETERS_7)->set(OPAQUE_T());
	resource(SMS_BINDING_KEY_PARAMETERS_7)->setDataVerifier((VERIFY_OPAQUE_T)[](const OPAQUE_T& value) { return value.size() == SMS_BIND_KEY_PARAMS_SIZE; });
	#endif

	#if RES_0_8_
	resource(SMS_BINDING_SECRET_KEY_S__8)->set(OPAQUE_T());
	resource(SMS_BINDING_SECRET_KEY_S__8)->setDataVerifier((VERIFY_OPAQUE_T)[](const OPAQUE_T& value) { return MIN_SMS_KEY_LEN <= value.size() && value.size() <= MAX_SMS_KEY_LEN; });
	#endif

	#if RES_0_9
	resource(LWM2M_SERVER_SMS_NUMBER_9)->set(STRING_T(""));                                                                                                                                                                                                                        
	#endif 

	#if RES_0_10
	resource(SHORT_SERVER_ID_10)->set(INT_T(0));
	resource(SHORT_SERVER_ID_10)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return SINGLE_INSTANCE_ID < value && value < ID_T_MAX_VAL; });
	#endif
	
	#if RES_0_11
	resource(CLIENT_HOLD_OFF_TIME_11)->set(INT_T(0));
	#endif
	
	#if RES_0_12
	resource(BOOTSTRAP_SERVER_ACCOUNT_TIMEOUT_12)->set(INT_T(0));                                                                                                                                                                                                             
	#endif

	#if RES_0_13
	resource(MATCHING_TYPE_13)->set(UINT_T(MAX_MATCH_TYPE));
	resource(MATCHING_TYPE_13)->setDataVerifier((VERIFY_UINT_T)[](const UINT_T& value) { return EXACT_MATCH <= value && value < MAX_MATCH_TYPE; });
	#endif

	#if RES_0_14
	resource(SNI_14)->set(STRING_T(""));                                                                                                                                                                                                                                     
	#endif

	#if RES_0_15
	resource(CERTIFICATE_USAGE_15)->set(UINT_T(MAX_MATCH_TYMAX_CERT_USAGEPE));
	resource(CERTIFICATE_USAGE_15)->setDataVerifier((VERIFY_UINT_T)[](const UINT_T& value) { return CA_CONSTRAINT <= value && value < MAX_CERT_USAGE; });
	#endif

	#if RES_0_16                                                                                                                                                                                                                            
	resource(DTLS_TLS_CIPHERSUITE_16)->set(UINT_T(0));
	#endif   

	#if RES_0_17
	resource(OSCORE_SECURITY_MODE_17)->set(OBJ_LINK_T());                                                                                                                                                                                                                        
	#endif  
	/* --------------- Code_cpp block 9 end --------------- */
}

/* --------------- Code_cpp block 10 start --------------- */
/* --------------- Code_cpp block 10 end --------------- */

} /* namespace wpp */
