//
//  SimpleVoiceManager.h
//  SimpleVoiceManager
//
//  Created by Valeriy Nikitin on 3/28/13.
//  Copyright (c) 2013 Valery N. All rights reserved.
//

//#import <Foundation/Foundation.h>
#import "SimpleVoiceManager-C-Interface.h"



#define kXmppHost "talk.google.com"
#define kXmppPort 5222
#define kSTUN "stun.l.google.com:19302"

#define SimpleVoiceManagerErrorDomain @"SimpleVoiceManagerErrorDomain"

typedef NS_ENUM(NSUInteger, StateType) {
    STATE_NONE      = 0,        //!< Nonexistent state
    STATE_START     = 1,        //!< Initial state.
    STATE_OPENING   = 2,        //!< Exchanging stream headers, authenticating and so on.
    STATE_OPEN      = 3,        //!< Authenticated and bound.
    STATE_CLOSED    = 4,        //!< Session closed, possibly due to error.
};


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


@protocol SimpleVoiceManagerDelegate;

/*!
 @class SimpleVoiceManager
 @abstract A singleton object that manages one-2-one messaging and voice calls
 @inherits from NSObject
 @comform to NSObject
 */
@interface SimpleVoiceManager : NSObject {
    __weak id <SimpleVoiceManagerDelegate> _delegate;
    @private
    StateType _managerState;
}
/*!
 @property id <SimpleVoiceManagerDelegate> delegate
 @abstract A reciever's delegate that conforms SimpleVoiceManagerDelegate protocol
 @discussion A SimpleVoiceManager sends messages to its delegate regarding the changes of it's state, message and voice call signals.
 */
@property (nonatomic, weak) id <SimpleVoiceManagerDelegate> delegate;
/*!
 @property state
 @abstract the State of voice manager
 @discussion state is usefull to determine the state of manager. (See StateType enum)
 */
@property (nonatomic) StateType state;
/*!
 @method (SimpleVoiceManager*)sharedInstance
 @abstract create and return the singleton instance of class
 */
+(SimpleVoiceManager*)sharedInstance;
/*!
 @method destroy
 @abstract destroy singleton
 */
+(void)destroy;

/*
 *  Primary Methods
 */
/*!
 @method loginWithEmail:andPassword:
 @abstract create Session with particular user's credentials
 @param (NSString*)email - User's email. Must conform to 'userName@gmail.com' format.
 @param (NSString*)password - User's password
 @discussion loginWithEmail:andPassword: Initialize the Manager and create Session
 */
-(void)loginWithEmail:(NSString*)email andPassword:(NSString*)password;
/*!
 @method logout
 @abstract Destroy current user session
 @discussion You have to logout before login with new user.
 */
-(void)logout;
/*!
 @method sendMessage:toUserWithEmail:
 @abstract send text message to Buddy with particular emal
 @param (NSString*)aMessage - Message to Buddy. Lenght must be >=1
 @param (NSString*)email - Buddy's email. Must conform to 'userName@gmail.com' format.
 */
-(void)sendMessage:(NSString*)aMessage toUserWithEmail:(NSString*)email;
/*!
 @method voiceCall2UserWithEmail:
 @abstract Initiate voice session to another user.
 @param (NSString*)email - Buddy's email. Must conform to 'userName@gmail.com' format.
 @discussion voiceCall2UserWithEmail - initiate voice session. Session details would be send as updates with methods of SimpleVoiceManagerDelegate protocol. It is very important to store particular call_ID that will send to delegate with callbacks. All actions with Call operate throw call_ID
 */
-(void)voiceCall2UserWithEmail:(NSString*)email;
/*!
 @method acceptCallWithID:
 @abstract Accept incoming Call
 @param (int)callID - ID for particular Call.
 @discussion acceptCallWithID: uses call_ID to determine which call should be answered (for future multi-calls).
 */
-(void)acceptCallWithID:(int)callID;
/*!
 @method declineCallWithID:
 @abstract Decline incoming Call
 @param (int)callID - ID for particular Call.
 @discussion acceptCallWithID: uses call_ID to determine which incoming call should be declined (for future multi-calls).
 */
-(void)declineCallWithID:(int)callID;
/*!
 @method endCallWithID:
 @abstract End current Call
 @param (int)callID - ID for particular Call.
 @discussion endCallWithID: uses call_ID to determine which call should be ended (for future multi-calls). It doesn't matter - incoming or outcoming call should be managed
 */
-(void)endCallWithID:(int)callID;

@end

/*
 *  //////////////////////////////////////////////////////////////////////////
 */
@class VNVoiceCall;
/*!
 @protocol SimpleVoiceManagerDelegate
 @abstract Voice Manager delegate protocol
 @comform to <NSObject>
 @discussion The SimpleVoiceManagerDelegate protocol defines a set of required methods that you can use to receive voice and message updates, states of Call.
 */
@protocol SimpleVoiceManagerDelegate <NSObject>
/*!
 @method voiceManager:recievedMessage:fromUser:
 @abstract Tells the delegate that Manager receives the message from remote user
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param (NSString*)aMessage - a string object with the message
 @param (NSString*)aUser - aUser is a string object with JID format. It's not equal to user's email.
 @discussion You can use this method to show alert dialog or update list of messages
 */
-(void)voiceManager:(SimpleVoiceManager*)aManager recievedMessage:(NSString*)aMessage fromUser:(NSString*)aUser;
/*!
 @method voiceManager:recievedVoiceCall:
 @abstract Tells the delegate that Voice Manager receives the request for Voice call from remote user.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param recievedVoiceCall:(VNVoiceCall*)aCall - a VNVoiceCall instance that incapsulate call-relative data
 @discussion You could use this method to determine call_ID and choose a proper action (accept/decline, etc.)
 */
-(void)voiceManager:(SimpleVoiceManager*)aManager recievedVoiceCall:(VNVoiceCall*)aCall;
/*!
 @method voiceManager:voiceCallStateDidChanged:
 @abstract Tells the delegate that the state of Call was changed.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param recievedVoiceCall:(VNVoiceCall*)aCall - a VNVoiceCall instance that incapsulate call-relative data
 @discussion You could use this method to determine what happens
 */
-(void)voiceManager:(SimpleVoiceManager *)aManager voiceCallStateDidChanged:(VNVoiceCall*)aCall;
/*!
 @method voiceManager:didFinishWithError:
 @abstract Tells the delegate that the Voice Manager did finish with error.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param (NSError*)anError - an NSError instance
 @discussion You could use this method to why did error occurred
 */
-(void)voiceManager:(SimpleVoiceManager*)aManager didFinishWithError:(NSError*)anError;
/*!
 @method voiceManager:didChangeState:
 @abstract Informs the delegate that state of Voice Manager was changed.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param (StateType)aState - see StateType enum for particular state
 @discussion You could use this method to manage logic with state of Voice Manager
 */
-(void)voiceManager:(SimpleVoiceManager*)aManager didChangeState:(StateType)aState;

@end



/*
 *  //////////////////////////////////////////////////////////////////////////
 */

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
@property (nonatomic, copy) NSString *jid;
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
@property (nonatomic) int callID;
@end