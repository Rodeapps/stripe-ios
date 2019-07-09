//
//  STPPaymentHandler.h
//  StripeiOS
//
//  Created by Cameron Sabol on 5/10/19.
//  Copyright © 2019 Stripe, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class STPAPIClient;
@class STPPaymentIntent;
@class STPPaymentIntentParams;
@class STPSetupIntent;
@class STPSetupIntentConfirmParams;
@class STPThreeDSCustomizationSettings;
@protocol STPAuthenticationContext;

/**
 `STPPaymentHandlerActionStatus` represents the possible outcomes of requesting an action by `STPPaymentHandler`. An action could be confirming and/or handling the next action for a PaymentIntent.
 */
typedef NS_ENUM(NSInteger, STPPaymentHandlerActionStatus) {
    /**
     The action succeeded.
     */
    STPPaymentHandlerActionStatusSucceeded,

    /**
     The action was cancelled by the cardholder/user.
     */
    STPPaymentHandlerActionStatusCanceled,

    /**
     The action failed. See the error code for more details.
     */
    STPPaymentHandlerActionStatusFailed,
};

/**
 The error domain for errors in `STPPaymentHandler`.
 */
FOUNDATION_EXPORT NSString * const STPPaymentHandlerErrorDomain;

/**
 Error codes generated by `STPPaymentHandler`
 */
typedef NS_ENUM(NSInteger, STPPaymentHandlerErrorCode) {
    /**
     Indicates that the action requires an authentication method not recognized or supported by the SDK.
     */
    STPPaymentHandlerUnsupportedAuthenticationErrorCode,

    /**
     Attach a payment method to the PaymentIntent or SetupIntent before using `STPPaymentHandler`.
     */
    STPPaymentHandlerRequiresPaymentMethodErrorCode,

    /**
     The PaymentIntent or SetupIntent status cannot be resolved by `STPPaymentHandler`.
     */
    STPPaymentHandlerIntentStatusErrorCode,

    /**
     The action timed out.
     */
    STPPaymentHandlerTimedOutErrorCode,

    /**
     There was an error in the Stripe3DS2 SDK.
     */
    STPPaymentHandlerStripe3DS2ErrorCode,

    /**
     The transaction did not authenticate (e.g. user entered the wrong code).
     */
    STPPaymentHandlerNotAuthenticatedErrorCode,

    /**
     `STPPaymentHandler` does not support concurrent actions.
     */
    STPPaymentHandlerNoConcurrentActionsErrorCode,

    /**
     Payment requires an `STPAuthenticationContext`.
     */
    STPPaymentHandlerRequiresAuthenticationContextErrorCode,
};


/**
 Completion block typedef for use in `STPPaymentHandler` methods for Payment Intents.
 */
typedef void (^STPPaymentHandlerActionPaymentIntentCompletionBlock)(STPPaymentHandlerActionStatus, STPPaymentIntent * _Nullable, NSError * _Nullable);

/**
 Completion block typedef for use in `STPPaymentHandler` methods for Setup Intents.
 */
typedef void (^STPPaymentHandlerActionSetupIntentCompletionBlock)(STPPaymentHandlerActionStatus, STPSetupIntent * _Nullable, NSError * _Nullable);

/**
 `STPPaymentHandler` is a utility class that can confirm PaymentIntents and handle
 any additional required actions for 3DS(2) authentication. It can present authentication UI on top of your app or redirect users out of your app (to e.g. their banking app).
 */
NS_EXTENSION_UNAVAILABLE("STPPaymentHandler is not available in extensions")
@interface STPPaymentHandler : NSObject

/**
 The globally shared instance of `STPPaymentHandler`.
 */
+ (instancetype)sharedHandler;

/**
 By default `sharedHandler` initializes with [STPAPIClient sharedClient].
 */
@property (nonatomic) STPAPIClient *apiClient;

/**
 Customizable settings to use when performing 3DS2 authentication.
 
 Note: Configure this before calling any methods.
 Defaults to `[STPThreeDSCustomizationSettings defaultSettings]`.
 */
@property (nonatomic) STPThreeDSCustomizationSettings *threeDSCustomizationSettings;

/**
 Confirms the PaymentIntent with the provided parameters and handles any `nextAction` required
 to authenticate the PaymentIntent.
 
 Call this method if you are using automatic confirmation.  @see https://stripe.com/docs/payments/payment-intents/ios
 
 @param paymentParams The params used to confirm the PaymentIntent.
 @param authenticationContext The authentication context used to authenticate the payment.
 @param completion The completion block. If the status returned is `STPPaymentHandlerActionStatusSucceeded`, the PaymentIntent status will always be either STPPaymentIntentStatusSucceeded, STPPaymentIntentStatusRequiresCapture, or STPPaymentIntentStatusRequiresConfirmation. In the latter two cases, capture or confirm the PaymentIntent to complete the payment.
 */
- (void)confirmPayment:(STPPaymentIntentParams *)paymentParams
withAuthenticationContext:(id<STPAuthenticationContext>)authenticationContext
            completion:(STPPaymentHandlerActionPaymentIntentCompletionBlock)completion;

/**
 Handles any `nextAction` required to authenticate the PaymentIntent.
 
 Call this method if you are using manual confirmation.  @see https://stripe.com/docs/payments/payment-intents/ios
 
 @param paymentIntent The PaymentIntent to handle next actions for.
 @param authenticationContext The authentication context used to authenticate the payment.
 @param completion The completion block. If the status returned is `STPPaymentHandlerActionStatusSucceeded`, the PaymentIntent status will always be either STPPaymentIntentStatusSucceeded, STPPaymentIntentStatusRequiresCapture, or STPPaymentIntentStatusRequiresConfirmation. In the latter two cases, capture or confirm the PaymentIntent on your backend to complete the payment.
 */
- (void)handleNextActionForPayment:(STPPaymentIntent *)paymentIntent
         withAuthenticationContext:(id<STPAuthenticationContext>)authenticationContext
                        completion:(STPPaymentHandlerActionPaymentIntentCompletionBlock)completion;

/**
 Confirms the SetupIntent with the provided parameters and handles any `nextAction` required
 to authenticate the SetupIntent.
 
 @param setupIntentConfirmParams The params used to confirm the SetupIntent.
 @param authenticationContext The authentication context used to authenticate the SetupIntent.
 @param completion The completion block. If the status returned is `STPPaymentHandlerActionStatusSucceeded`, the SetupIntent status will always be either STPSetupIntentStatusSucceeded or STPSetupIntentStatusRequiresConfirmation. In the latter case, confirm the Setupintent to complete it.
 */
- (void)confirmSetupIntent:(STPSetupIntentConfirmParams *)setupIntentConfirmParams
 withAuthenticationContext:(id<STPAuthenticationContext>)authenticationContext
                completion:(STPPaymentHandlerActionSetupIntentCompletionBlock)completion;

@end

NS_ASSUME_NONNULL_END
