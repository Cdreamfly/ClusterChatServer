//
// Created by Cmf on 2022/5/31.
//

#ifndef CMFNETLIB_BUFFER_HPP
#define CMFNETLIB_BUFFER_HPP

#include "base/noncopyable.hpp"
#include "base/Log.hpp"
#include "SocketOps.hpp"
#include <memory>
#include <vector>
#include <atomic>
#include <cstring>


class Buffer : public noncopyable {
public:
    using ptr = std::shared_ptr<Buffer>;
    //8字节长度（解决粘包问题）
    static const size_t CheapPrepend = 8;
    static const size_t InitialSize = 1024;

public:

    explicit Buffer(size_t size = InitialSize) : _buffer(InitialSize + CheapPrepend),
                                                 _readIndex(CheapPrepend),
                                                 _writerIndex(CheapPrepend) {}

    //可写入字节数
    size_t WritableBytes() const {
        return _buffer.size() - _writerIndex;
    }

    //可读出字节数
    size_t ReadableBytes() const {
        return _writerIndex - _readIndex;
    }

    //头部字节数
    size_t PrependableBytes() const {
        return _readIndex;
    }

    //读出字节
    void Retrieve(size_t len) {
        if (len < ReadableBytes()) {
            _readIndex += len;
        } else {
            RetrieveAll();
        }
    }

    //把onMessage函数上报的Buffer数据，转成string类型的数据返回
    std::string RetrieveAllAsString() {
        return RetrieveAsString(ReadableBytes());//应用可读取数据的长度
    }

    std::string RetrieveAsString(size_t len) {
        std::string result(Peek(), len);
        Retrieve(len);
        return result;
    }

    //读出全部字节
    void RetrieveAll() {
        _readIndex = _writerIndex = CheapPrepend;
    }

    //追加数据
    void Append(const char *str, size_t len) {
        if (WritableBytes() < len) {
            MakeSpace(len);
        }
        std::copy(str, str + len, BeginWrite());
        _writerIndex += len;
    }

    void Append(const void *str, size_t len) {
        Append(static_cast<const char *>(str), len);
    }

    void Append(const std::string &str) {
        Append(str.c_str(), str.length());
    }

    //返回可写入数据段的首位指针
    char *BeginWrite() {
        return Begin() + _writerIndex;
    }

    const char *BeginWrite() const {
        return Begin() + _writerIndex;
    }

    const char *Peek() const {
        return Begin() + _readIndex;
    }

    ssize_t ReadFd(int fd, int *savedErrno) {
        char extrabuf[65536] = {0};
        const size_t writable = WritableBytes();
        iovec vec[2];
        vec[0].iov_base = Begin() + _writerIndex;   //第一块缓冲
        vec[0].iov_len = writable;
        vec[1].iov_base = extrabuf; //第二块缓冲
        vec[1].iov_len = sizeof(extrabuf);
        //iovcnt:vec数组的长度,先填充vec[0],填满了才填vec[1]
        const int iovcnt = writable < sizeof(extrabuf) ? 2 : 1;
        const ssize_t n = SocketOps::Readv(fd, vec, iovcnt);
        if (n < 0) {
            *savedErrno = errno;
        } else if (n <= writable) {  //如果读取的数据相等或比可写入空间还小
            _writerIndex += n;
        } else {    //如果读取数据大于可写入空间
            _writerIndex = _buffer.size();  //第一块缓冲区空间写满了
            Append(extrabuf, n - writable); //就把第二块缓冲区的数据追加到第一块缓冲区中，数据大小为：总读取数据大小n - 第一块缓冲区可写入大小
        }
        return n;
    }

    ssize_t WriteFd(int fd, int *savedErrno) const {
        LOG_INFO("-----HandleWrite-----%s",Peek());
        ssize_t len = SocketOps::Write(fd, Peek(), ReadableBytes());
        if (len < 0) {
            *savedErrno = errno;
        }
        return len;
    }

private:
    //扩充
    void MakeSpace(size_t len) {
        //如果首部＋尾部空余的空间都不够存储数据就扩充
        if (WritableBytes() + PrependableBytes() < len + CheapPrepend) {
            _buffer.resize(_writerIndex + len);
        } else {    //够的话就把首位空间合在一起
            size_t readSize = ReadableBytes();
            std::copy(Begin() + _readIndex, Begin() + _writerIndex, Begin() + CheapPrepend);
            _readIndex = CheapPrepend;
            _writerIndex = _readIndex + readSize;
        }
    }

    char *Begin() {
        return &*_buffer.begin();
    }

    const char *Begin() const {
        return &*_buffer.begin();
    }

private:
    std::vector<char> _buffer;
    std::atomic<size_t> _readIndex; //读出下标
    std::atomic<size_t> _writerIndex;//写入下标
};

#endif //CMFNETLIB_BUFFER_HPP
