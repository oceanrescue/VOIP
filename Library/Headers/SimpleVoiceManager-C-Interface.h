//
//  SimpleVoiceManager-C-Interface.h
//  SimpleVoiceMessenger
//
//  Created by Valeriy Nikitin on 3/28/13.
//  Copyright (c) 2013 Valery N. All rights reserved.
//

#ifndef SimpleVoiceMessenger_SimpleVoiceManager_C_Interface_h
#define SimpleVoiceMessenger_SimpleVoiceManager_C_Interface_h

void VNSignalCallStateChange(int state, const char *stateDescription, const char *remote_jid, int call_id);
void VNSignalCallTrackingId(int call_id, const char *call_tracker_id);
void VNSignalAudioPlayout();
void VNSignalCallError(int error, const char *errorDescription, int call_id);
void VNSignalXmppError(int error, const char *errorDescription);
void VNSignalXmppSocketClose(int state, const char *stateDescription);
void VNSignalXmppStateChange(int state);
void VNSignalXmppMessage(const char *from, const char *body);
void VNSignalBuddyListReset();
void VNSignalBuddyListRemove(const char *remote_jid);
void VNSignalBuddyListAdd(const char *remote_jid, const char *nick);
void VNSignalStatsUpdate(const char *stats);


#endif
