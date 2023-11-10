#pragma once

#include "cm/base/Copyable.hpp"

#include <vector>
#include <string>
#include <string_view>

namespace cm::net {
	/**premeditate
	 * +-------------------+------------------+------------------+
	 * | prependable bytes |  readable bytes  |  writable bytes  |
	 * |                   |     (CONTENT)    |                  |
	 * +-------------------+------------------+------------------+
	 * |                   |                  |                  |
	 * 0      <=      readerIndex   <=   writerIndex    <=     size
	 */
	class Buffer : public Copyable {
	public:
		static const size_t kCheapPrepend = 8;
		static const size_t kInitialSize = 1024;

		explicit Buffer(const size_t initialSize = kInitialSize)
				: buffer_(kCheapPrepend + initialSize), readerIndex_(kCheapPrepend), writerIndex_(kCheapPrepend) {}

		//可读出字节数
		[[nodiscard]] size_t readableBytes() const {
			return writerIndex_ - readerIndex_;
		}

		//可写入字节数
		[[nodiscard]] size_t writableBytes() const {
			return buffer_.size() - writerIndex_;
		}

		[[nodiscard]] size_t prependableBytes() const {
			return readerIndex_;
		}

		// 返回缓冲区中可读数据的起始地址
		[[nodiscard]] const char *peek() const {
			return begin() + readerIndex_;
		}

		// onMessage string <- Buffer
		void retrieve(const size_t len) {
			if (len < readableBytes()) {
				readerIndex_ += len; // 应用只读取了刻度缓冲区数据的一部分，就是len，还剩下readerIndex_ += len -> writerIndex_
			} else   // len == readableBytes()
			{
				retrieveAll();
			}
		}

		void retrieveAll() {
			readerIndex_ = writerIndex_ = kCheapPrepend;
		}

		// 把onMessage函数上报的Buffer数据，转成string类型的数据返回
		std::string retrieveAllAsString() {
			return retrieveAsString(readableBytes()); // 应用可读取数据的长度
		}

		std::string retrieveAsString(const size_t len) {
			std::string result(peek(), len);
			retrieve(len); // 上面一句把缓冲区中可读的数据，已经读取出来，这里肯定要对缓冲区进行复位操作
			return result;
		}

		// buffer_.size() - writerIndex_    len
		void ensureWriteableBytes(const size_t len) {
			if (writableBytes() < len) {
				makeSpace(len); // 扩容函数
			}
		}

		[[nodiscard]] std::string_view toStringPiece() const {
			return {peek(), readableBytes()};
		}

		void append(const std::string_view &str) {
			append(str.data(), str.size());
		}

		// 把[data, data+len]内存上的数据，添加到writable缓冲区当中
		void append(const char *data, const size_t len) {
			ensureWriteableBytes(len);
			std::copy(data, data + len, beginWrite());
			writerIndex_ += len;
		}

		char *beginWrite() {
			return begin() + writerIndex_;
		}

		[[nodiscard]] const char *beginWrite() const {
			return begin() + writerIndex_;
		}

		// 从fd上读取数据
		ssize_t readFd(int, int &);

		// 通过fd发送数据
		ssize_t writeFd(int, int &) const;

	private:
		char *begin() {
			return &*buffer_.begin();  // vector底层数组首元素的地址，也就是数组的起始地址
		}

		[[nodiscard]] const char *begin() const { return &*buffer_.begin(); }

		void makeSpace(const std::size_t len) {
			//如果readerIndex之前的空间＋writerIndex之后的空间都不够存储数据就扩充
			if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
				buffer_.resize(writerIndex_ + len);
			} else {    //否则buffer够用，就把当前未读出和已写入的数据向前挪挪
				const size_t readAble = readableBytes();
				std::copy(begin() + readerIndex_,
				          begin() + writerIndex_,
				          begin() + kCheapPrepend);
				readerIndex_ = kCheapPrepend;
				writerIndex_ = readerIndex_ + readAble;
			}
		}

	private:
		std::vector<char> buffer_;
		std::size_t readerIndex_;
		std::size_t writerIndex_;
	};
}