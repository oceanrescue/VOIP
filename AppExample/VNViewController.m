//
//  VNViewController.m
//  SimpleVoiceMessanger
//
//  Created by Valeriy Nikitin on 3/27/13.
//  Copyright (c) 2013 Valery N. All rights reserved.
//

#import "VNViewController.h"
#import "SimpleVoiceManager.h"

@interface VNViewController () {
    StateType _state;
    int _callID;
    BOOL _calling;
}
@property (nonatomic, weak) SimpleVoiceManager *voiceManager;
@property (nonatomic, retain) UIActionSheet *actions;
           
@end

@implementation VNViewController
@synthesize voiceManager = _voiceManager;

#pragma mark - View lifecycle
- (void)viewDidLoad
{
    [super viewDidLoad];
    
	// Do any additional setup after loading the view, typically from a nib.
    _state = STATE_NONE;
    _callID = 0;
    _calling = FALSE;
    
    self.callFromLabel.numberOfLines = 2;
    self.callFromLabel.font = [UIFont systemFontOfSize:10.];
    self.voiceManager = [SimpleVoiceManager sharedInstance];
    self.voiceManager.delegate = self;
    self.password.secureTextEntry = YES;
    self.call2Button.enabled = FALSE;
    self.call2Button.alpha = 0.65;
    self.message.enabled = FALSE;
    self.message.alpha = 0.65;
    self.login.text = @"example@gmail.com";
    self.password.text = @"password";
    self.call2.text = @"otherUser@gmail.com";
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - IBActions
- (IBAction)loginButtonClicked:(id)sender {
    if (_state != STATE_OPEN) {
        [self.voiceManager loginWithEmail:self.login.text andPassword:self.password.text];
    } else {
        [self.voiceManager logout];
    }
}

- (IBAction)call2ButtonClicked:(id)sender {
    if (_calling) {
        [self endCallButtonClicked:sender];
        return;
    }
    NSString *caller_ID = self.call2.text;
    _calling = TRUE;
    [self.voiceManager voiceCall2UserWithEmail:caller_ID];
}
- (IBAction)answerButtonClicked:(id)sender {
    [self.voiceManager acceptCallWithID:_callID];
    [self.call2Button setTitle:@"Cancel" forState:UIControlStateNormal];
    _calling = YES;
}
//- (IBAction)holdButtonClicked:(id)sender {
//    [self.voiceManager holdCallWithID:_callID];
//}
- (IBAction)declineButtonClicked:(id)sender {
    [self.voiceManager declineCallWithID:_callID];
}
-(IBAction)endCallButtonClicked:(id)sender {
    if (_callID != 0) {
        [self.voiceManager endCallWithID:_callID];
        _callID = 0;
        _calling = FALSE;
        [self.call2Button setTitle:@"Call TO" forState:UIControlStateNormal];        
    }
}

#pragma mark - VoiceManager Delegates
-(void)voiceManager:(SimpleVoiceManager *)aManager didFinishWithError:(NSError *)anError {
    NSLog(@"Error : %@", anError);
}
-(void)voiceManager:(SimpleVoiceManager *)aManager didChangeState:(StateType)aState {
    NSLog(@"\nNew state : %i\n", aState);
    _state = aState;
    if (aState == STATE_OPEN) {
        [self.loginButton setTitle:@"Logout" forState:UIControlStateNormal];
        self.call2Button.enabled = TRUE;
        self.call2Button.alpha = 1.0;
        self.message.enabled = TRUE;
        self.message.alpha = 1.;
        
    } else {
        [self.loginButton setTitle:@"Login" forState:UIControlStateNormal];
        self.call2Button.enabled = FALSE;
        self.call2Button.alpha = 0.65;
        self.message.enabled = FALSE;
        self.message.alpha = 0.65;
    }
}
-(void)voiceManager:(SimpleVoiceManager *)aManager recievedMessage:(NSString *)aMessage fromUser:(NSString *)aUser {
    self.message.text = [NSString stringWithFormat:@"%@ : %@", aUser, aMessage];
}
-(void)voiceManager:(SimpleVoiceManager*)aManager recievedVoiceCall:(VNVoiceCall *)aCall {
    NSString *message = [NSString stringWithFormat:@"Voice Call from <%@>", aCall.jid];
    self.callFromLabel.text = message;
    _callID = aCall.callID;
    if (_actions == nil) {
        UIActionSheet *sheet = [[UIActionSheet alloc] initWithTitle:message delegate:self cancelButtonTitle:nil destructiveButtonTitle:nil otherButtonTitles:@"Answer", @"Decline", nil];
        sheet.destructiveButtonIndex = 2;
        [sheet showInView:self.view];
        self.actions = sheet;
    }
}

-(void)voiceManager:(SimpleVoiceManager *)aManager voiceCallStateDidChanged:(VNVoiceCall *)aCall {
    CallState state = aCall.state;
    if (_callID == 0) {
        _callID = aCall.callID;
    }
    if (state == STATE_USER_OFFLINE
        || state == STATE_SENTTERMINATE
        || state == STATE_SENTREJECT
        || state == STATE_DEINIT
        || state == STATE_RECEIVEDBUSY
        || state == STATE_RECEIVEDREJECT
        || state == STATE_RECEIVEDTERMINATE) {
        self.callFromLabel.text = aCall.reason;
        _callID = 0;
        _calling = FALSE;
        if (_actions != nil) {
            [self.actions dismissWithClickedButtonIndex:2 animated:YES];
            self.actions = nil;
        }
        [self.call2Button setTitle:@"Call TO" forState:UIControlStateNormal];        
    }
    else if (state == STATE_RECEIVEDINITIATE)
    {
        self.callFromLabel.text = [NSString stringWithFormat:@"Voice call from <%@>", aCall.jid];
        [self.call2Button setTitle:@"Call TO" forState:UIControlStateNormal];
    }
    else if (state == STATE_RECEIVEDINITIATE_ACK)
    {
        self.callFromLabel.text = [NSString stringWithFormat:@"Voice call to <%@>", aCall.jid];
        [self.call2Button setTitle:@"Cancel" forState:UIControlStateNormal];
    }
    else
    {
        self.callFromLabel.text = [NSString stringWithFormat:@"CallID: %i, state : <%@>", aCall.callID, aCall.reason];
    }
    
}

#pragma mark - TextField Delegates
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    if (textField == self.message) {
        
        NSString *message = self.message.text;
        [self.voiceManager sendMessage:message toUserWithEmail:self.call2.text];
        [self.message resignFirstResponder];
    }
    return YES;
}
-(BOOL)textFieldShouldReturn:(UITextField *)textField {
    if (textField == self.message) {
        
        NSString *message = self.message.text;
        [self.voiceManager sendMessage:message toUserWithEmail:self.call2.text];
        [self.message resignFirstResponder];
    }
    return YES;
}
-(BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    if (textField == self.message) {
        self.message.text = @"";
    }
    return YES;
}

#pragma mark - ActionSheet
- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
    switch (buttonIndex) {
        case 0:
            [self answerButtonClicked:nil];
            break;
        case 1:
//            [self holdButtonClicked:nil];
//            break;
//        case 2:
            [self declineButtonClicked:nil];
        default:
            break;
    }
}

- (void)actionSheetCancel:(UIActionSheet *)actionSheet {
    [self declineButtonClicked:nil];
}

@end
