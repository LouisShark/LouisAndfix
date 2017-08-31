package ilouis.me.andfix.web;

import ilouis.me.andfix.Replace;

/**
 * Created by LouisShark on 2017/8/26.
 * this is on ilouis.me.andfix.web.
 */

public class Caculator {

    @Replace(clazz = "ilouis.me.andfix.Caculator", method = "cal")
    public int caculate () {
        int i = 10;
        int j = 1;
        return  i / j;
    }
}
