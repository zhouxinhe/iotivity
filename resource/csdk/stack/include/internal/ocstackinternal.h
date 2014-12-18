//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//

//-----------------------------------------------------------------------------
// Internal include file used by lower layers of the OC stack
//-----------------------------------------------------------------------------
#ifndef OCSTACKINTERNAL_H_
#define OCSTACKINTERNAL_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdbool.h>
#include "ocstack.h"
#include "ocstackconfig.h"
#include "occoaptoken.h"
#include "occlientcb.h"
#include <logger.h>
#include <ocrandom.h>

#ifdef CA_INT
    #include "cacommon.h"
    #include "cainterface.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern OCDeviceEntityHandler defaultDeviceHandler;

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define OC_COAP_SCHEME "coap://"
#define OC_OFFSET_SEQUENCE_NUMBER (4) // the first outgoing sequence number will be 5

typedef struct {
    // Observe option field
    uint32_t option;
    // IP address & port of client registered for observe
    OCDevAddr *subAddr;
    // CoAP token for the observe request
    OCCoAPToken *token;
    // The result of the observe request
    OCStackResult result;
} OCObserveReq;

// following structure will be created in occoap and passed up the stack on the server side
typedef struct {
    // Observe option field
    uint32_t observationOption;
    // the REST method retrieved from received request PDU
    OCMethod method;
    // resourceUrl will be filled in occoap using the path options in received request PDU
    unsigned char resourceUrl[MAX_URI_LENGTH];
    // resource query send by client
    unsigned char query[MAX_QUERY_LENGTH];
    // reqJSON is retrieved from the payload of the received request PDU
    unsigned char reqJSONPayload[MAX_REQUEST_LENGTH];
    // qos is indicating if the request is CON or NON
    OCQualityOfService qos;
    // An array of the received vendor specific header options
    uint8_t numRcvdVendorSpecificHeaderOptions;
    OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
#ifdef CA_INT
    /** Remote Endpoint address **/
    CAAddress_t addressInfo;
    /** Connectivity of the endpoint**/
    CAConnectivityType_t connectivityType;
    char token[32];   // TODO-CA:  What is max CAToken_t length?  Get rid of magic number
#endif
    //////////////////////////////////////////////////////////
    // TODO: Consider moving these member to CoAP
    // IP address & port of client registered for observe
    OCDevAddr requesterAddr;
    // CoAP token for the observe request
    OCCoAPToken requestToken;
    // The ID of CoAP pdu
    uint16_t coapID;
    uint8_t delayedResNeeded;
    uint8_t secured;
    //////////////////////////////////////////////////////////
    uint8_t reqMorePacket;
    uint32_t reqPacketNum;
    uint16_t reqPacketSize;
    uint32_t resPacketNum;
    uint16_t resPacketSize;
    size_t reqTotalSize;
} OCServerProtocolRequest;

typedef struct
{
    // Observe option field
    uint32_t observationOption;
    // qos is indicating if the request is CON or NON
    OCQualityOfService qos;
    // Allow the entity handler to pass a result with the response
    OCStackResult result;
    // IP address & port of client registered for observe
    OCDevAddr *requesterAddr;
    // CoAP token for the observe request
    OCCoAPToken *requestToken;
    // The ID of CoAP pdu
    uint16_t coapID;
    // Flag indicating that response is to be delayed before sending
    uint8_t delayedResNeeded;
    uint8_t secured;
    uint8_t slowFlag;
    uint8_t notificationFlag;
    // this is the pointer to server payload data to be transferred
    unsigned char *payload;
    // size of server payload data.  Don't rely on null terminated data for size
    uint16_t payloadSize;
    // An array of the vendor specific header options the entity handler wishes to use in response
    uint8_t numSendVendorSpecificHeaderOptions;
    OCHeaderOption *sendVendorSpecificHeaderOptions;
    // URI of new resource that entity handler might create
    unsigned char * resourceUri;
} OCServerProtocolResponse;

// following structure will be created in occoap and passed up the stack on the client side
typedef struct {
    // handle is retrieved by comparing the token-handle pair in the PDU.
    ClientCB * cbNode;
    // This is how long this response is valid for (in seconds).
    uint32_t maxAge;
    // This is the Uri of the resource. (ex. "coap://192.168.1.1/a/led")
    unsigned char * fullUri;
    // This is the relative Uri of the resource. (ex. "/a/led")
    unsigned char * rcvdUri;
    // This is the received payload.
    unsigned char * bufRes;
    // This is the token received OTA.
    OCCoAPToken * rcvdToken;
    // this structure will be passed to client
    OCClientResponse * clientResponse;
} OCResponse;

// following typedef is to represent our Server Instance identification.
typedef uint32_t ServerID;

//-----------------------------------------------------------------------------
// Internal function prototypes
//-----------------------------------------------------------------------------
OCStackResult OCStackFeedBack(OCCoAPToken * token, uint8_t status);
OCStackResult HandleStackRequests(OCServerProtocolRequest * protocolRequest);
OCStackResult HandleStackResponses(OCResponse * response);
#ifdef WITH_PRESENCE
OCStackResult SendPresenceNotification(OCResourceType *resourceType);
#endif // WITH_PRESENCE
int ParseIPv4Address(unsigned char * ipAddrStr, uint8_t * ipAddr, uint16_t * port);

OCStackResult BindResourceInterfaceToResource(OCResource* resource,
                                            const char *resourceInterfaceName);

OCStackResult BindResourceTypeToResource(OCResource* resource,
                                            const char *resourceTypeName);
OCResourceType *findResourceType(OCResourceType * resourceTypeList, const char * resourceTypeName);

// returns the internal representation of the server instance ID.
// Note: This will NOT seed the RNG, so it must be called after the RNG is seeded.
// This is done automatically during the OCInit process (via the call to OCInitCoAP),
// so ensure that this call is done after that.
const ServerID OCGetServerInstanceID(void);

// returns a string representation  the server instance ID.
// The memory is managed internal to this function, so freeing externally will result
// in a compiler error
// Note: This will NOT seed the RNG, so it must be called after the RNG is seeded.
// This is done automatically during the OCInit process (via the call to OCInitCoAP),
// so ensure that this call is done after that.
const char* OCGetServerInstanceIDString(void);
#ifdef WITH_PRESENCE
//TODO: should the following function be public?
OCStackResult OCChangeResourceProperty(OCResourceProperty * inputProperty,
        OCResourceProperty resourceProperties, uint8_t enable);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* OCSTACKINTERNAL_H_ */
