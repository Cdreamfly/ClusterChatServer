//
// Created by Cmf on 2022/5/28.
//
#pragma once
/**
 * copyable类是一个空的标记类，指示所有继承copyable的子类都应该是值类型，也就是可以安全拷贝的，
 * 而之所以设置成protected，意味着该类无法产生对象，而只能派生子类
 */
class copyable
{
protected:
    copyable() = default;
    ~copyable() = default;
};