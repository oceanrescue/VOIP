//
//  VNVoiceCallProxyObjects.h
//  SimpleVoiceMessenger
//
//  Created by Valeriy Nikitin on 4/12/13.
//  Copyright (c) 2013 Valery N. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

#pragma mark - Message
typedef NS_ENUM(NSUInteger, VNXMPPMessageState) {
    VNXMPP_CHAT_NONE        = 0,
    VNXMPP_CHAT_ACTIVE      = 1,
    VNXMPP_CHAT_COMPOSING   = 2,
    VNXMPP_CHAT_PAUSED      = 3,
    VNXMPP_CHAT_INACTIVE    = 4,
    VNXMPP_CHAT_GONE        = 5
};

@class VNJidProxyObject;

@interface VNMessageProxyObjects : NSObject
@property (nonatomic, retain) VNJidProxyObject *jid;
@property (nonatomic) VNXMPPMessageState state;
@property (nonatomic, retain) NSString *body;
@property (nonatomic, retain) NSString *serviceName;
@property (nonatomic) BOOL incoming;
@property (nonatomic, retain) NSString *errorString;
@property (nonatomic, retain) NSString *account;
@property (nonatomic, retain) NSString *chatRoomID;
@end

#pragma mark - Roster
typedef NS_ENUM(NSUInteger, VNJIDStatus) {
    VNJID_SHOW_NONE     = 0,
    VNJID_SHOW_OFFLINE  = 1,
    VNJID_SHOW_XA       = 2,
    VNJID_SHOW_AWAY     = 3,
    VNJID_SHOW_DND      = 4,
    VNJID_SHOW_ONLINE   = 5,
    VNJID_SHOW_CHAT     = 6,
};

typedef NS_ENUM(NSUInteger, VN_SUBSCRIPTION_STATUS) {
        VN_SUBSCRIPTION_STATUS_NONE                 = 0,
        VN_SUBSCRIPTION_STATUS_NONE_PENDING_IN      = 1,
        VN_SUBSCRIPTION_STATUS_NONE_PENDING_OUT     = 2,
        VN_SUBSCRIPTION_STATUS_NONE_PENDING_IN_OUT  = 3,
        VN_SUBSCRIPTION_STATUS_TO                   = 4,
        VN_SUBSCRIPTION_STATUS_TO_PENDING_IN        = 5,
        VN_SUBSCRIPTION_STATUS_FROM                 = 6,
        VN_SUBSCRIPTION_STATUS_FROM_PENDING_OUT     = 7,
        VN_SUBSCRIPTION_STATUS_BOTH                 = 8,
};

@class VNJidProxyObject;

@interface VNRosterProxyObject : NSObject
+(NSString*)statusReadableForStatus:(VNJIDStatus)status;
@property (nonatomic, retain) VNJidProxyObject *jid;
@property (nonatomic) VNJIDStatus status;
@property (nonatomic, readonly) NSString *statusReadable;
@property (nonatomic, retain) NSString *statusString;
@property (nonatomic, retain) NSString *serviceName;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *nick;
@property (nonatomic, retain) NSString *photo_sha1;
@property (nonatomic, retain) NSArray *groups;
@property (nonatomic, retain) NSString *subscription;

@property (nonatomic, assign) VN_SUBSCRIPTION_STATUS subscriptionType;

@property (nonatomic) BOOL feedback_probation;
@property (nonatomic) BOOL know_capabilities;
@property (nonatomic) BOOL non_standard_capabilities;
@property (nonatomic) BOOL voice_capability;
@property (nonatomic) BOOL pmuc_capability;
@property (nonatomic) BOOL video_capability;
@property (nonatomic) BOOL camera_capability;

@property (nonatomic, assign) int error_code;
@property (nonatomic, retain) NSString *error;
@end

#pragma mark - JID
@interface VNJidProxyObject : NSObject {

@private
    __strong NSString *_jid_string;
}
@property (nonatomic, retain) NSString *node_name;
@property (nonatomic, retain) NSString *domain_name;
@property (nonatomic, retain) NSString *resource_name;
@property (nonatomic, retain) NSString *jid_string;
-(void)parseJidString;
-(NSString*)email;
@end


typedef struct {
    BOOL isMicOff;
    BOOL isVideoOutOff;
    BOOL isSpeakerOff;
    BOOL isHolded;
} VNCallOptions;

typedef NS_ENUM(NSUInteger, CallState) {
    STATE_INIT                  = 0,
    STATE_SENTINITIATE          = 1,    // sent initiate, waiting for Accept or Reject
    STATE_RECEIVEDINITIATE      = 2,    // received an initiate. Call Accept or Reject
    STATE_RECEIVEDINITIATE_ACK  = 3,    // received an initiate ack. Client is alive.
    STATE_SENTPRACCEPT          = 4,    // sent provisional Accept
    STATE_SENTACCEPT            = 5,    // sent accept. begin connecting transport
    STATE_RECEIVEDPRACCEPT      = 6,    // received provisional Accept, waiting for Accept
    STATE_RECEIVEDACCEPT        = 7,    // received accept. begin connecting transport
    STATE_SENTMODIFY            = 8,    // sent modify, waiting for Accept or Reject
    STATE_RECEIVEDMODIFY        = 9,    // received modify, call Accept or Reject
    STATE_SENTBUSY              = 10,   // sent busy after receiving initiate
    STATE_SENTREJECT            = 11,   // sent reject after receiving initiate
    STATE_RECEIVEDBUSY          = 12,   // received busy after sending initiate
    STATE_RECEIVEDREJECT        = 13,   // received reject after sending initiate
    STATE_SENTREDIRECT          = 14,   // sent direct after receiving initiate
    STATE_SENTTERMINATE         = 15,   // sent terminate (any time / either side)
    STATE_RECEIVEDTERMINATE     = 16,   // received terminate (any time / either side)
    STATE_INPROGRESS            = 17,   // session accepted and in progress
    STATE_DEINIT                = 18,   // session is being destroyed
    STATE_USER_OFFLINE          = 19,   // User is offline or unavailible
};

typedef NS_ENUM(NSUInteger, VNCallAction) {
    VNCallActionInitiated   = 0,
    VNCallActionBusy        = 1,
    VNCallActionReject      = 2,
    VNCallActionAnswered    = 3
};

/*
 *  //////////////////////////////////////////////////////////////////////////
 */
@class VNConversation;
/*!
 @class VNVoiceCall
 @abstract Simple object that encapsulates the call-related data
 @inherits from NSObject
 @comform to NSObject
 */
@interface VNVoiceCall : NSObject
/*!
 @property jid
 @abstract the Jabber Identifiers (JID)
 @discussion see http://xmpp.org/extensions/xep-0029.html for more information
 */
@property (nonatomic, retain) VNJidProxyObject *jid;
/*!
 @property reason
 @abstract a reason or state in human-readable format
 */
@property (nonatomic, copy) NSString *reason;
/*!
 @property state
 @abstract current state of Call. See possible values in CallState enum
 */
@property (nonatomic) CallState state;
/*!
 @property callID
 @abstract call identifier
 */
@property (nonatomic, retain) NSNumber *callID;
@property (nonatomic, retain) NSString *callTrackerID;
@property (nonatomic, retain) NSManagedObjectID *eventID;
@property (nonatomic, retain) VNConversation *event;
@property (nonatomic, retain, readonly) NSString *readableState;
@property (nonatomic, retain) NSString *serviceName;
@property (nonatomic) BOOL isIncoming;
@property (nonatomic, assign) BOOL isVideoCall;
@property (nonatomic, readonly) BOOL alive;
@property (nonatomic, readonly) BOOL isConnected;
@property (nonatomic, assign) VNCallOptions options;
@property (nonatomic, assign) VNCallAction userAction;
@property (nonatomic, retain) NSDate *startTime;
@property (nonatomic, retain) NSString *account;
+(NSString*)readableState:(CallState)state;
@end

