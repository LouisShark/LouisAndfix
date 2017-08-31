#include <jni.h>
#include "dalvik.h"
//#include "art_7.0.h"
#include "art_6.0.h"
#include <string>
#include <dlfcn.h>

typedef Object *(*FindObject)(void *thread, jobject jobject1);

typedef void *(*FindThread)();

FindObject findObject;
FindThread findThread;
extern "C" {


JNIEXPORT void JNICALL
Java_ilouis_me_andfix_DexManager_replace6(JNIEnv *env, jobject instance, jobject wrongMethod,
                                          jobject rightMethod) {
    art::mirror::ArtMethod* wrong = (art::mirror::ArtMethod *) env -> FromReflectedMethod(wrongMethod);
    art::mirror::ArtMethod* right = (art::mirror::ArtMethod *) env -> FromReflectedMethod(rightMethod);

    wrong -> declaring_class_ = right -> declaring_class_;
    wrong -> dex_cache_resolved_methods_ = right -> dex_cache_resolved_methods_;
    wrong -> dex_cache_resolved_types_ = right -> dex_cache_resolved_types_;
    wrong -> access_flags_ = right -> access_flags_ | 0x0001;
    wrong -> dex_code_item_offset_ = right -> dex_code_item_offset_;
    wrong -> dex_method_index_ = right -> dex_method_index_;
    wrong -> method_index_ = right -> method_index_;

    wrong -> ptr_sized_fields_.entry_point_from_interpreter_ = right -> ptr_sized_fields_.entry_point_from_interpreter_;
    wrong -> ptr_sized_fields_.entry_point_from_jni_ = right -> ptr_sized_fields_.entry_point_from_jni_;
    wrong -> ptr_sized_fields_.entry_point_from_quick_compiled_code_ = right -> ptr_sized_fields_.entry_point_from_quick_compiled_code_;

}

/**
 * 不同的版本ArtMethod的数据结构也不一样，所以要对各个数据结构进行适配
 */
//JNIEXPORT void JNICALL
//Java_ilouis_me_andfix_DexManager_replace7(JNIEnv *env, jobject instance, jobject wrongMethod,
//                                          jobject rightMethod) {
//    art::mirror::ArtMethod* wrong = (art::mirror::ArtMethod *) env -> FromReflectedMethod(wrongMethod);
//    art::mirror::ArtMethod* right = (art::mirror::ArtMethod *) env -> FromReflectedMethod(rightMethod);
//
//    wrong -> declaring_class_ = right -> declaring_class_;
//    wrong -> hotness_count_ = right -> hotness_count_;
//    wrong -> access_flags_ = right -> access_flags_ | 0x0001;
//    wrong -> dex_code_item_offset_ = right -> dex_code_item_offset_;
//    wrong -> dex_method_index_ = right -> dex_method_index_;
//    wrong -> method_index_ = right -> method_index_;
//
//
//    wrong -> ptr_sized_fields_.entry_point_from_jni_ = right -> ptr_sized_fields_.entry_point_from_jni_;
//    wrong -> ptr_sized_fields_.entry_point_from_quick_compiled_code_ = right -> ptr_sized_fields_.entry_point_from_quick_compiled_code_;
//
//    wrong -> ptr_sized_fields_.dex_cache_resolved_methods_ = right ->ptr_sized_fields_.dex_cache_resolved_methods_;
//    wrong -> ptr_sized_fields_.dex_cache_resolved_types_ = right -> ptr_sized_fields_.dex_cache_resolved_types_;
//}


JNIEXPORT void JNICALL
Java_ilouis_me_andfix_DexManager_replace(JNIEnv *env, jobject instance, jint sdk,
                                         jobject wrongMethod,
                                         jobject rightMethod) {
    //找到虚拟机对应的method结构体
    Method *wrong = (Method *) env->FromReflectedMethod(wrongMethod);
    Method *right = (Method *) env->FromReflectedMethod(rightMethod);

    //下一步  把right 对应Object   第一个成员变量ClassObject   status


//    ClassObject
    void *dvm_hand = dlopen("libdvm.so", RTLD_NOW);
//    sdk  10    以前是这样   10会发生变化
    findObject = (FindObject) dlsym(dvm_hand, sdk > 10 ?
                                              "_Z20dvmDecodeIndirectRefP6ThreadP8_jobject" :
                                              "dvmDecodeIndirectRef");
    findThread = (FindThread) dlsym(dvm_hand, sdk > 10 ? "_Z13dvmThreadSelfv" : "dvmThreadSelf");
// method   所声明的Class

    jclass methodClaz = env->FindClass("java/lang/reflect/Method");
    jmethodID rightMethodId = env->GetMethodID(methodClaz, "getDeclaringClass",
                                               "()Ljava/lang/Class;");
//
    jobject ndkObject = env->CallObjectMethod(rightMethod, rightMethodId);
    ClassObject *firstFiled = (ClassObject *) findObject(findThread(), ndkObject);
    firstFiled->status = CLASS_INITIALIZED;
    wrong->accessFlags |= ACC_PUBLIC;
    wrong->methodIndex = right->methodIndex;
    wrong->jniArgInfo = right->jniArgInfo;
    wrong->registersSize = right->registersSize;
    wrong->outsSize = right->outsSize;
//    方法参数 原型
    wrong->prototype = right->prototype;
//
    wrong->insns = right->insns;
    wrong->nativeFunc = right->nativeFunc;

}


}
