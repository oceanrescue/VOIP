//
//  VNViewController.h
//  SimpleVoiceMessanger
//
//  Created by Valeriy Nikitin on 3/27/13.
//  Copyright (c) 2013 Valery N. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SimpleVoiceManager.h"

@interface VNViewController : UIViewController <UITextFieldDelegate, UIAlertViewDelegate, UIActionSheetDelegate, SimpleVoiceManagerDelegate>

@property (assign, nonatomic) IBOutlet UITextField *login;
@property (assign, nonatomic) IBOutlet UITextField *password;
@property (assign, nonatomic) IBOutlet UIButton *loginButton;
- (IBAction)loginButtonClicked:(id)sender;

@property (assign, nonatomic) IBOutlet UITextField *message;

@property (assign, nonatomic) IBOutlet UIButton *call2Button;
- (IBAction)call2ButtonClicked:(id)sender;
@property (assign, nonatomic) IBOutlet UITextField *call2;

@property (assign, nonatomic) IBOutlet UILabel *callFromLabel;
- (IBAction)answerButtonClicked:(id)sender;
//- (IBAction)holdButtonClicked:(id)sender;
- (IBAction)declineButtonClicked:(id)sender;

@end
