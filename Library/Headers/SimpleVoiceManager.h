//
//  SimpleVoiceManager.h
//  SimpleVoiceManager
//
//  Created by Valeriy Nikitin on 3/28/13.
//  Copyright (c) 2013 Valery N. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kXmppHost "talk.google.com"
#define kXmppPort 5222
#define kSTUN "stun.l.google.com:19302"

#define SimpleVoiceManagerErrorDomain       @"SimpleVoiceManagerErrorDomain"
#define SimpleVoiceManagerXMPPErrorDomain   @"SimpleVoiceManagerXMPPErrorDomain"
#define SimpleVoiceManagerCallErrorDomain   @"SimpleVoiceManagerCallErrorDomain"

typedef NS_ENUM(NSUInteger, StateType) {
    STATE_NONE      = 0,        //!< Nonexistent state
    STATE_START     = 1,        //!< Initial state.
    STATE_OPENING   = 2,        //!< Exchanging stream headers, authenticating and so on.
    STATE_OPEN      = 3,        //!< Authenticated and bound.
    STATE_CLOSED    = 4,        //!< Session closed, possibly due to error.
};

typedef NS_ENUM(NSUInteger, VNXMPPErrors) {
    VNXMPP_ERROR_NONE = 0,          //!< No error
    VNXMPP_ERROR_XML,               //!< Malformed XML or encoding error
    VNXMPP_ERROR_STREAM,            //!< XMPP stream error - see GetStreamError()
    VNXMPP_ERROR_VERSION,           //!< XMPP version error
    VNXMPP_ERROR_UNAUTHORIZED,      //!< User is not authorized (rejected credentials)
    VNXMPP_ERROR_TLS,               //!< TLS could not be negotiated
    VNXMPP_ERROR_AUTH,              //!< Authentication could not be negotiated
    VNXMPP_ERROR_BIND,              //!< Resource or session binding could not be negotiated
    VNXMPP_ERROR_CONNECTION_CLOSED, //!< Connection closed by output handler.
    VNXMPP_ERROR_DOCUMENT_CLOSED,   //!< Closed by </stream:stream>
    VNXMPP_ERROR_SOCKET,            //!< Socket error
    VNXMPP_ERROR_NETWORK_TIMEOUT,   //!< Some sort of timeout (eg., we never got the roster)
    VNXMPP_ERROR_MISSING_USERNAME,   //!< User has a Google Account but no nickname
    VNXMPP_ERROR_MISSING_Credential //!< User has a Google Account but no nickname
};

//@class SimpleVoiceManager;
@protocol SimpleVoiceManagerDelegate;

/*!
 @protocol SimpleVoiceManager
 @abstract A singleton object that manages one-2-one messaging and voice calls
 @inherits from NSObject
 @comform to NSObject
 */

@protocol SimpleVoiceManagerProtocol <NSObject>
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
+(id <SimpleVoiceManagerProtocol>)sharedInstance;
/*!
 @method destroy
 @abstract destroy singleton
 */
+(void)destroy;
/*!
 @method serviceName
 @abstract return human readable service name,ex. "Google Talk" (localized)
 */
//+(NSString*)serviceName;

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
//-(void)declineCallWithID:(int)callID busy:(BOOL)flag;
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


@class VNVoiceCall, VNMessageProxyObjects;
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
-(void)voiceManager:(id <SimpleVoiceManagerProtocol>)aManager receivedMessage:(VNMessageProxyObjects*)aMessage;
/*!
 @method voiceManager:recievedVoiceCall:
 @abstract Tells the delegate that Voice Manager receives the request for Voice call from remote user.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param recievedVoiceCall:(VNVoiceCall*)aCall - a VNVoiceCall instance that incapsulate call-relative data
 @discussion You could use this method to determine call_ID and choose a proper action (accept/decline, etc.). In some cases (signal schema could change) you should check the state of call in voiceManager:voiceCallStateDidChanged:
 */
-(void)voiceManager:(id <SimpleVoiceManagerProtocol>)aManager receivedVoiceCall:(VNVoiceCall*)aCall;
/*!
 @method voiceManager:voiceCallStateDidChanged:
 @abstract Tells the delegate that the state of Call was changed.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param recievedVoiceCall:(VNVoiceCall*)aCall - a VNVoiceCall instance that incapsulate call-relative data
 @discussion You could use this method to determine what happens
 */
-(void)voiceManager:(id <SimpleVoiceManagerProtocol>)aManager voiceCallStateDidChanged:(VNVoiceCall*)aCall;
/*!
 @method voiceManager:didFinishWithError:
 @abstract Tells the delegate that the Voice Manager did finish with error.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param (NSError*)anError - an NSError instance
 @discussion You could use this method to why did error occurred
 */
-(void)voiceManager:(id <SimpleVoiceManagerProtocol>)aManager didFinishWithError:(NSError*)anError;
/*!
 @method voiceManager:didChangeState:
 @abstract Informs the delegate that state of Voice Manager was changed.
 @param (SimpleVoiceManager*)aManager - a Voice Manager instance
 @param (StateType)aState - see StateType enum for particular state
 @discussion You could use this method to manage logic with state of Voice Manager
 */
-(void)voiceManager:(id <SimpleVoiceManagerProtocol>)aManager didChangeState:(StateType)aState;

@end


/*
 *  Google Talk Manager
 */
@interface GTalkManager : NSObject <SimpleVoiceManagerProtocol> {
    __weak id <SimpleVoiceManagerDelegate> _delegate;
@private
    StateType _managerState;
    NSMutableDictionary *_call2Track;
}
+(GTalkManager*)sharedInstance;


@end
