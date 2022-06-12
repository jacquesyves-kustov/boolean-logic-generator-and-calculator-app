#include <jni.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "logic_tools.h"


extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_logictools_MainActivity_getTruthTable(JNIEnv *env, jobject thiz,
                                                       jstring json_string) {
    const char *cstr = env->GetStringUTFChars(json_string, nullptr);
    std::string str = std::string(cstr);

    return env->NewStringUTF(sendTruthTable(str).c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_logictools_MainActivity_sendRandomFormula(JNIEnv *env, jobject thiz) {
    // 'it' is
    return env->NewStringUTF(getNewFormula().c_str());
}