package ilouis.me.andfix;

import android.content.Context;
import android.os.Build;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Enumeration;

import dalvik.system.DexFile;

/**
 * Created by LouisShark on 2017/8/26.
 * this is on ilouis.me.andfix.
 */

public class DexManager {
    private Context mContext;
    private DexManager(Context context){
        this.mContext = context;
    }
    private volatile static  DexManager INSTANCE = null;
    public static DexManager getInstance(Context context){
        if (INSTANCE == null) {
            synchronized (DexManager.class) {
                if (INSTANCE == null) {
                    INSTANCE = new DexManager(context);
                }
            }
        }
        return INSTANCE;
    }

    public void loadFile(File file) {
        try {
            DexFile dexFile = DexFile.loadDex(file.getAbsolutePath(), new File(mContext.getCacheDir(), "opt").getAbsolutePath(), Context.MODE_PRIVATE);
            //得到class --取出修复好的method
            Enumeration<String> enumeration = dexFile.entries();
            while (enumeration.hasMoreElements())
            {
                //拿到全类名reuse
                String classNmae = enumeration.nextElement();
                //class.forName
                Class clazz = dexFile.loadClass(classNmae, mContext.getClassLoader());
                if (clazz != null) {
                    fixClazz(clazz);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void fixClazz(Class realClazz) {
        //服务器修复好drealClazz
        Method[] methods = realClazz.getDeclaredMethods();
        for (Method method : methods) {
            Replace replace = method.getAnnotation(Replace.class);
            if (replace == null)
            {
                continue;
            }
            //找到修复好的Method 找到出bug的Method
            String wrongClazz = replace.clazz();
            String wrongMethodName = replace.method();
            try {
                Class clazz = Class.forName(wrongClazz);
                Method wrongMethod = clazz.getDeclaredMethod(wrongMethodName, method.getParameterTypes());
                if (Build.VERSION.SDK_INT <= 19) {
                    replace(Build.VERSION.SDK_INT, wrongMethod, method);
                } else if (Build.VERSION.SDK_INT <= 23){
                    replace6(wrongMethod, method);
                } /*else if (Build.VERSION.SDK_INT <= 25){
                    replace7(wrongMethod, method);
                }*/
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public native void replace(int sdkVersion, Method wrongMethod, Method rightMethod);
    public native void replace6(Method wrongMethod, Method rightMethod);
//    public native void replace7(Method wrongMethod, Method rightMethod);

    static {
        System.loadLibrary("andfix");
    }
}
