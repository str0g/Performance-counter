#include "perfCounter.h"

PerformanceCounter::PerformanceCounter(int _mode):
    mode(mode_t(_mode)),description(NULL),lenOfDescription(0){
/** Default constructor */
    init();
}
PerformanceCounter::PerformanceCounter(const char* description,int _mode): 
    mode(mode_t(_mode)),description(description),lenOfDescription(strlen(description)){
/** Constructor with capabilities of set description and automatically cout description length */
    init();
}
PerformanceCounter::~PerformanceCounter(){
/** Depends on preprocessor options: 
*   Invoke clearing list of allocated data/ deletes allocated message
*   Prints execution time in time scale, chosen by @parm[in] _mode
*/
    const char *ct;
    if (mode == OFF)
        ct = NULL;
    if (mode == AS_SEC){
        ct = getSecAsStr();
    }
    if (mode == AS_MS){
        ct = getMSAsStr();
    }
    if (mode == AS_NS){
        ct = getNSAsStr();
    }

    if (ct)
        printf("%s\n",ct);

    #ifdef AUTOMM
    clear();     
    #else
    SAFE_DELETE_TAB(ct)
    #endif
}
void PerformanceCounter::init(){
/** Gets start time
*   Converts start time to double
*   Sets default value for "get end time" lock
*/
   clock_gettime(CLOCK_REALTIME,&spc_time_start);
   dtime_start = spc_time_start.tv_nsec;
   bLocked = false;
   setFormatter("Execution of %s took %3.6lf%s","Execution took %3.6lf%s");
}
void PerformanceCounter::lock(){
/** Lock getting new execution time */
    bLocked = true;
}
void PerformanceCounter::unlock(){
/** Unlock getting new execution time */
    bLocked = false;
}
const bool PerformanceCounter::isLocked(){
/** Returns state of get new execution time lock */
    return bLocked;
}

void PerformanceCounter::setFormatter(const char* execof,const char* exec){
/**
* This method allow to change default formatters for execution of and execution string.
* @parm[in] execof formatter for execution of string which is 3 argument %s description, %x.ylf , %s time unit scale
* @parm[in] exec formatter for execution string which is 2 argument %x.ylf, %s time unit scale.
*/
    if(execof){
        formatterExecOf = execof;
        lenOfFormatterExecOf = strlen(formatterExecOf) + sizeOfPrecission(formatterExecOf,2);
        ++lenOfFormatterExecOf;//terminating character
          }
    if(exec){
        formatterExec = exec;
        lenOfFormatterExec = strlen(formatterExec) + sizeOfPrecission(formatterExec,1);
        ++lenOfFormatterExec;//terminating character
    }
}
int PerformanceCounter::stringToInt(const char *ct){
/** Converts from const char * to int using stringstream */
    stringstream ss(ct);
    int i = 0;
    ss >> i;
    return i;
}
int PerformanceCounter::sizeOfPrecission(const char *ct,int looplimit){
/** Counts maximal size of data buffer */
        size_t s = 0;
        size_t d,e;
        int i = 0;
        string str(ct);
        while(i++<looplimit){
            s = str.find('%',++s);
            CHECK_STR_SEARCH(s)
        }
        d = str.find('.',s++);
        CHECK_STR_SEARCH(d)
        e = str.find("lf",d);
        CHECK_STR_SEARCH(e)
    return 1 + stringToInt(str.substr(s,d-s).c_str()) + 
           stringToInt(str.substr(++d,--e-d).c_str());
}
void PerformanceCounter::countExecTime(){
/** Sets execution time variable if getting new execution time is unlocked */
    if (bLocked)
        return;
    clock_gettime(CLOCK_REALTIME,&spc_time_stop);
    dtime_stop = spc_time_stop.tv_nsec - spc_time_start.tv_nsec;
    dtime_stop += double(spc_time_stop.tv_sec - spc_time_start.tv_sec) * SEC;
}
#ifdef AUTOMM
void PerformanceCounter::removeLast(){
/** Removes last element from pointer list @parm list_pch*/
    it_list_pch_t it = --list_pch.end();
    SAFE_DELETE_TAB(*it)
    list_pch.pop_back();
}
void PerformanceCounter::clear(){
/**
* Delete pointers from list @parm list_pch and then invoke list::clear() to remove all elements
*/
    for(it_list_pch_t it = list_pch.begin(); it != list_pch.end(); it++){
        SAFE_DELETE_TAB(*it);
    }
    list_pch.clear();
}
#endif
char *PerformanceCounter::AllocateBuffer(int &buf_size){
/** Allocates buffer of size limited by define size limit @parm BUFLEN
*   If preprocessor option is set pointer will add to the end @parm list_pch
*/
    buf_size > BUFLEN ? buf_size = BUFLEN : buf_size;
    char *p = new char[buf_size];
     #ifdef AUTOMM
    list_pch.push_back(p);
    #endif
    return p;
}

const char *PerformanceCounter::setRetStr(const char*ts,double time){
/**
* Allocates buffer of size formatter + description + reduced length of exec time
*/
    int n = 0;
    char *buf = NULL;

    if (description){
        buf = AllocateBuffer(lenOfFormatterExecOf);
        n = snprintf(buf,lenOfFormatterExecOf,formatterExecOf,description,time,ts);
    }else{
        buf = AllocateBuffer(lenOfFormatterExec);
        n = snprintf(buf,lenOfFormatterExec,formatterExec,time,ts);
    }
    return buf;
}
const char *PerformanceCounter::getSecAsStr(){
/** Returns execution message in seconds */
    return setRetStr("s",getSecAsD());
}
const char *PerformanceCounter::getMSAsStr(){
/** Returns execution message in milliseconds */
   return setRetStr("ms",getMSAsD());
}
const char *PerformanceCounter::getNSAsStr(){
/** Returns execution message in nanoseconds */
   return setRetStr("ns",getNSAsD());
}
const double PerformanceCounter::getSecAsD(){
/** Returns execution time in seconds */
   countExecTime();
    return dtime_stop / SEC;
}
const double PerformanceCounter::getMSAsD(){
/** Returns execution time in milliseconds */
   countExecTime();
    return dtime_stop / MSC;
}
const double PerformanceCounter::getNSAsD(){
/** Returns execution time in nanoseconds */
   countExecTime();
    return dtime_stop;
}


