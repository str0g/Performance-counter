#include <iostream>
#include <cstring>
#include "perfCounter.h"
using namespace std;

#define FAST_CT(x)\
    {\
    const char* ct = x;\
    cout<<ct<<endl;\
    SAFE_DELETE_TAB(ct)\
    }

void testNS(){
    PerformanceCounter pfc;

    for(int i=0; i < 1000; i++);
    
    const double dt = pfc.getNSAsD();
    pfc.lock();
    #ifndef AUTOMM
    FAST_CT(pfc.getNSAsStr())
    FAST_CT(pfc.getMSAsStr())
    FAST_CT(pfc.getSecAsStr())
    #else
    cout<<pfc.getNSAsStr()<<endl;
    cout<<pfc.getMSAsStr()<<endl;
    cout<<pfc.getSecAsStr()<<endl;
    #endif
    if(pfc.isLocked())
        cout<<"Lock: PASS"<<endl;
    else
        cout<<"Lock: FAIL"<<endl;

    if (dt==pfc.getNSAsD())
        cout<<"Get new time locked: PASS"<<endl;
    else
        cout<<"Get new time locked: FAIL"<<endl;
  
    if(pfc.isLocked())
        pfc.unlock();
        
    if(!pfc.isLocked())
        cout<<"Unlock: PASS"<<endl;
    else
        cout<<"Unlock: FAIL"<<endl;
        
    for(int i=0; i < 100000; i++);
    cout<<pfc.getNSAsD()<<endl;
    if (dt<pfc.getNSAsD())
        cout<<"Get new time unlocked: PASS"<<endl;
    else
        cout<<"Get new time unlocked: FAIL"<<endl;
    const char *pt = pfc.getNSAsStr();
    #ifdef AUTOMM
    pfc.removeLast();
    #else
    SAFE_DELETE_TAB(pt)
    #endif
}


int main(int argc,char **argv){
    PerformanceCounter pfc("App");
    testNS();
    
    return 0;
}
