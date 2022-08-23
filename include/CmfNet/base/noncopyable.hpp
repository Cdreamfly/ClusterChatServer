//
// Created by Cmf on 2022/5/23.
//

#ifndef CMFNETLIB_NONCOPYABLE_HPP
#define CMFNETLIB_NONCOPYABLE_HPP

/*
 * 允许构造和析构禁止拷贝和赋值
 */
class noncopyable {
private:
    noncopyable(const noncopyable &) = delete;

    const noncopyable &operator=(const noncopyable &) = delete;

protected:  //派生类可以访问，外部无法访问
    noncopyable() = default;

    ~noncopyable() = default;
};

#endif //CMFNETLIB_NONCOPYABLE_HPP
