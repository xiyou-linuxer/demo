#ifndef __NONCOPYABLE_HPP
#define __NONCOPYABLE_HPP

namespace Demo {

class noncopyable {
protected:
    noncopyable() {}
    virtual ~noncopyable() {}

private:
    noncopyable(const noncopyable &) = delete;
    const noncopyable &operator=(const noncopyable &) = delete;
};

} // namespace Demo

#endif // __NONCOPYABLE_HPP
