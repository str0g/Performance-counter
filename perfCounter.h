#ifndef _perfCounter_h_included
#define _perfCounter_h_included
//$Id$
/*
* @file perfCounter.h
* @brief bla bla
* @author Łukasz Buśko
* License GPL v2 
*/
//$Log$

#include <ctime>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
using std::string;
using std::stringstream;
#include <exception>
#ifdef AUTOMM
#include <list>
#endif

#define SEC 1000000000L/**< Nanosecond 10^9*/
#define MSC 1000000L /**< Millisecond 10^6*/
#define BUFLEN 127 /**< Max length of text buffer*/
/**
* Macro for safe table deletion
* if pointer is not null delete it then set pointer to null
*/
#define SAFE_DELETE_TAB(x)\
    if(x){\
        delete[] x;\
        x = NULL;\
    }
#define CHECK_STR_SEARCH(x)\
    if(x==size_t(-1))\
        throw myex("Unsupported string formatting");

typedef enum {AS_SEC,AS_MS,AS_NS,OFF} mode_t; /**< Enumerator for work options */
#ifdef AUTOMM
typedef std::list<char*> list_pch_t; /**< Type of <const char*> list */
typedef std::list<char*>::iterator it_list_pch_t; /**< Type of <const char*> list interator */
#endif

class myexception: public exception{
    private:
        const char *msg;
    public:
        myexception(const char *_msg):msg(_msg);
        virtual const char* what() const throw(){
            return msg;
        }
} myex;

/**
* Class get time at constructor.
* Message can have custom description and formatter.
* get stop time can be invoked manually or by destructor.
* Time can be omitted as seconds,milliseconds, nanoseconds.
* @parm[in] _mode ,option if time will be printed in destructor as s,ms,n or not printed.
* @parm[in] description, custom message for time trace.
*/

class PerformanceCounter{
    public:
       //
        PerformanceCounter(int _mode =0);
        PerformanceCounter(const char* description,int _mode = 0);
        ~PerformanceCounter();
        void init();
        void lock();
        void unlock();
        const bool isLocked();
        void setFormatter(const char *execof,const char *exec);
        #ifdef AUTOMM
        void removeLast();
        void clear();
        #endif
        const char *getSecAsStr();
        const char *getMSAsStr();
        const char *getNSAsStr();
        const double getSecAsD();
        const double getMSAsD();
        const double getNSAsD();
    private:
        timespec spc_time_start, spc_time_stop; /**< Timer start and stop structure */
        mode_t mode; /**< Enumerator for time report in desctructor */
        const char *description; /**< Execution of $(description) took... */
        double dtime_start, dtime_stop; /**< Timer as double for math work */
        int lenOfDescription; /**< Length of description for faster buffer size recounting */
        bool bLocked; /**< Lock/unlock getting new end time */
        const char *formatterExecOf;
        const char *formatterExec;
        int lenOfFormatterExecOf;
        int lenOfFormatterExec;
        #ifdef AUTOMM
        list_pch_t list_pch; /**< List of objects registered by memory management mechanism */
        #endif
        //
        const char *setRetStr(const char*,double);
        char *AllocateBuffer(int &);
        void countExecTime();
        int stringToInt(const char*);
        int sizeOfPrecission(const char*,int);
};  
#endif
