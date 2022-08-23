//
// Created by Cmf on 2022/8/21.
//

#ifndef CMFNETLIB_CURRENTTHREAD_HPP
#define CMFNETLIB_CURRENTTHREAD_HPP

#include<unistd.h>
#include<sys/syscall.h>

namespace CurrentThread {
    extern __thread int t_cachedTid;

    void cache_tid();

    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {
            cache_tid();
        }
        return t_cachedTid;
    }
}
#endif //CMFNETLIB_CURRENTTHREAD_HPP
