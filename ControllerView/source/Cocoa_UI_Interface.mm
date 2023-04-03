//
//  Cocoa_UI_Interface.m
//  Cocoa_UI_Interface
//
//  Created by John Carlson on 2/5/23.
//

#import "Cocoa_UI_Interface.h"
//
#include <stdio.h>
//
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <WebKit/WebKit.h>
//
////
@interface Cocoa_UI_Interface : NSViewController <WKUIDelegate> //NSObject
{
    
}

@property (strong) WKWebView* m_childWebView;
@property (strong) WKWebViewConfiguration* m_childWebViewConfig;

@property (nonatomic, weak) NSView* m_parentView;

@end

///
///
///

@implementation Cocoa_UI_Interface

@synthesize m_childWebView;
@synthesize m_childWebViewConfig;

-(void) printToConsole
{
    NSLog(@"\n\nHello world, from inside a .mm Objective-C file within a .dylib!\n\n");
}

-(int) initChildView:(void*) parent
{
    self.m_parentView = (NSView*)parent;
    //
    self.m_childWebViewConfig = [[WKWebViewConfiguration alloc] init];
    //
    self.m_childWebView = [[WKWebView alloc] init];
    self.m_childWebView = [self.m_childWebView initWithFrame:self.m_parentView.frame configuration:self.m_childWebViewConfig];
    //
    NSBundle *myBundle = [NSBundle bundleForClass:[Cocoa_UI_Interface class]];
    NSURL* address;
    address = [NSURL alloc];
    address = [myBundle URLForResource:@"WKWebView_Test_UI"
                         withExtension:@"html"
                        //localization:@"en"
          ];
    //
    self.m_childWebView.UIDelegate = self;
    //
    NSURLRequest* urlRequest;
    urlRequest = [[NSURLRequest alloc] init];
    urlRequest = [NSURLRequest requestWithURL:address];
    //
    [self.m_childWebView loadRequest:urlRequest];
    //
    /*
    NSURL * baseURL = [address URLByDeletingLastPathComponent];
    [self.m_childWebView loadFileURL:address
             allowingReadAccessToURL:baseURL];
    */
    [self.m_parentView addSubview:self.m_childWebView];
    
    return 1;
}


@end

////
///
Cocoa_UI_Interface* myInstance;
//
//

void printToConsoleObjC()
{
    if(myInstance == NULL)
    {
        myInstance = [[Cocoa_UI_Interface alloc] init];
    }
    [myInstance printToConsole];
}

//

int createSubview(void* parent)
{
    if(myInstance == NULL)
    {
        myInstance = [[Cocoa_UI_Interface alloc] init];
    }
    int i = [myInstance initChildView:parent];
    //
    return i;
}

//

int closeSubview()
{
    return 1;
}

int resizeSubviewFunction(void* newSize)
{
    return 1;
}
