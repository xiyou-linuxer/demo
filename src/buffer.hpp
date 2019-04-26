#ifndef __BUFFER_HPP
#define __BUFFER_HPP

#include <algorithm>
#include <cassert>

#include "noncopyable.hpp"
#include "util.hpp"
#include "uv.h"

namespace Demo {

// Modified from muduo project http://github.com/chenshuo/muduo
//
// +-------------------+------------------+------------------+
// | prependable bytes |  readable bytes  |  writable bytes  |
// |     reserved      |     (CONTENT)    |                  |
// +-------------------+------------------+------------------+
// |                   |                  |                  |
// 0      <=      readerIndex   <=   writerIndex    <=     size
//
class Buffer final : private noncopyable {

private:
    char *buffer_;
    size_t capacity_;
    size_t read_index_;
    size_t write_index_;
    size_t reserved_size_;

    static const size_t kReservedSize = 0;
    static const size_t kInitSize = 1024 * 16;
    static constexpr char kCRLF[] = "\r\n";

public:
    explicit Buffer(size_t init_size = kInitSize, size_t reserved_size = kReservedSize)
            : capacity_(reserved_size + init_size),
            read_index_(reserved_size),
            write_index_(reserved_size),
            reserved_size_(reserved_size) {
        buffer_ = new char[capacity_];
    }

    ~Buffer() final {
        delete[] buffer_;
        buffer_ = nullptr;
        capacity_ = 0;
    }

    void swap(Buffer &rhs) {
        std::swap(buffer_, rhs.buffer_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(read_index_, rhs.read_index_);
        std::swap(write_index_, rhs.write_index_);
        std::swap(reserved_size_, rhs.reserved_size_);
    }

    void reserve(size_t len) {
        if (capacity_ >= len + reserved_size_) {
            return;
        }
        ensureCapacity(len + reserved_size_);
    }

    void ensureWritableBytes(size_t len) {
        if (writableBytes() < len) {
            ensureCapacity(len);
        }
        assert(writableBytes() >= len);
    }

    void writeData(const void *data, size_t len) {
        ensureWritableBytes(len);
        memcpy(beginWrite(), data, len);
        assert(write_index_ + len <= capacity_);
        incrWriteIndex(len);
    }

    void append(const void *data, size_t len) {
        writeData(data, len);
    }

    void prepend(const void *data, size_t len) {
        assert(len <= prependableBytes());
        read_index_ -= len;
        memcpy(begin() + read_index_, data, len);
    }

    void retrieveAll() {
        read_index_ = reserved_size_;
        write_index_ = reserved_size_;
    }

    void retrieve(size_t len) {
        assert(len <= readableBytes());
        if (len < readableBytes()) {
            read_index_ += len;
        } else {
            retrieveAll();
        }
    }

    void retrieve(const char *pos) {
        assert(pos >= beginRead() && pos <= beginWrite());
        if (pos < beginWrite()) {
            read_index_ += (beginWrite() - pos);
        } else {
            retrieveAll();
        }
    }

    const char *findCRLF() const {
        const char *crlf = std::search(beginRead(), beginWrite(), kCRLF, kCRLF + 2);
        return (crlf == beginWrite()) ? nullptr : crlf;
    }

    const char *findCRLF(const char *start) const {
        assert(start > beginRead() && start <= beginWrite());
        const char *crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
        return (crlf == beginWrite()) ? nullptr : crlf;
    }

    void decrWriteIndex(size_t n) {
        write_index_ -= n;
        assert(write_index_ >= reserved_size_);
    }

    void incrWriteIndex(size_t n) {
        write_index_ += n;
        assert(write_index_ <= capacity_);
    }

    int8_t peekInt8() const {
        assert(readableBytes() >= sizeof(int8_t));
        int8_t x = *beginRead();
        return x;
    }

    int16_t peekInt16() const {
        assert(readableBytes() >= sizeof(int16_t));
        int16_t x = *beginRead();
        return x;
    }

    int32_t peekInt32() const {
        assert(readableBytes() >= sizeof(int32_t));
        int32_t x = *beginRead();
        return x;
    }

    int8_t readInt8() {
        int8_t result = peekInt8();
        retrieve(sizeof(int8_t));
        return result;
    }

    int16_t readInt16() {
        int16_t result = peekInt16();
        retrieve(sizeof(int16_t));
        return result;
    }

    int32_t readInt32() {
        int32_t result = peekInt32();
        retrieve(sizeof(int32_t));
        return result;
    }

    size_t capacity() const {
        return capacity_;
    }

    char *begin() const {
        return buffer_;
    }

    char *beginRead() {
        return begin() + read_index_;
    }

    const char *beginRead() const {
        return begin() + read_index_;
    }

    char *beginWrite() {
        return begin() + write_index_;
    }

    const char *beginWrite() const {
        return begin() + write_index_;
    }

    size_t readableBytes() const {
        assert(write_index_ >= read_index_);
        return write_index_ - read_index_;
    }

    size_t writableBytes() const {
        assert(capacity_ >= write_index_);
        return capacity_ - write_index_;
    }

    size_t prependableBytes() const {
        return read_index_;
    }

    uv_buf_t toUvBuf() {
        return uv_buf_init(beginRead(), static_cast<unsigned int>(readableBytes()));
    }

    std::string toString() const {
        return std::string(beginRead(), readableBytes());
    }

private:
    void ensureCapacity(size_t len) {
        if (writableBytes() + prependableBytes() < len + reserved_size_) {
            // grow the capacity
            size_t new_capacity = (capacity_ << 1) + len;
            size_t readable = readableBytes();
            char *new_buffer = new char[new_capacity];
            memcpy(new_buffer + reserved_size_, beginRead(), readable);
            write_index_ =  reserved_size_ + readable;
            read_index_ = reserved_size_;
            capacity_ = new_capacity;
            delete[] buffer_;
            buffer_ = new_buffer;
            assert(writableBytes() >= len);
        } else {
            // move readable data to the front, make space inside buffer
            size_t readable = readableBytes();
            memmove(begin() + reserved_size_, beginRead(), readableBytes());
            read_index_ = reserved_size_;
            write_index_ = read_index_ + readable;
            assert(readable == readableBytes());
            assert(writableBytes() >= len);
        }
    }
};

} // namespace Demo

#endif //__BUFFER_HPP
