/*
 * Generated on: 2023-11-02 16:43:32
 * Created by: SinaiR&D
 */

#ifndef WPP_LWM2M_SECURITY_INFO_H
#define WPP_LWM2M_SECURITY_INFO_H

#include "ObjectInfo.h"

#if OBJ_M_0_LWM2M_SECURITY_V11

namespace wpp {

static const ObjectInfo LWM2M_SECURITY_OBJ_INFO = {
	/* Name */
	"LWM2M Security",
	/* Object ID */
	OBJ_ID::LWM2M_SECURITY,
	/* URN */
	"urn:oma:lwm2m:oma:0:1.1",
	/* Object version */
	{1,1},
	/* Lwm2m version */
	{1,1},
	/* Is single */
	IS_SINGLE::MULTIPLE,
	/* Is Mandatory */
	IS_MANDATORY::MANDATORY,
	/* Object supported operations */
	/* --------------- Info block 0 start --------------- */
	InstOp(	InstOp::CREATE |
			InstOp::DELETE),
	ResOp(	ResOp::READ|
			ResOp::WRITE|
			ResOp::DISCOVER|
			ResOp::EXECUTE|
			ResOp::DELETE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_M_0_LWM2M_SECURITY_V11 */
#endif // WPP_LWM2M_SECURITY_INFO_H
