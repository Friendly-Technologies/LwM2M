/*
 * File type: FILE_TYPE_OBJ_IMPL_H  
 * Generated on: 2023-10-13 18:40:08
 * Author: valentin
 * Generated by: Vadimzakreva
 */

#ifndef WPP_M_1_LWM2M_SERVER_V11_H
#define WPP_M_1_LWM2M_SERVER_V11_H

#include "LwM2MServerConfig.h"
#include "LwM2MServerInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
#include "WppBindings.h"
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class LwM2MServer : public Instance, public InstSubject<LwM2MServer> {
public:
	enum ID: ID_T {
		SHORT_SERVER_ID_0 = 0,
		LIFETIME_1 = 1,
	#if RES_1_2
		DEFAULT_MINIMUM_PERIOD_2 = 2,
	#endif
	#if RES_1_3
		DEFAULT_MAXIMUM_PERIOD_3 = 3,
	#endif
	#if RES_1_4
		DISABLE_4 = 4,
	#endif
	#if RES_1_5
		DISABLE_TIMEOUT_5 = 5,
	#endif
		NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6 = 6,
		BINDING_7 = 7,
		REGISTRATION_UPDATE_TRIGGER_8 = 8,
	#if RES_1_9
		BOOTSTRAP_REQUEST_TRIGGER_9 = 9,
	#endif
	#if RES_1_10
		APN_LINK_10 = 10,
	#endif
	#if RES_1_11
		TLS_DTLS_ALERT_CODE_11 = 11,
	#endif
	#if RES_1_12
		LAST_BOOTSTRAPPED_12 = 12,
	#endif
	#if RES_1_13
		REGISTRATION_PRIORITY_ORDER_13 = 13,
	#endif
	#if RES_1_14
		INITIAL_REGISTRATION_DELAY_TIMER_14 = 14,
	#endif
	#if RES_1_15
		REGISTRATION_FAILURE_BLOCK_15 = 15,
	#endif
	#if RES_1_16
		BOOTSTRAP_ON_REGISTRATION_FAILURE_16 = 16,
	#endif
	#if RES_1_17
		COMMUNICATION_RETRY_COUNT_17 = 17,
	#endif
	#if RES_1_18
		COMMUNICATION_RETRY_TIMER_18 = 18,
	#endif
	#if RES_1_19
		COMMUNICATION_SEQUENCE_DELAY_TIMER_19 = 19,
	#endif
	#if RES_1_20
		COMMUNICATION_SEQUENCE_RETRY_COUNT_20 = 20,
	#endif
	#if RES_1_21
		TRIGGER_21 = 21,
	#endif
	#if RES_1_22
		PREFERRED_TRANSPORT_22 = 22,
	#endif
	#if RES_1_23
		MUTE_SEND_23 = 23,
	#endif
	};

	/* --------------- Code_h block 1 start --------------- */
	/* --------------- Code_h block 1 end --------------- */

public:
	LwM2MServer(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~LwM2MServer();

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/* 
	 * Returns Resource object if it is exist
	 */
	Resource * getResource(ID_T id) override;
	/*
	 * Returns list with available resources
	 */
	std::vector<Resource *> getResourcesList() override;
	std::vector<Resource *> getResourcesList(const ResOp& filter) override;
	/*
	 * Returns list with available instantiated resources
	 */
	std::vector<Resource *> getInstantiatedResourcesList() override;
	std::vector<Resource *> getInstantiatedResourcesList(const ResOp& filter) override;
	/*
	 * Reset all resources values and internal state to default.
	 */
	void setDefaultState() override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Initialize resources with default values
	 * Resource always must have at least one instance.
	 * Note: From server side, empty resource == undefined resource.
	 */	
	void resourcesInit();
	
	/* --------------- Code_h block 3 start --------------- */
	/* --------------- Code_h block 3 end --------------- */

private:
	std::unordered_map<ID_T, Resource> _resources = {
		// KEY   VALUE
		{SHORT_SERVER_ID_0,                                      {SHORT_SERVER_ID_0,                                      ResOp(ResOp::READ),                     IS_SINGLE::SINGLE,        IS_MANDATORY::MANDATORY,        TYPE_ID::INT }},             
		{LIFETIME_1,                                             {LIFETIME_1,                                             ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::MANDATORY,        TYPE_ID::INT }},             
		#if RES_1_2                                                                                                                                                                                                                                      
		{DEFAULT_MINIMUM_PERIOD_2,                               {DEFAULT_MINIMUM_PERIOD_2,                               ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::INT }},             
		#endif                                                                                                                                                                                                                                           
		#if RES_1_3                                                                                                                                                                                                                                      
		{DEFAULT_MAXIMUM_PERIOD_3,                               {DEFAULT_MAXIMUM_PERIOD_3,                               ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::INT }},             
		#endif                                                                                                                                                                                                                                           
		#if RES_1_4                                                                                                                                                                                                                                      
		{DISABLE_4,                                              {DISABLE_4,                                              ResOp(ResOp::EXECUTE),                  IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::EXECUTE }},         
		#endif                                                                                                                                                                                                                                           
		#if RES_1_5                                                                                                                                                                                                                                      
		{DISABLE_TIMEOUT_5,                                      {DISABLE_TIMEOUT_5,                                      ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::INT }},             
		#endif                                                                                                                                                                                                                                           
		{NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6,        {NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6,        ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::MANDATORY,        TYPE_ID::BOOL }},            
		{BINDING_7,                                              {BINDING_7,                                              ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::MANDATORY,        TYPE_ID::STRING }},          
		{REGISTRATION_UPDATE_TRIGGER_8,                          {REGISTRATION_UPDATE_TRIGGER_8,                          ResOp(ResOp::EXECUTE),                  IS_SINGLE::SINGLE,        IS_MANDATORY::MANDATORY,        TYPE_ID::EXECUTE }},         
		#if RES_1_9                                                                                                                                                                                                                                      
		{BOOTSTRAP_REQUEST_TRIGGER_9,                            {BOOTSTRAP_REQUEST_TRIGGER_9,                            ResOp(ResOp::EXECUTE),                  IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::EXECUTE }},         
		#endif                                                                                                                                                                                                                                           
		#if RES_1_10                                                                                                                                                                                                                                     
		{APN_LINK_10,                                            {APN_LINK_10,                                            ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::OBJ_LINK }},        
		#endif                                                                                                                                                                                                                                           
		#if RES_1_11                                                                                                                                                                                                                                     
		{TLS_DTLS_ALERT_CODE_11,                                 {TLS_DTLS_ALERT_CODE_11,                                 ResOp(ResOp::READ),                     IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_12                                                                                                                                                                                                                                     
		{LAST_BOOTSTRAPPED_12,                                   {LAST_BOOTSTRAPPED_12,                                   ResOp(ResOp::READ),                     IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::TIME }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_13                                                                                                                                                                                                                                     
		{REGISTRATION_PRIORITY_ORDER_13,                         {REGISTRATION_PRIORITY_ORDER_13,                         ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_14                                                                                                                                                                                                                                     
		{INITIAL_REGISTRATION_DELAY_TIMER_14,                    {INITIAL_REGISTRATION_DELAY_TIMER_14,                    ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_15                                                                                                                                                                                                                                     
		{REGISTRATION_FAILURE_BLOCK_15,                          {REGISTRATION_FAILURE_BLOCK_15,                          ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::BOOL }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_16                                                                                                                                                                                                                                     
		{BOOTSTRAP_ON_REGISTRATION_FAILURE_16,                   {BOOTSTRAP_ON_REGISTRATION_FAILURE_16,                   ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::BOOL }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_17                                                                                                                                                                                                                                     
		{COMMUNICATION_RETRY_COUNT_17,                           {COMMUNICATION_RETRY_COUNT_17,                           ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_18                                                                                                                                                                                                                                     
		{COMMUNICATION_RETRY_TIMER_18,                           {COMMUNICATION_RETRY_TIMER_18,                           ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_19                                                                                                                                                                                                                                     
		{COMMUNICATION_SEQUENCE_DELAY_TIMER_19,                  {COMMUNICATION_SEQUENCE_DELAY_TIMER_19,                  ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_20                                                                                                                                                                                                                                     
		{COMMUNICATION_SEQUENCE_RETRY_COUNT_20,                  {COMMUNICATION_SEQUENCE_RETRY_COUNT_20,                  ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_21                                                                                                                                                                                                                                     
		{TRIGGER_21,                                             {TRIGGER_21,                                             ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::BOOL }},            
		#endif                                                                                                                                                                                                                                           
		#if RES_1_22                                                                                                                                                                                                                                     
		{PREFERRED_TRANSPORT_22,                                 {PREFERRED_TRANSPORT_22,                                 ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::STRING }},          
		#endif                                                                                                                                                                                                                                           
		#if RES_1_23                                                                                                                                                                                                                                     
		{MUTE_SEND_23,                                           {MUTE_SEND_23,                                           ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::BOOL }},            
		#endif                                                                                                                                                                                                                                           
	};

	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_M_1_LWM2M_SERVER_V11_H */
