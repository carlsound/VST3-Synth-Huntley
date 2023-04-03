//
//  Cocoa_UI_Interface.h
//  Cocoa_UI_Interface
//
//  Created by John Carlson on 2/5/23.
//

// https://andreicalazans.medium.com/how-to-interop-between-objective-c-and-c-cd0d7ff0e100

//__attribute__((visibility("default")))

extern "C"
{
    void printToConsoleObjC();
    //
    int createSubview(void* parent);
    int closeSubview();
    int resizeSubviewFunction(void* newSize);
}
